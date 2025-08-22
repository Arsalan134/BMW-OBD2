#include "main.h"

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT);

  while (!OBD2.begin())
    delay(200);

  FastLED.addLeds<WS2813, LED_PIN, RGB>(leds, NUM_LEDS, 0);
  // FastLED.setBrightness(ledBrightnesses[currentBrightnessIndex]);
  FastLED.setBrightness(250);
}

void loop() {
  buttonListener();
  ledsLoop();

  rainbow();
  FastLED.show();
}

int rpm = 0;

void ledsLoop() {
  rpm = OBD2.pidRead(ENGINE_RPM);

  if (!isnan(rpm)) {
    int level = map(rpm, RPM_MIN, RPM_MAX, 0, NUM_LEDS);
    level = constrain(level, 0, NUM_LEDS);

    fill_gradient(leds, NUM_LEDS, CRGB{255, 255, 0}, CRGB{255, 0, 0});

    for (int i = level + 1; i < NUM_LEDS; i++)
      leds[i] = CRGB::Black;

    // Blink
    if (rpm >= BLINK_RPM_THRESHOLD) {
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
  currentBrightnessIndex++;
  currentBrightnessIndex %= 4;

  FastLED.setBrightness(ledBrightnesses[currentBrightnessIndex]);
}

void longPressed() {}

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
      shortPressed();
  }

  if (isPressing && !isLongDetected) {
    long pressDuration = millis() - pressedTime;

    if (pressDuration >= LONG_PRESS_TIME) {
      isLongDetected = true;
      longPressed();
      pressedRecently = false;
    }
  }

  buttonWasPressed = buttonIsPressed;
}