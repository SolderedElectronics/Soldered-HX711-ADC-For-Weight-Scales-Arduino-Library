/**
 **************************************************
 *
 * @file        Template for attiny_firmware
 * @brief       Fill in sensor specific code.
 *

 *
 * @authors     @ soldered.com
 ***************************************************/

#include "HX711_Firmwire.h"
#include "easyC.h"
#include <Wire.h>

HX711_Firmwire hxSensor;

int addr = DEFAULT_ADDRESS;

void setup()
{
    initDefault();
    addr = getI2CAddress();

    Wire.begin(addr);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
    hxSensor.begin();
}

void loop()
{
}


void receiveEvent(int howMany)
{
    while (1 < Wire.available())
    {
        uint8_t c = Wire.read();
    }

    uint8_t c = Wire.read();
    hxSensor.getGain(c);
}

void requestEvent()
{
    int n = 4;
    uint32_t data = hxSensor.readData();
    uint8_t a[n];

    a[0] = (data >> 24) & 0xFF;
    a[1] = (data >> 16) & 0xFF;
    a[2] = (data >> 8) & 0xFF;
    a[3] = (data >> 0) & 0xFF;

    Wire.write(a, n);
}
