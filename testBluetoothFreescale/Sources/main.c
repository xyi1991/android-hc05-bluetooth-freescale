/*
 *  main.c is part of testBluetoothFreescale.
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

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#include "CPU.h"
#include "SCI1.h"
#include "TPM1.h"
#include "global.h"

/* evento que hace parpadear el led que no hay conexion bluetooth */
tTimerTime eventBlinkingLedSistema;

dword timeLED = 1000;

void main(void) {
	initCPU();
	initSCI1(115200);
	initTPM1();
	EnableInterrupts;
	setTimerEvent(&eventBlinkingLedSistema, timeLED);
	/* include your code here */
	for(;;) {
		if (TimerHasExpired(&eventBlinkingLedSistema)) {
			LED_RGB_AZUL = !LED_RGB_AZUL;
			setTimerEvent(&eventBlinkingLedSistema, timeLED);
		}
		if (sci1GetAvailablesChars()) {
			byte rcvChar;
			sci1RecvChar(&rcvChar);
			if (rcvChar == '+') {
				LED_RGB_VERDE = 0;
				timeLED += 10;
				LED_RGB_VERDE = 1;
			} else if(rcvChar == '-') {
				LED_RGB_ROJO = 0;
				timeLED -= 10;
				LED_RGB_ROJO = 1;
			} else {
				// descarto el caracter recibido
			}			
		}
		__RESET_WATCHDOG();	/* feeds the dog */
	} /* loop forever */
	/* please make sure that you never leave main */
}
