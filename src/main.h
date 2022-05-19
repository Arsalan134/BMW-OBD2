#include <Arduino.h>

int preset = 2;
int numberOfPresets = 3;
bool displayIsOn = true;

const int buttonPin = 7;

const int SHORT_PRESS_TIME = 900;
const int LONG_PRESS_TIME = 1000;

int lastState = LOW;
int currentState;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;
bool isPressing = false;
bool isLongDetected = false;

byte Heart[8] = {0b00000, 0b01010, 0b11111, 0b11111,
                 0b11111, 0b01110, 0b00100, 0b00000};

byte Degree[8] = {0b01100, 0b10010, 0b10010, 0b01100,
                  0b00000, 0b00000, 0b00000, 0b00000};

void intro();

void enableDisplay(boolean turnOn);

void shortPressed();

void longPressed();

void buttonListener();

void printDataToScreen();

void printRPM(int column, int row);

void printValue(String title, int pid, bool printUnits, bool isFloat,
                int column, int row);

void printTemp(String title, int pid, int column, int row);
