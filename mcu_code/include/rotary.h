#ifndef DIMMER_H_
#define DIMMER_H_
#ifndef DIMMER_H

//============ Includes ====================
#include "arduinoGlue.h"

#define DIMMER_H

#include "shared_variables.h"
#include "ESPRotary.h"

//============ Added by Convertor ==========


extern ESPRotary r;
extern int led_duty;

void rotaryEncSetup();
void rotate(ESPRotary& r);

#endif // DIMMER_H
#endif // DIMMER_H_