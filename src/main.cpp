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

  if (!isnan(rpm))
    Serial.println(rpm);

  // if (!isnan(rpm)) {
  //   int level = map(rpm, RPM_MIN, RPM_MAX, 0, NUM_LEDS);
  //   level = constrain(level, 0, NUM_LEDS);

  //   fill_gradient_RGB(leds, NUM_LEDS, CRGB{255, 255, 0}, CRGB{255, 0, 0});

  //   for (int i = level + 1; i < NUM_LEDS; i++)
  //     leds[i] = CRGB::Black;

  //   // Blink
  //   if (rpm >= BLINK_RPM_THRESHOLD) {
  //     if (!isBlinkingRPMLimitPassed) {
  //       isBlinkingRPMLimitPassed = true;
  //       ledBlinkStartMillis = millis();
  //     }

  //     if (millis() - ledBlinkStartMillis >= BLINK_DURATION) {
  //       colorsAreTurnedOn = !colorsAreTurnedOn;
  //       ledBlinkStartMillis = millis();
  //     }

  //     if (!colorsAreTurnedOn)
  //       FastLED.clear();

  //   } else {
  //     isBlinkingRPMLimitPassed = false;
  //   }

  //   FastLED.show();
  // }
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

void rainbow() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;

  uint8_t sat8 = beatsin88(87, 220, 250);
  uint8_t brightdepth = beatsin88(341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88(203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;  // gHue * 256;
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