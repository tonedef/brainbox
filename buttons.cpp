#include "shared_variables.h"
#include "buttons.h"
#include "screen.h"
#include "pulse.h"
#include "timer.h"

#define START_STOP_PIN 16 // S2 6 // C3
#define DEBOUNCE_TIME 50
#define HOLD_TIME 5000 // 5 seconds

ezButton startStopButton(START_STOP_PIN);
unsigned long buttonPressTime = 0;

/* Initialize button configurations */
void setupButtons() {
  pinMode(START_STOP_PIN, INPUT_PULLUP);
  startStopButton.setDebounceTime(DEBOUNCE_TIME);
}

void handleStartStop() {
  if (startStopButton.isPressed()) {
    buttonPressTime = millis();
  }

  if (startStopButton.isReleased()) {
    if (millis() - buttonPressTime >= HOLD_TIME) {
      displayConfigs();
    } else {
      playing = !playing;
      startTime = millis();
      statusDisplay();
      updateOutputs();
    }
  }

  if (playing && (millis() - startTime) >= max_time) {
    playing = false;
    statusDisplay();
    updateOutputs();
  }
}

void displayConfigs() {
  // Implement the displayConfigs function to show configurations on the screen
  Serial.println("Displaying configurations...");
  configsDisplay();
}
