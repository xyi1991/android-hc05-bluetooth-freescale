/*
 *  CPU.h is part of testBluetoothFreescale.
 *
 *  testBluetoothFreescale is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  testBluetoothFreescale is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with testBluetoothFreescale.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPU_H_
#define CPU_H_

/* Init the MCU */
void initCPU(void);

static void initSystemOptions(void);
static void initSystemClockOptions(void);
static void initMCGPeripheral(void);
static void initIOPorts(void);

/* Reset watch dog */                            
void WatchDog_Clear( void );


#endif /* CPU_H_ */
