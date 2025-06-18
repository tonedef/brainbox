#include "mcu_code.h"
#include "PreferencesManager.h"
#include "shared_variables.h"
#include "screen.h"
#include "buttons.h"
#include "rotary.h"
#include "pulse.h"
#include "timer.h"
#include "ServerManager.h"

/**
 * @brief Global instance for accessing ESP32 Non-Volatile Storage (NVS).
 */
Preferences preferences;

unsigned long startTime;      ///< Timestamp (millis) when playback was last started.
unsigned long lastTime = 0;   ///< Timestamp (millis) of the last display update during playback.

// Conditionally compile setup() and loop() to avoid conflicts with test firmware
#ifndef UNIT_TEST

/**
 * @brief Main setup function, called once after power-on or reset.
 * Initializes serial communication, preferences, peripherals, UI, and web server.
 */
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n--- ESP32 BOOTING ---");
  Serial.flush();
  
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
  display_sleep = preferences.getBool("display_sleep", false);
  
  LOCAL_STA_SSID = preferences.getString("sta_ssid", DEFAULT_STA_SSID);
  Serial.print("Loaded STA SSID from Preferences: '"); Serial.print(LOCAL_STA_SSID); Serial.println("'");
  LOCAL_STA_PASSWORD = preferences.getString("sta_pass", DEFAULT_STA_PASSWORD);

  Serial.println("Preferences loaded");
  
  setupPulse();
  setupScreen();
  setupButtons();
  showLogo();
  rotaryEncSetup();
  setupWebServer();
}

/**
 * @brief Main loop function, called repeatedly.
 * Handles button/rotary events, manages playback state, updates display, and services web client requests.
 */
void loop() {
  startStopButton.loop();
  onOffButton.loop();
  configButton.loop();
  r.loop();
  handleStartStop();
  handleOnOff();
  handleConfigs();
  if (playing) {
    // For efficiency. Only update if longer than 900ms
    if ((millis() - lastTime) > 900) {
      int timeRemaining = max_time - (millis() - startTime);

      // Stop playback if time has expired or if timeRemaining is unexpectedly large
      // (e.g., due to millis() wrap-around or incorrect startTime).
      if (timeRemaining < 0 || timeRemaining > 90000000) { 
        playing = false;
        updateOutputs();
      }
      statusDisplay();
    }
  } 

  handleClient();
}
#endif // UNIT_TEST