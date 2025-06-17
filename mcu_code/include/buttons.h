#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "arduinoGlue.h"
/**
 * @brief Defines the various states the main display or application can be in.
 */
enum DisplayState {
  NORMAL,           ///< Normal operational mode, showing playback status.
  CONFIGS,          ///< Configuration mode, allowing settings adjustments.
  ENTERING_NORMAL,  ///< Intermediate state when transitioning from CONFIGS to NORMAL.
  ENTERING_CONFIGS  ///< Intermediate state when transitioning from NORMAL to CONFIGS.
};

/**
 * @brief Defines the different pages available within the configuration menu.
 */
enum ConfigsPage {
  HOME,       ///< Main configuration home page.
  WIFI_SETUP, ///< Page for setting up WiFi in AP mode.
  APP_PAGE,   ///< Page for accessing the web application (via QR code).
  TIME_ADJ    ///< TODO: Page for adjusting the maximum session time.
};

extern ezButton startStopButton;
extern ezButton onOffButton;
extern ezButton configButton;
extern ConfigsPage configPage;      ///< Current configuration page being displayed.
extern DisplayState displayState;   ///< Current state of the display/application.
extern bool playing;

/**
 * @brief Initializes the button objects and their settings.
 * Sets up pin modes, debounce times, and initial states.
 */
void setupButtons();

/**
 * @brief Handles logic for the On/Off button presses.
 * Typically toggles display sleep or power state.
 */
void handleOnOff();

/**
 * @brief Handles logic for the Start/Stop button presses.
 * Toggles the main playback state of the device.
 */
void handleStartStop();

/**
 * @brief Handles logic for the Config button presses.
 * Manages navigation into and within the configuration menu.
 */
void handleConfigs();

#endif // BUTTONS_H_