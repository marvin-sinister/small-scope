//-----------------------------------------------------------------------------
// Interface.cpp
//-----------------------------------------------------------------------------
// Copyright 2015 Marvin Sinister
//
// This file is part of small-scope.
//
//	small-scope is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	small-scope is distributed in the hope that it will be useful,
//	but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with small-scope.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include "small-scope.h"

//-----------------------------------------------------------------------------
void error (void) {
	digitalWrite( errorPin, HIGH );
	delay( 200 );
	digitalWrite( errorPin, LOW );
}

//-----------------------------------------------------------------------------
// fillBuffer
//-----------------------------------------------------------------------------
// Fills the given buffer with bufferSize chars from a Serial object

void fillBuffer( char *buffer, byte bufferSize, HardwareSerial* serial )
{
	// Clean buffer
	memset( (void *)buffer, '\0', sizeof(char) * bufferSize );

	dprint(serial->available());
	
	byte limit = ( bufferSize < serial->available() ) ? bufferSize : serial->available();

	dprint(serial->available());
	dprint(bufferSize);
	dprint(limit);

	// Fill buffer
	for ( byte i = 0; i < limit; i++ ) {
		dprint(serial->available());
		dprint(i);
		dprint(buffer);

		buffer[i] = serial->read();
	}
}

void printStatus( void )
{
	Serial.print("Buffer size: ");
	Serial.println(ADCBUFFERSIZE);
	Serial.print("Baud rate: ");
	Serial.println(BAUDRATE);
	Serial.print("Wait duration: ");
	Serial.println(waitDuration);
	Serial.print("Prescaler: ");
	Serial.println(prescaler);
	Serial.print("Trigger event: ");
	Serial.println(triggerEvent);
	Serial.print("Threshold: ");
	Serial.println(threshold);
}
