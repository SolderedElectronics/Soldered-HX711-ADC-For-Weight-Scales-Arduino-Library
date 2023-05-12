/**
 **************************************************
 *
 * @file        readInUnits.ino
 * @brief       Example for reading weight in a set unit.
 *              Using the HX711 this way can actually measure weight quite precisely.
 *
 *              To successfully run the sketch:
 *              -Connect the breakout to your Dasduino board according to the diagrams below
 *              -Connect the load cell to the breakout
 *              -Change DAT and SCK pins if needed
 *              -Don't put any load on the load cell for the first few seconds so it calibrates!
 *              -Do the test measurement to set units, as described in setup()
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

// Scale units, for more info see setup() below
#define SCALE_UNITS 1.0

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
    hx711.setZero();

    /**
     * Reading values from the Load Cell in units requires doing one test measurement:
     * 1. Get an object which you know the weight of (for example, a 0.5kg weight)
     * 2. Run this sketch, don't change the SCALE_UNITS value fow now
     * 3. After running the sketch, place the known weight on the load cell
     * 4. Note the displayed value, let's call it X
     * 5. Your SCALE_UNITS is X / known weight, set it to that
     * 
     * As an example, our 5kg load cell returned 391254 for a 0.5 kg weight.
     * So, SCALE_UNITS in that case would be 391254 * 0.5
     */
    hx711.setScale(SCALE_UNITS);
}

void loop()
{
    // Make raw reading of the calibrated sensor and store in variable
    // Note that this is not the same function as getRawReading()
    // The applied offset is the calibration
    double readingInUnits = hx711.getReadingInUnits();

    // You may also call getReadingInUnits(n) for the result to be an average of n readings

    // Print the reading
    // Try reading this over the serial plotter!
    Serial.print("HX711 Reading: ");
    Serial.println(readingInUnits);
    
    // Wait a short while until the next reading
    // This serial print is quite fast because it looks better on the serial plotter
    delay(200);
}
