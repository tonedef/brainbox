#ifndef SHARED_VARIABLES_H_
#define SHARED_VARIABLES_H_

#include "arduinoGlue.h"

// Declare const values
extern const int DEFAULT_AUDIO_DUTY;                ///< Default duty cycle for audio output.
extern const uint32_t DEFAULT_AUDIO_FREQUENCY;      ///< Default frequency for audio output in Hz.
extern const int DEFAULT_LED_DUTY;                  ///< Default duty cycle for LED output.
extern const uint32_t DEFAULT_LED_FREQUENCY;        ///< Default frequency for LED output in Hz.
extern const char* DEFAULT_AP_SSID;                 ///< Default SSID when the device is in Access Point mode.
extern const char* DEFAULT_AP_PASSWORD;             ///< Default password for the Access Point mode.
extern const char* DEFAULT_STA_SSID;                ///< Default SSID for Station mode if not found in preferences.
extern const char* DEFAULT_STA_PASSWORD;            ///< Default password for Station mode if not found in preferences.
extern String LOCAL_STA_SSID;                       ///< Current SSID for local WiFi connection (Station mode), loaded from Preferences.
extern String LOCAL_STA_PASSWORD;                   ///< Current Password for local WiFi connection (Station mode), loaded from Preferences.
extern const int DEFAULT_MAX_TIME;                  ///< Default maximum session time in milliseconds.
extern const float VERSION;                         ///< Firmware version number.

/**
 * @brief Structure to hold bitmap data and its dimensions.
 */
struct BitmapAsset {
    const uint8_t* data; ///< Pointer to the PROGMEM bitmap data.
    int16_t width;       ///< Width of the bitmap in pixels.
    int16_t height;      ///< Height of the bitmap in pixels.
};

/// @brief Collection of named bitmap assets.
extern const struct NamedBitmaps {
    BitmapAsset logo; ///< Logo bitmap asset.
    BitmapAsset knob; ///< Knob icon bitmap asset.
} bitmaps;

// Declare shared variables
extern char wifi_ap_ip[16];                         ///< Buffer to store the IP address when in AP mode.
extern int audio_duty;                              ///< Current duty cycle for audio output.
extern uint32_t audio_frequency;                    ///< Current frequency for audio output.
extern int led_duty;                                ///< Current duty cycle for LED output.
extern uint32_t led_frequency;                      ///< Current frequency for LED output.
extern int max_time;                                ///< Current maximum session time in milliseconds.
extern bool playing;                                ///< Flag indicating if playback is active.
extern bool display_sleep;                          ///< Flag indicating if display auto-sleep is enabled.
extern bool show_display;                           ///< Flag indicating if the display should be active (not asleep).

#endif // SHARED_VARIABLES_H_