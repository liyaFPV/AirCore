#pragma once
#include <Arduino.h>
#include <FastLED.h>
#include "config.h"

CRGB leds[LED_COUNT];

inline void setup_led(){
    FastLED.addLeds<WS2812B, LED_PIN_DATA, RGB>(leds, LED_COUNT);
    // FastLED.addLeds<SK6812, LED_PIN_DATA, RGB>(leds, NUM_LEDS);
}

inline void setledcolor_ful(int r, int g, int b){
    for(int i = 0; i < LED_COUNT; i++){
        leds[i] = CRGB(r, g, b);
    }
    FastLED.show();
}

inline void setledcolor(int r, int g, int b, int i){
    leds[i] = CRGB(r, g, b);
    FastLED.show();
}

inline void led_tick(){
    return;
}