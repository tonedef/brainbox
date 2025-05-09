#include <Wire.h>
#include "screen.h"
#include "timer.h"
#include "buttons.h"
#include "shared_variables.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* Initialize the screen */
void setupScreen() {
  Wire.begin(16, 18);  // Use default ESP32 I2C pins
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
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

void configsDisplay(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.println("Configs");
  display.display();
}
