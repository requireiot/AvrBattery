# AvrBattery
Measure ATmega supply voltage without external resistors

This uses the well-known technique to measre the AVR supply voltage by measuring the 1.1V band-gap reference built into the ATmega chips, with supply voltage as the voltage reference. 

There are other, similar Arduino libraries. This one does not use floating point calculations, to save program memory.

Call `AvrBattery::measureVCC()`to get the supply voltage in millivolts. This static method will turn on the ADC before measurement, and restore the state of the ADC on/off before returning.

Call `calcVCC_Percent(uint16_t voltage, uint16_t vmin=VCC_MIN, uint16_t vmax=VCC_MAX)` to convert the voltage (in millivolts) into percent of useable battery range, default 2.0 to 3.0V. Define teh `VCC_MIN` and `VCC_MAX` constants before including `AvrBattery.h` if you want a different  range.
