/**
 * @file simpleDemo.ino
 * @brief Basic usage example for SBK_HT16K33 LED driver.
 *
 * This example demonstrates how to initialize an HT16K33 LED matrix driver,
 * set brightness, and light up a diagonal line on an 8x8 LED area using the SBK_HT16K33 class.
 *
 * This sketch is part of the SBK_HT16K33 library.
 * https://github.com/sbarabe/SBK_HT16K33
 *
 * @author
 * Samuel Barabé (Smart Builds & Kits)
 * @version 2.0.0
 * @date 2025
 * @license MIT
 */

#include <Arduino.h>
#include <Wire.h>
#include <SBK_HT16K33.h>

// Create an instance of the SBK_HT16K33 driver with 2 active devices
const uint8_t NUM_DEV = 2;          ///< Number of HT16K33 devices in use
const uint8_t ADD[] = {0x70, 0x71}; ///< I2C address for device 0 (typically 0x70–0x77)
const uint8_t NUM_ROWS[] = {8, 8};  ///< Device 0: 20-SOP = 8 rows, 24-SOP = 12 rows, 28-SOP = 16 rows
SBK_HT16K33 ht(NUM_DEV);

void setup()
{
  // Initialize I2C communication
  Wire.begin();

  // First setup HT16K33 devices I2C address and devices rows (anodes) outputs number
  for (uint8_t dev = 0; dev < NUM_DEV; dev++)
  {
    ht.setAddress(dev, ADD[dev]);         // Set I2C address
    ht.setDriverRows(dev, NUM_ROWS[dev]); // Set number of active anode outputs (rows)
  }

  // Initialize the HT16K33 driver
  ht.begin();

  // Set display brightness (0 = dim, 15 = max)
  ht.setBrightness(10);

  // Clear the internal display buffer
  ht.clear();

  // For both devices :
  // Light up a diagonal pattern from top-left to bottom-right on an 8×8 section
  // Each LED is addressed by row and column (0–7)
  for (uint8_t dev = 0; dev < NUM_DEV; dev++)
  {
    for (uint8_t i = 0; i < NUM_ROWS[dev]; i++)
    {
      ht.setLed(dev, i, i, true); // Turn on LED at (row=i, col=i)
    }
  }

  // Push the buffer contents to the physical display
  ht.show();
}

void loop()
{
  // No animation or updates in loop; pattern stays static
}
