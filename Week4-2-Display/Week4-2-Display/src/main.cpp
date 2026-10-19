
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // number of pixels in width
#define SCREEN_HEIGHT 64 // number of pixels in height

#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

void setup() {
    Serial.begin(115200); // serial code ma value ajati hai

    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED initialization failed!");
        while (true) {
            delay(100);
        }
    } // intialize success or fail


    display.clearDisplay();// clear the display buffer
    display.setTextSize(1);// set text size to 1
    display.setTextColor(SSD1306_WHITE);// set text color to white

    display.setCursor(0, 0);// set cursor position to (0, 0)
    display.println("ESP32 + OLED");// print text to the display buffer

    display.println("PlatformIO Ready!");// print text to the display buffer
    display.println("I2C Address: 0x3C");// print text to the display buffer

    display.display();// send the display buffer to the OLED

    Serial.println("OLED initialized successfully.");
}

void loop() {
    // Add your application code here.
     display.clearDisplay();// clear the display buffer
    display.setTextSize(1);// set text size to 1
    display.setTextColor(SSD1306_WHITE);// set text color to white

    display.setCursor(0, 0);// set cursor position to (0, 0)
    display.println("Hello,IOT");// print text to the display buffer

    display.println("PlatformIO Ready!");// print text to the display buffer
    display.println("I2C Address: 0x3C");// print text to the display buffer

    display.display();
}
