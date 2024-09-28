#include <Arduino.h>
#include <CAN.h>
#include <FastLED.h>
#include <OBD2.h>

// Pins
#define LED_PIN 3
#define BUTTON_PIN 7

// Button
// Duration in milliseconds to hold to activate long press
#define LONG_PRESS_TIME 1000

#define DOUBLE_PRESS_TIME_THRESHOLD 1000

// LED
#define NUM_LEDS 9              // Number of LEDS to use
#define LED_MAX_BRIGHTNESS 255  // 0 - 255
#define BLINK_DURATION 75       // milliseconds

#define RPM_MIN 3000    // Start rpm value for the leds 3000
#define RPM_MAX 5000    // End rpm value for the leds 5000
#define BLINK_RPM 5200  // RPM threshold for blink to start 5200

// Threshold at which arduino considers engine turned off
#define TURN_OFF_RPM 100

// Objects
CRGB leds[NUM_LEDS];

// Changing Variables
bool buttonWasPressed = false;
bool buttonIsPressed = false;
bool isPressing = false;
bool isLongDetected = false;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
bool pressedRecently = false;

bool colorsAreTurnedOn = true;
unsigned long ledBlinkStartMillis = 0;
bool isBlinkingRPMLimitPassed = false;

int ledBrightnesses[] = {5, 50, 150, 255};
int currentBrightnessIndex = 1;

void shortPressed();
void doublePressed();
void longPressed();
void buttonListener();
void ledsLoop();