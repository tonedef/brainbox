#ifndef PULSE_H_
#define PULSE_H_

//============ Includes ====================
#include "arduinoGlue.h"
#include "PreferencesManager.h"
#include "shared_variables.h"

//============ Added by Convertor ==========

//#include <driver/ledc.h> // moved to arduinoGlue.h

void setupPulse();
void pulseInit(int pin, int duty, int frequency, ledc_timer_t timer, ledc_channel_t channel, ledc_mode_t mode, ledc_timer_bit_t res);
void updateOutputs(bool forceInit = false);

#endif // PULSE_H_