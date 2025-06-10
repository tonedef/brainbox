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

QRCodeGFX qrcode(display);

/* Initialize the screen */
void setupScreen() {
  Wire.begin(3, 5);  // Use default ESP32 I2C pins
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }
  display.clearDisplay();
}

// Display the current status (Playing/Stopped) and elapsed time
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

// Display the QR code and SSID for WiFi configuration when in AP mode
void apConnectDisplay() {
  char wifiConfig[100];
  String ssid = DEFAULT_AP_SSID;
  snprintf(wifiConfig, sizeof(wifiConfig), "WIFI:S:%s;T:WPA;P:%s;H:false;;", 
            ssid, DEFAULT_AP_PASSWORD);
 
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  qrcode.setScale(2);

  qrcode.draw(wifiConfig, 0, 0);
  display.setCursor(72, 0);
  display.println("WiFi SSID:");
  display.setCursor(72, 16);
  
  // Truncate SSID if longer than 9 characters
  if (ssid.length() > 9) {
    ssid = ssid.substring(0, 6) + "...";
  }

  display.println(ssid);

  display.display();
}

// Display the QR code and IP address for the root URL when in AP mode
void apRootUrlDisplay() {
  char ip_copy[16];
  strncpy(ip_copy, wifi_ap_ip, sizeof(ip_copy));
  ip_copy[sizeof(ip_copy) - 1] = '\0';
  char* token = strtok(ip_copy, ".");
  String URL = "http://" + String(wifi_ap_ip);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  qrcode.setScale(2);

  qrcode.draw(URL, 0, 0);
  display.setCursor(70, 0);
  display.print("IP: ");

  int y = 0; // Start below "IP:" label
  while (token != NULL) {
    display.setCursor(70, y);
    display.print("    ");
    display.print(token);
    y += 12; // 8 for text size 1, add a little extra for spacing
    token = strtok(NULL, ".");
    if (token != NULL) {
      display.println(".");
    }
  }

  display.display();
}

// Display the "Configs" screen
void configsDisplay(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(5, 0);
  display.println("Configs");
  display.display();
}