#include "shared_variables.h"
#include "buttons.h"
#include "screen.h"
#include "pulse.h"
#include "timer.h"

#define START_STOP_PIN 35
#define ON_OFF_PIN 16
#define CONFIG_PIN 16
#define DEBOUNCE_TIME 50
#define HOLD_TIME 1000 // 2 seconds

ezButton startStopButton(START_STOP_PIN);
ezButton onOffButton(ON_OFF_PIN);

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

  pinMode(ON_OFF_PIN, INPUT_PULLUP);
  onOffButton.setDebounceTime(DEBOUNCE_TIME);
}

void handleOnOff() {
  if (onOffButton.isPressed()) {
    show_display = !show_display;
    if (!show_display) {
      playing = false;
      startTime = millis();
      display.clearDisplay();
      display.display();
      updateOutputs();
    } else {
      statusDisplay();
    }
  }
}

void handleStartStop() {
  bool isPressed = startStopButton.isPressed();
  playing = !playing;
  startTime = millis();
  statusDisplay();
  updateOutputs();
}

void handleConfigs() {

}
