#include "main.h"

bool displayLoop(void *) {

  switch (stateOfDevices) {
  case onAll:
  case onlyDisplay:
    printDataToScreen();
    break;

  default:
    break;
  }

  return true;
}

void setup() {
  pinMode(buttonPin, INPUT);

  Serial.begin(9600);

  // Display
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.createChar(1, Heart);
  lcd.createChar(2, Degree);
  lcd.clear();

  // OBD
  while (!OBD2.begin())
    delay(500);

  // LED
  FastLED.addLeds<WS2813, LedPin, RGB>(leds, NUM_LEDS, 0);
  FastLED.setBrightness(LED_MAX_BRIGHTNESS);

  intro();

  enableDisplay(false);

  timer.every(1000, displayLoop);
}

void loop() {

  delay(100);

  timer.tick();

  buttonListener();

  switch (stateOfDevices) {
  case onAll:
  case onlyLed:
    ledsLoop();
    break;

  default:
    break;
  }
}

void intro() {

  lcd.clear();

  lcd.setCursor(3, 0);
  lcd.write(1);
  lcd.print(" Salam Azer! ");
  lcd.write(1);
  delay(1000);

  lcd.setCursor(2, 1);
  lcd.print("Mind surrounding");
  delay(1000);

  lcd.setCursor(8, 2);
  lcd.print("and");
  delay(500);

  lcd.setCursor(2, 3);
  lcd.print("Enjoy your ride!");
  delay(2000);

  lcd.clear();
}

void enableDisplay(bool turnOn) {
  if (turnOn) {
    lcd.display();
    lcd.backlight();
  } else {
    lcd.noDisplay();
    lcd.noBacklight();
  }
}

void ledsLoop() {

  int rpm = OBD2.pidRead(ENGINE_RPM);

  if (!isnan(rpm)) {

    int level = map(rpm, RPM_MIN, RPM_MAX, 0, NUM_LEDS);
    level = constrain(level, 0, NUM_LEDS);

    fill_gradient_RGB(leds, NUM_LEDS, CRGB{255, 255, 0}, CRGB{255, 0, 0});

    for (int i = level; i < NUM_LEDS; i++)
      leds[i] = CRGB::Black;

    // Blink
    if (rpm >= BLINK_RPM) {
      if (!isBlinkingRPMLimitPassed) {
        isBlinkingRPMLimitPassed = true;
        ledBlinkStartMillis = millis();
      }

      if (millis() - ledBlinkStartMillis >= BLINK_DURATION) {
        colorsAreTurnedOn = !colorsAreTurnedOn;
        ledBlinkStartMillis = millis();
      }

      if (!colorsAreTurnedOn)
        FastLED.clear();

    } else {
      isBlinkingRPMLimitPassed = false;
    }

    FastLED.show();
  }
}

void shortPressed() {
  preset++;
  preset %= numberOfPresets;

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
  if (stateOfDevices == offAll)
    checkOBD();

  switchState();
}

void switchState() {
  switch (stateOfDevices) {
  case offAll:
    stateOfDevices = onAll;

    enableDisplay(true);
    break;

  case onAll:
    stateOfDevices = onlyLed;

    enableDisplay(false);
    break;

  case onlyLed:
    stateOfDevices = onlyDisplay;

    enableDisplay(true);
    break;

  case onlyDisplay:
    stateOfDevices = offAll;

    enableDisplay(false);
    break;

  default:
    stateOfDevices = onAll;
    enableDisplay(true);
    break;
  }
}

void checkOBD() {
  if (stateOfDevices != offAll)
    while (!OBD2.begin())
      delay(500);
  else
    OBD2.end();
}

void buttonListener() {
  currentState = digitalRead(buttonPin);

  // button is pressed
  if (lastState && !currentState) {
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;

    // button is released
  } else if (!lastState && currentState) {
    isPressing = false;
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < LONG_PRESS_TIME)
      shortPressed();
  }

  if (isPressing && !isLongDetected) {
    long pressDuration = millis() - pressedTime;

    if (pressDuration >= LONG_PRESS_TIME) {
      isLongDetected = true;
      longPressed();
    }
  }

  lastState = currentState; // save the the last state
}

void printDataToScreen() {

  lcd.home();

  switch (preset) {

  default:
    printValue("Injection:", FUEL_RAIL_GAUGE_PRESSURE, 0, 0);
    printValue("Intake:    ", INTAKE_MANIFOLD_ABSOLUTE_PRESSURE, 0, 1);
    printTemp("Intake Temp: ", AIR_INTAKE_TEMPERATURE, 0, 2);
    printTemp("Coolant Temp: ", ENGINE_COOLANT_TEMPERATURE, 0, 3);
    break;

  case 1:
    printTemp("Outside Temp: ", AMBIENT_AIR_TEMPERATURE, 0, 0);
    printTemp("Intake Temp:  ", AIR_INTAKE_TEMPERATURE, 0, 1);
    printTemp("Catalyst 1:   ", CATALYST_TEMPERATURE_BANK_1_SENSOR_1, 0, 2);
    printTemp("Catalyst 2:   ", CATALYST_TEMPERATURE_BANK_2_SENSOR_1, 0, 3);
    break;

  case 2:
    printValue("RPM:    ", ENGINE_RPM, 0, 0);
    printValue("Speed:  ", VEHICLE_SPEED, 0, 1);
    printValue("Load:   ", CALCULATED_ENGINE_LOAD, 0, 2);
    printValue("Fuel:   ", FUEL_TANK_LEVEL_INPUT, 0, 3);
    break;
  }
}

void printValue(String title, int pid, int column, int row) {

  float value = OBD2.pidRead(pid);

  if (!isnan(value)) {
    lcd.setCursor(column, row);

    // if (roundf(value) == value) {
    //   int value = int(value);
    //   Serial.println(title);
    //   Serial.print("int : ");
    //   Serial.println(value);
    //   Serial.println();

    //   sprintf(buffer, "%s%5d", title.c_str(), value);

    // } else {
    //   Serial.println(title);
    //   Serial.print("float: ");
    //   Serial.println(value);
    //   Serial.println();

    // rpm int 0
    // speed int 0
    // load fuel float 465

    // sprintf(buffer, "%s%5.2f", title.c_str(), value);
    // lcd.print(buffer);

    lcd.print(title);
    lcd.print(value);
    // }

    // lcd.print(buffer);

    lcd.print(" ");
    lcd.print(OBD2.pidUnits(pid));
  } else {
    Serial.println("Is nan!!!");
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
