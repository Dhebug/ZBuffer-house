/************************************************/
/* 						*/
/*          SuperVGA BGI driver defines		*/
/*		Copyright (c) 1991		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

#include <dos.h>
#include "svga16.h"
#include "svga256.h"
#include "twk16.h"
#include "twk256.h"
#include "svga32k.h"

/* Getvgapalette16 gets the entire 16 color palette */
/* PalBuf contains RGB values for all 16 colors     */
/* R,G,B values range from 0 to 63	            */
/* Usage: 					    */ 
/*  DacPalette16 dac16;                             */
/*						    */
/*  getvgapalette(&dac16);			    */
void getvgapalette16(DacPalette16 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1017;
  reg.r_bx = 0;
  reg.r_cx = 16;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10,&reg);
}

/* Getvgapalette256 gets the entire 256 color palette */
/* PalBuf contains RGB values for all 256 colors      */
/* R,G,B values range from 0 to 63	              */
/* Usage:					      */
/*  DacPalette256 dac256;			      */
/*						      */
/* getvgapalette256(&dac256);			      */
void getvgapalette256(DacPalette256 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1017;
  reg.r_bx = 0;
  reg.r_cx = 256;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10,&reg);
}

/* Setvgapalette16 sets the entire 16 color palette */
/* PalBuf contains RGB values for all 16 colors     */
/* R,G,B values range from 0 to 63	            */
/* Usage: 					    */ 
/*  DacPalette16 dac16;                             */
/*						    */
/*  setvgapalette(&dac16);			    */
void setvgapalette16(DacPalette16 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1012;
  reg.r_bx = 0;
  reg.r_cx = 16;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10,&reg);
}

/* Setvgapalette256 sets the entire 256 color palette */
/* PalBuf contains RGB values for all 256 colors      */
/* R,G,B values range from 0 to 63	              */
/* Usage:					      */
/*  DacPalette256 dac256;			      */
/*						      */
/* setvgapalette256(&dac256);			      */
void setvgapalette256(DacPalette256 *PalBuf)
{
  struct REGPACK reg;

  reg.r_ax = 0x1012;
  reg.r_bx = 0;
  reg.r_cx = 256;
  reg.r_es = FP_SEG(PalBuf);
  reg.r_dx = FP_OFF(PalBuf);
  intr(0x10,&reg);
}
