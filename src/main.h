#include <Arduino.h>

#include <CAN.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <OBD2.h>
#include <Wire.h>
#include <arduino-timer.h>

// Pins
#define LedPin 3
#define buttonPin 7

// Button
// Duration in milliseconds to hold to activate long press
#define LONG_PRESS_TIME 1000

// LED
#define NUM_LEDS 9            // Number of LEDS to use
#define LED_MAX_BRIGHTNESS 50 // 0 - 255
#define BLINK_DURATION 75     // milliseconds

#define RPM_MIN 3000   // Start rpm value for the leds
#define RPM_MAX 5000   // End rpm value for the leds
#define BLINK_RPM 5200 // RPM threshold for blink to start

// Display
char buffer[21];

// Objects
LiquidCrystal_I2C lcd(0x27, 20, 4);
CRGB leds[NUM_LEDS];

// Changing Variables
bool lastState = false;
bool currentState = false;
bool isPressing = false;
bool isLongDetected = false;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;

bool colorsAreTurnedOn = true;
unsigned long ledBlinkStartMillis = 0;
bool isBlinkingRPMLimitPassed = false;

int preset = 0;
int numberOfPresets = 3;

bool displayIsOn = true;
bool ledIsOn = true;

byte Heart[8] = {0b00000, 0b01010, 0b11111, 0b11111,
                 0b11111, 0b01110, 0b00100, 0b00000};

byte Degree[8] = {0b01100, 0b10010, 0b10010, 0b01100,
                  0b00000, 0b00000, 0b00000, 0b00000};

auto timer = timer_create_default();

void intro();
void ledsLoop();
void enableDisplayAndLED(bool turnOn);
void shortPressed();
void longPressed();
void checkOBD();
void buttonListener();
void printDataToScreen();
void printTemp(String title, int pid, int column, int row);
void displayLoop();

/**
 * @brief  Prints values to a display
 * @param  title: Text to display on screen
 * @param  pid: PID from OBD2 protocol
 * @param  numberOfDigits: Number of digits to reserve
 * @param  column: column index
 * @param  row: row index
 */
void printValue(String title, int pid, int column, int row);

void pride();
