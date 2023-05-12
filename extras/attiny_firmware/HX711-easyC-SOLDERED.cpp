/**
 **************************************************
 *
 * @file        HX711-easyC-SOLDERED.cpp
 * @brief       Functions file for firmware for HX711 Load Cell easyC breakout
 *
 * @note        Based on HX711easyC Arduino Library by Bogdan Necula
 *              This is essentially a stripped down version of the full library
 *
 * @copyright   GNU General Public License v3.0
 * @authors     Robert Soric for Soldered.com
 ***************************************************/

#include "HX711-easyC-SOLDERED.h"

HX711easyC::HX711easyC()
{
}

void HX711easyC::begin(uint8_t gain)
{
    // Set pins and pin modes
    pinMode(PIN_DAT, INPUT_PULLUP);
    pinMode(PIN_SCK, OUTPUT);

    _gain = gain;
}

long HX711easyC::getRawReading()
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
        digitalWrite(PIN_SCK, HIGH);
        delayMicroseconds(1);
        digitalWrite(PIN_SCK, LOW);
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

void HX711easyC::waitUntilReady()
{
    while (!digitalRead(PIN_DAT) == LOW)
    {
        delay(1);
    }
}

void HX711easyC::setGain(uint8_t gain)
{
    _gain = gain;
}

void HX711easyC::setDeepSleep(bool sleep)
{
    if (!sleep)
    {
        digitalWrite(PIN_SCK, LOW);
    }
    else
    {
        digitalWrite(PIN_SCK, LOW);
        digitalWrite(PIN_SCK, HIGH);
    }
}

uint8_t HX711easyC::readRegister()
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        digitalWrite(PIN_SCK, HIGH);
        delayMicroseconds(1);
        value |= digitalRead(PIN_DAT) << (7 - i);
        digitalWrite(PIN_SCK, LOW);
        delayMicroseconds(1);
    }
    return value;
}
