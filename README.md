# small-scope
single channel arduino ~150kHz oscilloscope

This is a single channel oscilloscope using arduino capable of ~150kHz sampling
rate on ATmega328 with 16Mhz crystal.

It is heavily based on Girino project by Cristiano Lino Fontana
(http://www.instructables.com/id/Girino-Fast-Arduino-Oscilloscope/).

This is the arduino code for the project, it samples the input and sends the
data over usb serial to computer.

It was tested on Arduino Uno and while a bit jittery it does work at ~150kHz.

There are still some things to be improved, especially interface with PC.