/***************************************************/
/*                                                 */
/*                                                              FCT_PICT.C                     */
/*                                                 */
/* Gestion du Format PICT (affichage / sauvegarde) */
/*                                                 */
/***************************************************/


#include <conio.h>
#include <stdio.h>
#include <sys\stat.h>
#include <io.h>
#include <fcntl.h>
#include <alloc.h>
#include <math.h>
#include "fct_pict.h"


/* Constante … mettre en commentaire pour compiler un programme */
/* sur un processeur … base de 68000, et inversement, … activer */
/* sur un processeur de la famille des 80x86                    */
/* --> D‚finit la maniŠre de stocker un entier (int)            */
/*     de 2 octets : <Poids faible> <Poids fort>                */
#define PROCESSEUR_80x86        1

/* A mettre en commentaire pour compiler un programme sur un processeur */
/* de la famille des 80x86, et inversement, … activer sur un processeur */
/* … base de 68000                                                      */
/* --> D‚finit la maniŠre de stocker un entier (int)                    */
/*     de 2 octets : <Poids fort> <Poids faible>                        */
/*
#ifdef PROCESSEUR_80x86
#undef PROCESSEUR_80x86
#endif
*/

/* Taille de la premiŠre partie de l'entˆte (compos‚ de 0) */
#define TAILLE_ENTETE                   512

/* Taille maximale d'un bloc de ligne */
#define TAILLE_MAX_BLOC         127

/* D‚but de l'image */
#define DEBUT_IMAGE                             634

/* Constantes utilis‚es pour la v‚rification du format du fichier */
#define VERSION_OPCODE                                          0x0011
#define PICT_VERSION_2                                          0x02FF
#define HEADER_OPCODE                                                   0x0C00
#define RESERVED_OPCODE                                         0x009A
#define COMPACT_ADOBE_PHOTOSHOP         0x0004
#define PIXEL_32_BITS                                                   0x0020
#define PIXEL_3_COMPOSANTES       0x0003
#define COMPOSANTE_8_BITS                                       0x0008


/****************************************************************************/
/* D‚clarations des fonctions "internes" au module */

int      lecture_entier(int);
int      sauvegarde_entier(int, unsigned *);
int      sauvegarde_rectangle(int, unsigned *, unsigned *, unsigned *, unsigned *);
int      ConstruitLigne(int, Ligne *);
unsigned sauvegarde_entete(int, int, int);
void     lire_ligne(int, int, int, Ligne *);
unsigned sauve_ligne_compresse(int, Ligne *);

/****************************************************************************/
/* Pointeurs de fonction pour acc‚der aux fonctions graphiques */

void (*SetGraphMode)(void) = NULL;
void (*SetTextMode)(void) = NULL;
Color32 (*ReadPixel)(int, int) = NULL;


/****************************************************************************/
/****************************************************************************/

/******************************************/
/* Lecture d'un entier cod‚ sur 2 octets  */
/* dans un fichier dont le handle est num */
/******************************************/

int lecture_entier(num)
	int num;
{
	int res;

#ifdef PROCESSEUR_80x86
	/* <Poids Faible><Poids Fort> */
	read(num, ((char *) (&res))+1, 1);
	read(num, (char *) (&res), 1);
#else
	/* <Poids Fort><Poids Faible> */
	read(num, &res, 2);
#endif
	return(res);
}


/******************************************/
/* Ecriture d'un entier cod‚ sur 2 octets */
/* dans un fichier dont le handle est num */
/******************************************/

int sauvegarde_entier(num,entier)
	int               num;
	unsigned          *entier;
{
#ifdef PROCESSEUR_80x86
	/* <Poids Faible><Poids Fort> */
	write(num, ((char *) entier)+1, 1);
	write(num, (char *) entier, 1);
#else
	/* <Poids Fort><Poids Faible> */
	write(num, &entier, 2);
#endif
	return(2);
}


/********************************************/
/* Ecriture de 4 entiers cod‚s sur 2 octets */
/* dans un fichier dont le handle est num   */
/********************************************/

int sauvegarde_rectangle(num,xi,yi,xf,yf)
	int               num;
	unsigned          *xi,*yi,*xf,*yf;
{
	sauvegarde_entier(num,xi);
	sauvegarde_entier(num,yi);
	sauvegarde_entier(num,xf);
	sauvegarde_entier(num,yf);
	return(8);
}


/**********************************************************/
/* Initialisation des pointeurs permettant l'accŠs aux    */
/*  fonctions graphiques (d‚finies dans un autre module). */
/**********************************************************/

void InitGraphMode(GrMode, TxtMode, GetP)
	void (*GrMode)();                    /* Mode Graphique              */
	void (*TxtMode)();                   /* Mode Texte                  */
	Color32 (*GetP)(int, int);           /* Lecture d'un pixel 32 bits  */
{
	SetGraphMode = GrMode;
	SetTextMode  = TxtMode;
	ReadPixel    = GetP;
}


/***************************************************************/
/* Lecture d'une ligne du fichier PICT d‚sign‚ par handle et   */
/*   construction de la ligne de pixels 32 bits correspondante */
/* Retourne un code d'erreur :                                 */
/*              0 --> pas d'erreur                                       */
/*   -4 --> pas assez de m‚moire pour le tableau interm‚diaire */
/***************************************************************/

int ConstruitLigne(handle, l)
	int   handle;           /* handle du fichier PICT (ouvert en lecture) */
	Ligne *l;                               /* ligne r‚sultat */
{
	int           i, j, vv , ll;
	signed char   longueur;
	unsigned char *c, color;
	int           Erreur = 0;

	vv = 3 * (l->Dx);               /* Taille maximale de la ligne */
	/* Allocation m‚moire d'un tableau de stockage temporaire */
	if((c = (unsigned char *) malloc(vv)) != NULL)
	{
		/* On a assez de m‚moire pour l'allocation */
		/* On saute la marque de d‚but de ligne */
		if((l->Dx) < 63)
			/* Res X = [0 .. 62] */
			lseek(handle, 1, SEEK_CUR);
		else
			/* Res X = [63 .. ] */
			lseek(handle, 2, SEEK_CUR);

		i = 0;
		/* "Remplissage" du tableau interm‚diaire, o— l'on stocke   */
		/*  d'abord toutes les composantes rouges, puis les vertes, */
		/*  et pour finir les bleues */
		while(i < vv)
		{
			/* On n'a pas fini de remplir le tableau : bloc suivant */
			/* Lecture de la marque de bloc */
			read(handle, &longueur, 1);
			ll = abs(longueur) + 1;
			if(longueur < 0)
			{
				/* Compress‚ : on lit 1 fois la composante */
				read(handle, &color, 1);
				/* Il y a 'll' composantes … stocker */
				for(j = 0 ; j < ll ; j++)
					c[i+j] = color;
				i += ll;
			}
			else
			{
				/* non compress‚ : il y a 'll' composantes … lire et stocker */
				read(handle, &c[i], ll);
				i += ll;
			}
		}

		/* Tableau interm‚diaire rempli : stockage dans la ligne r‚sultat */
		for(i = 0 ; i < (l->Dx) ; i++)
		{
			(l->p)[i].Rouge = c[i];
			(l->p)[i].Vert  = c[(l->Dx) + i];
			(l->p)[i].Bleu  = c[((l->Dx) << 1) + i];
		}
		/* Lib‚ration de l'espace m‚moire allou‚ au tableau interm‚diaire */
		free(c);
	}
	else
		Erreur = -4;    /* Erreur : Pas assez de m‚moire pour le tableau interm‚diaire */

	return(Erreur);
}


/*******************************************************************/
/* Ecriture de l'entˆte d'une image de 'dx' colonnes * 'dy' lignes */
/*   dans le fichier d‚sign‚ par le handle 'num'                   */
/* Retourne la longueur totale en octets de l'entˆte cr‚‚          */
/*******************************************************************/

unsigned sauvegarde_entete(num, dx, dy)
	int num;                                /* handle du fichier cr‚‚                         */
	int dx, dy;     /* R‚solution en X et en Y de l'image sauvegard‚e */
{
	int           i;
	unsigned      longueur;
	unsigned      ll ;
	/* Constantes */
	unsigned      zero = 0x00;
	unsigned char uc = 0x00;
	unsigned char version[4] = { 0x00,0x11,0x02,0xFF };
	unsigned char mess1[14]  = { 0x0C,0x00,0xFF,0xFE,0x00,0x00,0x00,0x48,
															 0x00,0x00,0x00,0x48,0x00,0x00 };
	unsigned char mess2[8]   = { 0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x0A };
	unsigned char mess3[6]   = { 0x00,0x9A,0x00,0x00,0x00,0xFF };
	unsigned char mess4[36] =  { 0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,
															 0x00,0x48,0x00,0x00,0x00,0x48,0x00,0x00,
															 0x00,0x10,0x00,0x20,0x00,0x03,0x00,0x08,
															 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
															 0x00,0x00,0x00,0x00 };
	unsigned char mess5[2]   = { 0x00,0x40 };

	/* Ecriture du d‚but de l'entˆte (512 octets … 0) */
	for ( i = 0 ; i < TAILLE_ENTETE ; i++ )
		write(num,&uc,1);

	ll = (dx * 4) | 0x8000;         /* Valeur incertaine */
	longueur = sauvegarde_entier(num, &longueur);
	longueur += sauvegarde_rectangle(num, &zero, &zero, (unsigned *)&dy, (unsigned *)&dx);
	longueur += write(num, version, 4);
	longueur += write(num, mess1, 14);
	longueur += sauvegarde_rectangle(num, &zero, &zero, (unsigned *)&dy, (unsigned *)&dx);
	longueur += write(num, mess2, 8);
	longueur += sauvegarde_rectangle(num, &zero, &zero, (unsigned *)&dy, (unsigned *)&dx);
	longueur += write(num, mess3, 6);
	longueur += sauvegarde_entier(num, &ll);
	longueur += sauvegarde_rectangle(num, &zero, &zero, (unsigned *)&dy, (unsigned *)&dx);
	longueur += write(num, mess4, 36);
	longueur += sauvegarde_rectangle(num, &zero, &zero, (unsigned *)&dy, (unsigned *)&dx);
	longueur += sauvegarde_rectangle(num, &zero, &zero, (unsigned *)&dy, (unsigned *)&dx);
	longueur += write(num, mess5, 2);

	return(longueur);       /* Qui est en fait ‚gale … 634 */
}


/************************************************/
/* Stocke dans l les pixels 32 bits de la ligne */
/*  y, entre les colonnes x0 et x1 (fenˆtrage)  */
/************************************************/

void lire_ligne(x0, x1, y, l)
	int   x0, x1, y;
	Ligne *l;
{
	int i, ResX;

	ResX = MIN(l->Dx , x1 + 1);             /* Fenˆtrage : il s'agit de ne pas lire */
																/*   en dehors de l'‚cran !             */
	for(i = 0 ; i < ResX ; i++)
		(l->p)[i] = ReadPixel(x0+i, y);
}


/************************************************************/
/* Sauvegarde d'une ligne l dans le fichier de handle 'num' */
/*   en utilisant la compression lorsque c'est possible     */
/************************************************************/

unsigned sauve_ligne_compresse(num, l)
	int   num;
	Ligne *l;
{
	unsigned      longueur, v2;
	int           i, NbPixBloc, NbPixLigne, vv;
	unsigned char Val1, Val2, Val3;
	signed char   Marque;
	Byte          *c, *d, v;

	vv = 3 * l->Dx;                                                                 /* Taille maximale du tableau des donn‚es brutes (composantes des pixels) */
	v2 = vv + (vv / 128 + 1) + 2;           /* Taille maximale du tableau stockant une ligne */
	longueur = 0;
	/* Allocation de la m‚moire n‚cessaire aux tableaux */
	if(((c = malloc(vv)) != NULL) && ((d = malloc(v2)) != NULL))
	{
		/* Transformation de la ligne de pixels en un tableau lin‚aire d'octets    */
		/* (d'abord tous les rouges, puis tous les verts, et enfin tous les bleus) */
		for(i = 0 ; i < (l->Dx) ; i++)
		{
			c[i] = (l->p)[i].Rouge;
			c[(l->Dx) + i] = (l->p)[i].Vert;
			c[((l->Dx) << 1) + i]   = (l->p)[i].Bleu;
		}

		/* Sauvegarde de la ligne elle-mˆme */
		NbPixLigne = 0;       /* Indice de d‚but du bloc actuellement trait‚ */
		do
		{
			/* On commence un nouveau bloc */
			NbPixBloc  = 0;     /* Indice (offset) de l'octet courant dans le bloc trait‚ */
			Marque = 0;
			Val1   = c[NbPixLigne];
			if(NbPixLigne != (vv - 1))
			{
				/* On n'est pas sur le dernier octet : on lit le suivant */
				Val2 = c[NbPixLigne+1];
				if(Val1 == Val2)
				{
					/* Compression */
					do
					{
						Marque--;
						NbPixBloc++;
						if(((NbPixLigne + NbPixBloc) != vv) && (NbPixBloc != TAILLE_MAX_BLOC))
							/* On n'est ni sur le dernier octet de la ligne, */
							/*          ni sur le dernier octet du bloc      */
							/* --> On continue sur l'octet suivant           */
							Val2 = c[NbPixLigne+NbPixBloc+1];
						else
							/* Fin de compression : dernier octet du bloc ou de la ligne */
							break;
					}
					while((Val1 == Val2) && (((NbPixLigne+NbPixBloc) % 128) != 0));
					/* <Fin de compression> ou <Fin de bloc> ou <Fin de ligne> */
					/* Rem : on a ‚galement une fin de bloc tous les 128 octets trait‚s */
					NbPixBloc++;
					d[longueur] = Marque;
					longueur++;
					d[longueur] = Val1;
					longueur++;
				}
				else
				{
					/* Pas de compression */
					do
					{
						Marque++;
						NbPixBloc++;
						Val1 = Val2;
						if(((NbPixLigne + NbPixBloc) != vv) && (NbPixBloc != TAILLE_MAX_BLOC))
						{
							/* On n'est ni sur le dernier octet de la ligne, */
							/*          ni sur le dernier octet du bloc      */
							/* --> On continue sur l'octet suivant           */
							Val2 = c[NbPixLigne+NbPixBloc+1];
							if(Val1 == Val2)
							{
								/* les deux octets trait‚s sont ‚gaux : */
								/* on les compare avec l'octet suivant  */
								if((NbPixLigne + NbPixBloc + 1) != vv)
								{
									/* Il y a bien un octet suivant */
									Val3 = c[NbPixLigne+NbPixBloc+2];
									if(Val3 == Val1)
										/* Les 3 octets sont ‚gaux : on passe … la compression */
										break;
								}
							}
						}
						else
							/* on a fini la ligne */
							break;
					}
					while(((NbPixLigne+NbPixBloc) % 128) != 0); /* Tant qu'on n'a pas fait un break */
										   /* ou trait‚ 128 octets */
					/* Stockage du bloc non compress‚e dans le tableau 'd' */
					Marque--;
					d[longueur] = Marque;
					longueur++;
					for(i = 0 ; i < NbPixBloc ; i++)
					{
						d[longueur] = c[NbPixLigne + i];
						longueur++;
					}
				}
			}
			else
			{
				/* Il ne reste qu'un octet … sauvegarder (cas particulier)*/
				Marque = 0;
				NbPixBloc++;
				d[longueur] = Marque;
				longueur++;
				d[longueur] = c[NbPixLigne];
				longueur++;
			}
			NbPixLigne += NbPixBloc;
		}
		while(NbPixLigne < vv);
		/* On a trait‚ toute la ligne (stock‚e dans 'd'). */
		/* Il faut l'‚crire dans le fichier en stockant   */
		/*   en premier lieu la longueur de la ligne,     */
		/*   puis le tableau 'd' lui-mˆme                 */
		if(l->Dx < 63)
		{
			/* la longueur de la ligne est stock‚e sur 1 octet */
			v = (Byte) longueur & 0x00FF;
			write(num, &v, 1);
			write(num, d, longueur);
			longueur++;
		}
		else
		{
			/* la longueur de la ligne est stock‚e sur 2 octets */
			sauvegarde_entier(num, &longueur);
			write(num, d, longueur);
			longueur += 2;
		}
		/* lib‚ration de l'espace m‚moire allou‚ */
		free(c);
		free(d);
	}
	/* on retourne le nombre d'octets effectivement ‚crits dans le fichier */
	return(longueur);
}


/********************************************************************/
/* Sauve une image PICT 24 affich‚e … l'‚cran, comprise dans une    */
/*  fenˆtre de coin sup‚rieur gauche de coordonn‚es (x0,y0), et de  */
/*  coin inf‚rieur droit de coordonn‚es (x1,y1), dans le fichier    */
/*  d‚sign‚ par le nom 'fichier'                                    */
/*                                                                  */
/* Retourne un code d'erreur :                                      */
/*              0 --> pas d'erreur                                            */
/*   -1 --> librairie Graphique non initialis‚e                     */
/*   -2 --> fichier impossible … cr‚er                              */
/*   -3 --> pas assez de place sur le disque pour la sauvegarde     */
/*   -4 --> pas assez de m‚moire pour stocker une ligne             */
/*   -5 --> pas assez de m‚moire pour le tableau interm‚diaire      */
/********************************************************************/

int sauvegarde_image_PICT24(fichier, x0, y0, x1, y1)
	char *fichier;
	int  x0, y0, x1, y1;
{
	int           num, Erreur = 0;
	unsigned      longueur, long_ligne;
	int           y;
	Ligne         l;
	Byte          v;
	unsigned char fin[2]     = { 0x00,0xFF };

	if((SetGraphMode != NULL) && (SetTextMode != NULL) && (ReadPixel != NULL))
	{
		/* Librairie graphique initialis‚e */
		if((num = open(fichier, O_WRONLY|O_TRUNC|O_CREAT|O_BINARY,S_IREAD|S_IWRITE)) != -1)
		{
			/* fichier ouvert en cr‚ation */
			l.Dx = x1 - x0 + 1;             /* largeur de l'image (en pixels) */
			/* sauvegarde de l'entˆte */
			longueur = sauvegarde_entete(num, l.Dx, (y1 - y0 + 1));
			/* Allocation de la m‚moire n‚cessaire pour stocker une ligne de pixels */
			if((l.p = malloc(l.Dx * sizeof(Color32))) != NULL)
			{
				/* pas de problŠmes d'allocation */
				/* on sauvegarde ligne aprŠs ligne l'image ele-mˆme */
				for(y = y0 ; y <= y1 ; y++)
				{
					/* Acquisition de la ligne */
					lire_ligne(x0, x1, y, &l);
					/* et sauvegarde en compressant si possible */
					if((long_ligne = sauve_ligne_compresse(num, &l)) != 0)
						/* pas de problŠme de sauvegarde */
						longueur += long_ligne;
					else
						/* Erreur : Pas assez de m‚moire pour le tableau interm‚diaire  */
						Erreur = -5;
				}
				/* on libŠre l'espace m‚moire allou‚ pour le stockage de la ligne */
				free(l.p);
			}
			else
				/* Erreur : pas assez de m‚moire pour stocker une ligne */
				Erreur = -4;

			/* V‚rification de la parit‚ de la taille du fichier */
			if (longueur % 2)
			{
				/* longueur impaire : il faut rajouter un octet nul */
				v = 0;
				longueur += write(num, &v, 1);
			}
			/* Ecriture de la marque de fin de fichier (endPICT OpCode) */
			/* On contr“le l'‚criture des 2 derniers octets du fichier : s'ils ne peuvent */
			/*   ˆtre ‚crits correctement, c'est qu'il manque une partie du fichier !     */
			if(write(num, fin, 2) == 2)
				/* Pas de problŠme ... */
				longueur += 2;
			else
				/* Erreur :  pas assez de place sur le disque pour la sauvegarde */
				Erreur = -3;
			/* Maintenant qu'on connaŒt la longueur effective du fichier, on l'ecrit */
			/*   … l'emplacement qui lui a ‚t‚ r‚serv‚ plus haut.                    */
			lseek(num, TAILLE_ENTETE, SEEK_SET);
			sauvegarde_entier(num, &longueur);
			close(num);
		}
		else
			/* Erreur : fichier impossible … cr‚er */
			Erreur = -2;
	}
	else
		/* Erreur : librairie graphique non initialis‚e */
		Erreur = -1;
	return(Erreur);
}


/**********************************************************************/
/* Ecran_Graphique fournit … la couche application un moyen d'appeler */
/*   le pointeur de fonction de commutation en mode graphique         */
/**********************************************************************/
void Ecran_Graphique()
{
	SetGraphMode();
}

/******************************************************************/
/* Ecran_Texte fournit … la couche application un moyen d'appeler */
/*   le pointeur de fonction de commutation en mode texte         */
/******************************************************************/
void Ecran_Texte()
{
	SetTextMode();
}
