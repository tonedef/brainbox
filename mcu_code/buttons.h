#ifndef BUTTONS_H
#define BUTTONS_H

#include <ezButton.h>

extern ezButton startStopButton;
extern bool playing;

void setupButtons();
void handleStartStop();
void displayConfigs();

#endif // BUTTONS_H
