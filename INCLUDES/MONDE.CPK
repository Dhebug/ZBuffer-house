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
/*
struct point3d far ps[]=
{
{-5,-5,-5,0}, // Les 4 points du dessus
{-5,5,-5,0},
{5,5,-5,0},
{5,-5,-5,0},

{-5,-5,5,0},  // Les 4 points du dessous
{-5,5,5,0},
{5,5,5,0},
{5,-5,5,0},
};


// 0
{-5,5,-5,0},  // Fenetre de derriere
{-5,5,5,0},
{5,5,5,0},
{5,5,-5,0},

// 4
{-5,-5,5,0},	// D‚finition du sol en terre devant la porte (1)
{5,-5,5,0},
{5,-15,5.5,0},
{-5,-15,5.5,0},

// 8
{5,-5,-5,0},   // D‚finition de la porte avant
{5,-5,5,0},
{-5,-5,5,0},
{-5,-5,-5,0},

// 12
{-5,-5,-5,0},  // D‚finition du toit
{-5,5,-5,0},
{5,5,-5,0},
{5,-5,-5,0},

// 16
{-5,5,-5,0},  // Le cot‚ gauche
{-5,5,5,0},
{-5,-5,5,0},
{-5,-5,-5,0},

// 20
{5,5,5,0},    // Le cot‚ droit
{5,5,-5,0},
{5,-5,-5,0},
{5,-5,5,0},

// 24
{-5,-15,5.5,0},	// D‚finition du sol en terre devant la porte (2)
{5,-15,5.5,0},
{5,-25,6,0},
{-5,-25,6.5,0},

// 28
{5,-5,5,200},	// D‚finition de l'herbe … gauche de la porte (1)
{15,-5,5.5,220},
{15,-15,4.5,240},
{5,-15,5.5,200},

// 32
{5,-15,5.5,200},	// D‚finition de l'herbe … gauche de la porte (2)
{15,-15,4.5,240},
{15,-25,5,220},
{5,-25,7,200},

// 36
{-5,-5,5,200},	   // D‚finition de l'herbe … droite de la porte (1)
{-15,-5,5.5,220},
{-15,-15,5.5,240},
{-5,-15,5.5,200},

// 40
{-5,-15,5.5,200},	// D‚finition de l'herbe … droite de la porte (2)
{-15,-15,5.5,240},
{-15,-25,6,220},
{-5,-25,6.5,200},

// 44
{5,5,5,200},	// D‚finition de l'herbe … gauche de la porte (3)
{15,5,5,220},
{15,-5,5.5,220},
{5,-5,5,200},

// 48
{-5,5,5,200},	// D‚finition de l'herbe … droite de la porte (3)
{-15,5,5,220},
{-15,-5,5.5,220},
{-5,-5,5,200},

// 52
{-5,5,5,200},	  // D‚finition de l'herbe devant la fenetre (1)
{5,5,5,220},
{5,15,5,220},
{-5,15,5,200},

// 56
{5,5,5,200},	  // D‚finition de l'herbe … droite de la fenetre (4)
{5,15,5,220},
{15,15,5,220},
{15,5,5,200},

// 60
{-5,5,5,200},	  // D‚finition de l'herbe … gauche de la fenetre (4)
{-5,15,5,220},
{-15,15,5,220},
{-15,5,5,200},

// 64
};


struct facette f[]=
{
{2,0,texture_fenetre,4,{1,5,6,2}}, // 0
{2,0,texture_porte,4,{3,7,4,0}},
{2,0,texture_bois,4,{0,1,2,3}},
{2,0,texture_mur,4,{2,6,7,3}},
{2,0,texture_mur,4,{0,4,5,1}}, // 5
/*
{2,0,texture_sol,4,{4,5,6,7}},
{2,0,texture_sol,4,{24,25,26,27}},
{1,0,texture_sol,4,{28,29,30,31}},
{1,0,texture_sol,4,{32,33,34,35}},
{1,0,texture_sol,4,{36,37,38,39}},
{1,0,texture_sol,4,{40,41,42,43}}, // 10
{1,0,texture_sol,4,{44,45,46,47}},
{1,0,texture_sol,4,{48,49,50,51}},
{1,0,texture_sol,4,{52,53,54,55}},
{1,0,texture_sol,4,{56,57,58,59}},
{1,0,texture_sol,4,{60,61,62,63}}, // 15

};
*/

struct point3d far ps[]=
{
{-15 ,-7.5 ,-2.65625 , 35.27737 },
{-15 ,-7.5 , 2.65625 , 26.6712 },
{-10 ,-7.5 ,-2.65625 , 28.97593 },
{-10 ,-7.5 , 2.65625 , 14.47812 },
{-5 ,-7.5 ,-2.65625 , 15.0974 },
{-5 ,-7.5 , 2.65625 , 38.737 },
{ 0 ,-7.5 ,-2.65625 , .7008821 },
{ 0 ,-7.5 , 2.65625 , 38.03618 },
{ 5 ,-7.5 ,-2.65625 , 40.7245 },
{ 5 ,-7.5 , 2.65625 , 35.4519 },
{ 10 ,-7.5 ,-2.65625 , 2.267638 },
{ 10 ,-7.5 , 2.65625 , 20.70164 },
{ 15 ,-7.5 ,-2.65625 , 43.13097 },
{ 15 ,-7.5 , 2.65625 , 39.524 },
{ 15 ,-2.5 ,-2.65625 , 18.67681 },
{ 15 ,-2.5 , 2.65625 , 48.09766 },
{ 15 , 2.5 ,-2.65625 , 43.57229 },
{ 15 , 2.5 , 2.65625 , 2.811843 },
{ 10 , 2.5 ,-2.65625 , 47.47783 },
{ 10 , 2.5 , 2.65625 , 18.20093 },
{ 10 , 7.5 ,-2.65625 , 26.24342 },
{ 10 , 7.5 , 2.65625 , 38.35558 },
{ 5 , 7.5 ,-2.65625 , 2.675226 },
{ 5 , 7.5 , 2.65625 , 29.62291 },
{ 0 , 7.5 ,-2.65625 , 23.43501 },
{ 0 , 7.5 , 2.65625 , 14.90827 },
{-5 , 7.5 ,-2.65625 , 31.13483 },
{-5 , 7.5 , 2.65625 , 32.39106 },
{-10 , 7.5 ,-2.65625 , 13.18965 },
{-10 , 7.5 , 2.65625 , 13.9671 },
{-15 , 7.5 ,-2.65625 , 41.49008 },
{-15 , 7.5 , 2.65625 , 41.23011 },
{-15 , 2.5 ,-2.65625 , 29.45815 },
{-15 , 2.5 , 2.65625 , 49.30466 },
{-15 ,-2.5 ,-2.65625 , 45.54821 },
{-15 ,-2.5 , 2.65625 , 11.3433 },
};
 
struct facette far f[]=
{
{2,0,texture_mur,4,{ 0 , 1 , 3 , 2 }},
{2,0,texture_mur,4,{ 2 , 3 , 5 , 4 }},
{2,0,texture_fenetre,4,{ 4 , 5 , 7 , 6 }},
{2,0,texture_mur,4,{ 6 , 7 , 9 , 8 }},
{2,0,texture_mur,4,{ 8 , 9 , 11 , 10 }},
{2,0,texture_fenetre,4,{ 10 , 11 , 13 , 12 }},
{2,0,texture_mur,4,{ 12 , 13 , 15 , 14 }},
{2,0,texture_fenetre,4,{ 14 , 15 , 17 , 16 }},
{2,0,texture_porte,4,{ 16 , 17 , 19 , 18 }},
{2,0,texture_mur,4,{ 18 , 19 , 21 , 20 }},
{2,0,texture_mur,4,{ 20 , 21 , 23 , 22 }},
{2,0,texture_fenetre,4,{ 22 , 23 , 25 , 24 }},
{2,0,texture_mur,4,{ 24 , 25 , 27 , 26 }},
{2,0,texture_fenetre,4,{ 26 , 27 , 29 , 28 }},
{2,0,texture_mur,4,{ 28 , 29 , 31 , 30 }},
{2,0,texture_mur,4,{ 30 , 31 , 33 , 32 }},
{2,0,texture_fenetre,4,{ 32 , 33 , 35 , 34 }},
{2,0,texture_mur,4,{ 34 , 35 , 1 , 0 }},
};
