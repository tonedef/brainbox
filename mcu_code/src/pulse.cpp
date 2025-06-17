#include "PreferencesManager.h"
#include "shared_variables.h"
#include "pulse.h"
#include "buttons.h"
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

void setupPulse() {
  pinMode(AUDIO_SD_IO, OUTPUT);
  digitalWrite(AUDIO_SD_IO, LOW);
}


/* Audio and LED Pulse initialization */
void pulseInit(int pin, int duty, int frequency, ledc_timer_t timer, ledc_channel_t channel, ledc_mode_t mode, ledc_timer_bit_t res) {
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

/* Toggle the outputs based on the playing state */
void updateOutputs(bool forceInit) {
 /*Serial.println("Playing: " + String(playing));
  Serial.println("Audio Duty: " + String(audio_duty));
  Serial.println("LED Duty: " + String(led_duty));*/
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
    digitalWrite(AUDIO_SD_IO, LOW);

    if (audio_initialized) {
      ledc_stop(AUDIO_MODE, AUDIO_CHANNEL, 0);
      pinMode(AUDIO_OUTPUT_IO, INPUT);
      digitalWrite(AUDIO_OUTPUT_IO, LOW);
      audio_initialized = false;
    }
    if (led_initialized) {
      ledc_stop(LED_MODE, LED_CHANNEL, 0);
      pinMode(LED_OUTPUT_IO, INPUT);
      digitalWrite(LED_OUTPUT_IO, LOW);
      led_initialized = false;
    }/*
    Serial.print("led_duty: ");
    Serial.println(led_duty);
    Serial.print("saved led_duty: ");
    Serial.println(preferences.getInt("led_duty", LED_DUTY));*/

    // Save the LED duty cycle if it's changed. Do on stop to save on writes
    if (led_duty != preferences.getInt("led_duty", LED_DUTY)) {
      Serial.println("Saving!!");

      preferences.putInt("led_duty", led_duty);
    }
  }
}
