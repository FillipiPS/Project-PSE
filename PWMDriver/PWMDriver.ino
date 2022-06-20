#include <PWMDriver.h>

const int led = LED_BUILTIN;
uint8_t dutyCycle = 10;
PWMDriver PWM;

void setup() {
  // put your setup code here, to run once:
  Timer1.initialize(10000000); // 10 segundos
  Timer1.attachInterrupt(updatePWM);
  PWM.pwm_config(led);
  PWM.pwm_set(dutyCycle);
  Serial.begin(9600);
}

void updatePWM(void) {
  PWM.pwm_update();
}

void loop() {
  // put your main code here, to run repeatedly:
  PWM.pwm_write();
}
