#ifndef mon_type
#define mon_type

struct point2d
{
	int x;
	int y;
};

struct point3d
{
	float x;
	float y;
	float z;
	float c;
};


struct facette
{
	unsigned char type;	   	  // 0-> Uni (=MOTIF) // 1-> Gouraud // 2-> Texture
	unsigned char motif;     // Indice de couleur,
	unsigned char* texture; // Adresse de la texture
	unsigned char nb;	 	 	 // Nombre de points de la facette
	unsigned int p[4];		// Liste des points affect‚s … la facette
};


struct clipping
{
	int gauche;
	int droite;
	int haut;
	int bas;
};


#endif
