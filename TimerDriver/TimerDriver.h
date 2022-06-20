#ifndef TimerDriver_h_
#define TimerDriver_h_

#include "Arduino.h"

#include "config/known_16bit_timers.h"

#define TIMER1_RESOLUTION 65536UL  // Timer1 is 16 bit

class TimerDriver
{
  public:
    void initialize(unsigned long microseconds);
    void setPeriod(unsigned long microseconds);
    void start();
    void stop();
    void restart();
    void resume();

    void attachInterrupt(void (*isr)());
    void attachInterrupt(void (*isr)(), unsigned long microseconds);
    void detachInterrupt();
    static void (*isrCallback)();

  private:
    static unsigned short pwmPeriod;
    static unsigned char clockSelectBits;
};

extern TimerDriver Timer1;

#endif
