/**
 **************************************************
 *
 * @file        HX711_Firmware.h
 * @brief       Arduino library for hx711 breakout firmwire
 *
 *
 *
 * @authors     @ soldered.com
 ***************************************************/


#ifndef HX711_FIRMWIRE_H
#define HX711_FIRMWIRE_H

#include "Arduino.h"
//#include "easyC.h"

#define PD_SCLK 0
#define DOUT    1


class HX711_Firmwire
{
  public:
    HX711_Firmwire();
    void begin();
    void powerUp();
    void powerDown();
    uint32_t readData();
    void setGain(uint8_t value);
    void getGain(uint8_t gain);

  private:
    int _gain = 25;
};

#endif