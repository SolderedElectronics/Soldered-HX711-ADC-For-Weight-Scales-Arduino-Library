/**
 **************************************************
 *
 * @file        calibrate.ino
 * @brief       Example for calibrating the weight signal HX711 load-cell amplifier
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagrams below
 *              -Connect the load cell to the breakout
 *              -Change DAT and SCK pins if needed
 *              -Don't put any load on the load cell for the first few seconds so it calibrates!
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

    // While calibrating - don't put any load on the load cell!
    // It has to measure the value of the signal without any weight
    // This way we know where the 0 is
    // 15 measurements are made and their average is considered the 0 (to reduce noise)
    hx711.setZero();
}

void loop()
{
    // Make raw reading of the calibrated sensor and store in variable
    // Note that this is not the same function as getRawReading()
    // The applied offset is the calibration
    long ofsettedReading = hx711.getOffsettedReading();

    // You may also call getOffsettedReading(n) for the result to be an average of n readings

    // Print the reading
    Serial.print("HX711 Reading: ");
    Serial.println(ofsettedReading);

    // Wait a bit until the next reading
    delay(200);
}
