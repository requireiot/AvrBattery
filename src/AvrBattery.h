/**
 * @file          AvrBattery.h
 *
 * Author		: Bernd Waldmann
 * Creation Date: 17-Jan-2021
 * Tabsize		: 4
 *
 * This Revision: $Id: AvrBattery.h 1256 2021-09-28 14:35:36Z  $
 */

/*
   Copyright (C) 2017,2021 Bernd Waldmann

   This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. 
   If a copy of the MPL was not distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/

   SPDX-License-Identifier: MPL-2.0
*/

#ifndef __AVRBATTERY_H
#define __AVRBATTERY_H

#include <stdint.h>
#include <limits.h>


#ifndef VCC_MIN
	#define VCC_MIN 2000L
#endif
#ifndef VCC_MAX
	#define VCC_MAX 3000L
#endif


class AvrBattery {
public:
	static uint16_t measureVCC();
	static uint8_t calcVCC_Percent(
		uint16_t voltage, 
		uint16_t vmin=VCC_MIN, 
		uint16_t vmax=VCC_MAX
	);
	static uint8_t measureVCC_Percent(
		uint16_t vmin=VCC_MIN, 
		uint16_t vmax=VCC_MAX
	) { return calcVCC_Percent( measureVCC(), vmin, vmax ); }
};

#endif  // __AVRBATTERY_H
