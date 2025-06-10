#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#ifndef ServerManager_h
	//-- moved to arduinoGlue.h // #define ServerManager_h

//#include <WiFi.h>                                 		//-- moved to arduinoGlue.h
//#include <WebServer.h>                            		//-- moved to arduinoGlue.h
#include "PreferencesManager.h"

// Declare global variables
/*extern Preferences preferences;
extern int led_frequency;
extern int led_duty;
extern int audio_frequency;
extern int audio_duty;
extern int MAX_TIME;*/
//============ Includes ====================
#include "arduinoGlue.h"

//============ Added by Convertor ==========


// Function declarations
void handleRoot();
void handleUpdate();
void setupWebServer();
void handleClient();

#endif // ServerManager_h

#endif // SERVERMANAGER_H