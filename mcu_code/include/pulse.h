/**
 * @file pulse.h
 * @brief Manages PWM pulse generation for audio and LED outputs.
 */
#ifndef PULSE_H_
#define PULSE_H_

#include "arduinoGlue.h"
#include "shared_variables.h"

/**
 * @brief Sets up initial configuration for pulse-related pins (e.g., audio shutdown).
 */
void setupPulse();

/**
 * @brief Initializes an LEDC timer and channel for PWM output.
 * @param pin GPIO pin for PWM output.
 * @param duty Initial duty cycle.
 * @param frequency PWM frequency in Hz.
 * @param timer LEDC timer to use.
 * @param channel LEDC channel to use.
 * @param mode LEDC speed mode.
 * @param res LEDC duty resolution.
 */
void pulseInit(int pin, int duty, uint32_t frequency, ledc_timer_t timer, ledc_channel_t channel, ledc_mode_t mode, ledc_timer_bit_t res);

/**
 * @brief Updates the state of audio and LED outputs based on global settings.
 * @param forceInit If true, forces re-initialization of PWM channels.
 */
void updateOutputs(bool forceInit = false);

#endif // PULSE_H_