#include "TimerDriver.h"

TimerDriver Timer1;

unsigned short TimerDriver::pwmPeriod = 0;
unsigned char TimerDriver::clockSelectBits = 0;
void (*TimerDriver::isrCallback)() = NULL;

void TimerDriver::initialize(unsigned long microseconds = 1000000) {
  TCCR1B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer
  TCCR1A = 0;                 // clear control register A
  setPeriod(microseconds);
}

void TimerDriver::setPeriod(unsigned long microseconds) {
  const unsigned long cycles = (F_CPU / 2000000) * microseconds;
  if (cycles < TIMER1_RESOLUTION) {
    clockSelectBits = _BV(CS10);
    pwmPeriod = cycles;
  } else if (cycles < TIMER1_RESOLUTION * 8) {
    clockSelectBits = _BV(CS11);
    pwmPeriod = cycles / 8;
  } else if (cycles < TIMER1_RESOLUTION * 64) {
    clockSelectBits = _BV(CS11) | _BV(CS10);
    pwmPeriod = cycles / 64;
  } else if (cycles < TIMER1_RESOLUTION * 256) {
    clockSelectBits = _BV(CS12);
    pwmPeriod = cycles / 256;
  } else if (cycles < TIMER1_RESOLUTION * 1024) {
    clockSelectBits = _BV(CS12) | _BV(CS10);
    pwmPeriod = cycles / 1024;
  } else {
    clockSelectBits = _BV(CS12) | _BV(CS10);
    pwmPeriod = TIMER1_RESOLUTION - 1;
  }
  ICR1 = pwmPeriod;
  TCCR1B = _BV(WGM13) | clockSelectBits;
}

void TimerDriver::start() {
  TCCR1B = 0;
  TCNT1 = 0;
  resume();
}

void TimerDriver::stop() {
  TCCR1B = _BV(WGM13);
}

void TimerDriver::restart() {
  start();
}

void TimerDriver::resume() {
  TCCR1B = _BV(WGM13) | clockSelectBits;
}

void TimerDriver::attachInterrupt(void (*isr)()) {
  isrCallback = isr;
  TIMSK1 = _BV(TOIE1);
}

void TimerDriver::attachInterrupt(void (*isr)(), unsigned long microseconds) {
  if (microseconds > 0) setPeriod(microseconds);
  attachInterrupt(isr);
}

void TimerDriver::detachInterrupt() {
  TIMSK1 = 0;
}

ISR(TIMER1_OVF_vect)
{
  Timer1.isrCallback();
}
