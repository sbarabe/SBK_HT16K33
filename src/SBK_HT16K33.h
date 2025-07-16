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
 * @version 2.0.0
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
   *
   * @param devsNum  Number of HT16K33 devices included this driver instance (up to 8). Default is 1.
   */
  SBK_HT16K33(uint8_t devsNum = 1);

  /**
   * @brief Destructor. Frees allocated buffer memory.
   */
  ~SBK_HT16K33();

  /**
   * @brief Set the number of active rows (anode outputs) for a specific HT16K33 device.
   *
   * @param devIdx    Index of the target device (0–7).
   * @param rowsCount Number of active row lines (must be 8, 12, or 16).Default is 8.
   *
   * HT16K33 comes in multiple package variants that determine the number of available anode outputs:
   * - **20-SOP** → 8 rows (R0–R7)
   * - **24-SOP** → 12 rows (R0–R11)
   * - **28-SOP** → 16 rows (R0–R15)
   *
   * Use this function to limit the active row count per device if you're using a smaller variant.
   *
   * @note Invalid device indices or unsupported row counts are ignored.
   */
  void setDriverRows(uint8_t devIdx, uint8_t rowsCount = 8)
  {
    if (devIdx >= 8 || (rowsCount != 8 && rowsCount != 12 && rowsCount != 16))
      return;

    _maxRows[devIdx] = rowsCount;
  }

  /**
   * @brief Returns the number of active row lines (anode outputs) for a specific HT16K33 device.
   *
   * @param devIdx Index of the target device (0–7).
   *
   * Each row corresponds to a physical R-line (R0–R15) on the HT16K33 chip, which controls the anode side of the matrix.
   * The number of active rows depends on the chip package:
   * - **20-SOP**: 8 rows
   * - **24-SOP**: 12 rows
   * - **28-SOP**: 16 rows (full range)
   *
   * This function returns the current row configuration for the specified device,
   * which can be set using `setDriverRows()`.
   *
   * @return Number of active row lines for the given device.
   */
  uint8_t maxRows(uint8_t devIdx) const { return _maxRows[devIdx]; }

  /**
   * @brief Returns the number of active column lines (cathode outputs = C0–C7) configured for this instance.
   *
   * Each column corresponds to a HT16K33 C-line (C0–C7), controlling the cathode side of the LEDs.
   * This defines how many columns (scan lines) are used in the matrix.
   *
   * @return Number of active columns (cathode lines), always 8 for HT16K33.
   */
  uint8_t maxColumns() const { return _defaultColBufferSize; }

  /**
   * @brief Returns the total number of addressable LED segments for the specified device.
   *
   * @param devIdx Index of the device (0–7).
   *
   * Computed as:
   * `maxRows(devIdx) × maxColumns()`
   *
   * This represents the number of individually addressable LEDs (or matrix pixels)
   * available on the specified HT16K33 device, depending on its configured row count.
   *
   * HT16K33 supports up to:
   * - 64 segments (8×8) for 20-SOP
   * - 96 segments (12×8) for 24-SOP
   * - 128 segments (16×8) for 28-SOP
   *
   * @return Total number of addressable LED segments for the given device.
   */
  uint8_t maxSegments(uint8_t devIdx) const { return maxRows(devIdx) * maxColumns(); }

  /**
   * @brief Set the I2C address for a specific HT16K33 device.
   *
   * By default, device 0 uses address 0x70, device 1 uses 0x71, ..., up to device 7 using 0x77.
   * This function allows you to override that default address mapping.
   *
   * @param devIdx Device Index (0–7).
   * @param addr   I2C address to assign (must be in range 0x70–0x77).
   * @return 1 if the address was successfully set, 0 if devIdx or addr is invalid.
   */
  uint8_t setAddress(uint8_t devIdx, uint8_t addr);

  /**
   * @brief Initialize the HT16K33 device at a given I2C address.
   */
  void begin();

  /**
   * @brief Clear the display buffer for the specified device.
   *
   * @param devIdx Index of the target device (0–7).
   *
   * This clears the internal buffer for a single HT16K33 device.
   * Call `.show(devIdx)` to apply the cleared state to the hardware.
   */
  void clear(uint8_t devIdx);

  /**
   * @brief Clear the display buffers for all devices.
   *
   * This clears the internal LED state buffers across all managed HT16K33 devices.
   * Call `.show()` afterward to push the changes to the physical displays.
   */
  void clear();

  /**
   * @brief Set the brightness level (0–15) for a specific device.
   *
   * @param devIdx     Index of the target device (0–7).
   * @param brightness Brightness level (0 = dimmest, 15 = brightest).
   *
   * This function sets the brightness for a single HT16K33 device.
   * It is API-compatible with the MAX72xx interface used in SBK_BarDrive.
   */
  void setBrightness(uint8_t devIdx, uint8_t brightness);

  /**
   * @brief Set the brightness level (0–15) for all devices.
   *
   * @param brightness Brightness level (0 = dimmest, 15 = brightest).
   *
   * This function applies the specified brightness level to all HT16K33 devices managed
   * by this driver instance.
   */
  void setBrightness(uint8_t brightness);

  /**
   * @brief Returns the number of active HT16K33 devices managed by this driver instance.
   *
   * @return Number of active driver devices for this instance.
   */
  uint8_t devsNum() const { return _devsNum; }

  /**
   * @brief Set the state of an individual LED for a specific device.
   *
   * @param devIdx  Index of the target device (0–7).
   * @param rowIdx  Row index (0 to maxRows(devIdx) - 1).
   * @param colIdx  Column index (0 to maxColumns() - 1).
   * @param state   true = LED ON, false = LED OFF.
   *
   * This function updates the internal buffer for a single LED on a specific HT16K33 device.
   * You must call `.show()` to apply the changes to the physical display.
   *
   * @note For HT16K33:
   * - **rowIdx** corresponds to Rx pins (anode, V+ source)
   * - **colIdx** corresponds to Cx pins (cathode, GND sink)
   */
  void setLed(uint8_t devIdx, uint8_t rowIdx, uint8_t colIdx, bool state); ///< Set LED at (rowIdx, colIdx)

  /**
   * @brief Get the current state of an LED from the internal display buffer.
   *
   * @param devIdx  Not used (for API compatibility only).
   * @param rowIdx  Row index (0–7).
   * @param colIdx  Column index (0–15).
   * @return true if the LED is ON in the internal buffer, false otherwise.
   *
   * This function reads the last known LED state from the driver’s RAM buffer.
   * It does not query the physical device directly.
   *
   * @note For HT16K33:
   * - **rowIdx** maps to Rx lines (anodes)
   * - **colIdx** maps to Cx lines (cathodes)
   */
  bool getLed(uint8_t devIdx, uint8_t rowIdx, uint8_t colIdx) const; ///< Get LED state at (rowIdx, colIdx)

  /**
   * @brief Push the internal display buffer to a specific device.
   *
   * @param devIdx Index of the target device (0–7).
   *
   * This sends the buffered LED states to the physical HT16K33 display
   * for the specified device only.
   *
   * @note This function must be called after `setLed()` or `clear()` to reflect changes on the hardware.
   */
  void show(uint8_t devIdx); ///< Push buffer to display for one device

  /**
   * @brief Push the internal display buffer to all devices.
   *
   * This sends the buffered LED states to all HT16K33 devices managed by this driver.
   * Use this to update the entire display after modifying any LED states.
   */
  void show();

private:
  uint8_t _devsNum = 1;
  uint8_t _i2c_addr[8];
  uint8_t _maxRows[8];
  uint16_t *_buffer;                                  ///< 8 cols × 16-bit for 16 rows
  static constexpr uint8_t _defaultRowBufferSize = 8; // HT16K33 comes in 3 versions 20SOP, 24SOP and 28SOP: 8, 12 or 16 rows (anodes)
  static constexpr uint8_t _defaultColBufferSize = 8;

  void _write(uint8_t devIdx); ///< Write full display buffer
  inline uint8_t _colIndex(uint8_t devIdx, uint8_t colIdx) const;
};
