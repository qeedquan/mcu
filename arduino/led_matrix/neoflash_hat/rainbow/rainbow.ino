#include <FastLED.h>

#define DATA_PIN 8
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 126

#define SATURATION 255
#define BRIGHTNESS 50

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
  for (int j = 0; j < 255; j++) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(i - (j * 2), SATURATION, BRIGHTNESS);
    }
    FastLED.show();
    delay(30);
  }
}
