#include "PreferencesManager.h"
#include "shared_variables.h"
#include "ServerManager.h"
#include "pulse.h"
#include "timer.h"

// Define global variables
WebServer server(80);

String ssid;
String password;
String ipAddress;

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "body { font-family: Arial, sans-serif; margin: 0; padding: 20px; }";
  html += "h1 { text-align: center; }";
  html += "input[type='number'], input[type='text'] { width: 100%; padding: 10px; margin: 10px 0; box-sizing: border-box; }";
  html += "button { width: 100%; padding: 10px; background-color: #4CAF50; color: white; border: none; cursor: pointer; }";
  html += "button:hover { background-color: #45a049; }";
  html += "</style></head><body>";
  html += "<h1>ESP32 Control Panel</h1>";
  html += "<p>SSID: <input type='text' id='ssid' value='" + ssid + "'></p>";
  html += "<p>Password: <input type='text' id='password' value='" + password + "'></p>";
  html += "<p>IP Address: <input type='text' id='ipAddress' value='" + ipAddress + "'></p>";
  html += "<p>LED Frequency: <input type='number' id='ledFreq' value='" + String(led_frequency) + "'></p>";
  html += "<p>LED Duty Cycle: <input type='number' id='ledDuty' value='" + String(led_duty) + "'></p>";
  html += "<p>Audio Frequency: <input type='number' id='audioFreq' value='" + String(audio_frequency) + "'></p>";
  html += "<p>Audio Duty Cycle: <input type='number' id='audioDuty' value='" + String(audio_duty) + "'></p>";
  html += "<p>Max Time (ms): <input type='number' id='maxTime' value='" + String(max_time) + "'></p>";
  html += "<button onclick='sendData()'>Update</button>";
  html += "<script>function sendData() {";
  html += "var ssid = document.getElementById('ssid').value;";
  html += "var password = document.getElementById('password').value;";
  html += "var ipAddress = document.getElementById('ipAddress').value;";
  html += "var ledFreq = document.getElementById('ledFreq').value;";
  html += "var ledDuty = document.getElementById('ledDuty').value;";
  html += "var audioFreq = document.getElementById('audioFreq').value;";
  html += "var audioDuty = document.getElementById('audioDuty').value;";
  html += "var maxTime = document.getElementById('maxTime').value;";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/update?ssid=' + ssid + '&password=' + password + '&ipAddress=' + ipAddress + '&ledFreq=' + ledFreq + '&ledDuty=' + ledDuty + '&audioFreq=' + audioFreq + '&audioDuty=' + audioDuty + '&maxTime=' + maxTime, true);";
  html += "xhr.send();";
  html += "}</script></body></html>";
  server.send(200, "text/html", html);
}

void handleUpdate() {
  if (server.hasArg("ssid")) {
    ssid = server.arg("ssid");
    preferences.putString("ssid", ssid);
  }
  if (server.hasArg("password")) {
    password = server.arg("password");
    preferences.putString("password", password);
  }
  if (server.hasArg("ipAddress")) {
    ipAddress = server.arg("ipAddress");
    preferences.putString("ipAddress", ipAddress);
  }
  if (server.hasArg("ledFreq")) {
    led_frequency = server.arg("ledFreq").toInt();
    preferences.putInt("led_frequency", led_frequency);
  }
  if (server.hasArg("ledDuty")) {
    led_duty = server.arg("ledDuty").toInt();
    preferences.putInt("led_duty", led_duty);
  }
  if (server.hasArg("audioFreq")) {
    audio_frequency = server.arg("audioFreq").toInt();
    preferences.putInt("audio_frequency", audio_frequency);
  }
  if (server.hasArg("audioDuty")) {
    audio_duty = server.arg("audioDuty").toInt();
    preferences.putInt("audio_duty", audio_duty);
  }
  if (server.hasArg("maxTime")) {
    max_time = server.arg("maxTime").toInt();
    preferences.putInt("max_time", max_time);
  }
  server.send(200, "text/plain", "Values updated");
}

void setupWebServer() {
  ssid = preferences.getString("ssid", DEFAULT_SSID);
  password = preferences.getString("password", DEFAULT_PASSWORD);
  ipAddress = preferences.getString("ipAddress", DEFAULT_IP_ADDRESS);

  IPAddress local_IP;
  local_IP.fromString(ipAddress);
  Serial.print("Work!!!: ");
  Serial.println(ssid.c_str());
  Serial.println(password.c_str());
  Serial.println(ipAddress.c_str());
  Serial.println("... or f'n don't");

  WiFi.softAP(ssid.c_str(), password.c_str());
  WiFi.softAPConfig(local_IP, local_IP, IPAddress(255, 255, 255, 0));
  WiFi.setTxPower(WIFI_POWER_13dBm);

  Serial.print("SoftAP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/update", handleUpdate);
  server.begin();
  Serial.println("Server started");
}

void handleClient() {
  server.handleClient();
}
