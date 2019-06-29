### Low-Power
Lightweight low power library for Arduino.

- Version: 1.80
- Date: 04-10-2018

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

#### Notes:
External interrupt during standby on ATSAMD21G18A requires a patch to the [Arduino SAMD Core](https://github.com/arduino/ArduinoCore-samd) in order for it to work. Fix is provided by this particular [pull request](https://github.com/arduino/ArduinoCore-samd/pull/90).
