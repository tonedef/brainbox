#include "shared_variables.h"
#include "buttons.h"
#include "screen.h"
#include "pulse.h"
#include "timer.h"

#define START_STOP_PIN 33
#define DEBOUNCE_TIME 50
#define HOLD_TIME 1000 // 2 seconds

ezButton startStopButton(START_STOP_PIN);
unsigned long buttonPressTime = 0;
bool buttonPressed = false;
typedef enum {
    IN_CONFIGS,
    LEAVING_CONFIGS,
    WAITING_NORMAL,
    NORMAL
} DisplayState;

DisplayState state;
/* Initialize button configurations */
void setupButtons() {
  state = NORMAL;
  pinMode(START_STOP_PIN, INPUT_PULLUP);
  startStopButton.setDebounceTime(DEBOUNCE_TIME);
}

void handleStartStop() {
  bool isReleased = startStopButton.isReleased();
  bool isPressed = startStopButton.isPressed();

  if (isPressed) {
    buttonPressTime = millis();
    buttonPressed = true;
  }

  // Handle hold states
  if (buttonPressed && state == NORMAL && millis() - buttonPressTime >= HOLD_TIME) {
    if (playing) {
      playing = false;  // Ensure playing stops when entering configs
      statusDisplay();  // Update display to "stopped"
    }
    configsDisplay();
    buttonPressed = false;
    state = IN_CONFIGS;
  } else if (buttonPressed && state == WAITING_NORMAL && millis() - buttonPressTime >= HOLD_TIME) {
    statusDisplay();
    buttonPressed = false;
    state = LEAVING_CONFIGS;
  }

  // Handle configs and clearing button press
  if (isReleased) {
    buttonPressed = false; // Reset button state

    if (state == IN_CONFIGS) {
      state = WAITING_NORMAL;
    } else if (state == LEAVING_CONFIGS) {
      state = NORMAL;  // Ensure it resets without toggling `playing`
    }
  }

  // Handle start/stop 
  if (state == NORMAL && isReleased || (playing && (millis() - startTime) >= max_time)) {
      playing = !playing;
      startTime = millis();
      statusDisplay();
      updateOutputs();
  }
}

