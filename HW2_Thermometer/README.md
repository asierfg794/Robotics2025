# Temperature Display Project (HW2)

### Overview
This project is an **embedded task** that measures temperature using an **LM35 sensor** and displays it on a **16x2 I2C LCD**.  
The user can toggle between **Celsius** and **Fahrenheit** using a button.  
The unit preference is **saved in EEPROM**, so it persists after power cycles.  

---

### Features
- **Periodic temperature reading** (every 500 ms) using a **hardware timer interrupt**  
- **External interrupt** on button press (with PULLUP and debounce)  
- **EEPROM storage** for user preference  
- **Visual feedback** through LCD and LED  
- **Asynchronous design:** main loop reacts to flags set by interrupts  
- **Non-blocking execution** (no `delay()` except brief user feedback)

---

### Pin Connections
| Component | Arduino Pin | Notes |
|------------|--------------|-------|
| LM35 Sensor | A0 | Analog input |
| LED | 13 | Status indicator |
| Button | 2 | External interrupt (FALLING) |
| LCD I2C | SDA (A4), SCL (A5) | Uses `LiquidCrystal_I2C` library |
| Power | 5V / GND | Common ground |

---

### EEPROM Layout
| Address | Description |
|----------|--------------|
| 0 | Magic number (42) for validation |
| 1 | Unit preference: `0` = Celsius, `1` = Fahrenheit |

---

### Timer Configuration
- **Timer1** configured in **CTC mode**
- Compare value (`OCR1A = 31249`) gives ~500 ms period  
  Formula:  
  ```
  (16 MHz) / (256 * 2 Hz) - 1 = 31249
  ```
- Interrupt: `ISR(TIMER1_COMPA_vect)` sets `tempFlag = true`

---

### ISR Roles
- **Timer ISR:** triggers periodic temperature read  
- **Button ISR:** toggles unit and saves to EEPROM  

---

### Build Steps
1. Install **LiquidCrystal_I2C** and **EEPROM** libraries  
2. Wire the components as in the table above  
3. Upload the `.ino` file  
4. Open the Serial Monitor (optional, for debugging)  
5. Power cycle the board — the last used unit should persist  