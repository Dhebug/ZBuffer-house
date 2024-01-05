extern unsigned char texture_mur[];
extern unsigned char texture_bois[];
extern unsigned char texture_pierre[];
extern unsigned char texture_sol[];
extern unsigned char texture_porte[];
extern unsigned char texture_fenetre[];

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
