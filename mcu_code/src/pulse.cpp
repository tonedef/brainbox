#include "PreferencesManager.h"
#include "shared_variables.h"
#include "pulse.h"
#include <Arduino.h>

#define AUDIO_TIMER LEDC_TIMER_0
#define AUDIO_MODE LEDC_LOW_SPEED_MODE
#define AUDIO_OUTPUT_IO 18
#define AUDIO_SD_IO 33
#define AUDIO_CHANNEL LEDC_CHANNEL_0
#define AUDIO_DUTY_RES LEDC_TIMER_13_BIT

#define LED_TIMER LEDC_TIMER_1
#define LED_MODE LEDC_LOW_SPEED_MODE
#define LED_OUTPUT_IO 12
#define LED_CHANNEL LEDC_CHANNEL_1
#define LED_DUTY_RES LEDC_TIMER_13_BIT

bool audio_initialized = false;
bool led_initialized = false;

/**
 * @brief Sets up the initial pin mode for the audio shutdown control pin.
 * The audio output is initially disabled (LOW).
 */
void setupPulse() {
  pinMode(AUDIO_SD_IO, OUTPUT);
  digitalWrite(AUDIO_SD_IO, LOW);
}


/**
 * @brief Configures and initializes an LEDC timer and channel for PWM output.
 * @param pin GPIO pin number for the PWM output.
 * @param duty Initial duty cycle value (0-8191 for 13-bit resolution).
 * @param frequency PWM frequency in Hz.
 * @param timer LEDC timer to use (e.g., LEDC_TIMER_0).
 * @param channel LEDC channel to use (e.g., LEDC_CHANNEL_0).
 * @param res Duty cycle resolution (e.g., LEDC_TIMER_13_BIT).
 */
void pulseInit(int pin, int duty, uint32_t frequency, ledc_timer_t timer, ledc_channel_t channel, ledc_mode_t mode, ledc_timer_bit_t res) {
  ledc_timer_config_t ledc_timer = {
    .speed_mode = mode,
    .duty_resolution = res,
    .timer_num = timer,
    .freq_hz = frequency,
    .clk_cfg = LEDC_AUTO_CLK
  };
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  ledc_channel_config_t ledc_channel = {
    .gpio_num = pin,
    .speed_mode = mode,
    .channel = channel,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = timer,
    .duty = 0,
    .hpoint = 0
  };
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

/**
 * @brief Updates the audio and LED outputs based on the current playback state.
 * If playing, the outputs are enabled and configured according to current settings.
 * If not playing, outputs are disabled. Saves LED duty cycle to preferences when stopping playback.
 */
void updateOutputs(bool forceInit) {
  if (playing) {
    if (!audio_initialized || forceInit) {
      pulseInit(AUDIO_OUTPUT_IO, audio_duty, audio_frequency, AUDIO_TIMER, AUDIO_CHANNEL, AUDIO_MODE, AUDIO_DUTY_RES);
      audio_initialized = true;
    }
    if (!led_initialized || forceInit) {
      pulseInit(LED_OUTPUT_IO, led_duty, led_frequency, LED_TIMER, LED_CHANNEL, LED_MODE, LED_DUTY_RES);
      led_initialized = true;
    }

    // Set duty and update duty for AUDIO
    ESP_ERROR_CHECK(ledc_set_duty(AUDIO_MODE, AUDIO_CHANNEL, audio_duty));
    ESP_ERROR_CHECK(ledc_update_duty(AUDIO_MODE, AUDIO_CHANNEL));

    // Set duty and update duty for LED
    ESP_ERROR_CHECK(ledc_set_duty(LED_MODE, LED_CHANNEL, led_duty));
    ESP_ERROR_CHECK(ledc_update_duty(LED_MODE, LED_CHANNEL));
    digitalWrite(AUDIO_SD_IO, HIGH);
  } else {
    // Deactivate outputs.
    digitalWrite(AUDIO_SD_IO, LOW);

    if (audio_initialized) {
      ledc_stop(AUDIO_MODE, AUDIO_CHANNEL, 0);
      // Setting pins to INPUT and LOW when deactivating PWM.
      // This is done to avoid any residual output or power draw.
      pinMode(AUDIO_OUTPUT_IO, INPUT);
      digitalWrite(AUDIO_OUTPUT_IO, LOW);
      audio_initialized = false;
    }
    if (led_initialized) {
      ledc_stop(LED_MODE, LED_CHANNEL, 0);
      pinMode(LED_OUTPUT_IO, INPUT);
      digitalWrite(LED_OUTPUT_IO, LOW);
      led_initialized = false;
    }

    // Save the LED duty cycle if it's changed. Do on stop to save on writes
    if (led_duty != preferences.getInt("led_duty", DEFAULT_LED_DUTY)) {
      Serial.println("Saving!!");

      preferences.putInt("led_duty", led_duty);
    }
  }
}
