#ifndef TIMER_H_
#define TIMER_H_
#ifndef TIMER_H

//============ Includes ====================
#include "arduinoGlue.h"

#define TIMER_H

//#include <Arduino.h>                              		//-- moved to arduinoGlue.h
#include "PreferencesManager.h"

//============ Added by Convertor ==========


extern unsigned long startTime;
extern unsigned long lastTime;

void ellapsedTime(String &minSec);

#endif // TIMER_H
#endif // TIMER_H_