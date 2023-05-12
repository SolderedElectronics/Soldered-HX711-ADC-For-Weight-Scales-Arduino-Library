/**
 **************************************************
 *
 * @file        HX711-SOLDERED.cpp
 * @brief       Functions for using the HX711 Load Cell breakout by Soldered
 *
 * @note        Based on HX711 Arduino Library by Bogdan Necula
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Robert Soric for Soldered.com
 ***************************************************/

#include "HX711-SOLDERED.h"

/**
 * @brief           HX711 native constructor.
 *
 * @param           int dout pin connected to hx711 dout pin
 * @param           int pd_sck pin connected to hx711 pd_sck pin
 */
HX711::HX711(uint8_t pin_dout, uint8_t pin_sck)
{
    // Set pins and pin modes

    _pin_dout = pin_dout;
    _pin_sck = pin_sck;

    native = 1;
}

/**
 * @brief           HX711 native initialization.
 *
 */
void HX711::initializeNative()
{
        pinMode(_pin_dout, INPUT_PULLUP);
        pinMode(_pin_sck, OUTPUT);
}

/**
 * @brief           HX711 easyC constructor.
 */
HX711::HX711()
{
    native = 0;
}

/**
 * @brief           HX711 default destructor.
 */
HX711::~HX711()
{
}

/**
 * @brief           Get raw reading from the HX711.
 *                  Works for both native and easyC.
 *
 * @note            Disables interrupts for a very short period of time
 *
 * @returns         long of the raw reading
 */
long HX711::getRawReading()
{
    if (native)
    {
        waitUntilReady();

        // Define structures for reading data into.
        unsigned long value = 0;
        uint8_t data[3] = {0};
        uint8_t filler = 0x00;

        noInterrupts();

        data[2] = readRegister();
        data[1] = readRegister();
        data[0] = readRegister();

        for (unsigned int i = 0; i < _gain; i++)
        {
            digitalWrite(_pin_sck, HIGH);
            delayMicroseconds(1);
            digitalWrite(_pin_sck, LOW);
            delayMicroseconds(1);
        }

        interrupts();

        // Replicate the most significant bit to pad out a 32-bit signed integer
        if (data[2] & 0x80)
            filler = 0xFF;
        else
            filler = 0x00;

        // Construct a 32-bit signed integer
        value = (static_cast<unsigned long>(filler) << 24 | static_cast<unsigned long>(data[2]) << 16 |
                 static_cast<unsigned long>(data[1]) << 8 | static_cast<unsigned long>(data[0]));

        return static_cast<long>(value);
    }
    else
    {
        // Request the value over easyC and read it
        uint8_t data[4];
        readData((char *)data, 4);

        // Assemble it back together in a long and return
        long receivedData = ((long)data[0] << 24) | ((long)data[1] << 16) | ((long)data[2] << 8) | (long)data[3];
        return receivedData;
    }
}

/**
 * @brief           Make a number of readings and get the average of them
 * 
 * @param           uint8_t numReadings
 *                  The number of readings to be done to average from
 *
 * @returns         long of the averaged raw reading
 */
long HX711::getAveragedReading(uint8_t numReadings)
{
    float sum = 0.0;
    for (int i = 0; i < numReadings; i++)
        sum += getRawReading();

    sum /= (float)numReadings;

    return sum;
}

/**
 * @brief           Get an averaged reading minus the offset (set by setOffset)
 * 
 * @param           uint8_t numReadings
 *                  The number of readings to be done to average from
 *
 * @returns         double of the averaged and offsetted raw reading
 */
double HX711::getOffsettedReading(uint8_t numReadings)
{
    return getAveragedReading(numReadings) - _offset;
}

/**
 * @brief           Set the 'Zero' value (offset) of the load cell, used for calibration.
 *                  While calling this function, don't put any weight on the load cell!
 *
 * @returns         none
 */
void HX711::setZero()
{
    // Make 15 average readings to get the zero and set it
    float zero = getAveragedReading(15);
    setOffset(zero);
}

/**
 * @brief           Get the reading in set units. See readInUnits.ino example for more info.
 * 
 * @param           uint8_t numReadings
 *                  The number of readings to be done to average from
 * 
 * @returns         double of the reading in set units
 */
double HX711::getReadingInUnits(uint8_t numReadings)
{
    return getOffsettedReading() / _scale;
}

/**
 * @brief           Wait until the sensor is ready, this is a blocking function.
 * 
 * @returns         none
 */
void HX711::waitUntilReady()
{
    while (!digitalRead(_pin_dout) == LOW)
    {
        delay(1);
    }
}

/**
 * @brief           Read a data on the HX711 by pulsing the clock, used for getRawReading
 * 
 * @returns         uint8_t of the value read from HX711
 */
uint8_t HX711::readRegister()
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        digitalWrite(_pin_sck, HIGH);
        delayMicroseconds(1);
        value |= digitalRead(_pin_dout) << (7 - i);
        digitalWrite(_pin_sck, LOW);
        delayMicroseconds(1);
    }
    return value;
}

/**
 * @brief           Set the gain of the sensor. For both native and easyC.
 * 
 * @param           uint8_t gain
 *                  Can either be GAIN_128, GAIN_64, GAIN_32
 * 
 * @returns         none
 */
void HX711::setGain(uint8_t gain)
{
    _gain = gain;

    // If using easyC
    if (!native)
    {
        uint8_t gainToSet[1];
        switch (gain)
        {
        case GAIN_32:
            gainToSet[0] = SET_GAIN_32;
            break;
        case GAIN_64:
            gainToSet[0] = SET_GAIN_64;
            break;
        case GAIN_128:
            gainToSet[0] = SET_GAIN_128;
            break;
        default:
            gainToSet[0] = SET_GAIN_128;
            break;
        }
        sendData(gainToSet, 1);
    }
}

/**
 * @brief           Get the set gain value, in case you need it.
 * 
 * @returns         float of the set gain
 */
float HX711::getGain()
{
    return _gain;
}

/**
 * @brief           Set the scale applied to getReadingInUnits.
 *                  See readInUnits.ino for more info!
 * 
 * @param           float scale
 *                  The scale to set
 * 
 * @returns         none
 */
void HX711::setScale(float scale)
{
    _scale = scale;
}

/**
 * @brief           Get the set scale value, in case you need it.
 * 
 * @returns         float of the set scale
 */
float HX711::getScale()
{
    return _scale;
}

/**
 * @brief           Set the offset applied to getOffsettedReading
 * 
 * @param           float offset
 *                  The offset to set
 * 
 * @returns         none
 */
void HX711::setOffset(float offset)
{
    _offset = offset;
}

/**
 * @brief           Get the set offset value, in case you need it.
 * 
 * @returns         float of the set offset
 */
float HX711::getOffset()
{
    return _offset;
}

/**
 * @brief           Put the HX711 in deep sleep (turn it off). Works for both easyC and native.
 * 
 * @param           bool sleep
 *                  'True' puts the device to sleep, 'False' turns it off.
 * 
 * @returns         none
 */
void HX711::setDeepSleep(bool sleep)
{
    if (native)
    {
        if (!sleep)
        {
            digitalWrite(_pin_sck, LOW);
        }
        else
        {
            digitalWrite(_pin_sck, LOW);
            digitalWrite(_pin_sck, HIGH);
        }
    }
    else
    {
        uint8_t sleepToSet[1];
        switch (sleep)
        {
        case true:
            sleepToSet[0] = SET_SLEEP_ON;
            break;
        case false:
            sleepToSet[0] = SET_SLEEP_OFF;
            break;
        }
        sendData(sleepToSet, 1);
    }
}
