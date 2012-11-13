/*
 *  types.h is part of testBluetoothFreescale.
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

#ifndef __TYPES_H
#define __TYPES_H

#include "derivative.h"

/* Undef for little endian selection */
#define BIG_ENDIAN

#if !defined ( __CWCC__ )
/* No include in CodeWarrior compiler */
  typedef unsigned char byte;
  typedef unsigned short word;
  typedef unsigned long dword;
#endif 

typedef unsigned char bool;

#ifndef FALSE
  #define  FALSE  0                    /* Boolean value FALSE. FALSE is defined always as a zero value. */
#endif
#ifndef TRUE
  #define  TRUE   1                    /* Boolean value TRUE. TRUE is defined always as a non zero value. */
#endif

#ifndef NULL
  #define NULL (( void * ) 0 )
#endif

#ifdef BIG_ENDIAN
/* Esay convertion dword-word-byte */
typedef struct 
{
  word hw;    
  word lw; 
} tW;

typedef struct
{
  byte bhh;     
  byte bhl;     
  byte blh;     
  byte bll;     
} tB;

typedef struct
{
  byte bh;     
  byte bl;     
} tBw;

#else
typedef struct 
{
  word lw;    
  word hw; 
} tW;

typedef struct
{
  byte bll;     
  byte blh;     
  byte bhl;     
  byte bhh;     
} tB;

typedef struct
{
  byte bl;     
  byte bh;     
} tBw;

#endif 

typedef union
{
  dword dw;     /* Dword access ( 32 bits ) */ 
  tW w;         /* Word access ( 16 bits )  */
  tB b;         /* Byte access ( 8 bits )   */
} tDWBNumber;

typedef union
{
  word w;       /* Word access ( 16 bits )  */ 
  tBw b;        /* Byte access ( 8 bits )   */ 
} tWBNumber;

/* Get High byte of the an word */
#define HI( Data )                                                \
           (( byte ) (((( word ) Data ) >> 8 ) & 0xFF ))          \

/* Get Low byte of an word */
#define LO( Data )                                                \
           (( byte ) ((( word )( Data )) & 0xFF ))                \

#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )

#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )


#endif 

