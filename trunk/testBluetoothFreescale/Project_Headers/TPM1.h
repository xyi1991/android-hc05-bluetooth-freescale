/*
 *  TPM1.h is part of testBluetoothFreescale.
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

#ifndef TPM1_H_
#define TPM1_H_

#include "types.h"

/* Timer events */
typedef struct {
  dword Start;
  dword Expiration;
}tTimerTime;

/* Timer ticks init */
void initTPM1(void);

/* Set time out timer event */
void setTimerEvent( tTimerTime *event, dword miliSeconds );

/* Check if timer event has expired */
bool TimerHasExpired( tTimerTime *Event );

/* Get a number of milliseconds since the program started (unsigned long) */
dword getMillis(void);

#endif /* TPM1_H_ */
