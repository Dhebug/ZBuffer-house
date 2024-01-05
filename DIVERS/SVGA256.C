/*******************************/
/*                             */
/*         SVGA256.C           */
/*                             */
/* VGA 640*400 en 256 couleurs */
/*                             */
/*******************************/

#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include "svga256.h"

#define RESOLUTION_X		640
#define RESOLUTION_Y		400
#define MODE_VIDEO			1

DacPalette256 dac256;

// on met la palette "… jour" avec ce qui il y a dans
// le tableau pass‚ en param‚tre
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

/************************************************************/
/* Active le mode 0x13 et initialise la palette de couleurs */
/************************************************************/

int huge DetectVGA256()
{
	return NULL;
}

void EcranGraphique()
{
	int Gd = DETECT ,Gm = MODE_VIDEO;
	installuserdriver("Svga256",DetectVGA256);
	initgraph(&Gd,&Gm,"e:\\ig\\");
}

/**************************/
/* Retour … l'‚cran texte */
/**************************/

void EcranTexte()
{
	closegraph();
}

/***************************************************/
/* Get_Pixel d'une image affich‚e en mode SVGA     */
/* 256 couleurs Retourne une couleur 32 bits       */
/***************************************************/

Color32 Get_Pixel(int x , int y)
//	int x, y;		/* Coordonn‚es du pixel … lire */
{
	int      couleur;
	Color32  color = {0, 0, 0, 0};

	if( (x >= 0) && (x < RESOLUTION_X) && (y >= 0) && (y < RESOLUTION_Y) )
	{
		/* Calcul de l'adresse du pixel (x,y) */
		couleur = getpixel(x,y);

		/* Conversion couleur 8 bits => couleur 32 bits */
		color.Rouge = dac256[couleur][0]*4;
		color.Vert  = dac256[couleur][1]*4;
		color.Bleu  = dac256[couleur][2]*4;
		color.Sup   = 0;
	}
	return(color);
}

/*********************************************************/
/* Initialise la palette pour avoir un d‚grad‚ de bleu   */
/* pour le fond, et un d‚grad‚ de blanc pour la sph‚re   */
/*********************************************************/

void Init_Palette()
{ int i;

	for(i=0;i<64;i++)
	{
	 dac256[i][0]=i;
	 dac256[i][2]=i;
	 dac256[i][1]=i;
	}
	for(i=64;i<128;i++)
	{
	 dac256[i][0]=dac256[i][1]=0;
	 dac256[i][2]=i-64;
	}
	setvgapalette256(&dac256);
}
