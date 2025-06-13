/**
 * @file SBK_HT16K33.h
 * @brief Arduino driver for controlling a single HT16K33 I2C LED matrix or bar meter.
 *
 * This class provides a simple API for communicating with HT16K33 LED drivers using I2C.
 * It supports 8x16 LED matrix configurations and is compatible with `SBK_BarDrive` as an optional backend.
 * The API mirrors `SBK_MAX72xx` to allow easy driver interchange.
 *
 * @example examples/simpleDemo/simpleDemo.ino
 *
 * @author
 * Samuel Barabé (Smart Builds & Kits)
 * 
 * @version 1.0.0
 * 
 * @license MIT
 * 
 * @copyright
 * Copyright (c) 2025 Samuel Barabé
 * 
 * Repository: https://github.com/sbarabe/SBK_HT16K33
 */

/**********************************************************************
 * This library is a modified version of the original HT16K33 Arduino driver
 * from MikeS11's ProtonPack repository:
 * https://github.com/MikeS11/ProtonPack/tree/master/Source/Libraries/ht16k33-arduino-master
 *
 * Modifications were made specifically to drive vertical bar meters for use in
 * SBK projects such as the Wrist Blaster.
 *
 * All credit for the original implementation goes to Mike S11.
 **********************************************************************/

#pragma once

#define SBK_HT16K33_IS_DEFINED

#include <Arduino.h>
#include <Wire.h>

// HT16K33 Command Definitions
#define HT16K33_CMD_RAM 0x00
#define HT16K33_CMD_KEYS 0x40
#define HT16K33_CMD_SETUP 0x80
#define HT16K33_CMD_ROWINT 0xA0
#define HT16K33_CMD_DIMMING 0xE0

#define HT16K33_DISPLAY_OFF 0x00
#define HT16K33_DISPLAY_ON 0x01
#define HT16K33_BLINK_OFF 0x00
#define HT16K33_BLINK_1HZ 0x02
#define HT16K33_BLINK_2HZ 0x04
#define HT16K33_BLINK_0HZ5 0x06

/**
 * @class SBK_HT16K33
 * @brief I2C driver wrapper for HT16K33 compatible with SBK_BarMeter and SBK_BarDrive.
 */
class SBK_HT16K33
{
public:
  /**
   * @brief Construct a new SBK_HT16K33 instance.
   * @param addr I2C address of the HT16K33 device.
   */
  SBK_HT16K33(uint8_t addr);

  /**
   * @brief Destructor. Frees allocated buffer memory.
   */
  ~SBK_HT16K33();

  /**
   * @brief Initialize the HT16K33 device at a given I2C address.
   */
  void begin();

  /**
   * @brief Clear the display buffer (device parameter ignored)(API-compatible wrapper).
   * @param device Not used, for compatibility only.
   */
  void clear(uint8_t device);
  /**
   * @brief Clear the internal display buffer.
   */
  void clear();

  /**
   * @brief Set the brightness level (0–15) of the device (API-compatible wrapper).
   * @param device not used, for compatibility only.
   * @param brightness Brightness level (0–15).
   */
  void setBrightness(uint8_t device, uint8_t brightness);

  /**
   * @brief Set the brightness level (0–15).
   * @param brightness Brightness level from 0 (dim) to 15 (bright).
   */
  void setBrightness(uint8_t brightness);

  /**
   * @brief Set an individual LED on or off (device-specific call) (API-compatible wrapper).
   * @param device not used, for compatibility only.
   * @param row Row index (0–7).
   * @param col Column index (0–15).
   * @param state true = ON, false = OFF.
   */
  void setLed(uint8_t device, uint8_t row, uint8_t col, bool state); ///< Set LED at (row, col)

  /**
   * @brief Set an individual LED on or off.
   * @param row Row index (0–7).
   * @param col Column index (0–15).
   * @param state true = ON, false = OFF.
   */
  void setLed(uint8_t row, uint8_t col, bool state);

  /**
   * @brief Get the current state of an LED in the buffer (API-compatible wrapper).
   * @param device not used, for compatibility only.
   * @param row Row index (0–7).
   * @param col Column index (0–15).
   * @return true if the LED is ON, false otherwise.
   */
  bool getLed(uint8_t device, uint8_t row, uint8_t col) const; ///< Get LED state at (row, col)

  /**
   * @brief Get the current state of an LED in the buffer.
   * @param row Row index (0–7).
   * @param col Column index (0–15).
   * @return true if the LED is ON, false otherwise.
   */
  bool getLed(uint8_t row, uint8_t col) const; ///< Get LED state at (row, col)

  /**
   * @brief Push internal buffer to the device (API-compatible wrapper).
   * @param device not used, for compatibility only.
   */
  void show(uint8_t device); ///< Push buffer to display

  /**
   * @brief Push internal buffer to the display.
   */
  void show();

private:
  uint8_t _i2c_addr;
  uint16_t *_buffer; ///< 8 rows × 16-bit for 16 columns

  void _write();               ///< Write full display buffer
  void _writeRow(uint8_t row); ///< Write a single row
};
