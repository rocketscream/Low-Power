### Low-Power
Lightweight low power library for Arduino.

Version: 1.90

Date: 11-01-2020

Devices Supported:
* ATMega88
* ATMega168
* ATMega168P
* ATMega328P
* ATMega32U4
* ATMega644P
* ATMega1284P
* ATMega2560
* ATMega256RFR2
* ATSAMD21G18A
* ATtiny24
* ATtiny24A
* ATtiny44
* ATtiny44A
* ATtiny84
* ATtiny84A
* ATtiny25
* ATtiny45
* ATtiny85
* ATtiny261
* ATtiny461
* ATtiny461A
* ATtiny861
* ATtiny861A
* ATtiny43U
* ATtiny87
* ATtiny167
* All other MCU types for which at least WDT\_vect or WDT\_OVERFLOW\_vect
  are defined, and which support at least 'idle' and 'power down' sleep modes.

####Notes:
External interrupt during standby on ATSAMD21G18A requires a patch to the <a href="https://github.com/arduino/ArduinoCore-samd">Arduino SAMD Core</a> in order for it to work. Fix is provided by this particular <a href="https://github.com/arduino/ArduinoCore-samd/pull/90">pull request</a>.
