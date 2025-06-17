#include "rotary.h"
#include "pulse.h"
#include "buttons.h"
#include "shared_variables.h"
#include <Arduino.h>

#define ROTARY_PIN1 11
#define ROTARY_PIN2 9
#define CLICKS_PER_STEP 4
#define MAX_LED_DUTY 7935

ESPRotary r;

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

void updateLedDuty () {
  Serial.println(r.directionToString(r.getDirection()));
  bool cw_dir = r.getDirection() == rotary_direction::right;
  int change_amount = 256; // This is the increment value set in setup

  updateValue(MAX_LED_DUTY, 0, 256, led_duty);

  Serial.print("LED Duty: ");
  Serial.println(led_duty);
  updateOutputs();
}

/* Setup rotary encoder */
void rotaryEncSetup() {
  delay(50);
  // Serial.println("\n\nSimple Counter"); // Kept the second, more specific, r.begin()
  Serial.println("\n\nRotary Encoder Setup");
  // Set rotary range to match led_duty range (0-8191 for 13-bit LEDC)
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, 0, MAX_LED_DUTY, led_duty, 1);
  r.setChangedHandler(rotate);
  r.setIncrement(256); // This will be the "amount" of change per step
}

void rotate(ESPRotary& r) {
  if (playing && displayState == NORMAL) {
    updateLedDuty();
  } else if (displayState == CONFIGS) {

  }
}
