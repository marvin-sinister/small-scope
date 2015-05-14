//-----------------------------------------------------------------------------
// ISR.cpp
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
// ADC Conversion Complete Interrupt
//-----------------------------------------------------------------------------
ISR(ADC_vect)
{
	// When ADCL is read, the ADC Data Register is not updated until ADCH
	// is read. Consequently, if the result is left adjusted and no more
	// than 8-bit precision is required, it is sufficient to read ADCH.
	// Otherwise, ADCL must be read first, then ADCH.
	ADCBuffer[ADCCounter] = ADCH;

	// Incerase counter.
	if (++ADCCounter >= ADCBUFFERSIZE) ADCCounter = 0;
	
	// Wait for prebuffer to be filled.
	if (waitRemaining-- <= 0) {
		// When stop position is reached.
		if ( stopIndex == ADCCounter )
		{
			// Freeze situation
			// Disable ADC and stop Free Running Conversion Mode
			cbi( ADCSRA, ADEN );

			freeze = true;
		}
	}
}

//-----------------------------------------------------------------------------
// Analog Comparator interrupt
//-----------------------------------------------------------------------------
ISR(ANALOG_COMP_vect)
{

	// When wait is over disable Analog Comparator interrupt
	if (waitRemaining <= 0) cbi( ACSR,ACIE );


	// Save position of trigger and calculate the position of end of sample
	triggerIndex = ADCCounter;
	stopIndex = triggerIndex + waitDuration;
	if (stopIndex >= ADCBUFFERSIZE) stopIndex -= ADCBUFFERSIZE;
}
