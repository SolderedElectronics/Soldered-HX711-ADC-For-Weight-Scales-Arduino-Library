/**
 **************************************************
 *
 * @file        easyCExample.ino
 * @brief       Example for using the easyC variant of the HX711 breakout.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagrams below
 *              -Connect the load cell to the breakout
 *              -Open the Serial plotter at 115200 baud!
 *
 *              Load-cell amplifier HX711 board: solde.red/333005
 *              Dasduino Core: www.solde.red/333037
 *              Dasduino Connect: www.solde.red/333034
 *              Dasduino ConnectPlus: www.solde.red/333033
 *
 * @authors     Robert @ soldered.com
 ***************************************************/

// Include the library
#include "HX711-SOLDERED.h"

/**
 * Connecting diagram:
 *
 * HX711 load-cell amplifier   Dasduino
 * EasyC----------------------->EasyC
 * 
 * HX711 load-cell amplifier    Load-cell
 * (E+)------------------------>RED
 * (E-)------------------------>BLACK
 * (A-)------------------------>GREEN
 * (A+)------------------------>WHITE
 * 
 */

// Create the easyC variant of the HX711 object
HX711 hx711;

void setup()
{
    Serial.begin(115200); // For debugging

    // Init HX711
    hx711.begin();

    // Wait a bit until it initializes fully
    delay(200);
}

void loop()
{
    // Make raw reading and store in variable
    long reading = hx711.getRawReading();
    
    // Print the reading
    // Try reading this over the serial plotter!
    Serial.print("HX711 Reading: ");
    Serial.println(reading);
    
    // Wait a short while until the next reading
    // This serial print is quite fast because it looks better on the serial plotter
    delay(200);
}

