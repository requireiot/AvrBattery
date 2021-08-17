/**
 * @file 		  AvrBattery.cpp
 *
 * @author		: Bernd Waldmann
 * Creation Date: 17-Jan-2021
 * Tabsize		: 4
 *
 * This Revision: $Id: AvrBattery.cpp 1232 2021-08-12 13:32:17Z  $
 *
 * @brief Measure AVR supply voltage, typically a battery
 */

/*
   Copyright (C)2021 Bernd Waldmann

   This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. 
   If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/

   SPDX-License-Identifier: MPL-2.0
*/

#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>

#include "AvrBattery.h"

// for ATmega324
#ifdef PRR0
#define PRR PRR0
#endif

#if     ((F_CPU /  2L) < 200000uL)
 #define ADC_PRESCALER 1
#elif ((F_CPU /  4L) < 200000uL)
 #define ADC_PRESCALER 2
#elif ((F_CPU /  8L) < 200000uL)
 #define ADC_PRESCALER 3
#elif ((F_CPU / 16L) < 200000uL)
 #define ADC_PRESCALER 4
#elif ((F_CPU / 32L) < 200000uL)
 #define ADC_PRESCALER 5
#elif ((F_CPU / 64L) < 200000uL)
 #define ADC_PRESCALER 6
#else 
 #define ADC_PRESCALER 7
#endif

/**
 * @brief Measure VCC. 
 * 
 * @return uint16_t ... supply voltage in mV
 */
uint16_t AvrBattery::measureVCC()
{
	uint16_t result;
	uint8_t savePRR = PRR;
	uint8_t saveADCSRA = ADCSRA;
	
	PRR &= ~_BV(PRADC);
	ADCSRA |= _BV(ADEN);

	// measure bandgap reference, following Atmel ApplicationNote AVR126

	// 1. measure GND
	ADMUX	= (1 << REFS0) 	// ref 1 = AVCC
			| (15 << MUX0)	// channel 15 = GND
			;
	ADCSRA  = (1 << ADEN)
            | (1 << ADSC)
            | (ADC_PRESCALER << ADPS0)
            ;
	while (ADCSRA & _BV(ADSC)) {}      // 25 clocks = 125 us
	result = ADC;

	// 2. measure bandgap reference
    ADMUX	= (1 << REFS0) 	// ref 1 = AVCC
			| (14 << MUX0)	// channel 14 = 1.1V bandgap
			;
	ADCSRA  = (1 << ADEN)
            | (ADC_PRESCALER << ADPS0)
            ;
    /* 
	 * Refer datasheet section 'Electrical characteristics
	 * -> System and Reset Characteristics'
	 * for the Bandgap Reference start up time (70us)
     *
     * based on practical measurements: need to wait 350us
	 */
	_delay_us(350);
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC)) {}       // 13 clocks = 65 us
	result = ADC;          // total time: 125+350+65us = 0.5 ms
	ADCSRA = saveADCSRA;
	PRR = savePRR;

	// return Vcc in mV
	// 1023 for VCC mV, ADC for 1100mV, so (ADC/1023) * VCC == 1100 so VCC = 1100 * 1023 / ADC
	result = (1023L * 1100L) / result;
	return result;
}


/**
 * @brief  calculate VCC in % of useable battery range
 * @param  voltage  VCC in mV 
 * @param  vmin     lower limit of battery range [mV], default 2000 mV
 * @param  vmax     upper limit of battery range [mV], default 3300 mV for Li battery
 * @return  VCC in percent
 */
uint8_t AvrBattery::calcVCC_Percent(uint16_t voltage, uint16_t vmin, uint16_t vmax)
{
	int32_t percent = ((voltage - vmin) * 100L) / (int32_t)(vmax-vmin);
	if (percent < 0) percent=0;
	if (percent > 100L) percent=100L;
	return percent;
}
