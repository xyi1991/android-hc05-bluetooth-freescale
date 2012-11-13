/*
 *  TPM1.c is part of testBluetoothFreescale.
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
#include "TPM1.h"
#include "types.h"

#define SECONDS_PER_DAY   			86400UL
#define TICS_PER_DAY ( SECONDS_PER_DAY * TIMER_TICKS_PER_SECOND )
#define MSECS_PER_SECOND   			1000UL
#define TIMER_TICKS_PER_SECOND    	100UL

/* File scope global vars located in zero page */
static volatile dword TimerTicks;         /* Timer ticks counter */

/* Internal functions */
static dword getTimerTicks( void );

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : initTPM1
** Description   : Timer ticks init
** Input         : Nothing
** Output        : Nothing
** Notes         : Time out is 10 ms
----------------------------------------------------------------------------------------------------------------------------------*/
void initTPM1(void) {

	/* TPM1SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=0,PS2=0,PS1=0,PS0=0 */
	TPM1SC = 0x00; 

	/* TPM1C0SC: CH0F=0,CH0IE=1,MS0B=0,MS0A=1,ELS0B=0,ELS0A=0,??=0,??=0 */
	TPM1C0SC = 0x50; 
	TPM1C0V = ( word ) 0xEA5FU;
	TPM1MOD = ( word ) 0xEA5FU;

	/* TPM1CNTH: BIT15=0,BIT14=0,BIT13=0,BIT12=0,BIT11=0,BIT10=0,BIT9=0,BIT8=0 */
	TPM1CNTH = 0x00;  

	/* TPM1SC: TOF=0,TOIE=0,CPWMS=0,CLKSB=0,CLKSA=1,PS2=0,PS1=1,PS0=0 */
	TPM1SC = 0x0A; 
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : SetTimerEvent
** Description   : Init timer tick event
** Input         : Event and desired time
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void setTimerEvent( tTimerTime *event, dword miliSeconds ) {
	dword Tics = ( miliSeconds * TIMER_TICKS_PER_SECOND ) / MSECS_PER_SECOND;

	/* The largest accepted timer event is 24 hours */
	if( Tics > TICS_PER_DAY )   {
		Tics = TICS_PER_DAY;
	}

	event->Start = getTimerTicks();
	event->Expiration = event->Start + Tics;
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : TimerHasExpired
** Description   : Check if one timer has expired
** Input         : The timer
** Output        : TRUE is timer has expired
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
bool TimerHasExpired( tTimerTime *Event ) {
	dword CurrentTics;

	/* get current tics; assume timer has expired */
	CurrentTics = getTimerTicks();

	/* Check normal expiration */
	if ( CurrentTics > Event->Expiration )  {
		return( TRUE );
	}

	/* Check wrapped CurrentTics */
	if (( CurrentTics < Event->Start ) && (( CurrentTics + TICS_PER_DAY ) > Event->Expiration ))  {
		return( TRUE );
	}

	return ( FALSE );
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : GetTimerTicks
** Description   : Return the number of timer ticks
** Input         : Nothing
** Output        : Timer ticks
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
dword getTimerTicks( void ) {
	return( TimerTicks );
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : getMillis
** Description   : Return the number of milliseconds since the program started (unsigned long)
** Input         : Nothing
** Output        : milliseconds since the program started 
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
dword getMillis(void) {
	return ((getTimerTicks() * MSECS_PER_SECOND) / TIMER_TICKS_PER_SECOND);
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : TimerTicks_Interrupt
** Description   : Interrupt handler for TIM0 
** Input         : Nothing
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void __interrupt VectorNumber_Vtpm1ch0 TimerTicks_Interrupt( void ) {
	/* ACK channel interrupt */
	TPM1C0SC_CH0F = 0;

	/* Update internal timer tick counter */
	TimerTicks ++;
}
