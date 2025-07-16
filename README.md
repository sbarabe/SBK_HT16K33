# SBK_HT16K33 Library

Arduino-compatible driver for controlling HT16K33 LED matrix ICs using I2C. This library is designed for 8x16 LED matrix control and is compatible with `SBK_BarDrive` if used, but can also be used standalone.

---

## ✅ Features

- HT16K33 driver for 8x16 LED matrices or bar meters
- I2C-based control with brightness control and display clearing
- Compatible with SBK_BarDrive (optional)
- Efficient buffer-based updates
- **Supports up to 8 HT16K33 devices on the I2C bus**
- **Independent brightness, address, and row configuration per device**

---

## 📦 Installation

1. Download or clone this repository:
   ```bash
   git clone https://github.com/sbarabe/SBK_HT16K33.git
   ```
2. Copy it to your Arduino `libraries/` folder

---

## 🧪 Example (standalone usage)

```cpp
#include <Wire.h>
#include <SBK_HT16K33.h>

SBK_HT16K33 ht(2); // support for 2 devices

void setup() {
  Wire.begin();
  ht.setAddress(0, 0x70); // optional: override default address for device 1
  ht.setAddress(1, 0x71); // optional: override default address for device 1
  ht.setDriverRows(0,8);
  ht.setDriverRows(1,8);
  ht.begin();
  ht.setBrightness(0, 10);
  ht.setBrightness(1, 5);
  ht.clear();

  // Light up diagonals on both devices
  for (uint8_t i = 0; i < 8; i++) {
    ht.setLed(0, i, i, true);         // Device 0
    ht.setLed(1, i, 7 - i, true);     // Device 1
  }

  ht.show();
}

void loop() {
  // Nothing here for now
}
```

---

## 📘 HT16K33 Device Variants

| Package  | Rows (anodes) | Recommended Configuration               |
|----------|----------------|------------------------------------------|
| 20-SOP   | 8              | `setDriverRows(dev, 8)` (default)       |
| 24-SOP   | 12             | `setDriverRows(dev, 12)`                |
| 28-SOP   | 16             | `setDriverRows(dev, 16)`                |

Use `setDriverRows()` to match your HT16K33 chip configuration.

---

## 🔧 API Summary

| Method                     | Description                                      |
|----------------------------|--------------------------------------------------|
| `begin()`                  | Initializes the HT16K33 driver                   |
| `setLed(dev,row,col,v)`    | Sets LED at (row, col) for a device             |
| `getLed(dev,row,col)`      | Gets the LED state from internal buffer         |
| `clear()`                  | Clears buffer for all devices                   |
| `clear(dev)`               | Clears buffer for a specific device             |
| `show()`                   | Pushes buffer to all devices                    |
| `show(dev)`                | Pushes buffer to a specific device              |
| `setBrightness(dev, val)`  | Sets brightness for one device (0–15)           |
| `setBrightness(val)`       | Sets brightness for all devices                 |
| `setAddress(dev, addr)`    | Override default I2C address (0x70–0x77)        |
| `setDriverRows(dev, rows)` | Configure active rows (8, 12, or 16)            |
| `maxRows(dev)`             | Returns number of active rows                   |
| `maxColumns()`             | Always returns 8 columns                        |
| `maxSegments(dev)`         | Returns total LEDs for a device (rows × 8)      |
| `devsNum()`                | Returns number of managed HT16K33 devices       |

---

## 🧩 Integration with SBK_BarDrive (optional)

To use this library with [`SBK_BarDrive`](https://github.com/sbarabe/SBK_BarDrive):
- Include `<SBK_HT16K33.h>` **before** including `SBK_BarDrive.h`
- Then create a `SBK_BarDrive<SBK_HT16K33>` instance as shown in SBK_BarDrive examples.

---

## 🪪 License

This library is released under the MIT License.
See the `LICENSE` file for full details.

---

## 🧠 Credits

- Adapted from [MikeS11's ProtonPack HT16K33 library](https://github.com/MikeS11/ProtonPack)
- Developed and maintained by **Samuel Barabé** (Smart Builds & Kits)

---
