#include "main.h"

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  lcd.init();
  lcd.clear();
  lcd.createChar(1, Heart);
  lcd.createChar(2, Degree);

  while (!OBD2.begin())
    delay(500);

  FastLED.addLeds<WS2813, LED_PIN, RGB>(leds, NUM_LEDS, 0);
  FastLED.setBrightness(LED_MAX_BRIGHTNESS);

  // intro();

  switchState(stateOfDevices);

  timer.every(1000, displayLoop);
}

void loop() {

  timer.tick();

  buttonListener();

  switch (stateOfDevices) {
  case onAll:
  case onlyLed:
    ledsLoop();
    break;

  default:
    FastLED.clear();
    FastLED.show();
    break;
  }

  int rpm = OBD2.pidRead(ENGINE_RPM);

  // Check Engine State
  if (rpm < TURN_OFF_RPM) {
    if (stateOfDevices != offAll) {
      delay(500);
      switchState(offAll);
      introPresented = false;
    } else
      delay(2000); // Delay programm if engine is off

  } else if (rpm > TURN_OFF_RPM && stateOfDevices == offAll &&
             !introPresented) {
    // delay(100);
    switchState(onlyDisplay);
    delay(200);
    intro();
    introPresented = true;
    switchState(offAll);
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
  lcd.clear();

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
  switch (stateOfDevices) {
  case offAll:
    switchState(onlyLed);
    break;

  case onlyLed:
    switchState(onlyDisplay);
    break;

  case onlyDisplay:
    switchState(onAll);
    break;

  case onAll:
    switchState(offAll);
    break;
  }
}

void doublePressed() {
  currentBrightnessIndex++;

  currentBrightnessIndex %=
      sizeof(ledBrightnesses) / sizeof(typeof(ledBrightnesses[0]));

  FastLED.setBrightness(ledBrightnesses[currentBrightnessIndex]);
}

void switchState(StateOfDevices to) {
  stateOfDevices = to;

  switch (to) {
  case offAll:
    enableDisplay(false);
    break;

  case onAll:
    enableDisplay(true);
    break;

  case onlyLed:
    enableDisplay(false);
    break;

  case onlyDisplay:
    enableDisplay(true);
    break;
  }
}

void buttonListener() {
  buttonIsPressed = !digitalRead(BUTTON_PIN);

  // button is pressed
  if (!buttonWasPressed && buttonIsPressed) {
    pressedTime = millis();
    isPressing = true;
    isLongDetected = false;

    // button is released
  } else if (buttonWasPressed && !buttonIsPressed) {
    isPressing = false;
    releasedTime = millis();
    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < LONG_PRESS_TIME)
      if (pressedRecently) {
        doublePressed();
        buttonWasPressed = buttonIsPressed;
      } else if (millis() - releasedTime >= DOUBLE_PRESS_TIME_THRESHOLD) {
        shortPressed();
        buttonWasPressed = buttonIsPressed;
      }

    if (millis() - releasedTime < DOUBLE_PRESS_TIME_THRESHOLD)
      pressedRecently = true;
    else
      pressedRecently = false;
  }

  if (isPressing && !isLongDetected) {
    long pressDuration = millis() - pressedTime;

    if (pressDuration >= LONG_PRESS_TIME) {
      isLongDetected = true;
      longPressed();
    }
  }
}

void printDataToScreen() {

  lcd.home();

  switch (preset) {

  default:
    printValue("Injection: ", FUEL_RAIL_GAUGE_PRESSURE, 0, 0);
    printValue("Intake:       ", INTAKE_MANIFOLD_ABSOLUTE_PRESSURE, 0, 1);
    printTemp("Intake Temp:  ", AIR_INTAKE_TEMPERATURE, 0, 2);
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
  String units = OBD2.pidUnits(pid);

  if (!isnan(value)) {
    lcd.setCursor(column, row);

    lcd.print(title);

    // Clear value spaces
    for (unsigned int i = 0; i < (20 - strlen(title.c_str())); i++)
      lcd.print(" ");

    lcd.setCursor(column + strlen(title.c_str()), row);

    if (roundf(value) == value)
      lcd.print(int(value));
    else
      lcd.print(value);

    lcd.print(" ");
    lcd.print(units);
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
