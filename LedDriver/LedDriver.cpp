/*
  LedDriver.cpp
*/

#include "LedDriver.h"

LedDriver::LedDriver(uint8_t pin) {
  pinMode(pin, OUTPUT);
  ledPin = pin;
  value = LOW;
}

void LedDriver::led_config() {
  pinMode(ledPin, OUTPUT);
}

void LedDriver::led_on() {
  value = HIGH;
  digitalWrite(ledPin, value);
}

void LedDriver::led_off() {
  value = LOW;
  digitalWrite(ledPin, value);
}

void LedDriver::led_toggle() {
  digitalWrite(ledPin, !value);
}
