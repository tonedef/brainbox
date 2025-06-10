#ifndef BUTTONS_H_
#define BUTTONS_H_
#ifndef BUTTONS_H

//============ Includes ====================
#include "arduinoGlue.h"

//============ Added by Convertor ==========


#define BUTTONS_H

//#include <ezButton.h>                             		//-- moved to arduinoGlue.h

extern ezButton startStopButton;
extern ezButton onOffButton;
extern bool playing;

void setupButtons();
void handleOnOff();
void handleStartStop();
void handleConfigs();

#endif // BUTTONS_H
#endif // BUTTONS_H_