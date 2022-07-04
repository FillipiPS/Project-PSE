/*
  ADCDriver.cpp
*/

#include "ADCDriver.h"

ADCDriver::ADCDriver() { }

void ADCDriver::adc_config(uint16_t adc_freq) {
  analogReference(adc_freq);
}

uint16_t ADCDriver::adc_read(uint8_t adc_chan) {
  return analogRead(adc_chan);
}
