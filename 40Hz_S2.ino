#include <Arduino.h>
#include "PreferencesManager.h"
#include "shared_variables.h"
#include "screen.h"
#include "buttons.h"
#include "dimmer.h"
#include "pulse.h"
#include "timer.h"
#include "ServerManager.h"

Preferences preferences; // Define the Preferences instance

unsigned long startTime;
unsigned long lastTime = 0; // Declare lastTime in the global scope

/* Initial setup function */
void setup() {
  Serial.begin(115200);
  
  // Initialize preferences
  Serial.println("Initializing preferences...");
  preferences.begin("settings", false);
  led_frequency = preferences.getInt("led_frequency", DEFAULT_LED_FREQUENCY);
  led_duty = preferences.getInt("led_duty", DEFAULT_LED_DUTY);
  audio_frequency = preferences.getInt("audio_frequency", DEFAULT_AUDIO_FREQUENCY);
  audio_duty = preferences.getInt("audio_duty", DEFAULT_AUDIO_DUTY);
  max_time = preferences.getInt("max_time", DEFAULT_MAX_TIME);
  Serial.println("Preferences loaded");
  
  setupPulse();
  setupScreen();
  setupButtons();
  statusDisplay();
  dimmerEncSetup();
  setupWebServer();
}

/* Main loop function */
void loop() {
  startStopButton.loop();
  r.loop();
  handleStartStop();
  if (playing && (millis() - lastTime > 900)) {
    statusDisplay();
  }
  handleClient();
}