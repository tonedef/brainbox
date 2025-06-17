/**
 * @file timer.h
 * @brief Manages time-related variables and functions for playback duration and display.
 */
#ifndef TIMER_H_
#define TIMER_H_

#include "arduinoGlue.h"

/**
 * @brief Timestamp (millis) when playback was last started.
 * Defined in mcu_code.cpp.
 */
extern unsigned long startTime;

/**
 * @brief Timestamp (millis) of the last display update during playback.
 * Defined in mcu_code.cpp.
 */
extern unsigned long lastTime;

/**
 * @brief Formats the current elapsed or remaining time into an "MM:SS" string.
 * @param minSec Reference to a String object where the formatted time will be stored.
 */
void ellapsedTime(String &minSec);

#endif // TIMER_H_