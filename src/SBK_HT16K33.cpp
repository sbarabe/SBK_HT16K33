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
 * @version 2.0.0
 * @license MIT
 */

#include "SBK_HT16K33.h"

SBK_HT16K33::SBK_HT16K33(uint8_t devsNum)
    : _devsNum(constrain(devsNum, 1, 8)),
      _buffer(nullptr)
{
    for (uint8_t i = 0; i < _devsNum; i++)
        _i2c_addr[i] = 0x70 + i; // 0x70 == 112 decimal

    for (uint8_t i = 0; i < _devsNum; i++)
        _maxRows[i] = _defaultRowBufferSize; // 8 rows (anodes) default value
}

SBK_HT16K33::~SBK_HT16K33()
{
    if (_buffer)
    {
        free(_buffer);
        _buffer = nullptr;
    }
}

uint8_t SBK_HT16K33::setAddress(uint8_t devIdx, uint8_t addr)
{
    if (devIdx >= _devsNum || addr < 0x70 || addr > 0x77)
        return 0; // invalid

    _i2c_addr[devIdx] = addr;
    return 1; // success
}

void SBK_HT16K33::begin()
{
    // assign + zero some buffer data
    _buffer = (uint16_t *)calloc(maxColumns() * _devsNum, sizeof(uint16_t));
    if (!_buffer)
        return; // Allocation failed

    Wire.begin();

    for (uint8_t i = 0; i < _devsNum; i++)
    {
        uint8_t addr = _i2c_addr[i];

        // Start oscillator
        Wire.beginTransmission(addr);
        Wire.write(0x21); // turn it on
        Wire.endTransmission();

        // Enable display, disable blink
        Wire.beginTransmission(addr);
        Wire.write(HT16K33_CMD_SETUP | HT16K33_DISPLAY_ON | HT16K33_BLINK_OFF);
        Wire.endTransmission();

        // Set default brightness
        setBrightness(i, 8);
        clear(i);
        show(i);
    }
}

void SBK_HT16K33::clear(uint8_t devIdx)
{
    if (devIdx >= _devsNum)
        return;

    if (_buffer)
    {
        for (uint8_t i = 0; i < maxColumns(); i++)
            _buffer[_colIndex(devIdx, i)] = 0;
    }
}

void SBK_HT16K33::clear()
{

    for (uint8_t d = 0; d < _devsNum; d++)
    {
        clear(d);
    }
}

void SBK_HT16K33::setBrightness(uint8_t devIdx, uint8_t brightness)
{
    if (devIdx >= _devsNum)
        return;

    // constrain the brightness to a 4-bit number (0–15)
    brightness &= 0x0F; // limit to 0–15

    // send the command
    Wire.beginTransmission(_i2c_addr[devIdx]);
    Wire.write(HT16K33_CMD_DIMMING | brightness);
    Wire.endTransmission();
}

void SBK_HT16K33::setBrightness(uint8_t brightness)
{

    for (uint8_t d = 0; d < _devsNum; d++)
    {
        setBrightness(d, brightness);
    }
}

void SBK_HT16K33::setLed(uint8_t devIdx, uint8_t rowIdx, uint8_t colIdx, bool state)
{
    if (!_buffer || devIdx >= _devsNum || rowIdx >= maxRows(devIdx) || colIdx >= maxColumns())
        return;

    uint8_t index = _colIndex(devIdx, colIdx);

    if (state)
        _buffer[index] |= (1 << rowIdx);
    else
        _buffer[index] &= ~(1 << rowIdx);

    Serial.print("[setLed] Dev: ");
    Serial.print(devIdx);
    Serial.print(" Row: ");
    Serial.print(rowIdx);
    Serial.print(" Col: ");
    Serial.print(colIdx);
    Serial.print(" State: ");
    Serial.println(state ? "ON" : "OFF");
}

bool SBK_HT16K33::getLed(uint8_t devIdx, uint8_t rowIdx, uint8_t colIdx) const
{
    if (!_buffer || devIdx >= _devsNum || rowIdx >= maxRows(devIdx) || colIdx >= maxColumns())
        return false; // return a safe default when out of bounds

    uint8_t index = _colIndex(devIdx, colIdx);
    return (_buffer[index] >> rowIdx) & 0x01;
}

void SBK_HT16K33::_write(uint8_t devIdx)
{
    if (!_buffer || devIdx >= _devsNum)
        return;

    Wire.beginTransmission(_i2c_addr[devIdx]);
    Wire.write(HT16K33_CMD_RAM); // Start at address 0x00

    for (uint8_t colIdx = 0; colIdx < maxColumns(); colIdx++)
    {
        uint16_t data = _buffer[_colIndex(devIdx, colIdx)];
        Wire.write(data & 0xFF);        // LSB
        Wire.write((data >> 8) & 0xFF); // MSB
    }

    Wire.endTransmission();
}

void SBK_HT16K33::show(uint8_t devIdx)
{
    _write(devIdx);
}

void SBK_HT16K33::show()
{
    for (uint8_t d = 0; d < _devsNum; d++)
    {
        show(d);
    }
}

inline uint8_t SBK_HT16K33::_colIndex(uint8_t devIdx, uint8_t colIdx) const
{
    return devIdx * _defaultColBufferSize + colIdx;
}