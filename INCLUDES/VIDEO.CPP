#include <dos.h>
//
// Fonctions vid‚o utilisant le BIOS.
// - Pas besoin d'utiliser un BGI
// - Ne fonctionne que pour le 320x200 en 256 couleurs
// - Economise plus de 8ko de m‚moire compar‚ … un BGI
//

/* Setvgapalette256 sets the entire 256 color palette */
/* PalBuf contains RGB values for all 256 colors      */
/* R,G,B values range from 0 to 63	              */
/* Usage:					      */
/*  DacPalette256 dac256;			      */
/*						      */
/* setvgapalette256(&dac256);			      */
void change_palette(unsigned char* PalBuf)
{
	struct REGPACK reg;

	reg.r_ax = 0x1012;
	reg.r_bx = 0;
	reg.r_cx = 256;
	reg.r_es = FP_SEG(PalBuf);
	reg.r_dx = FP_OFF(PalBuf);
	intr(0x10,&reg);
}

//
// Fonction 0Fh de l'INT 10h
// -> Renvoie en AL la r‚solution courante
//
char get_rez()
{
	struct REGPACK reg;
	reg.r_ax = 0x0F00;
	intr(0x10,&reg);
	return(char(reg.r_ax & 0x00ff));
}


//
// Fonction 00h de l'INT 10h
// -> Mettre en AL la r‚solution … installer
//
void change_rez(char rez)
{
	struct REGPACK reg;
	reg.r_ax = (0x0000 | (unsigned int)rez);
	intr(0x10,&reg);
}

