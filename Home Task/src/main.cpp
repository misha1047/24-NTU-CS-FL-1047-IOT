<<<<<<< HEAD
// Homework-01: ESP32 OLED status display with non-blocking debounced button
// Wiring: OLED SDA->GPIO21, SCL->GPIO22, VCC->3V3, GND->GND
//         Button between GPIO18 and GND (INPUT_PULLUP, pressed = LOW)





#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_ADDRESS   0x3C
#define BUTTON_PIN     18

const unsigned long DEBOUNCE_MS = 50;   // input must be stable this long

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int  lastRawState    = HIGH;  // last raw reading (HIGH = released)
int  stableState     = HIGH;  // debounced state
unsigned long lastChangeTime = 0;  // when the raw reading last changed
unsigned long pressCount = 0;

void updateDisplay() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("ESP32 Button Counter");

  display.setCursor(0, 20);
  display.print("Button: ");
  display.println(stableState == LOW ? "PRESSED" : "RELEASED");

  display.setTextSize(2);
  display.setCursor(0, 42);
  display.print("Count:");
  display.print(pressCount);

  display.display();  // push buffer to the screen
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("SSD1306 init failed - check wiring/address");
    while (true) { delay(1000); }
  }
  updateDisplay();
  Serial.println("System ready. Count = 0");
}

void loop() {
  int rawState = digitalRead(BUTTON_PIN);

  // 1) Raw reading changed -> restart the stability timer
  if (rawState != lastRawState) {
    lastChangeTime = millis();
    lastRawState = rawState;
  }

  // 2) Reading has been stable longer than DEBOUNCE_MS -> accept it
  if ((millis() - lastChangeTime) >= DEBOUNCE_MS && rawState != stableState) {
    stableState = rawState;

    if (stableState == LOW) {      // count only on the press edge
      pressCount++;
      Serial.print("Accepted press #");
      Serial.println(pressCount);
    }
    updateDisplay();
  }
  // No delay(): loop stays free for other tasks
}
=======
#include <Arduino.h>

<<<<<<< HEAD
// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
=======
void setup() {
  pinMode(2, OUTPUT);
  
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);
  Serial.println("ESP32 Simulation Started!");
}

void loop() {
  digitalWrite(2, HIGH);   
  Serial.println("LED State: ON");
  delay(1000);             
  
  digitalWrite(2, LOW);    
  Serial.println("LED State: OFF");
  delay(1000);             
>>>>>>> 1e17ed552b285197b22213eda2630ac61662e16d
}
>>>>>>> 7f9b4872580786eb657ce208745e497759346f06
