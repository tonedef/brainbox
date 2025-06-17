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
IPAddress ipv4;
bool wifiConnected;
int offset;
int lineHeight = 10;
bool previouslyPlaying = false;


/**
 * @brief Displays the application logo and version information on the screen.
 */
void showLogo() { //
  display.clearDisplay();
  display.drawBitmap(0, 0, LOGO, 128, 38, WHITE);
  display.setCursor(94, 54);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("v");
  display.println(String(VERSION));
}


/**
 * @brief Helper function to print text centered horizontally within a specified range.
 * 
 * @param text The string to be printed.
 * @param y The Y-coordinate for the top of the text.
 * @param minX The starting X-coordinate of the horizontal range for centering.
 * @param maxX The ending X-coordinate of the horizontal range for centering.
 */
static void printCentered(const String& text, int y, int minX, int maxX) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int centeredX = minX + (maxX - minX - w) / 2;
  display.setCursor(centeredX, y);
  display.print(text);
}

/**
 * @brief Initializes the OLED display.
 * Sets up I2C communication and attempts to initialize the display.
 * If initialization fails, the program will halt.
 */
void setupScreen() {
  Wire.begin(3, 5);  // Initialize I2C: SDA on GPIO3, SCL on GPIO5
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    // If display initialization fails, halt execution.
    // Consider adding error logging or a visual indicator if possible.
    for (;;); 
  }
  display.clearDisplay();
  showLogo();
  display.display();
}

void goingToSleepDisplay() {
  int timer = 5;
  for (int i = 0; i < timer; i++)  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    printCentered("Display set to turn", 0, 0, SCREEN_WIDTH);
    printCentered("off automatically", 10, 0, SCREEN_WIDTH);
    display.setTextSize(5);
    display.setCursor(50, 27);
    printCentered(String(timer -i), 27, 0, SCREEN_WIDTH);
    display.display();
    delay(1000);
  }
  show_display = false;
  display.clearDisplay();
  display.display();
}

/**
 * @brief Displays a simple sine wave loading animation.
 * The logo is displayed in the background, and a sine wave animates below it.
 * 
 * @param step The current step of the animation, used to shift the phase of the sine wave.
 */
void loadingDisplay(int step) {
  int x_width = 60;
  int y_height = 20;
  int periods = 3;
  float amplitude = y_height / 2.0f;
  int y_offset = 42; // vertical offset for display

  float x_scale = (2 * PI * periods) / x_width; // Scale x-coordinate to fit the desired number of periods within the animation width.
  float y_center = y_offset + amplitude;        // Calculate the vertical center of the sine wave on the display.

  display.clearDisplay();
  showLogo(); // Redraw logo on each frame to keep it visible during animation.
  for (int x = 0; x <= x_width; x++) { // Draw the sine wave pixel by pixel
    float phase = (float)(x + step) * x_scale;
    int y = (int)(sin(phase) * amplitude + y_center);
    display.drawPixel(x, y, SSD1306_WHITE);
  }
  display.display();
  delay(10);
}

/**
 * @brief Displays the current operational status (Playing/Stopped) and the elapsed/remaining time.
 * If `display_sleep` is enabled and the device was playing but is now stopped, it will initiate the `goingToSleepDisplay` sequence.
 * 
 * @param forceShow If true, the status is displayed immediately, bypassing the sleep check. Defaults to false.
 */
void statusDisplay(bool forceShow) {
  if(!forceShow && !playing && display_sleep && previouslyPlaying) {
    goingToSleepDisplay();
  } else {
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
    lastTime = millis();
    display.display();
  }
  previouslyPlaying = playing;
}


/**
 * @brief Displays the main configuration screen (Home page).
 * Shows WiFi status, connection details (AP or STA mode), and IP address.
 */
void configsHomeDisplay() { //
  offset = 0;
  String title = "Configs ";

  // Inverted large title
  display.setTextSize(2);
  display.fillRect(0, 0, SCREEN_WIDTH, 18, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(1, 1);
  display.print(title);

  // Display WiFi status and relevant information
  display.setTextColor(SSD1306_WHITE);
  offset = 19;
  display.setCursor(0, offset);
  display.setTextSize(1);
  display.print("WiFi ");
  display.println(wifiConnected ? "Connected" : "Disconnected");
  offset += lineHeight;
  display.setCursor(0, offset);

  if (wifiConnected) {
    display.setCursor(0, offset);
    display.print("http://wavebox.local");
  } else {
    display.print("AP SSID:");
    display.println(DEFAULT_AP_SSID);
    offset += lineHeight;
    display.setCursor(0, offset);
    display.print("AP PWD:");
    display.println(DEFAULT_AP_PASSWORD);
  }

  offset += lineHeight;
  display.setCursor(0, offset);
  display.print("http://");
  display.println(ipv4.toString());
  display.display();
}

/**
 * @brief Displays a QR code for WiFi connection in AP mode and AP credentials.
 * This screen helps users connect their mobile devices to the Wavebox's Access Point
 * to configure Station (STA) mode WiFi settings.
 */
void apConnectDisplay() {
  char wifiConfig[100];
  String ssid = DEFAULT_AP_SSID;
  int xPosition = 76;
  offset = lineHeight;
  snprintf(wifiConfig, sizeof(wifiConfig), "WIFI:S:%s;T:WPA;P:%s;H:false;;", 
            ssid, DEFAULT_AP_PASSWORD);
            // Constructs the WiFi QR code string: S=SSID, T=Encryption Type (WPA), P=Password.

  display.setTextSize(1);
  qrcode.setScale(2); // Set QR code scale for better readability on the small display.
  
  qrcode.draw(wifiConfig, 0, 0);
  display.setCursor(xPosition, 0);
  display.println("AP SSID:");
  display.setCursor(xPosition, offset);
  
  // Truncate SSID if longer than 9 characters
  if (ssid.length() > 9) {
    ssid = ssid.substring(0, 6) + "...";
  }
  display.println(ssid);

  offset += 1.5 * lineHeight;
  display.setCursor(xPosition, offset);
  display.println("Join");
  offset += lineHeight;
  display.setCursor(xPosition, offset);
  display.println("first, ");
  offset += lineHeight;
  display.setCursor(xPosition, offset);
  display.println("then go");
  offset += lineHeight;
  display.setCursor(xPosition, offset);
  display.print("here ");
  display.write(0x10); // Display right arrow character (→)
  display.display();
}

/**
 * @brief Displays a QR code for accessing the device's web interface.
 * The URL points to the root or WiFi configuration page depending on the current WiFi connection state.
 */
void rootUrlDisplay() {
  offset = 0;
  int xPosition = 70;
  String URL = "http://" + ipv4.toString() + (wifiConnected ? "" : "/wifi-config");
  display.setTextSize(1);
  qrcode.setScale(2);

  qrcode.draw(URL, 0, 0); // Draw the QR code for the web interface URL.
  display.setCursor(xPosition, 0);
  display.println("Settings");
  offset += lineHeight;
  display.setCursor(xPosition, offset);
  if (wifiConnected) {
    display.println("(App)");
  } else {
    display.println("(WiFi)");
  }
  display.display();
}

/**
 * @brief Displays the interface for adjusting the maximum session time.
 * Shows the current time setting and a knob icon as a visual cue for adjustment.
 */
void timeAdjustDisplay() {
  int time = max_time / 60000;

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  printCentered("Set Time:", 0, 0, SCREEN_WIDTH);

  display.setTextSize(4);
  printCentered(String(time), 22, 0, SCREEN_WIDTH);
  display.drawBitmap(110, 25, KNOB, 15, 15, WHITE);
  // Draw a small knob icon next to the time.
  display.setTextSize(1);
  printCentered("(minutes)", 56, 0, SCREEN_WIDTH);


  display.display();
}

/**
 * @brief Main function to route to different configuration screen displays.
 * It clears the display and then calls the appropriate function based on the `page` parameter.
 * 
 * @param page The `ConfigsPage` enum value indicating which configuration screen to display.
 */
void configsDisplay(ConfigsPage page){
  wifiConnected = WiFi.status() == WL_CONNECTED;
  ipv4 = wifiConnected ? WiFi.localIP() : WiFi.softAPIP();

  display.clearDisplay();
  display.display();
  display.setTextColor(SSD1306_WHITE);
 
  switch (page) {
    case HOME: {
      configsHomeDisplay();
      break;
    }

    case WIFI_SETUP: {
      apConnectDisplay();
      break;
    }

    case APP_PAGE: {
      rootUrlDisplay();
      break;
    }

    case TIME_ADJ: {
      timeAdjustDisplay();
      break;
    }
  }

}