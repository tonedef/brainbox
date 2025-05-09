#ifndef ServerManager_h
#define ServerManager_h

#include <WiFi.h>
#include <WebServer.h>
#include "PreferencesManager.h"

// Declare global variables
/*extern Preferences preferences;
extern int led_frequency;
extern int led_duty;
extern int audio_frequency;
extern int audio_duty;
extern int MAX_TIME;*/

// Function declarations
void handleRoot();
void handleUpdate();
void setupWebServer();
void handleClient();

#endif // ServerManager_h
