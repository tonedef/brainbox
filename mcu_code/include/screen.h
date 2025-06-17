#ifndef SCREEN_H_
#define SCREEN_H_

#include "arduinoGlue.h"
#include "buttons.h"
#include "shared_variables.h"

/**
 * @brief Global instance of the Adafruit SSD1306 display object.
 */
extern Adafruit_SSD1306 display;

/**
 * @brief Displays the application logo and version on the screen.
 */
void showLogo();

/**
 * @brief Displays a countdown before the screen turns off due to inactivity.
 */
void goingToSleepDisplay();

/**
 * @brief Initializes the OLED screen.
 */
void setupScreen();

/**
 * @brief Displays a loading animation on the screen.
 * @param step The current step of the animation.
 */
void loadingDisplay(int step);

/**
 * @brief Displays the current status (e.g., Playing/Stopped, time).
 * @param forceShow If true, forces the display to update, bypassing sleep checks.
 */
void statusDisplay(bool forceShow = false);

/**
 * @brief Displays the specified configuration page.
 * @param page The ConfigsPage enum indicating which configuration screen to show.
 */
void configsDisplay(ConfigsPage page);
#endif // SCREEN_H_