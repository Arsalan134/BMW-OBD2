#include <Arduino.h>

#include <CAN.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>
#include <OBD2.h>
#include <Wire.h>

#define buttonPin 7
#define LONG_PRESS_TIME 1000

#define NUM_LEDS 20

#define DATA_PIN 3
#define CLOCK_PIN 13

bool lastState = LOW;
bool currentState;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
bool isPressing = false;
bool isLongDetected = false;

int preset = 2;
int numberOfPresets = 3;
bool displayIsOn = true;

byte Heart[8] = {0b00000, 0b01010, 0b11111, 0b11111,
                 0b11111, 0b01110, 0b00100, 0b00000};
byte Degree[8] = {0b01100, 0b10010, 0b10010, 0b01100,
                  0b00000, 0b00000, 0b00000, 0b00000};

void intro();
void ledsLoop();
void enableDisplay(bool turnOn);
void shortPressed();
void longPressed();
void buttonListener();
void printDataToScreen();
void printRPM(int column, int row);
void printValue(String title, int pid, bool printUnits, bool isFloat,
                int column, int row);
void printTemp(String title, int pid, int column, int row);
