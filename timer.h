#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>
#include "shared_variables.h"
#include "PreferencesManager.h"

extern unsigned long startTime;
extern unsigned long lastTime;

void ellapsedTime(String &minSec);

#endif // TIMER_H
