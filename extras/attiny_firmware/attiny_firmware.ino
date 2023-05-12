/**
 **************************************************
 *
 * @file        Template for attiny_firmware
 * @brief       Fill in sensor specific code.
 *

 *
 * @authors     @ soldered.com
 ***************************************************/

#include "easyC.h"
#include <Wire.h>
#include "HX711-easyC-SOLDERED.h"

int addr = DEFAULT_ADDRESS;

// The variable where we periodically write the data to
long sensorData = 0;

// The different options to set by the user via recieveEvent
enum HX711_FUNCTION
{
    SET_GAIN_32 = 1,
    SET_GAIN_64 = 2,
    SET_GAIN_128 = 3,
    SET_SLEEP_ON = 4,
    SET_SLEEP_OFF = 5
};

// Create the HX711easyC object
HX711easyC hx711;

void setup()
{
    initDefault();
    addr = getI2CAddress();

    // Init the sensor
    hx711.begin();

    Wire.begin(addr);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}

void loop()
{
    // Periodically make readings so sensor is always ready to get data from
    sensorData = hx711.getRawReading();
}


void receiveEvent(int howMany)
{
    while (1 < Wire.available())
    {
        uint8_t c = Wire.read();
    }

    uint8_t c = Wire.read();

    switch(c)
    {
        case SET_GAIN_32:
            hx711.setGain(GAIN_32);
        break;
        case SET_GAIN_64:
            hx711.setGain(GAIN_64);
        break;
        case SET_GAIN_128:
            hx711.setGain(GAIN_128);
        break;
        case SET_SLEEP_ON:
            hx711.setDeepSleep(true);
        break;
        case SET_SLEEP_OFF:
            hx711.setDeepSleep(false);
        break;
        default:
        break;
    }
}

void requestEvent()
{
    byte bytes[4];

    bytes[0] = (sensorData >> 24) & 0xFF;
    bytes[1] = (sensorData >> 16) & 0xFF;
    bytes[2] = (sensorData >> 8) & 0xFF;
    bytes[3] = (sensorData) & 0xFF;

    Wire.write(bytes, 4);
}
