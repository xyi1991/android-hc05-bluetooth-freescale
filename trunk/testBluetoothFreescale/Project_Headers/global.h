/*
 *  global.h is part of testBluetoothFreescale.
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

#ifndef DEFINICIONES_H_
#define DEFINICIONES_H_

#include "derivative.h"

/* Communications buffers sizes */
#define SERIAL_BUFFER_SIZE      80
#define MISC_BUFFER_SIZE        80

/* LED RGB */
#define LED_RGB_ROJO 		PTDD_PTDD3
#define LED_RGB_VERDE 		PTDD_PTDD4
#define LED_RGB_AZUL		PTDD_PTDD5

/* True False*/
#define TRUE 	1
#define FALSE 	0

/* Led manage */
typedef enum {
	led_on,
	led_off
} tLedType;

typedef union {
	byte SerialBuffer[SERIAL_BUFFER_SIZE];
	byte MiscBuffer[MISC_BUFFER_SIZE];
}tOverlayBuffer;

/* Manage the Led */
void setLed( tLedType LedType );

/* Manage the Led */
void setSystemLedIndicator( tLedType LedType );

/* Parpadeo led bluetooth */
void blinkingSystemthLed(void) ;

/* Inicializa todas las variables a cero*/
void initVariables(void);

/* This macro define behaviour of the storage declaration link */
#ifdef __FORCE_STORAGE_LINK__
  #define _STORAGE_LINK
#else
  #define _STORAGE_LINK extern
#endif

/* Constant Area */





#endif /* DEFINICIONES_H_ */
