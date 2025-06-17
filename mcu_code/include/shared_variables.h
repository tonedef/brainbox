#ifndef SHARED_VARIABLES_H_
#define SHARED_VARIABLES_H_
#ifndef SHARED_VARIABLES_H

//============ Includes ====================
#include "arduinoGlue.h"

//============ Added by Convertor ==========


#define SHARED_VARIABLES_H

// Declare const values
extern const int DEFAULT_AUDIO_DUTY;
extern const int DEFAULT_AUDIO_FREQUENCY;
extern const int DEFAULT_LED_DUTY;
extern const int DEFAULT_LED_FREQUENCY;
extern const char* DEFAULT_AP_SSID;       // SSID for Access Point mode
extern const char* DEFAULT_AP_PASSWORD;   // Password for Access Point mode
extern const char* DEFAULT_STA_SSID;     // Default STA SSID if not in Preferences
extern const char* DEFAULT_STA_PASSWORD; // Default STA Password if not in Preferences
extern String LOCAL_STA_SSID;        // Current SSID for local WiFi connection (Station mode)
extern String LOCAL_STA_PASSWORD;    // Current Password for local WiFi connection (Station mode)
extern const char* DEFAULT_IP_ADDRESS;
extern const int DEFAULT_MAX_TIME;
extern const float VERSION;
extern const uint8_t PROGMEM LOGO[];
extern const uint8_t PROGMEM KNOB[];

// Declare shared variables
extern char wifi_ap_ip[16];
extern int audio_duty;
extern int audio_frequency;
extern int led_duty;
extern int led_frequency;
extern int max_time;
extern bool playing;
extern bool display_sleep;
extern bool show_display;

#endif // SHARED_VARIABLES_H
#endif // SHARED_VARIABLES_H_