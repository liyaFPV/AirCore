#pragma once
#include <Arduino.h>
#include "config.h"

extern uint8_t ledBrightness;
void led_init();
void led_set(uint8_t r, uint8_t g, uint8_t b);
