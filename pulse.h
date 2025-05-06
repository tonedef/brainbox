#ifndef PULSE_H
#define PULSE_H

#include "PreferencesManager.h"
#include "shared_variables.h"
#include <driver/ledc.h>


void pulseInit(int pin, int duty, int frequency, ledc_timer_t timer, ledc_channel_t channel, ledc_mode_t mode, ledc_timer_bit_t res);
void updateOutputs();

#endif // PULSE_H
