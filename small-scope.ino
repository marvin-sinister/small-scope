//-----------------------------------------------------------------------------
// small-scope.ino
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
// Global Constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

volatile uint16_t waitDuration;
volatile  int16_t waitRemaining;
volatile uint16_t stopIndex;
volatile uint16_t triggerIndex;
volatile uint16_t ADCCounter;
volatile  uint8_t ADCBuffer[ADCBUFFERSIZE];
volatile  boolean freeze;

          uint8_t prescaler;
          uint8_t triggerEvent;
          uint8_t threshold;

             char commandBuffer[COMBUFFERSIZE+1];

//-----------------------------------------------------------------------------
// Main routines
//-----------------------------------------------------------------------------
//
// The setup function initializes registers.
//
void setup (void) {		// Setup of the microcontroller
	// Open serial port with a baud rate of BAUDRATE b/s
	Serial.begin(BAUDRATE);

	dshow("# setup()");
	// Clear buffers
	memset( (void *)ADCBuffer, 0, sizeof(ADCBuffer) );
	memset( (void *)commandBuffer, 0, sizeof(commandBuffer) );
	ADCCounter = 0;

	waitDuration = 256;
	stopIndex = -1;
	freeze = false;

	prescaler = 32;
	triggerEvent = 2;

	threshold = 128;

	// Activate interrupts
	sei();

	initPins();
	initADC();
	initAnalogComparator();
}

void loop (void) {
	dprint(ADCCounter);
	dprint(stopIndex);
	dprint(wait);
	dprint(freeze);
	#if DEBUG == 1
	Serial.println( ADCSRA, BIN );
	Serial.println( ADCSRB, BIN );
	#endif

	// If freeze flag is set, then it is time to send the buffer to the serial port
	if ( freeze )
	{
		dshow("# Frozen");

		//ADCBuffer[triggerIndex] = 0;
		//ADCBuffer[stopIndex] = 255;

		// Send the buffer to serial
		Serial.write( (uint8_t *)ADCBuffer + stopIndex, ADCBUFFERSIZE - stopIndex );
		Serial.write( (uint8_t *)ADCBuffer, stopIndex );

		freeze = false;

		// Time to prebuffer the next frame
		waitRemaining = ADCBUFFERSIZE - waitDuration;


		startADC();
		startAnalogComparator();

		#if DEBUG == 1
		delay(3000);
		#endif
	}

	if ( Serial.available() > 0 ) {
		// Read the incoming byte
		char theChar = Serial.read();
			// Parse character
		switch (theChar) {
			case 's':			// 's' for starting ADC conversions

				// Clear buffer
				memset( (void *)ADCBuffer, 0, sizeof(ADCBuffer) );

				startADC();
				startAnalogComparator();
				break;
			case 'S':			// 'S' for stopping ADC conversions
				stopAnalogComparator();
				stopADC();
				break;
			case 'p':			// 'p' for new prescaler setting
			case 'P': {
				// Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
				delay(COMMANDDELAY);

				fillBuffer( commandBuffer, COMBUFFERSIZE );

				// Convert buffer to integer
				uint8_t newP = atoi( commandBuffer );

				prescaler = newP;
				setADCPrescaler(newP);
				}
				break;

			case 'r':			// 'r' for new voltage reference setting
			case 'R': {
				// Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
				delay(COMMANDDELAY);

				fillBuffer( commandBuffer, COMBUFFERSIZE );

				// Convert buffer to integer
				uint8_t newR = atoi( commandBuffer );

				setVoltageReference(newR);
				}
				break;

			case 'e':			// 'e' for new trigger event setting
			case 'E': {
				// Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
				delay(COMMANDDELAY);

				fillBuffer( commandBuffer, COMBUFFERSIZE );

				// Convert buffer to integer
				uint8_t newE = atoi( commandBuffer );

				triggerEvent = newE;
				setTriggerEvent(newE);
				}
				break;

			case 'w':			// 'w' for new wait setting
			case 'W': {
				// Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
				delay(COMMANDDELAY);

				fillBuffer( commandBuffer, COMBUFFERSIZE );

				// Convert buffer to integer
				uint8_t newW = atoi( commandBuffer );

				waitDuration = newW;
				}
				break;

			case 't':			// 't' for new threshold setting
			case 'T': {
				// Wait for COMMANDDELAY ms to be sure that the Serial buffer is filled
				delay(COMMANDDELAY);

				fillBuffer( commandBuffer, COMBUFFERSIZE );

				// Convert buffer to integer
				uint8_t newT = atoi( commandBuffer );

				threshold = newT;
				analogWrite( thresholdPin, threshold );
				}
				break;

			case 'd':			// 'd' for display status
			case 'D':
				printStatus();
				break;

			default:
				error();
		}
	}
}
