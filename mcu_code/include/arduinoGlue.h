#ifndef ARDUINOGLUE_H
#define ARDUINOGLUE_H


//============ Includes ====================
#include <Arduino.h>
#include <ezButton.h>
#include <Preferences.h>
#include <driver/ledc.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <QRCodeGFX.h>

//============ Defines & Macros====================
//#define PreferencesManager_h 
#define LED_DUTY 256 // Duty cycle ~ 10%
//#define ServerManager_h 

//============ Extern Variables ============
extern void            handleRoot();                        		//-- from ServerManager
extern void            setupWebServer();                    		//-- from ServerManager
extern void            handleClient();                      		//-- from ServerManager
extern void            handleConfigs();                    		//-- from buttons
extern void            setupButtons();                      		//-- from buttons
extern void            dimmerEncSetup();                    		//-- from dimmer
extern Preferences     preferences;                       		  //-- from mcu_code
extern void            setupPulse();                        		//-- from pulse
extern void            updateOutputs(bool forceInit);  	//-- from pulse
extern void            configsDisplay();                    		//-- from screen
extern void            setupScreen();                       		//-- from screen
extern void            ellapsedTime();                      		//-- from timer

#endif // ARDUINOGLUE_H
