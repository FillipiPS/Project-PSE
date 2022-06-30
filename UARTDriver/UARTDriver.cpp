#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <stdio.h>
#include "UARTDriver.h"

static int serial_putchar(char c, FILE *f)
{
  if (c == '\n') {
    serial_putchar('\r', f);
  }
  return Serial.write(c) == 1 ? 0 : 1;
}

static int serial_getchar(FILE *)
{
  while (Serial.available() <= 0) {
    ;
  }
  return Serial.read();
}

static FILE serial_stdinout;

static void setup_stdin_stdout()
{
  fdev_setup_stream(&serial_stdinout, serial_putchar, serial_getchar, _FDEV_SETUP_RW);
  stdout = &serial_stdinout;
  stdin  = &serial_stdinout;
  stderr = &serial_stdinout;
}

size_t initializeSTDINOUT::initnum = 0;

initializeSTDINOUT::initializeSTDINOUT()
{
  if (initnum++ == 0) {
    setup_stdin_stdout();
  }
}
