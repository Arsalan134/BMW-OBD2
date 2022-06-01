#include "main.h"

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
    delay(750);

  // LED
  FastLED.addLeds<WS2813, LedPin, RGB>(leds, NUM_LEDS, 0);
  FastLED.setBrightness(150);
  // FastLED.setMaxPowerInVoltsAndMilliamps(12, 300);

  // intro();

  fill_gradient_RGB(leds, NUM_LEDS, CRGB{0, 255, 0}, CRGB{255, 0, 0});
}

void loop() {
  delay(20);

  buttonListener();

  if (displayIsOn)
    printDataToScreen();

  if (ledIsOn)
    ledsLoop();
}

void intro() {

  pride();
  FastLED.show();

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

void enableDisplayAndLED(bool turnOn) {

  displayIsOn = turnOn;
  ledIsOn = turnOn;

  if (turnOn) {
    lcd.display();
    lcd.backlight();
    fill_gradient_RGB(leds, NUM_LEDS, CRGB{0, 255, 0}, CRGB{255, 0, 0});
  } else {
    lcd.noDisplay();
    lcd.noBacklight();
    FastLED.clear();
    FastLED.show();
  }
}

void ledsLoop() {
  // FastLED.clear();

  int rpm = OBD2.pidRead(ENGINE_RPM);

  if (!isnan(rpm)) {

    int level = map(rpm, RPM_MIN, RPM_MAX, 0, NUM_LEDS);
    level = constrain(level, 0, NUM_LEDS);

    // Turn On
    for (int i = 0; i < level; i++)
      leds[i].maximizeBrightness(LED_MAX_BRIGHTNESS);

    // Fade
    for (int i = level; i < NUM_LEDS; i++)
      leds[i].fadeToBlackBy(fadeRate);

    // byte newBrightness = map(level, 0, NUM_LEDS, 20, 100);

    // Blink
    if (rpm >= BLINK_RPM) { // change later to define var
      if (!isBlinkingRPMLimitPassed) {
        isBlinkingRPMLimitPassed = true;
        ledBlinkPeriod = millis();
      }

      if (ledBlinkPeriod >= BLINK_DURATION) { // change later to define var
        colorsAreTurnedOn = !colorsAreTurnedOn;
        ledBlinkPeriod = millis();
      }

      if (!colorsAreTurnedOn)
        // FastLED.clear();
        // or
        for (int i = 0; i < NUM_LEDS; i++)
          // leds[i] = CRGB{0, 0, 0};
          leds[i].fadeToBlackBy(255);

    } else {
      isBlinkingRPMLimitPassed = false;
      ledBlinkPeriod = 0;
    }

    FastLED.show();
  }
}

void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16; // gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis;
  sLastMillis = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88(400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16 += brightnessthetainc16;
    uint16_t b16 = sin16(brightnesstheta16) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV(hue8, sat8, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;

    nblend(leds[pixelnumber], newcolor, 64);
  }
}

void shortPressed() {
  // Serial.println("Short Pressed");
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
  // Toggle Display
  enableDisplayAndLED(!displayIsOn);
  checkOBD();
}

void checkOBD() {
  if (displayIsOn) {
    while (!OBD2.begin()) {
      enableDisplayAndLED(false);
      delay(750);
    }
    intro();
  } else
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

  // save the the last state
  lastState = currentState;
}

void printDataToScreen() {

  lcd.home();

  switch (preset) {

  default:
    printValue("Injection:", FUEL_RAIL_GAUGE_PRESSURE, 0, 0);           // 5
    printValue("Intake:    ", INTAKE_MANIFOLD_ABSOLUTE_PRESSURE, 0, 1); // 4
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
    printValue("Load:   ", CALCULATED_ENGINE_LOAD, 0, 2); // 3 true
    printValue("Fuel:   ", FUEL_TANK_LEVEL_INPUT, 0, 3);  // 3 true
    break;
  }
}

void printValue(String title, int pid, int column, int row) {

  float value = OBD2.pidRead(pid);

  if (!isnan(value)) {
    lcd.setCursor(column, row);

    if (roundf(value) == value) {
      int value = int(value);
      sprintf(buffer, "%s%5d", title.c_str(), value);
    } else {
      Serial.println(value);
      Serial.println("float");
      sprintf(buffer, "%s%5.2f", title.c_str(), value);
    }

    lcd.print(buffer);

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
