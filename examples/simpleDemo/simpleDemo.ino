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
 * @version 1.0.0
 * @date 2025
 * @license MIT
 */

#include <Arduino.h>
#include <Wire.h>
#include <SBK_HT16K33.h>

// Create an instance of the SBK_HT16K33 driver
// 0x70 is the default I2C address for most HT16K33 modules
SBK_HT16K33 ht(0x70);

void setup() {
  // Initialize I2C communication
  Wire.begin();

  // Initialize the HT16K33 driver
  ht.begin();

  // Set display brightness (0 = dim, 15 = max)
  ht.setBrightness(10);

  // Clear the internal display buffer
  ht.clear();

  // Light up a diagonal pattern from top-left to bottom-right on an 8×8 section
  // Each LED is addressed by row and column (0–7)
  for (uint8_t i = 0; i < 8; i++) {
    ht.setLed(i, i, true); // Turn on LED at (row=i, col=i)
  }

  // Push the buffer contents to the physical display
  ht.show();
}

void loop() {
  // No animation or updates in loop; pattern stays static
}
