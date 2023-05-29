/**
 **************************************************
 *
 * @file        HX711-SOLDERED.h
 * @brief       Header file for HX711 Load Cell breakout
 *
 * @note        Based on HX711 Arduino Library by Bogdan Necula
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Robert Soric for Soldered.com
 ***************************************************/

#ifndef __HX711_SOLDERED__
#define __HX711_SOLDERED__

#include "Arduino.h"
#include "libs/Generic-easyC/easyC.hpp"

// The different options to set by the user via recieveEvent
// Only used if you are using the easyC version of the breakout
enum HX711_FUNCTION
{
    SET_GAIN_32 = 1,
    SET_GAIN_64 = 2,
    SET_GAIN_128 = 3,
    SET_SLEEP_ON = 4,
    SET_SLEEP_OFF = 5
};

// Gain 32 is for B+-, while others are for A+-
#define GAIN_128 1
#define GAIN_64  3
#define GAIN_32  2

class HX711 : public EasyC
{
  public:
    HX711();
    virtual ~HX711();
    HX711(uint8_t pin_dout, uint8_t pin_sck);
    void waitUntilReady();
    long getRawReading();
    long getAveragedReading(uint8_t numReadings = 1);
    double getOffsettedReading(uint8_t numReadings = 1);
    double getReadingInUnits(uint8_t numReadings = 1);

    void setZero();
    void setGain(uint8_t gain);
    float getGain();
    void setScale(float scale);
    float getScale();
    void setOffset(float offset);
    float getOffset();
    void setDeepSleep(bool sleep);

  protected:
    void initializeNative();

  private:
    int _pin_dout;
    int _pin_sck;
    uint8_t _gain = GAIN_128;
    float _scale = 1.0;
    float _offset = 0.0;
    uint32_t _zeroConstant = 0;
    uint8_t readRegister();
};

#endif
