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

// 🚸🏎️ Comment next line and save to change the Environment
#define isTesting
// ----------------------------------------------------

// LED
#ifdef isTesting
// --------------------------- Test Environment ---------------------------------------
#define NUM_LEDS 9         // Number of LEDS to use
#define BLINK_DURATION 75  // milliseconds

#define RPM_MIN 700               // Start rpm value
#define RPM_MAX 2000              // End rpm value
#define BLINK_RPM_THRESHOLD 2300  // RPM threshold for blink to start

#else
// --------------------------- Real Environment ---------------------------------------
#define NUM_LEDS 9         // Number of LEDS to use
#define BLINK_DURATION 75  // milliseconds

#define RPM_MIN 3000              // Start rpm value
#define RPM_MAX 5000              // End rpm value
#define BLINK_RPM_THRESHOLD 5200  // RPM threshold for blink to start

#endif

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