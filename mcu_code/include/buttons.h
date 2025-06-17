#ifndef BUTTONS_H_
#define BUTTONS_H_

//============ Includes ====================
#include "arduinoGlue.h"

//============ Added by Convertor ==========

// Enum for different display states
enum DisplayState {
  NORMAL,
  CONFIGS,
  ENTERING_NORMAL,
  ENTERING_CONFIGS
};

// Enum for different config pages
enum ConfigsPage {
  HOME,
  WIFI_SETUP,
  APP_PAGE,
  TIME_ADJ
};

extern ezButton startStopButton;
extern ezButton onOffButton;
extern ezButton configButton;
extern enum ConfigsPage configPage;
extern enum DisplayState displayState;
extern bool playing;

void setupButtons();
void handleOnOff();
void handleStartStop();
void handleConfigs();

#endif // BUTTONS_H_