#include "shared_variables.h"

// Define default values
const int DEFAULT_AUDIO_DUTY = 328;
const int DEFAULT_AUDIO_FREQUENCY = 40;
const int DEFAULT_LED_DUTY = 50;
const int DEFAULT_LED_FREQUENCY = 40;
const char* DEFAULT_SSID = "40Hz_S2";
const char* DEFAULT_PASSWORD = "djMagicMike";
const char* DEFAULT_IP_ADDRESS = "192.168.4.10";
const int DEFAULT_MAX_TIME = (1 * 8 * 1000); // 10 minutes

// Initialize shared variables with default values
int audio_duty = DEFAULT_AUDIO_DUTY;
int audio_frequency = DEFAULT_AUDIO_FREQUENCY;
int led_duty = DEFAULT_LED_DUTY;
int led_frequency = DEFAULT_LED_FREQUENCY;
int max_time = DEFAULT_MAX_TIME;
bool playing = false;