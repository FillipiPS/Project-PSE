#include <LedDriver.h>

LedDriver led(LED_BUILTIN); //Instancia o led

void setup() {
  // put your setup code here, to run once:
  led.led_config(); //Configura o led
}

void loop() {
  // put your main code here, to run repeatedly:
  led.led_on(); //Acende o led
  delay(1000);
  led.led_off(); //Apaga o led
  delay(1000);

  delay(100);
  led.led_toggle(); //Muda o valor do led
  delay(100);
}
