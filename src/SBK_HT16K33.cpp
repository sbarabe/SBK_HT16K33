/**
 * @file SBK_HT16K33.cpp
 * @brief Implementation of the SBK_HT16K33 class for controlling HT16K33 LED drivers.
 *
 * Part of the SBK BarDrive Arduino Library
 * https://github.com/yourusername/SBK_BarDrive
 *
 * This file provides an I2C-compatible display driver for the HT16K33 LED matrix controller,
 * intended to be API-compatible with SBK_MAX72xx to enable reuse in SBK_BarMeter animations.
 *
 * Based on a modified version of the HT16K33 library from:
 * https://github.com/MikeS11/ProtonPack
 *
 * Original Author: Mike S11
 * Adapted by: Samuel Barabé (Smart Builds & Kits)
 *
 * @version 1.0.0
 * @license MIT
 */

#include "SBK_HT16K33.h"

SBK_HT16K33::SBK_HT16K33(uint8_t addr): _buffer(nullptr), _i2c_addr(addr) {}

SBK_HT16K33::~SBK_HT16K33()
{
    if (_buffer)
    {
        free(_buffer);
        _buffer = nullptr;
    }
}

void SBK_HT16K33::begin()
{
  // assign + zero some buffer data
  _buffer = (uint16_t *)calloc(8, sizeof(uint16_t));
  if (!_buffer) return; // Allocation failed

 Wire.begin();

    // Start oscillator
  Wire.beginTransmission(_i2c_addr);
  Wire.write(0x21); // turn it on
  Wire.endTransmission();

    // Enable display, disable blink
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_SETUP | HT16K33_DISPLAY_ON | HT16K33_BLINK_OFF);
  Wire.endTransmission();

  // Set default brightness
  setBrightness(15);
  clear();
  show();
}

void SBK_HT16K33::clear(uint8_t device)
{
  (void)device;
  clear();
}

void SBK_HT16K33::clear()
{
   if (_buffer)
    {
        for (uint8_t i = 0; i < 8; i++)
            _buffer[i] = 0;
    }
}

void SBK_HT16K33::setBrightness(uint8_t device, uint8_t brightness)
{
  (void)device;
  setBrightness(brightness);
}

void SBK_HT16K33::setBrightness(uint8_t brightness)
{
  // constrain the brightness to a 4-bit number (0–15)
  brightness &= 0x0F; // limit to 0–15

  // send the command
  Wire.beginTransmission(_i2c_addr);
  Wire.write(HT16K33_CMD_DIMMING | brightness);
  Wire.endTransmission();
}

void SBK_HT16K33::setLed(uint8_t device, uint8_t row, uint8_t col, bool state)
{
    (void)device;
    setLed(row, col, state);
}

void SBK_HT16K33::setLed(uint8_t col, uint8_t row, bool state)
{
    if (!_buffer || row >= 8 || col >= 16)
        return;

    if (state)
        _buffer[row] |= (1 << col);
    else
        _buffer[row] &= ~(1 << col);
}

bool SBK_HT16K33::getLed(uint8_t device, uint8_t row, uint8_t col) const
{
    (void)device;
   return getLed(row,col);
}

bool SBK_HT16K33::getLed(uint8_t row, uint8_t col) const
{
    if (!_buffer || row >= 8 || col >= 16)
        return false;

    return (_buffer[row] >> col) & 0x01;
}



void SBK_HT16K33::_write()
{
 if (!_buffer)
        return;

    Wire.beginTransmission(_i2c_addr);
    Wire.write(HT16K33_CMD_RAM); // Start at address 0x00

    for (uint8_t row = 0; row < 8; row++)
    {
        _writeRow(row);
    }

    Wire.endTransmission();
}

void SBK_HT16K33::show(uint8_t device)
{
    (void)device;
    _write();
}

void SBK_HT16K33::show() {
    _write();
}

void SBK_HT16K33::_writeRow(uint8_t row)
{
   if (!_buffer || row >= 8) return;
    uint16_t val = _buffer[row];
    Wire.write(val & 0xFF);       // LSB
    Wire.write((val >> 8) & 0xFF); // MSB
}