#include "PreferencesManager.h"
#include "shared_variables.h"
#include "ServerManager.h"
#include "pulse.h"
#include <FS.h>
#include "screen.h"
#include <SPIFFS.h>
#include <WiFi.h> // Ensure WiFi library is included
#include "timer.h"
#include <ESPmDNS.h>

// Define global variables
const int WIFI_CONNECT_TIMEOUT_MS = 15000; // 15 seconds timeout for WiFi connection
WebServer server(80);

// Simple template processor
String processor(const String& var) {
  if (var == "LED_FREQ") {
    return String(led_frequency);
  } else if (var == "LED_DUTY") {
    return String(100 * led_duty/8191); // Convert to percentage for display
  } else if (var == "AUDIO_FREQ") {
    return String(audio_frequency);
  } else if (var == "AUDIO_DUTY") {
    return String(100 * audio_duty/8191); // Convert to percentage for display
  } else if (var == "MAX_TIME") {
    return String(max_time/60000); // Convert ms to minutes for display
  } else if (var == "STA_SSID") {
    return LOCAL_STA_SSID;
  } else if (var == "STA_PASSWORD") {
    return LOCAL_STA_PASSWORD.length() > 0 ? "(hidden)" : "";
  }
  return String();
}

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  String template_content = file.readString();
  file.close(); // Close file as soon as content is read

  // Replace placeholders
  template_content.replace("{{LED_FREQ}}", processor("LED_FREQ"));
  template_content.replace("{{LED_DUTY}}", processor("LED_DUTY"));
  template_content.replace("{{AUDIO_FREQ}}", processor("AUDIO_FREQ"));
  template_content.replace("{{AUDIO_DUTY}}", processor("AUDIO_DUTY"));
  template_content.replace("{{MAX_TIME}}", processor("MAX_TIME"));
  template_content.replace("{{PLAYING}}", playing ? "checked" : "");

  server.send(200, "text/html", template_content);
}

void handleWifiConfig() {
  File file = SPIFFS.open("/wifi_config.html", "r");
  String template_content = file.readString();
  file.close(); // Close file as soon as content is read

  // Replace placeholders
  template_content.replace("{{STA_SSID}}", processor("STA_SSID"));
  template_content.replace("{{HAS_PASSWORD}}", processor("STA_PASSWORD"));

  server.send(200, "text/html", template_content);
}

void handleUpdate() {
  if (server.hasArg("ledFreq")) {
    led_frequency = server.arg("ledFreq").toInt();
    preferences.putInt("led_frequency", led_frequency);
    Serial.printf("Saved led_frequency: %d\n", led_frequency);
  }

  if (server.hasArg("ledDuty")) {
    // Corrected integer division: multiply first, then divide.
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
    // Corrected integer division: multiply first, then divide.
    audio_duty = (server.arg("audioDuty").toInt() * 8191) / 100; 
    preferences.putInt("audio_duty", audio_duty);
    Serial.printf("Saved audio_duty: %d (from %s%%)\n", audio_duty, server.arg("audioDuty").c_str());
  }

  if (server.hasArg("maxTime")) {
    max_time = server.arg("maxTime").toInt() * 60000; // Convert minutes to ms
    preferences.putInt("max_time", max_time);
    Serial.printf("Saved max_time: %d\n", max_time);
  }

  if (server.hasArg("playing")) {
    bool setPlaying = server.arg("playing") == "true";
    if (setPlaying != playing) {
      playing = setPlaying;
      if (playing) {
        startTime = millis(); // Reset start time when starting playback
        Serial.println("Playback started");
      } else {
        Serial.println("Playback stopped");
      }
    }
    playing = false; // Default to false
    playing = server.arg("playing") == "true";
    Serial.printf("Playing: %d\n", playing);
  }
  
  // After updating variables, refresh outputs and display
  updateOutputs(true);
  statusDisplay();
  server.send(200, "text/plain", "Values updated");
}

void handleUpdateWifi() {
  bool updated = false;

  bool hasValidInputs = server.hasArg("staSsid") && server.arg("staSsid").length() > 1 &&
    server.hasArg("staPassword") && server.arg("staPassword").length() > 7;

    Serial.print("PWD: ");
    Serial.println(server.arg("staPassword"));

    Serial.print("SSID: ");
    Serial.println(server.arg("staSsid"));

  if (hasValidInputs) {
    String receivedSsid = server.arg("staSsid");
    LOCAL_STA_SSID = receivedSsid;
    preferences.putString("sta_ssid", LOCAL_STA_SSID);
 
    LOCAL_STA_PASSWORD = server.arg("staPassword");
    preferences.putString("sta_pass", LOCAL_STA_PASSWORD);
    updated = true;
  }
  
  server.send(200, "text/plain", updated ? "WiFi settings updated. Rebooting ..." : "Must have both SSID and password to update WiFi settings.");
  
  if (updated) {
    delay(500); // Allow time for response to be sent
    ESP.restart();
  }
}

void handleCss() {
  File file = SPIFFS.open("/main.css", "r");
  String css_content = file.readString();
  file.close(); // Close file as soon as content is read
  server.send(200, "text/css", css_content);
}

void handleJs() {
  File file = SPIFFS.open("/main.js", "r");
  String js_content = file.readString();
  file.close(); // Close file as soon as content is read
  server.send(200, "application/javascript", js_content);
}

void setupWebServer() {
  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){ // true = format SPIFFS if mount failed
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  } else {
    Serial.println("SPIFFS mounted successfully.");
  }

  Serial.println("Attempting to connect to local WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("wavebox");
  // Only attempt to connect if SSID is configured
  if (LOCAL_STA_SSID.length() > 0) {
    WiFi.begin(LOCAL_STA_SSID.c_str(), LOCAL_STA_PASSWORD.c_str());
  } else {
    Serial.println("STA SSID not configured. Skipping STA connection attempt.");
  }

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < WIFI_CONNECT_TIMEOUT_MS) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    // Initialize mDNS
    if (!MDNS.begin("wavebox")) { 
      Serial.println("Error setting up MDNS responder!");
      while(1) {
        delay(1000);
      }
    }
    Serial.println("mDNS responder started");

  } else {
    Serial.println("Failed to connect to local WiFi. Starting Access Point mode.");
    WiFi.mode(WIFI_AP); // Fallback to AP mode
    // Use DEFAULT_AP_SSID and DEFAULT_AP_PASSWORD from shared_variables.h
    if (WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD)) {
      Serial.println("SoftAP started successfully with SSID: " + String(DEFAULT_AP_SSID));
      WiFi.setTxPower(WIFI_POWER_13dBm); // Set TX power for AP mode
      Serial.print("SoftAP IP address: ");
      Serial.println(WiFi.softAPIP());
      strncpy(wifi_ap_ip, WiFi.softAPIP().toString().c_str(), sizeof(wifi_ap_ip) - 1);
      wifi_ap_ip[sizeof(wifi_ap_ip) - 1] = '\0'; // Ensure null-termination
      Serial.println("Stored SoftAP IP in wifi_ap_ip: " + String(wifi_ap_ip));
      statusDisplay();
    } else {
      Serial.println("Error: SoftAP failed to start. ESP32 may use a default SSID.");
      // Handle AP start failure if necessary, though it's rare
    }
  }

  server.on("/", handleRoot);
  server.on("/wifi-config", handleWifiConfig);
  server.on("/update", handleUpdate);
  server.on("/update-wifi", handleUpdateWifi);
  server.begin();
  Serial.println("Server started");
  server.on("/main.css", handleCss);
  server.on("/main.js", handleJs);
  

  // Display IP on screen if possible (optional, depends on screen capabilities and space)
  // Example:
  // if (WiFi.status() == WL_CONNECTED) updateScreenWithIP(WiFi.localIP().toString());
  // else updateScreenWithIP(WiFi.softAPIP().toString());
}

void handleClient() {
  server.handleClient();
}
