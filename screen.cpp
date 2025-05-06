#include "screen.h"
#include "timer.h"
#include "buttons.h"
#include "shared_variables.h"
#include "PreferencesManager.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_CLK 7//S2 1//C3
#define OLED_MOSI 11//S2 4//C3
#define OLED_RESET 5//S2 2//C3
#define OLED_DC 18//S2 7//C3
#define OLED_CS 12//S2 5//C3

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Initialize the screen */
void setupScreen() {
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
}

void statusDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.println(playing ? "Playing" : "Stopped");
  display.setTextSize(4);
  display.setCursor(5, 28);
  String minSec;
  ellapsedTime(minSec);
  display.println(minSec);
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

