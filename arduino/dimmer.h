#ifndef DIMMER_H
#define DIMMER_H

#include "shared_variables.h"
#include "ESPRotary.h"

extern ESPRotary r;
extern int led_duty;

void dimmerEncSetup();
void rotate(ESPRotary& r);

#endif // DIMMER_H
