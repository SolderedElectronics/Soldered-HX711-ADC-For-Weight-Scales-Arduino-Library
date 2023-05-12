/**
 **************************************************
 *
 * @file        HX711-easyC-SOLDERED.h
 * @brief       Header file for firmware for HX711 Load Cell easyC breakout
 *
 * @note        Based on HX711 Arduino Library by Bogdan Necula
 *              This is essentially a stripped down version of the full library
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Robert Soric for Soldered.com
 ***************************************************/

#ifndef __HX711_EASYC_SOLDERED__
#define __HX711_EASYC_SOLDERED__

#include "Arduino.h"

// Gain 32 is for B+-, while others are for A+-
#define GAIN_128 1
#define GAIN_64  3
#define GAIN_32  2

// The pin defines
// This is set in the hardware of the board itself
// So it can't be changed
#define PIN_SCK      0
#define PIN_DAT      1

class HX711easyC
{
  public:
    HX711easyC();
    void begin(uint8_t gain = GAIN_128);
    void waitUntilReady();
    long getRawReading();
    void setGain(uint8_t gain);
    void setDeepSleep(bool sleep);

  private:
    uint8_t _gain = GAIN_128;
    uint8_t readRegister();
};

#endif
