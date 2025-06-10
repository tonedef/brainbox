#include "shared_variables.h"

// Define default values
const int DEFAULT_AUDIO_DUTY = 328;
const int DEFAULT_AUDIO_FREQUENCY = 40;
const int DEFAULT_LED_DUTY = 50;
const int DEFAULT_LED_FREQUENCY = 40;
const char* DEFAULT_AP_SSID = "wavebox";
const char* DEFAULT_AP_PASSWORD = "djMagicMike";
const char* DEFAULT_STA_SSID = ""; // Default if not found in preferences
const char* DEFAULT_STA_PASSWORD = ""; // Default if not found in preferences
const char* DEFAULT_IP_ADDRESS = "192.168.4.10";
const int DEFAULT_MAX_TIME = (10 * 60 * 1000); // 10 minutes

// Initialize shared variables with default values
char wifi_ap_ip[16]; // Buffer to hold IP address string
int audio_duty = DEFAULT_AUDIO_DUTY;
int audio_frequency = DEFAULT_AUDIO_FREQUENCY;
int led_duty = DEFAULT_LED_DUTY;
int led_frequency = DEFAULT_LED_FREQUENCY;
int max_time = DEFAULT_MAX_TIME;
bool playing = false;
bool display_sleep = false;
bool show_display = true;
String LOCAL_STA_SSID;      // Will be loaded from Preferences or use DEFAULT_PREF_STA_SSID
String LOCAL_STA_PASSWORD;  // Will be loaded from Preferences or use DEFAULT_PREF_STA_PASSWORD