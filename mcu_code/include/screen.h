#ifndef SCREEN_H_
#define SCREEN_H_

//============ Includes ====================
#include "arduinoGlue.h"
#include "buttons.h"
#include "shared_variables.h"
#include "PreferencesManager.h"

//============ Added by Convertor ==========


extern Adafruit_SSD1306 display;
extern unsigned long lastTime;

void showLogo();
void goingToSleepDisplay();
void setupScreen();
void loadingDisplay(int step);
void statusDisplay(bool forceShow = false);
void configsDisplay(ConfigsPage page);
#endif // SCREEN_H_