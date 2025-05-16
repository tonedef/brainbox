#ifndef SHARED_VARIABLES_H
#define SHARED_VARIABLES_H

// Declare default values
extern const int DEFAULT_AUDIO_DUTY;
extern const int DEFAULT_AUDIO_FREQUENCY;
extern const int DEFAULT_LED_DUTY;
extern const int DEFAULT_LED_FREQUENCY;
extern const char* DEFAULT_SSID;
extern const char* DEFAULT_PASSWORD;
extern const char* DEFAULT_IP_ADDRESS;
extern const int DEFAULT_MAX_TIME;

// Declare shared variables
extern int audio_duty;
extern int audio_frequency;
extern int led_duty;
extern int led_frequency;
extern int max_time;
extern bool playing;

#endif // SHARED_VARIABLES_H