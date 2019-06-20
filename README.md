# Arduino Simple Clock

Simple desk clock project based on Arduino Mega.

### Features:
Showing time, date, temperature and humidity.

Possibility of setting date and time with physical buttons (Mode, Left and Right arrows).

Device distinguish short and long button press.

### Manual:
In basic mode (MODE_CLOCK) device displays current date, time, temperature and humidity.

Long press MODE button to enter settings (MODE_SETTINGS).

Use left and right arrows to move between different parameters (marked with cursor).

Use MODE short press to adjust values.

Use MODE long press to confirm and return to basic mode.


### Components used:
* Arduino Mega 2560
* LCD-1602 + I2C
* RTC Clock DS1302 – zegar RTC
* DHT-11 (3 pin)
* 3x Buttons
* 3x 220 Ω Resistor
* 1x 10k Ω Resistor
* Wires and breadboard


### Wiring

<p align="center">
  <img width="60%" height="60%" src="https://raw.githubusercontent.com/tburton12/arduinoSimpleClock/master/wiring.png">
</p>
