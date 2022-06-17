/*
  GPIODriver.h
*/

#ifndef GPIODriver_h
#define GPIODriver_h

#include <Arduino.h>

class GPIODriver {
  public:
    GPIODriver();
    void gpio_config(uint8_t gpio_num, uint8_t dir);
    void gpio_write(uint8_t gpio_num, uint8_t value);
    uint8_t gpio_read(uint8_t gpio_num);
    void gpio_toggle(uint8_t gpio_num);
    void gpio_dir(uint8_t gpio_num);

  private:
    uint8_t pinDirection;
};

#endif
