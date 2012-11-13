/*
 *  SCI1.h is part of testBluetoothFreescale.
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

#ifndef SER_H_
#define SER_H_

#define OVERRUN_ERR              0x01 
#define COMMON_ERR               0x02 
#define CHAR_IN_RX               0x04 
#define FULL_RX                  0x08 

#define ERR_OK           0            /* OK */
#define ERR_RXEMPTY      1            /* No data in receiver. */
#define ERR_TXFULL       2            /* Transmitter is full. */
#define ERR_OVERRUN      3            /* Overrun error is detected. */
#define ERR_FRAMING      4            /* Framing error is detected. */
#define ERR_PARITY       5            /* Parity error is detected. */
#define ERR_NOISE        6            /* Noise error is detected. */
#define ERR_COMMON       5            /* Common error of a device. */

#define SCI_INP_BUF_SIZE           80
#define SCI_OUT_BUF_SIZE           80

/* Valid buffer size value are only power of 2 (ex: 2,4,..,64,128) */
#define SER_BUFFER_SIZE				80		
#define SER_FIFO_MASK 		(SER_BUFFER_SIZE-1)


/* Inicializa el puerto serie 1 */
void initSCI1(unsigned long baudRate);

/* Envia un byte por el puerto serie */
void sci1SendByte(unsigned char byte);

/* Envia un string por el puerto serie */
void sci1SendString(unsigned char *string);

/* Get chars in SCI input buffer */
byte sci1GetAvailablesChars( void );

/* Si recibio un \n devuelve TRUE otra cosa FALSE */
byte sci1IsCharacterReceiverNewLine (void);

/* Borra el flag que acusa caracter recibido es un \n*/
void sci1SetCharacterReceiverNewLine(byte isNewLine);

/* SCI Receive block */
void sci1ReceiveBlock( byte *Ptr, byte Size, byte *Received );

/* SCI clear receive buffer */
void sci1ClearRxBuf( void );

byte sci1RecvChar( byte *Chr );

#endif /* SER_H_ */
