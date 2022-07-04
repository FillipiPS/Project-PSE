#include <ADCDriver.h>

int analogPin = A3; // terminal do meio de um potênciometro conectado ao pino analógico 3
                    // terminais mais externos são conectados um no ground e o outro em +5V
int val = 0;        // variável para guardar o valor lido

ADCDriver adc;

void setup() {
  adc.adc_config(INTERNAL1V1);
  Serial.begin(9600);           // configura a porta serial
}

void loop() {
  val = adc.adc_read(analogPin);    // lê o pino de entrada
  Serial.println(val);          // imprime o valor na porta serial
}
