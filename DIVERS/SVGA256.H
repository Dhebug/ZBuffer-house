//
// SVGA256.H
// ---------
//

typedef unsigned char DacPalette256[256][3];

typedef struct
{
	char Rouge;
	char Vert;
	char Bleu;
	char Sup;
}Color32;

extern int far _Cdecl Svga256_fdriver[];

#ifndef __MODE19_H
#define __MODE19_H

int huge DetectVGA256(void);
void setvgapalette256(DacPalette256 *);
void EcranGraphique(void);
void EcranTexte(void);
Color32 Get_Pixel(int, int);
void Init_Palette(void);

#endif

#ifndef SVGA320x200x256
#define	SVGA320x200x256		0	/* 320x200x256 Standard VGA */
#define	SVGA640x400x256		1	/* 640x400x256 Svga/VESA */
#define	SVGA640x480x256		2	/* 640x480x256 Svga/VESA */
#define	SVGA800x600x256		3	/* 800x600x256 Svga/VESA */
#define SVGA1024x768x256	4	/* 1024x768x256 Svga/VESA */
#define SVGA640x350x256		5	/* 640x350x256 Svga */
#define SVGA1280x1024x256	6	/* 1280x1024x256 VESA */
#endif
