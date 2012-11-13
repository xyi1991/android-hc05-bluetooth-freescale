/*
 *  CPU.c is part of testBluetoothFreescale.
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
#include "CPU.h"

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : initCPU
** Description   : Inicializa clock y perifericos
** Input         : Nothing
** Output        : Nothing
** Notes         : 
----------------------------------------------------------------------------------------------------------------------------------*/
void initCPU(void){
  initSystemOptions();
  initSystemClockOptions();
  initMCGPeripheral();
  initIOPorts();
  
  /* sci1_rx = interrupt source 18 = vector 82 = level 3, priority 6 */
  /* Setting INTC_PL6P7 to 18 (0x12), remaps sci1_rx as level 6, priority 7. */
  INTC_PL6P7 = 0x12;
}

static void initSystemOptions(void){	
	//SOPT1  = 0x30;  //COP disabled, STOP and WAIT enabled
	//SOPT2  = 0x86;  //Bus Clock is source to COP, input filters on SPI1 and SPI2 port pins enabled
	
	SOPT1_COPT=0;
	SOPT2  &= ~(1<<5); /* MUST clear bit 5 to use little endian for memory access */
}

static void initSystemClockOptions(void){
	//All peripherals of the microcontroller receive the bus clock
	SCGC1 = 0xFF;       
	SCGC2 = 0xFF;
	SCGC3 = 0xFF; 
}

static void initMCGPeripheral(void){
	/* Assume 12MHz external clock source connected. */

	/* In order to use the USB we need to enter PEE mode and MCGOUT set to 48 MHz. 
	     Out of reset MCG is in FEI mode. */

	/**** Moving from FEI (FLL engaged internal) to PEE (PLL engaged external) mode. */ 
	/* switch from FEI to FBE (FLL bypassed external) */ 
	/* enable external clock source */
	MCGC2 = MCGC2_HGO_MASK       /* oscillator in high gain mode */
	| MCGC2_EREFS_MASK   /* because crystal is being used */
	| MCGC2_RANGE_MASK   /* 12 MHz is in high freq range */
	| MCGC2_ERCLKEN_MASK;     /* activate external reference clock */
	while (MCGSC_OSCINIT == 0)
		;
	/* select clock mode */
	MCGC1 = (2<<6)         /* CLKS = 10 -> external reference clock. */
	| (3<<3);      /* RDIV = 3 -> 12MHz/8=1.5 MHz */

	/* wait for mode change to be done */
	while (MCGSC_IREFST != 0)
		;
	while (MCGSC_CLKST != 2)
		;

	/* switch from FBE to PBE (PLL bypassed internal) mode */
	MCGC3=MCGC3_PLLS_MASK
	| (8<<0);     /* VDIV=6 -> multiply by 32 -> 1.5MHz * 32 = 48MHz */
	while(MCGSC_PLLST != 1)
		;
	while(MCGSC_LOCK != 1)
		;
	/* finally switch from PBE to PEE (PLL enabled external mode) */
	MCGC1 = (0<<6)         /* CLKS = 0 -> PLL or FLL output clock. */
	| (3<<3);      /* RDIV = 3 -> 12MHz/8=1.5 MHz */
	while(MCGSC_CLKST!=3)
		;

	/* Now MCGOUT=48MHz, BUS_CLOCK=24MHz */
}

static void initIOPorts(void){
	// Inicializa PORT A para evitar GLITCHES
	PTAD = 0x00;
	// Configura PORT A
	// PTA0 Card Detect SD/MMC                          Entrada     28
	// PTA1 NC                                          Salida "0"  29
	// PTA2 NC                                          Salida "0"  30
	// PTA3 USB PULLUP Host                             Entrada     31
	// PTA4 USB Device                                  Entrada     32
	// PTA5 USB DN DOWN Device                          Entrada     31
	// PTA6 No existe pata asignada                     Entrada    
	// PTA7 No existe pata asignada                     Entrada    
	PTADD = 0x06;

	// Inicializa PORT B para evitar GLITCHES
	PTBD = 0x00;
	// Configura PORT B
	// PTB0 NC				                            Salida "0"  34
	// PTB1 NC                                          Salida "0"  35
	// PTB2 NC                                          Salida "0"  36
	// PTB3 NC                                          Salida "0"  37
	// PTB4 NC                                          Salida "0"  38
	// PTB5 NC                                          Salida "0"  39
	// PTB6 NC                                          Salida "1"  40
	// PTB7 NC                                          Salida "0"  41  
	PTBDD = 0xFF;

	// Inicializa PORT C para evitar GLITCHES
	PTCD = 0x00   ;
	// Configura PORT C
	// PTC0 NC                                          Salida "0"  60
	// PTC1 NC                                          Salida "0"  61
	// PTC2 NC                                          Salida "0"  62
	// PTC3 Tx2 RS232                                   Salida "0"  63
	// PTC4 NC                                          Salida "0"  1
	// PTC5 Rx2 RS232                                   Entrada     64
	// PTC6 NC                                          Salida "0"  9
	// PTC7 No existe pata asignada                     Entrada
	PTCDD = 0x5F; 

	// Inicializa PORT D para evitar GLITCHES
	PTDD = 0x38;
	// Configura PORT D
	// PTD0 NC                                          Salida "0"  42
	// PTD1 NC                                          Salida "0"  43
	// PTD2 NC                                          Salida "0"  48
	// PTD3 LED RGB                                     Salida "1"  49
	// PTD4 LED RGB                                     Salida "1"  50
	// PTD5 LED RGB                                     Salida "1"  51
	// PTD6 NC                                          Salida "0"  52
	// PTD7 NC                                          Salida "0"  53  
	PTDDD = 0xFF;

	// Inicializa PORT E para evitar GLITCHES
	PTED = 0x04;
	// Configura PORT E
	// PTE0 Tx1 Bluetooth                               Salida "0"  13
	// PTE1 Rx1 Bluetooth                               Entrada     14
	// PTE2 RESET HC-05                                 Salida "1"  15
	// PTE3 PIO11 HC05 BLUETOOTH                        Salida "0"  16
	// PTE4 MISO1  SD                                   Entrada     17
	// PTE5 MOSI1  SD                                   Salida "0"  18
	// PTE6 SPSCK1 SD                                   Salida "0"  19
	// PTE7 SS1    SD                                   Salida "0"  20
	PTEDD = 0xED;

	// Inicializa PORT F para evitar GLITCHES
	PTFD = 0x00;
	// Configura PORT F
	// PTF0 NC                                          Salida "0"  4
	// PTF1 NC                                          Salida "0"  5
	// PTF2 NC                                          Salida "0"  6
	// PTF3 NC                                          Salida "0"  7
	// PTF4 NC                                          Salida "0"  8
	// PTF5 NC                                          Salida "0"  11
	// PTF6 NC					                        Salida "0"  12
	// PTF7 NC                                          Salida "0"  10
	PTFDD = 0xFF;                

	// Inicializa PORT G para evitar GLITCHES
	PTGD = 0x00;
	// Configura PORT G
	// PTG0 NC                                          Salida "0"  26
	// PTG1 NC                                          Salida "0"  27
	// PTG2 NC       									Salida "0"  54
	// PTG3 NC       									Salida "0"  55
	// PTG4 XTAL                                        Entrada     57
	// PTG5 EXTAL                                       Entrada     58
	// PTG6 No existe pata asignada                     Entrada     
	// PTG7 No existe pata asignada                     Entrada     
	PTGDD = 0x0F;
}

/*----------------------------------------------------------------------------------------------------------------------------------
** Function Name : WatchDog_Clear
** Description   : Reset the wacht dog counter
** Input         : Nothing
** Output        : Nothing
** Notes         : Time out is 32 ms
----------------------------------------------------------------------------------------------------------------------------------*/
void WatchDog_Clear( void ) {
  /* Reset internal COP */
  SRS = 0x00;
}
