/**
 **************************************************
 *
 * @file        HX711_Firmware.cpp
 * @brief       Arduino library for hx711 breakout firmwire
 *
 *
 *
 * @authors     @ soldered.com
 ***************************************************/

#include "HX711_Firmwire.h"

/**
 * @brief       default constructor that sets pinMode for device
 */
HX711_Firmwire::HX711_Firmwire()
{
    pinMode(PD_SCLK, OUTPUT);
    pinMode(DOUT, INPUT);
}

/**
 * @brief       Doesn't do anything for this device, its here for Arduino compability
 */
void HX711_Firmwire::begin()
{
}

/**
 * @brief       powerUp starts device if its turned down
 */
void HX711_Firmwire::powerUp()
{
    digitalWrite(PD_SCLK, LOW);
    delayMicroseconds(100);
}

/**
 * @brief       powerDown puts device in low pover mode
 */
void HX711_Firmwire::powerDown()
{
    digitalWrite(PD_SCLK, HIGH);
    delayMicroseconds(70);
}

/**
 * @brief       setGain sets device mode of operation
 *
 * @param       uint8_t value is gain value that will be used in setting device
 */
void HX711_Firmwire::setGain(uint8_t value)
{
    if (value < 25 || value > 27)
    {
        return;
    }

    for (int i = 0; i < value; i++)
    {
        digitalWrite(PD_SCLK, HIGH);
        delayMicroseconds(4);
        digitalWrite(PD_SCLK, LOW);
        delayMicroseconds(4);
    }
}

/**
 * @brief       getGain sets variable that is used for seting device mode of operation
 *
 * @param       uint8_t gain value
 */
void HX711_Firmwire::getGain(uint8_t gain)
{
    if (gain < 25 || gain > 27)
    {
        _gain = gain;
    }
}

/**
 * @brief       readData will set gain mode and then read data from device
 *
 * @return      uint32_t weight data from internal register
 */
uint32_t HX711_Firmwire::readData()
{
    powerDown(); // power down just to make sure
    powerUp();   // power up

    // wait for DOUT pin to go LOW and signal data available
    uint32_t timer = millis();
    while (digitalRead(DOUT) == HIGH)
    {
        if (millis() - timer > 100)
        {
            return 0;
        }
    }

    // Set gain for next reading of data
    setGain(_gain);


    // Wait aggain for DOUT pin to signal available data
    timer = millis();
    while (digitalRead(DOUT) == HIGH)
    {
        if (millis() - timer > 100)
        {
            return 0;
        }
    }

    uint32_t result = 0;

    // read data
    for (int i = 0; i < 25; ++i)
    {
        digitalWrite(PD_SCLK, HIGH);
        delayMicroseconds(4);

        // check to not read last bit from DOUT pin since thas is always 1 (DOUT goes HIGH on 25th read)
        if (i < 24)
        {
            result = ((result << 1) | digitalRead(DOUT));
        }

        digitalWrite(PD_SCLK, LOW);
        delayMicroseconds(4);
    }

    // turn chip off
    powerDown();

    return result;
}