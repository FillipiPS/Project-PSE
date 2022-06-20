#ifndef PWMDriver_h_
#define PWMDriver_h_

#include "Arduino.h"
#include "TimerDriver.h"

class PWMDriver
{
  public:
    void pwm_config(uint8_t pin);
    void pwm_set(uint16_t duty);
    void pwm_update();
    void pwm_write();

  private:
    uint8_t pwmPin;
    uint16_t dutyCycle;
    uint16_t dutyCycleValue;
    uint16_t dutyDegree;
    bool isReverse;

};

extern PWMDriver PWM;

#endif
