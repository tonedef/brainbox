#include "shared_variables.h"
#include "buttons.h"
#include "screen.h"
#include "pulse.h"
#include "timer.h"

#define START_STOP_PIN 35
#define ON_OFF_PIN 16
#define CONFIG_PIN 7
#define DEBOUNCE_TIME 50
#define HOLD_TIME 2000 // 2 seconds

ezButton startStopButton(START_STOP_PIN);
ezButton onOffButton(ON_OFF_PIN);
ezButton configButton(CONFIG_PIN);
DisplayState displayState;

ConfigsPage configPage;

static unsigned long buttonDownTime = 0; // Time when onOffButton was pressed
static bool longPress = false;   // Flag to indicate a potential long press on onOffButton

/* Initialize button configurations */
void setupButtons() {
  displayState = NORMAL;

  pinMode(START_STOP_PIN, INPUT_PULLUP);
  startStopButton.setDebounceTime(DEBOUNCE_TIME);

  pinMode(ON_OFF_PIN, INPUT_PULLUP);
  onOffButton.setDebounceTime(DEBOUNCE_TIME);

  pinMode(CONFIG_PIN, INPUT_PULLUP);
  configButton.setDebounceTime(DEBOUNCE_TIME);
}

void handleOnOff() {
  if (onOffButton.isReleased()) {
    displayState = NORMAL;
    playing = false;
    startTime = 0;
    display.clearDisplay();
    display.display();
    show_display = !show_display;
    if (!show_display) {
      updateOutputs();
    } else {
      statusDisplay(true);
    }
  }
}

void handleStartStop() {
  if (startStopButton.isReleased()) {
    displayState = NORMAL;
    show_display = true;
    playing = !playing;
    
    if (playing) {
      startTime = millis();
    } else {
      startTime = 0;
    }

    updateOutputs();
    statusDisplay(true);
  }
}

void handleConfigs() {
  // This function handles the LONG PRESS of the OnOff/Config button
  // to enter a configuration mode. It also handles exiting config mode.

  if (configButton.isPressed()) { // Button is currently held down
    if (!longPress) { // Button just went down
      longPress = true;
      buttonDownTime = millis();
    }
  }

  if (longPress && millis() - buttonDownTime >= HOLD_TIME) {
    show_display = true;
    if (playing) {
      playing = false;
      updateOutputs();
    }
    if (displayState == CONFIGS) {
      statusDisplay(true);
      displayState = ENTERING_NORMAL;
    } else if (displayState == NORMAL) {
      configPage = HOME;
      configsDisplay(configPage);
      displayState = ENTERING_CONFIGS;
    }

    longPress = false;
  }
  
  if (configButton.isReleased() && (displayState == ENTERING_CONFIGS || displayState == ENTERING_NORMAL)) {
    if (displayState == ENTERING_CONFIGS) {
      displayState = CONFIGS;

    } else if (displayState == ENTERING_NORMAL) {
      displayState = NORMAL;
    }
  } else if (configButton.isReleased() && displayState == CONFIGS) {
    Serial.print("configPage");
    Serial.println(configPage);
    switch (configPage) {
      case HOME:
        configPage = WiFi.status() != WL_CONNECTED ? WIFI_SETUP : APP_PAGE;
        break;
      case WIFI_SETUP:
        configPage = APP_PAGE;
        break;
      case APP_PAGE:
        // TODO: code up encoder changes to update time (change to configPage = TIME_ADJ)
        configPage = HOME;
        break;
      case TIME_ADJ:
        // TODO: code up encoder changes to update time
        configPage = HOME;
        break;
    }
    configsDisplay(configPage);
  }
  
  if (configButton.isReleased()) {
    longPress = false;
  }
}
