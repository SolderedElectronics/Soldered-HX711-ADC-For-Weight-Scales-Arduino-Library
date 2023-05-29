/**
 **************************************************
 *
 * @file        deepSleep.ino
 * @brief       Example for shutting down the HX711 between readings (deep sleep)
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagrams below
 *              -Connect the load cell to the breakout
 *              -Change DAT and SCK pins if needed
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

// Define pins used for DAT and SCK here
#define PIN_DAT 4
#define PIN_SCK 3

/**
 * Connecting diagram:
 *
 * HX711 load-cell amplifier   Dasduino
 * VCC------------------------->VCC
 * GND------------------------->GND (Make sure to connect both GND's!)
 * DAT------------------------->PIN_DAT, set by user
 * SCK------------------------->PIN_SCK, set by user
 *
 * HX711 load-cell amplifier    Load-cell
 * (E+)------------------------>RED
 * (E-)------------------------>BLACK
 * (A-)------------------------>GREEN
 * (A+)------------------------>WHITE
 *
 */

// Create the HX711 object on the right pins
HX711 hx711(PIN_DAT, PIN_SCK);

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
    // Wake up the HX711 from deep sleep
    hx711.setDeepSleep(false);

    // Wait a bit until it initializes fully
    delay(200);

    // Make raw reading and store in variable
    long reading = hx711.getRawReading();

    // Print the reading
    Serial.print("HX711 Reading: ");
    Serial.println(reading);

    // Place the HX711 in deep sleep
    hx711.setDeepSleep(true);

    // Wait a long while until the next reading
    delay(15000);
}
