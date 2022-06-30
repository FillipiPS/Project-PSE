#ifndef _UARTDRIVER_H
#define _UARTDRIVER_H

#include <Arduino.h>

class initializeSTDINOUT
{
    static size_t initnum;
  public:
    initializeSTDINOUT();
};

static initializeSTDINOUT initializeSTDINOUT_obj;

#endif
