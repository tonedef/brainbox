#include <Wire.h>
#include "screen.h"
#include "timer.h"
#include "buttons.h"
#include "shared_variables.h"
#include "PreferencesManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1     // Reset pin (-1 if sharing Arduino reset)
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Initialize the screen */
void setupScreen() {
  Wire.begin(33, 35);  // Use default ESP32 I2C pins
  Wire.setClock(400000);

  // Initialize OLED
  Serial.println("Attempting to initialize OLED...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("ERROR: SSD1306 allocation failed! Check wiring and address.");
    for (;;)
      ;  // Stop execution
  }
  
  display.clearDisplay();
}

void statusDisplay() {
  Serial.println("Called!");
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.println(playing ? "Playing" : "Stopped");
  display.setTextSize(4);
  display.setCursor(5, 28);
  String minSec;
  ellapsedTime(minSec);
  Serial.println("Called! x2");
  display.display();
  lastTime = millis();
}

void configsDisplay() {
  String ssid = preferences.getString("ssid", DEFAULT_SSID);
  String password = preferences.getString("password", DEFAULT_PASSWORD);
  String ipAddress = preferences.getString("ipAddress", DEFAULT_IP_ADDRESS);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Configs");
  display.setTextSize(1);
  display.setCursor(0, 16);
  display.print("SSID: ");
  display.println(ssid);
  display.setCursor(0, 28);
  display.print("PWD: ");
  display.println(password);
  display.setCursor(0, 40);
  display.print("IP: ");
  display.println(ipAddress);
  display.println();
  display.display();
  lastTime = millis();
}

