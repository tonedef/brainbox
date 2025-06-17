#include "rotary.h"
#include "pulse.h"
#include "buttons.h"
#include "shared_variables.h"
#include <Arduino.h>

#define ROTARY_PIN1 11
#define ROTARY_PIN2 9
#define CLICKS_PER_STEP 4
#define MAX_DUTY 8191     // Maximum duty cycle for 13-bit resolution
#define MAX_LED_DUTY 7167 // Max duty cycle for LED to maintain strobe effect


/**
 * @brief Global instance of the ESPRotary encoder object.
 */
ESPRotary r;

/**
 * @brief Updates a generic integer value based on rotary encoder direction.
 * Clamps the value within the specified min and max limits.
 * @param max_value The maximum permissible value.
 * @param min_value The minimum permissible value.
 * @param increment The amount to change the value by per encoder step.
 * @param value Reference to the integer variable to be updated.
 */
void updateValue (int max_value, int min_value, int increment, int& value) {
  if (r.getDirection() == rotary_direction::right) {
    value += increment;
  } else {
    value -= increment;
  }

  if (value < min_value) {
    value = min_value;
    } else if (value > max_value) {
    value = max_value;
  }
}

/**
 * @brief Updates the LED duty cycle based on rotary encoder input.
 * Uses a dynamic increment value that increases with the current duty cycle.
 */
void updateLedDuty () {

  // Constants for dynamic increment calculation
  const float MIN_INC = 1.0f;
  const float MAX_INC_TARGET = 256.0f;

  // K_FACTOR determines how quickly the increment approaches MAX_INC_TARGET.
  // Chosen so increment is substantial (~240) when led_duty is around 4096 (half of 13-bit range).
  const float K_FACTOR = 0.000661f; 

  float calculated_increment = MIN_INC + (MAX_INC_TARGET - MIN_INC) * (1.0f - expf(-K_FACTOR * (float)led_duty));
  int dynamic_increment = roundf(calculated_increment);

  // Ensure increment is within practical bounds
  if (dynamic_increment < 1) {
    dynamic_increment = 1;
  }
  if (dynamic_increment > (int)MAX_INC_TARGET) { // Cap at the defined maximum target
    dynamic_increment = (int)MAX_INC_TARGET;
  }
  
  updateValue(MAX_LED_DUTY, 0, dynamic_increment, led_duty);
  updateOutputs();
}

/* Setup rotary encoder */
/**
 * @brief Initializes the rotary encoder.
 * Sets up pins, range, initial value, and callback handler. This is intentionally generic
 * as we will likely reuse the encoder at a later date to modify other variables 
 */
void rotaryEncSetup() {
  delay(50);
  // Initialize ESPRotary: pins, clicks per step, min value, max value, initial value, step increment.
  // The step increment here (1) is a base; actual increment for led_duty is dynamic.
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, 0, MAX_DUTY, 0, 1);
  r.setChangedHandler(rotate);
}

/**
 * @brief Callback function executed when the rotary encoder state changes.
 * @param r Reference to the ESPRotary object that triggered the event.
 */
void rotate(ESPRotary& r) {
  if (playing && displayState == NORMAL) {
    updateLedDuty();
  } else if (displayState == CONFIGS) {
    // TODO: Implement rotary encoder handling for configuration mode
  }
}
