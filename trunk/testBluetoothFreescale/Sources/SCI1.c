/*
 *  SCI1.c is part of testBluetoothFreescale.
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

#include "derivative.h"
#include "SCI1.h"

static byte SerFlag;                      /* SCI internal status */
static byte InpBuffer[SCI_INP_BUF_SIZE];  /* Input buffer for SCI commmunication */
static byte SCI_InpLen;                   /* Length of the input buffer content */
static byte InpIndxR;                     /* Index for reading from input buffer */
static byte InpIndxW;                     /* Index for writing to input buffer */

static byte OutBuffer[SCI_OUT_BUF_SIZE];	/* Output buffer for SCI commmunication */
static byte OutIndxR;                     	/* Index for reading from output buffer */
static byte OutIndxW;                     	/* Index for writing to output buffer */

static byte isCharacterNewLine;

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : initSCI1
** Description   : Inicializa el puerto serie SCI1 utilizado para el modulo HC-05 Bluetooth
** Input         : baudrate deseado
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void initSCI1(unsigned long baudRate){
	unsigned long ulBaudRateDivisor = ( 24000000 / ( 16ul * baudRate ) );
	
	/* Reset flags */
	SerFlag = 0;                         
	/* No char in the receive buffer */
	SCI_InpLen = 0;
	/* Reset indices */
	InpIndxR = InpIndxW = 0;
	OutIndxR = OutIndxW = 0;
	
	isCharacterNewLine = 0;
	
	SCI1C2 = 0x00;                       /* Disable the SCI2 module */
	(void)(SCI1S1 == 0);                 /* Dummy read of the SCI2S1 register to clear flags */
	(void)(SCI1D == 0);                  /* Dummy read of the SCI2D register to clear flags */
	/* SCI2S2: LBKDIF=1,RXEDGIF=1,RXINV=0,RWUID=0,BRK13=0,LBKDE=0,RAF=0 */
	SCI1S2 = 0xC0;
	SCI1BD = (word)ulBaudRateDivisor;
	/* SCI2C1: LOOPS=0,SCISWAI=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
	SCI1C1 = 0x00;                                      
	/* SCI2C3: R8=0,T8=0,TXDIR=0,TXINV=0,ORIE=0,NEIE=0,FEIE=0,PEIE=0 */
	SCI1C3 = 0x00;                                      
	/* SCI2C2: TIE=0,TCIE=0,RIE=1,ILIE=0,TE=1,RE=1,RWU=0,SBK=0 */
	SCI1C2 = 0x2C;
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : sci1SendByte
** Description   : Send a byte
** Input         : Nothing
** Output        : byte to send
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void sci1SendByte(unsigned char byte) {
	
	while(!SCI1S1_TC){
		continue;
	}
	OutBuffer[OutIndxW] = byte;	// colocamos un caracter en el buffer

	//increase ring input pointer and set it to zer0 if 
	//it has rolled-it over.
	OutIndxW = (OutIndxW + 1) & SER_FIFO_MASK;		
	
	SCI1C2_TIE = 1;			// habilitamos la interrupcion de TX1
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : sci1SendString
** Description   : Send a string
** Input         : Nothing
** Output        : string to send
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void sci1SendString(unsigned char *string) {
	while(*string != '\n'){			// while pointer s is not at end of string
		sci1SendByte(*string++);	// send the current character, 
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : sci1GetAvailablesChars
** Description   : Get the availables chars in RX queue 
** Input         : Nothing
** Output        : Chars in queue
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
byte sci1GetAvailablesChars( void ) {
	return( SCI_InpLen );
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : sci1IsCharacterReceiverNewLine
** Description   : Retorna TRUE si el caracter recibido es un \n 
** Input         : Nothing
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
byte sci1IsCharacterReceiverNewLine(void) {
	return isCharacterNewLine;
}

void sci1SetCharacterReceiverNewLine(byte isNewLine) {
	isCharacterNewLine = isNewLine;
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : sci1ReceiveBlock
** Description   : Receive a block from SCI
** Input         : User buffer, buffer length and pointer to received chars count
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void sci1ReceiveBlock( byte *Ptr, byte Size, byte *Received ) {
	byte count;                          

	for ( count = 0; count < Size; count++ )   {
		switch ( sci1RecvChar( Ptr++ ))     {   
		/* No data in the buffer */ 
		case ERR_RXEMPTY:                  
			/* Return number of received chars */
			*Received = count;                     
			return;
		case ERR_COMMON:                   
			/* Return info that an error was detected */
			break;
		default:
			break;
		}
	}
	
	/* Borramos la variable que indica que se recibio un \n */
	isCharacterNewLine = 0;

	/* Return the number of received chars */
	*Received = count;                        
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : sci1ClearRxBuf
** Description   : Clean RX queue
** Input         : Nothing
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void sci1ClearRxBuf( void ) {
	/* Save the PS register */ 
	//SaveStatusReg();	//cuac                   

	/* Init vars */
	SCI_InpLen = 0;                     
	InpIndxR = InpIndxW = 0;             
	SerFlag &= ~ (CHAR_IN_RX | FULL_RX); 

	/* Restore the PS register */ 
	//RestoreStatusReg();	//cuac
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : sci1RecvChar
** Description   : Receive a char from SCI
** Input         : Pointer to char
** Output        : Result
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
byte sci1RecvChar( byte *Chr ) {
	byte Result = ERR_OK;                

	if ( SCI_InpLen > 0 )   { 
		/* Is number of received chars greater than 0? */

		/* Save the PS register */
		//SaveStatusReg(); cuac                   

		/* Decrease number of received chars */
		SCI_InpLen--;                     

		/* Received char */
		*Chr = InpBuffer[InpIndxR];        

		if ( ++InpIndxR >= SCI_INP_BUF_SIZE ) { 
			/* Is the index out of the buffer? */
			InpIndxR = 0;      
		}

		Result = ( byte )(( SerFlag & ( OVERRUN_ERR|COMMON_ERR|FULL_RX )) ? ERR_COMMON : ERR_OK );

		/* Clear all errors in the status variable */
		SerFlag &= ~( OVERRUN_ERR|COMMON_ERR|FULL_RX|CHAR_IN_RX ); 

		/* Restore the PS register */
		//RestoreStatusReg();	cuac                    
	} else {
		/* Receiver is empty */
		return ( ERR_RXEMPTY );                
	}

	return( Result );  
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : SCITx_Interrupt
** Description   : SCI Tx Interrupt handler 
** Input         : Nothing
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void __interrupt VectorNumber_Vsci1tx SCI1Tx_Interrupt(void){
	SCI1D = OutBuffer[OutIndxR];
	++OutIndxR;
	OutIndxR &= SER_FIFO_MASK;
	if (OutIndxR == OutIndxW) {
		SCI1C2_TIE = 0;
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : SCIRx_Interrupt
** Description   : SCI Rx Interrupt handler 
** Input         : Nothing
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void __interrupt VectorNumber_Vsci1rx SCI1Rx_Interrupt(void){
	byte StatReg = SCI1S1;
	byte Data;          

	/* Read data from the receiver */ 
	Data = SCI1D;                         

	if ( SCI_InpLen < SCI_INP_BUF_SIZE ) {
		/* Increse number of chars in the receive buffer */
	    SCI_InpLen++;                     

	    /* Save received char to the receive buffer */
	    InpBuffer[InpIndxW] = Data;   
	    
	    if(Data == '\n') {
	    	isCharacterNewLine = 1;
	    }

	    if ( ++InpIndxW >= SCI_INP_BUF_SIZE ) { 
	      /* Set the index to the start of the buffer */
	      InpIndxW = 0;                    
	    }
	} else  {
		/* If yes then set flag buffer overflow */                            
	    SerFlag |= FULL_RX;                
	}
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : SCIRX_Error_Interrupt
** Description   : Interrupt handler for SCI Rx Error
** Input         : Nothing
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void __interrupt VectorNumber_Vsci1err isrSCI1ERR( void ) {
	byte StatReg = SCI1S1;

	/* Dummy read of data register - clear error bits */ 
	( void )SCI1D;                          

	if ( StatReg & ( SCI1S1_OR_MASK|SCI1S1_NF_MASK|SCI1S1_FE_MASK|SCI1S1_PF_MASK )) { 
		/* If yes then set an internal flag */
		SerFlag |= COMMON_ERR;             
	}
}
