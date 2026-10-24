#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- LED pins ----------
#define LED1_PIN 18
#define LED2_PIN 19

// ---------- OLED configuration ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// ---------- PWM configuration ----------
const uint32_t LED1_FREQ = 1000;  // Red LED: 1000 Hz
const uint32_t LED2_FREQ = 5000;  // Green LED: 5000 Hz
const uint8_t PWM_RES = 10;       // 10-bit resolution
const int MAX_DUTY = (1 << PWM_RES) - 1;  // 1023

// Arduino-ESP32 core 2.x uses explicit LEDC channels.
#if ESP_ARDUINO_VERSION_MAJOR < 3
const uint8_t LED1_CH = 0;
const uint8_t LED2_CH = 2;
#endif

// ---------- Fade variables ----------
int duty1 = 0;
int duty2 = 0;

int dir1 = 1;
int dir2 = 1;

const int step1 = 8;
const int step2 = 5;

// ---------- Timing ----------
unsigned long lastFadeTime = 0;
unsigned long lastDisplayTime = 0;

const unsigned long FADE_INTERVAL = 15;
const unsigned long DISPLAY_INTERVAL = 100;

// ---------- Write PWM duty ----------
void writeDuty(int led, int duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(led == 1 ? LED1_PIN : LED2_PIN, duty);
#else
    ledcWrite(led == 1 ? LED1_CH : LED2_CH, duty);
#endif
}

// ---------- Update fading ----------
void fade(int &duty, int &direction, int step) {
    duty += direction * step;

    if (duty >= MAX_DUTY) {
        duty = MAX_DUTY;
        direction = -1;
    }

    if (duty <= 0) {
        duty = 0;
        direction = 1;
    }
}

// ---------- Update OLED ----------
void updateOLED() {
    int percent1 = duty1 * 100 / MAX_DUTY;
    int percent2 = duty2 * 100 / MAX_DUTY;

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);

    display.setCursor(0, 0);
    display.println("DUAL LED PWM");

    display.setCursor(0, 16);
    display.print("Red LED: ");
    display.print(percent1);
    display.println("%");

    display.setCursor(0, 26);
    display.print("Freq: ");
    display.print(LED1_FREQ);
    display.println(" Hz");

    display.setCursor(0, 40);
    display.print("Green LED: ");
    display.print(percent2);
    display.println("%");

    display.setCursor(0, 50);
    display.print("Freq: ");
    display.print(LED2_FREQ);
    display.println(" Hz");

    display.display();
}

// ---------- Setup ----------
void setup() {
    Serial.begin(115200);

    // Initialize I2C OLED on ESP32 pins 21 and 22.
    Wire.begin(OLED_SDA, OLED_SCL);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("OLED initialization failed!");
        while (true) {
            delay(100);
        }
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Dual LED PWM");
    display.println("Initializing...");
    display.display();

    // Configure both PWM outputs.
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    bool led1Ready = ledcAttach(LED1_PIN, LED1_FREQ, PWM_RES);
    bool led2Ready = ledcAttach(LED2_PIN, LED2_FREQ, PWM_RES);

    if (!led1Ready || !led2Ready) {
        Serial.println("PWM setup failed!");
        while (true) {
            delay(100);
        }
    }
#else
    ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
    ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);

    ledcAttachPin(LED1_PIN, LED1_CH);
    ledcAttachPin(LED2_PIN, LED2_CH);
#endif

    writeDuty(1, duty1);
    writeDuty(2, duty2);

    updateOLED();

    Serial.println("Task 2: LEDs and OLED started.");
}

// ---------- Main loop ----------
void loop() {
    unsigned long now = millis();

    // Fade both LEDs without blocking delays.
    if (now - lastFadeTime >= FADE_INTERVAL) {
        lastFadeTime = now;

        fade(duty1, dir1, step1);
        fade(duty2, dir2, step2);

        writeDuty(1, duty1);
        writeDuty(2, duty2);

        Serial.printf(
            "Red: %d%% @ %lu Hz | Green: %d%% @ %lu Hz\n",
            duty1 * 100 / MAX_DUTY,
            (unsigned long)LED1_FREQ,
            duty2 * 100 / MAX_DUTY,
            (unsigned long)LED2_FREQ
        );
    }

    // Refresh OLED every 100 milliseconds.
    if (now - lastDisplayTime >= DISPLAY_INTERVAL) {
        lastDisplayTime = now;
        updateOLED();
    }
}