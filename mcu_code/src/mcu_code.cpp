//#include <Arduino.h>                              		//-- moved to arduinoGlue.h
#include "mcu_code.h"
#include "PreferencesManager.h"
#include "shared_variables.h"
#include "screen.h"
#include "buttons.h"
#include "rotary.h"
#include "pulse.h"
#include "timer.h"
#include "ServerManager.h"

Preferences preferences; // Define the Preferences instance

unsigned long startTime;
unsigned long lastTime = 0; // Declare lastTime in the global scope

/* Initial setup function */
void setup() {
  // Add a small delay to allow serial monitor to connect
  delay(1000); // Wait 1 second
  Serial.begin(115200);
  Serial.println("\n\n--- ESP32 BOOTING ---"); // Very first message
  Serial.flush(); // Ensure it's sent
  
  // Initialize preferences
  Serial.println("Initializing preferences...");
  if (!preferences.begin("settings", false)) {
    Serial.println("!!! Failed to initialize preferences. Settings may not save/load. !!!");
  } else {
    Serial.println("Preferences initialized successfully.");
  }

  led_frequency = preferences.getInt("led_frequency", DEFAULT_LED_FREQUENCY);
  led_duty = preferences.getInt("led_duty", DEFAULT_LED_DUTY);
  audio_frequency = preferences.getInt("audio_frequency", DEFAULT_AUDIO_FREQUENCY);
  audio_duty = preferences.getInt("audio_duty", DEFAULT_AUDIO_DUTY);
  max_time = preferences.getInt("max_time", DEFAULT_MAX_TIME);
  display_sleep = preferences.getInt("display_sleep", false);
  
  LOCAL_STA_SSID = preferences.getString("sta_ssid", DEFAULT_STA_SSID);
  Serial.print("Loaded STA SSID from Preferences: '"); Serial.print(LOCAL_STA_SSID); Serial.println("'");
  LOCAL_STA_PASSWORD = preferences.getString("sta_pass", DEFAULT_STA_PASSWORD);
  // Serial.print("Loaded STA Password from Preferences: '"); Serial.print(LOCAL_STA_PASSWORD); Serial.println("'"); // Be cautious logging passwords

  Serial.println("Preferences loaded");
  
  setupPulse();
  setupScreen();
  setupButtons();
  statusDisplay();
  rotaryEncSetup();
  setupWebServer();
}

/* Main loop function */
void loop() {
  startStopButton.loop();
  onOffButton.loop();
  r.loop();
  handleStartStop();
  handleOnOff();
  if (playing && (millis() - lastTime > 900)) {
    statusDisplay();
  }
  handleClient();
}