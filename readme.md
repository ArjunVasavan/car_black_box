# Car Black Box

A vehicle data logger built on a PIC18F4580 microcontroller. Records speed, gear, and time on every gear change. Stores up to 10 logs in external EEPROM with circular buffer overwrite.

---

## Hardware

| Component | Purpose |
|-----------|---------|
| PIC18F4580 | Microcontroller |
| DS1307 RTC | Real-time clock (I2C) |
| 24LC series EEPROM | Log storage (I2C) |
| 16x2 CLCD | Display |
| 3x4 Matrix Keypad | User input |
| Potentiometer | Speed input (ADC CH4) |
| UART | Serial log download |

---

## Features

- Live dashboard — time, gear (N/1-5/R), speed (0-100)
- Auto-logs speed + gear + time on every gear change
- Collision event detection (SW3)
- View logs on LCD (up to 10)
- Download logs via UART to PC terminal
- Set RTC time using keypad
- Clear all logs

---

## Key Bindings (Dashboard)

| Key | Action |
|-----|--------|
| SW1 | Gear up |
| SW2 | Gear down |
| SW3 | Collision event |
| SW11 | Open menu |

---

## EEPROM Layout

Each log = 5 bytes. Max 10 logs (50 bytes). Circular buffer wraps at byte 50.

```
Byte 0  → speed
Byte 1  → gear
Byte 2  → hours
Byte 3  → minutes
Byte 4  → seconds
...
Byte 100 → write index
Byte 101 → full flag
Byte 102 → init flag (0xBB)
```

---

## Project Structure

```
adc.c/h             ADC driver
i2c.c/h             I2C bus driver
ds1307.c/h          RTC driver
clcd.c/h            LCD driver
timer.c/h           Timer0 + ISR
matrix_keypad.c/h   Keypad driver
uart.c/h            UART driver
external_eeprom.c/h EEPROM driver
dashboard.c         Live display + gear + collision logic
menu.c              Menu, view/download/clear logs, set time
main.c              State machine + init
black_box.h         Shared defines, globals, function prototypes
```

---

## Build

Open `car_black_box.X` in MPLAB X IDE with XC8 compiler. Set target device to PIC18F4580 at 20 MHz.

