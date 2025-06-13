# SBK_HT16K33 Library

Arduino-compatible driver for controlling HT16K33 LED matrix ICs using I2C. This library is designed for 8x16 LED matrix control and is compatible with `SBK_BarDrive` if used, but can also be used standalone.

---

## ✅ Features

- HT16K33 driver for 8x16 LED matrices or bar meters
- I2C-based control with brightness control and display clearing
- Compatible with SBK_BarDrive (optional)
- Efficient buffer-based updates

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

SBK_HT16K33 ht(0x70); // default I2C address

void setup() {
  Wire.begin();
  ht.begin();
  ht.setBrightness(10);
  ht.clear();

  // Light up a diagonal on 8x8 block
  for (uint8_t i = 0; i < 8; i++) {
    ht.setLed(i, i, true);
  }

  ht.show();
}

void loop() {
  // Nothing here for now
}
```

---

## 🔧 API Summary

| Method              | Description                           |
|---------------------|----------------------------------------|
| `begin()`           | Initializes the HT16K33 driver         |
| `setLed(row,col,v)` | Sets LED at (row, col) to ON/OFF       |
| `getLed(...)`       | Gets the current buffer state          |
| `clear()`           | Clears the internal buffer             |
| `show()`            | Pushes the buffer to the display       |
| `setBrightness()`   | Adjusts brightness level (0–15)        |

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
