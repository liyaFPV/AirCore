#include "led.h"

uint8_t ledBrightness = LED_BRIGHTNESS;
static uint8_t lastR = 255, lastG = 255, lastB = 255;

static inline uint8_t scaleVal(uint8_t v) {
  return (uint8_t)((uint16_t)v * ledBrightness / 255);
}

void led_init() {
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  pinMode(ledBluePin, OUTPUT);
  led_set(255, 255, 255);
}

void led_set(uint8_t r, uint8_t g, uint8_t b) {
  uint8_t sr = scaleVal(r);
  uint8_t sg = scaleVal(g);
  uint8_t sb = scaleVal(b);
  if (sr == lastR && sg == lastG && sb == lastB) return;
  analogWrite(ledRedPin, sr);
  analogWrite(ledGreenPin, sg);
  analogWrite(ledBluePin, sb);
  lastR = sr; lastG = sg; lastB = sb;
}
