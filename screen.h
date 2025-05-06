#ifndef SCREEN_H
#define SCREEN_H

#include "shared_variables.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "PreferencesManager.h"

extern Adafruit_SSD1306 display;
extern unsigned long lastTime;

void setupScreen();
void statusDisplay();
void configsDisplay();
void ellapsedTime(String &minSec);

#endif // SCREEN_H
