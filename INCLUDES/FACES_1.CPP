//
// D‚finition des faces d‚finissant l'univers en 3D
//

struct facette
{
	unsigned char type;	   	  // 0-> Uni (=MOTIF) // 1-> Gouraud // 2-> Texture
	unsigned char motif;     // Indice de couleur,
	unsigned char* texture; // Adresse de la texture
	unsigned char nb;	 	 	 // Nombre de points de la facette
	unsigned int p[4];		// Liste des points affect‚s … la facette
};

extern unsigned char texture_mur[];
extern unsigned char texture_bois[];
extern unsigned char texture_pierre[];
extern unsigned char texture_sol[];
extern unsigned char texture_porte[];
extern unsigned char texture_fenetre[];

struct facette f[]=
{
{2,0,texture_fenetre,4,{0,1,2,3}},
{2,0,texture_sol,4,{4,5,6,7}},
{2,0,texture_porte,4,{8,9,10,11}},
{2,0,texture_bois,4,{12,13,14,15}},
{2,0,texture_mur,4,{16,17,18,19}},
{2,0,texture_mur,4,{20,21,22,23}},
{2,0,texture_sol,4,{24,25,26,27}},
{1,0,texture_sol,4,{28,29,30,31}},
{1,0,texture_sol,4,{32,33,34,35}},
{1,0,texture_sol,4,{36,37,38,39}},
{1,0,texture_sol,4,{40,41,42,43}},
{1,0,texture_sol,4,{44,45,46,47}},
{1,0,texture_sol,4,{48,49,50,51}},
{1,0,texture_sol,4,{52,53,54,55}},
{1,0,texture_sol,4,{56,57,58,59}},
{1,0,texture_sol,4,{60,61,62,63}},
};
