#ifndef SCREEN_H_
#define SCREEN_H_
#ifndef SCREEN_H

//============ Includes ====================
#include "arduinoGlue.h"

#define SCREEN_H

#include "shared_variables.h"
#include "PreferencesManager.h"

//============ Added by Convertor ==========


extern Adafruit_SSD1306 display;
extern unsigned long lastTime;

void setupScreen();
void statusDisplay();
void configsDisplay();
void ellapsedTime(String &minSec);
void configsDisplay();
#endif // SCREEN_H
#endif // SCREEN_H_