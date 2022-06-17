#include <GPIODriver.h>

GPIODriver pinDriver; // Instancia pinDriver

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinDriver.gpio_config(24, INPUT); // Configura pin 24 como INPUT
  pinDriver.gpio_config(31, OUTPUT); // Configura pin 31 como OUTPUT
}

void loop() {
  // put your main code here, to run repeatedly:
  int pin24Value = pinDriver.gpio_read(24); // Lê o pin 24
  Serial.println(pin24Value); // Mostra o valor do pin 24 no Monitor serial

  pinDriver.gpio_write(31, pin24Value); // Escreve o valor do pin 24 no pin 31
  pinDriver.gpio_toggle(31); // Inverte o valor do pin 31
  pinDriver.gpio_dir(31); // Inverte a direção do pin 31
}
