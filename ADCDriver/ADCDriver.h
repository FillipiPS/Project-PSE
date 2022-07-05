/*
  ADCDriver.h
*/

#ifndef ADCDriver_h
#define ADCDriver_h

#include <Arduino.h>

class ADCDriver {
  public:
    ADCDriver();
    void adc_config(uint16_t adc_freq);
    uint16_t adc_read(uint8_t adc_chan);
};

#endif
