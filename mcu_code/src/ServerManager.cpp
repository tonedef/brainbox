#include "PreferencesManager.h"
#include "shared_variables.h"
#include "ServerManager.h"
#include "pulse.h"
#include <FS.h>
#include "screen.h"
#include <SPIFFS.h>
#include "timer.h"
#include <ESPmDNS.h>

const int WIFI_CONNECT_TIMEOUT_MS = 15000; //< Timeout in milliseconds for attempting to connect to a WiFi network in Station mode.
WebServer server(80);

/**
 * @brief Handles requests to the root ("/") URL.
 * Serves the main control page (index.html) with dynamic content.
 * Placeholders in the HTML template are replaced with current device settings.
 */
void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  String template_content = file.readString();
  file.close(); // Close file as soon as content is read

  // Replace placeholders
  template_content.replace("{{LED_FREQ}}", String(led_frequency));
  template_content.replace("{{LED_DUTY}}", String(100 * led_duty/8191));
  template_content.replace("{{AUDIO_FREQ}}", String(audio_frequency));
  template_content.replace("{{AUDIO_DUTY}}", String(100 * audio_duty/8191));
  template_content.replace("{{MAX_TIME}}", String(max_time/60000));
  template_content.replace("{{DISPLAY_SLEEP}}", display_sleep ? "checked" : "");
  template_content.replace("{{PLAYING}}", playing ? "checked" : "");
  template_content.replace("{{WIFI_CONNECTED}}", WiFi.status() == WL_CONNECTED ? "connected" : "disconnected");

  server.send(200, "text/html", template_content);
}

/**
 * @brief Handles requests to the "/wifi-config" URL.
 * Serves the WiFi configuration page (wifi_config.html).
 * Displays the currently configured STA SSID.
 */
void handleWifiConfig() {
  File file = SPIFFS.open("/wifi_config.html", "r");
  String template_content = file.readString();
  file.close(); // Close file as soon as content is read

  // Replace placeholders
  template_content.replace("{{STA_SSID}}", LOCAL_STA_SSID);
  template_content.replace("{{HAS_PASSWORD}}", LOCAL_STA_PASSWORD.length() > 0 ? "(hidden)" : "");

  server.send(200, "text/html", template_content);
}

/**
 * @brief Handles POST requests to the "/update" URL to update device settings.
 * Reads parameters from the request, updates corresponding global variables, and saves them to Preferences.
 */
void handleUpdate() {
  if (server.hasArg("ledFreq")) {
    led_frequency = server.arg("ledFreq").toInt();
    preferences.putInt("led_frequency", led_frequency);
    Serial.printf("Saved led_frequency: %d\n", led_frequency);
  }

  if (server.hasArg("ledDuty")) {
    led_duty = (server.arg("ledDuty").toInt() * 8191) / 100; 
    preferences.putInt("led_duty", led_duty);
    Serial.printf("Saved led_duty: %d (from %s%%)\n", led_duty, server.arg("ledDuty").c_str());
  }

  if (server.hasArg("audioFreq")) {
    audio_frequency = server.arg("audioFreq").toInt();
    preferences.putInt("audio_frequency", audio_frequency);
    Serial.printf("Saved audio_frequency: %d\n", audio_frequency);
  }

  if (server.hasArg("audioDuty")) {
    audio_duty = (server.arg("audioDuty").toInt() * 8191) / 100; 
    preferences.putInt("audio_duty", audio_duty);
    Serial.printf("Saved audio_duty: %d (from %s%%)\n", audio_duty, server.arg("audioDuty").c_str());
  }

  if (server.hasArg("maxTime")) {
    max_time = server.arg("maxTime").toInt() * 60000; // Convert minutes to ms
    preferences.putInt("max_time", max_time);
    Serial.printf("Saved max_time: %d\n", max_time);
  }

  if (server.hasArg("displaySleep")) {
    display_sleep = server.arg("displaySleep") == "true";
    preferences.putBool("display_sleep", display_sleep);
    Serial.printf("Saved display_sleep: %s\n", display_sleep ? "true" : "false");
  }

  if (server.hasArg("playing")) {
    bool newPlayingState = server.arg("playing") == "true";
    if (newPlayingState != playing) { // If the state is actually changing
      playing = newPlayingState;      // Update the global playing state
      Serial.printf("Playing state updated: %s\n", playing ? "true" : "false");
      if (playing) {
        startTime = millis(); // Reset start time only when starting playback
        Serial.println("Playback started by server.");
      } else {
        Serial.println("Playback stopped by server.");
      }
    }
  }

  
  // After updating variables, refresh outputs and display
  updateOutputs(true);
  statusDisplay();
  server.send(200, "text/plain", "Values updated");
}

/**
 * @brief Handles POST requests to the "/update-wifi" URL to update WiFi credentials.
 * Validates SSID and password, saves them to Preferences, and reboots the device to apply changes.
 */
void handleUpdateWifi() {
  bool updated = false;

  bool hasValidSsid = server.hasArg("staSsid") && server.arg("staSsid").length() > 1;
  bool hasValidPwd = server.hasArg("staPassword") && server.arg("staPassword").length() > 7;

  if (hasValidSsid && hasValidPwd) {
    String receivedSsid = server.arg("staSsid");
    LOCAL_STA_SSID = receivedSsid;
    preferences.putString("sta_ssid", LOCAL_STA_SSID);
 
    LOCAL_STA_PASSWORD = server.arg("staPassword");
    preferences.putString("sta_pass", LOCAL_STA_PASSWORD);
    server.send(200, "text/plain", "WiFi settings updated. Rebooting ...");
    updated = true;
  } else {
    String errorMsg;
    if (!hasValidSsid) {
      errorMsg = "SSID must be at least 2 characters long. ";
    }

    if (!hasValidPwd) {
      errorMsg += "Password must be at least 8 characters long.";
    }

    server.send(400, "text/plain", errorMsg);
  }
  
  
  if (updated) {
    delay(500); // Allow time for response to be sent
    ESP.restart();
  }
}

/**
 * @brief Handles requests for the main CSS file ("/main.css").
 * Serves the content of main.css from SPIFFS.
 */
void handleCss() {
  File file = SPIFFS.open("/main.css", "r");
  String css_content = file.readString();
  file.close(); // Close file as soon as content is read
  server.send(200, "text/css", css_content);
}

/**
 * @brief Handles requests for the main JavaScript file ("/main.js").
 * Serves the content of main.js from SPIFFS.
 */
void handleJs() {
  File file = SPIFFS.open("/main.js", "r");
  String js_content = file.readString();
  file.close(); // Close file as soon as content is read
  server.send(200, "application/javascript", js_content);
}

/**
 * @brief Sets up the web server, including SPIFFS, WiFi connection, and route handlers.
 * Attempts to connect to a configured WiFi network (STA mode).
 * If STA connection fails or is not configured, it starts an Access Point (AP mode).
 * Initializes mDNS for service discovery.
 */
void setupWebServer() {
  // Initialize SPIFFS. true = format SPIFFS if mount failed.
  if(!SPIFFS.begin(true)){ // true = format SPIFFS if mount failed
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Serial.println("Attempting to connect to local WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("wavebox");
  // Only attempt to connect if STA SSID is configured in preferences
  if (LOCAL_STA_SSID.length() > 0) {
    WiFi.begin(LOCAL_STA_SSID.c_str(), LOCAL_STA_PASSWORD.c_str());
  } else {
    Serial.println("STA SSID not configured. Skipping STA connection attempt.");
  }

  unsigned long startAttemptTime = millis();
  int step = 0;
  // Wait for WiFi connection or timeout
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_CONNECT_TIMEOUT_MS) {
    int loadingSteps = 50; //loadingDisplay has a 10ms delay 50x10ms = 500
    Serial.print(".");
    while (--loadingSteps >= 0) {
      loadingDisplay(++step);
    }
    
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    MDNS.begin("wavebox"); // Start mDNS responder for wavebox.local
  } else {
    Serial.println("Failed to connect to local WiFi. Starting Access Point mode.");
    WiFi.mode(WIFI_AP); // Fallback to AP mode
    if (WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD)) {
      Serial.println("SoftAP started successfully with SSID: " + String(DEFAULT_AP_SSID));
      WiFi.setTxPower(WIFI_POWER_13dBm); // Set TX power for AP mode
      Serial.print("SoftAP IP address: ");
      Serial.println(WiFi.softAPIP());
      strncpy(wifi_ap_ip, WiFi.softAPIP().toString().c_str(), sizeof(wifi_ap_ip) - 1);
      wifi_ap_ip[sizeof(wifi_ap_ip) - 1] = '\0'; // Ensure null-termination
      Serial.println("Stored SoftAP IP in wifi_ap_ip: " + String(wifi_ap_ip));
    } else {
      Serial.println("Error: SoftAP failed to start.");
      // If we fail, it's fine. The device still works, just with fewer features
    }
  }
  
  statusDisplay();

  server.on("/", handleRoot);
  server.on("/wifi-config", handleWifiConfig);
  server.on("/update", handleUpdate);
  server.on("/update-wifi", handleUpdateWifi);
  server.on("/main.css", handleCss);
  server.on("/main.js", handleJs);
  server.begin(); // Start the server
  Serial.println("HTTP server started");
}

/**
 * @brief Handles incoming client requests to the web server.
 * This should be called in the main loop to process web traffic.
 */
void handleClient() {
  server.handleClient();
}
