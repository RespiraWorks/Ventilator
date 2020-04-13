/* Copyright 2020, RespiraWorks

  This file is part of VentilatorSoftware.

  VentilatorSoftware is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  VentilatorSoftware is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with VentilatorSoftware.  If not, see <https://www.gnu.org/licenses/>.
*/

/*
 module contributors: verityRF
 The purpose of this module is to recreate fake versions of Arduino routines so that unit tests of the controller modules can be done on a x86 host.
 This module should never end up on an actual Arduino target.
*/

//@TODO: Write unit tests for this module

#include "ArduinoSim.h"
#include <stdlib.h>
#include <math.h>

const int ANALOG_SIGNAL_DEPTH = 256; //how (maximally) long are the simulated analog signals
const int NUM_ANALOG_CHANNELS = 4; //how many channels (analog pins) are supported
const float ADC_VREFP = 5.0f; //Arduino ADC VREF_P setting/value [V]
const float ADC_COUNTS_PER_VOLT = 1024.0f / ADC_VREFP; //10 bit ADC with given ADC VREF_P [Count/V]

static float analogSignalMemory[NUM_ANALOG_CHANNELS][ANALOG_SIGNAL_DEPTH];
static int analogSignalMemoryIndexes[NUM_ANALOG_CHANNELS];

void delay(unsigned long count) {
	//method to remain unimplemented; isn't useful from a unit test perspective
}

int getSignalChannelDepth() {
	return ANALOG_SIGNAL_DEPTH;
}

void resetAnalogMemoryIndexes() {
	for (int i = 0; i < NUM_ANALOG_CHANNELS; i++) {
		analogSignalMemoryIndexes[i] = 0;
	}
}

void createDynamicAnalogSignal(int pin, float *buffer, int count) {
	if (buffer == NULL) {
		throw "Input buffer pointer is null";
	}
	if (count > ANALOG_SIGNAL_DEPTH) {
		throw "Input count greater than analog memory depth";
	}
	//copy input buffer to corresponding analog pin memory, constraining the allowable voltage on the pin
	for (int i = 0; i < count; i++) {
		float currentValue = buffer[i];
		if (currentValue < 0.0f) {
			currentValue = 0.0f;
		}
		if (currentValue > ADC_VREFP) {
			currentValue = ADC_VREFP;
		}
		analogSignalMemory[pin][i] = currentValue;
	}
}

void createStaticAnalogSignal(int pin, float val) {
	if (val < 0.0f) {
		val = 0.0f;
	} 
	if (val > ADC_VREFP) {
		val = ADC_VREFP;
	}
	for (int i = 0; i < ANALOG_SIGNAL_DEPTH; i++) {
		analogSignalMemory[pin][i] = val;
	}
}

int analogRead(int pin) {
	if (pin < 0 || pin > (NUM_ANALOG_CHANNELS - 1)) {
		throw "Analog pin is out of bounds";
	}
	
	int output = (int)roundf((analogSignalMemory[pin][analogSignalMemoryIndexes[pin]] * ADC_COUNTS_PER_VOLT));
	//rollover the index if necessary
	if (analogSignalMemoryIndexes[pin] == (ANALOG_SIGNAL_DEPTH - 1)) {
		analogSignalMemoryIndexes[pin] = 0;
	}
	else {
		analogSignalMemoryIndexes[pin] += 1;
	}
	//output constrained to 10 bits
	return output & 0x3FF;
}