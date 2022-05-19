#include <Arduino.h>

#include "main.h"

#include <CAN.h>
#include <OBD2.h>

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void intro() {
  lcd.clear();

  lcd.setCursor(3, 0);
  lcd.write(1);
  lcd.print(" Salam Azer! ");
  lcd.write(1);
  delay(1000);

  lcd.setCursor(2, 2);
  lcd.print("Mind surrounding");
  delay(1000);

  lcd.setCursor(2, 3);
  lcd.print("Enjoy your ride!");

  delay(2000);

  lcd.clear();

  delay(1000);
}

void enableDisplay(boolean turnOn) {
  if (turnOn) {
    lcd.display();
    lcd.backlight();
    displayIsOn = true;
  } else {
    lcd.noDisplay();
    lcd.noBacklight();
    displayIsOn = false;
  }
}

void setup() {
  pinMode(buttonPin, INPUT);

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.createChar(1, Heart);
  lcd.createChar(2, Degree);
  lcd.clear();

  intro();

  while (!OBD2.begin()) {
    enableDisplay(false);
    delay(750);
  }

  enableDisplay(true);
}

void shortPressed() {
  preset++;
  preset = preset % numberOfPresets;

  lcd.clear();

  lcd.setCursor(6, 1);
  lcd.print("Preset ");
  lcd.print(preset + 1);

  lcd.setCursor(5, 2);
  lcd.print(" activated");
  delay(750);

  lcd.clear();
}

void longPressed() {
  // Toggle Display
  enableDisplay(!displayIsOn);

  if (displayIsOn)
    intro();
}

void buttonListener() {
  currentState = digitalRead(buttonPin);

  // button is pressed
  if (lastState == HIGH && currentState == LOW) {
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;

    // button is released
  } else if (lastState == LOW && currentState == HIGH) {
    isPressing = false;
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < SHORT_PRESS_TIME)
      shortPressed();
  }

  if (isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - pressedTime;

    if (pressDuration >= LONG_PRESS_TIME) {
      isLongDetected = true;
      longPressed();
    }
  }

  // save the the last state
  lastState = currentState;
}

void printDataToScreen() {

  lcd.home();

  switch (preset) {
  default:
    printRPM(0, 0);
    printValue("Pressure:  ", INTAKE_MANIFOLD_ABSOLUTE_PRESSURE, true, false, 0,
               1);
    printTemp("Coolant Temp: ", ENGINE_COOLANT_TEMPERATURE, 0, 2);
    printTemp("Intake Temp:  ", AIR_INTAKE_TEMPERATURE, 0, 3);
    break;

  case 1:
    printTemp("Intake Temp : ", AIR_INTAKE_TEMPERATURE, 0, 0);
    printTemp("Ambient Temp: ", AMBIENT_AIR_TEMPERATURE, 0, 1);
    printTemp("Catalyst 1  : ", CATALYST_TEMPERATURE_BANK_1_SENSOR_1, 0, 2);
    printTemp("Catalyst 2  : ", CATALYST_TEMPERATURE_BANK_2_SENSOR_1, 0, 3);
    break;

  case 2:
    printValue("Speed: ", VEHICLE_SPEED, true, false, 0, 0);
    printValue("Load:  ", CALCULATED_ENGINE_LOAD, true, true, 0, 1);
    printValue("Fuel:  ", FUEL_TANK_LEVEL_INPUT, true, true, 0, 2);
    printValue("Start: ", RUN_TIME_SINCE_ENGINE_START, true, true, 0, 3);
    break;
  }
}

void loop() {

  delay(100);

  buttonListener();

  if (displayIsOn)
    printDataToScreen();
}

void printRPM(int column, int row) {
  int rpm = OBD2.pidRead(ENGINE_RPM);

  if (!isnan(rpm)) {
    lcd.setCursor(11, row);
    lcd.print(" ");

    lcd.setCursor(column, row);
    lcd.print("RPM:    ");
    lcd.print(rpm);
  }
}

void printValue(String title, int pid, boolean printUnits, boolean isFloat,
                int column, int row) {

  if (isFloat) {
    float value = OBD2.pidRead(pid);
    if (!isnan(value)) {
      lcd.setCursor(column, row);
      lcd.print(title);
      lcd.print(value);

      if (printUnits) {
        lcd.print(" ");
        lcd.print(OBD2.pidUnits(pid));
      }
    }
  } else {
    int value = OBD2.pidRead(pid);
    if (!isnan(value)) {
      lcd.setCursor(column, row);
      lcd.print(title);
      lcd.print(value);

      if (printUnits) {
        lcd.print(" ");
        lcd.print(OBD2.pidUnits(pid));
      }
    }
  }
}

void printTemp(String title, int pid, int column, int row) {
  int temperature = OBD2.pidRead(pid);

  if (!isnan(temperature)) {
    lcd.setCursor(column, row);
    lcd.print(title);
    lcd.print(temperature);

    lcd.print(" ");
    lcd.write(2);
    lcd.print("C");
  }
}
