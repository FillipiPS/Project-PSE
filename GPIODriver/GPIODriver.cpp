/*
  GPIODriver.cpp
*/

#include "GPIODriver.h"

GPIODriver::GPIODriver() { }

void GPIODriver::gpio_config(uint8_t gpio_num, uint8_t dir) {
  pinDirection = dir;
  pinMode(gpio_num, pinDirection);
}

void GPIODriver::gpio_write(uint8_t gpio_num, uint8_t value) {
  digitalWrite(gpio_num, value);
}

uint8_t GPIODriver::gpio_read(uint8_t gpio_num) {
  return digitalRead(gpio_num);
}

void GPIODriver::gpio_toggle(uint8_t gpio_num) {
  digitalWrite(gpio_num, !digitalRead(gpio_num));
}

void GPIODriver::gpio_dir(uint8_t gpio_num) {
  digitalWrite(gpio_num, !pinDirection);
}
