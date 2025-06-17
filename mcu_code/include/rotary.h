/**
 * @file rotary.h
 * @brief Manages rotary encoder input and related actions.
 */
#ifndef ROTARY_H_
#define ROTARY_H_

#include "arduinoGlue.h"
#include "shared_variables.h"
#include "ESPRotary.h"

/**
 * @brief Global instance of the ESPRotary encoder object.
 */
extern ESPRotary r;

/**
 * @brief Initializes the rotary encoder settings.
 */
void rotaryEncSetup();

/**
 * @brief Callback function invoked when the rotary encoder's state changes.
 * @param r Reference to the ESPRotary object that triggered the callback.
 */
void rotate(ESPRotary& r);

#endif // ROTARY_H_