/*
  LedDriver.h
*/

#ifndef LedDriver_h
#define LedDriver_h

#include <Arduino.h>

class LedDriver {
  public:
    LedDriver(uint8_t pin);
    void led_config();
    void led_on();
    void led_off();
    void led_toggle();

  private:
    uint8_t ledPin;
    uint8_t value;
};

#endif
