#include "shared_variables.h"
#include "buttons.h"
#include "screen.h"
#include "pulse.h"
#include "timer.h"

#define START_STOP_PIN 18 // S2 6 // C3
#define DEBOUNCE_TIME 50
#define HOLD_TIME 2000 // 5 seconds

ezButton startStopButton(START_STOP_PIN);
unsigned long buttonPressTime = 0;
int displayState = 0;
bool isPressed = false;
/* Initialize button configurations */
void setupButtons() {
  pinMode(START_STOP_PIN, INPUT_PULLUP);
  startStopButton.setDebounceTime(DEBOUNCE_TIME);
}

void handleStartStop() {
  bool isHeld = isPressed && millis() - buttonPressTime >= HOLD_TIME;
  if (startStopButton.isPressed()) {
    buttonPressTime = millis();
    isPressed = true;
  }

  if (isHeld && displayState == 0) {
    displayConfigs();
    playing = false;
    updateOutputs();
    displayState = 1;
  } else if (isHeld && displayState == 2) {
    statusDisplay();
    displayState = 3;
  }

  if (startStopButton.isReleased()) {
    isPressed = false;
    
    if (displayState == 0) {
      playing = !playing;
      startTime = millis();
      Serial.println("displayState: 0");
      statusDisplay();
      updateOutputs();
      displayState = 0;
    }
        
    if (displayState == 1) {
      displayState = 2;
    } else if (displayState == 3) {
      displayState = 0;
    } 
  }

  if (playing && (millis() - startTime) >= max_time) {
    Serial.println("playing...");
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
