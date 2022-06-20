#include "PWMDriver.h"

bool isReverse = false;

void PWMDriver::pwm_config(uint8_t pin) {
  dutyCycle = 0;
  dutyCycleValue = 0;
  dutyDegree = 0;
  
  pwmPin = pin;
  pinMode(pwmPin, OUTPUT);
}

void PWMDriver::pwm_update() {
  if (dutyCycle == 100) {
    isReverse = true;
  } else if (dutyCycle == 0) {
    isReverse = false;
  }

  if (isReverse) {
    dutyCycle -= dutyDegree;
  } else {
    dutyCycle += dutyDegree;
  }

  dutyCycleValue = dutyCycle * 2.55;
}

void PWMDriver::pwm_write() {
  analogWrite(pwmPin, dutyCycleValue);
}

void PWMDriver::pwm_set(uint16_t duty) {
  dutyDegree = duty;
}
