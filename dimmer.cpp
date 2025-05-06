#include "dimmer.h"
#include "pulse.h"
#include "shared_variables.h"
#include <Arduino.h>

#define ROTARY_PIN1 9//S2 0//C3
#define ROTARY_PIN2 37//S2 RX//C3
#define CLICKS_PER_STEP 8

ESPRotary r;

/* Setup dimmer encoder */
void dimmerEncSetup() {
  delay(50);
  Serial.println("\n\nSimple Counter");
  r.begin(ROTARY_PIN1, ROTARY_PIN2, CLICKS_PER_STEP, 0, 4096, led_duty, 1);
  r.setChangedHandler(rotate);
  r.setIncrement(256);
}

void rotate(ESPRotary& r) {
  if (playing) {
    led_duty = r.getPosition();
    updateOutputs();
  } else {
    r.resetPosition(led_duty);
  }
}
