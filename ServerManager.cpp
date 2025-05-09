#include "PreferencesManager.h"
#include "shared_variables.h"
#include "ServerManager.h"
#include "pulse.h"
#include "timer.h"

// Define global variables
WebServer server(80);

String ssid = "Br@1n14c3";
String password = "40and40";

void handleRoot() {
  String html = "<html><body><h1>ESP32 Control Panel</h1>";
  html += "<p>LED Frequency: <input type='number' id='ledFreq' value='" + String(led_frequency) + "'></p>";
  html += "<p>LED Duty Cycle: <input type='number' id='ledDuty' value='" + String(led_duty) + "'></p>";
  html += "<p>Audio Frequency: <input type='number' id='audioFreq' value='" + String(audio_frequency) + "'></p>";
  html += "<p>Audio Duty Cycle: <input type='number' id='audioDuty' value='" + String(audio_duty) + "'></p>";
  html += "<p>Max Time (ms): <input type='number' id='maxTime' value='" + String(max_time) + "'></p>";
  html += "<button onclick='sendData()'>Update</button>";
  html += "<script>function sendData() {";
  html += "var ledFreq = document.getElementById('ledFreq').value;";
  html += "var ledDuty = document.getElementById('ledDuty').value;";
  html += "var audioFreq = document.getElementById('audioFreq').value;";
  html += "var audioDuty = document.getElementById('audioDuty').value;";
  html += "var maxTime = document.getElementById('maxTime').value;";
  html += "var xhr = new XMLHttpRequest();";
  html += "xhr.open('GET', '/update?ledFreq=' + ledFreq + '&ledDuty=' + ledDuty + '&audioFreq=' + audioFreq + '&audioDuty=' + audioDuty + '&maxTime=' + maxTime, true);";
  html += "xhr.send();";
  html += "}</script></body></html>";
  server.send(200, "text/html", html);
}

void handleUpdate() {
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
  WiFi.softAP(ssid, password);
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
