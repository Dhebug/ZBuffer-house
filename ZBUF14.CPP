//
// Projet d'informatique graphique,
// Licence Informatique,
// Ann‚e 1993-94,
// Binome Dubief ArlŠne & Pointier Micka‰l
//
// Sujet:
// - Animation d'une scŠne en utilisant la m‚thode du ®Z-Buffer¯
// (Stockage des profondeurs) pour r‚soudre le problŠme de la d‚termination
// des faces cach‚es.
// - Utilisation de textures bitmap, et lissage de Gouraud.
// - Param‚trage de l'animation en temps r‚el (raccourcis clavier).
//


#include <math.h>
#include <dos.h>
#include <mem.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>


//
// D‚finitions des structures
// - Points
// - Facettes
//
#include "includes\types.h"


// 
// G‚n‚r‚ par le modeleur:
// - Nombre de points
// - Nombre de faces
//
#include "includes\nb_data.h"


//
// La liste des textures. Chaque texture est un tableau de
// 6800 octets (unsigned char), correspondant … un bloc
// bitmap de 80x85 pixels, au format bitmap VGA (La valeur de chaque
// octet donne directement la teinte du pixel dans la palette des
// 256 couleurs
//
extern unsigned char texture_mur[];
extern unsigned char texture_bois[];
extern unsigned char texture_pierre[];
extern unsigned char texture_sol[];
extern unsigned char texture_porte[];
extern unsigned char texture_fenetre[];


//
// La palette de 256 couleurs. Cette palette contient 4 d‚grad‚s
// distincts.
// De  0 … 63, un d‚grad‚ allant du Noir au Blanc en passant par le brun.
// De 64 … 127, un d‚grad‚ de gris allant du Noir au Blanc.
// De 128 … 191, un d‚grad‚ allant du Noir au Bleu.
// De 192 … 255, un d‚grad‚ allant du Noir au Vert.
//
// Une couleur est stock‚e sous la forme de 3 octets (unsigned char),
// d‚finissant sa composition en Rouge, Vert et Bleu.
//
extern unsigned char palette[];


//
// Table de correspondance pour les ombrages. Les textures bitmap ont
// ‚t‚ cr‚es avec un logiciel de dessin en 16 couleurs. La palette des
// 16 couleurs originales contenait un d‚grad‚ de Bruns en 8 phases, et
// un d‚grad‚ de Gris, lui aussi en 8 phases. Pour obtenir 64 ombrages
// diff‚rents, ces deux d‚grad‚s ont ‚t‚ ‚tendus (voir ci-dessus), et il
// suffit donc d'effectuer une correspondance entre un pixel de la texture
// et la palette de couleur, en fonction d'un coefficient de luminosit‚
// pour tracer les textures en diff‚rentes intensit‚s lumineuses. Cette
// table contient donc 64 valeurs pour chaqu'une des 16 couleurs de la
// palette de base.
// [NB: En utilisant ce principe, il aurait ‚t‚ possible de m‚langer la
// routine d'affichage textur‚ et celle qui affiche en ombrage de Gouraud.
// Cela aurait permis d'afficher des textures Bitmap tout en appliquant
// l'algorithme de Gouraud (ou de Phong) pour effectuer un lissage entre
// les diff‚rentes facettes. -> Manque de temps]
//
extern unsigned char colors[64][16];


//
// La description de la scŠne sous la forme d'une liste de points et de
// facettes.
//
extern struct facette far f[];
extern struct point3d far ps[];


//
// La routine de sauvegarde au format PICT 24 bits...
// Je n'ai pu r‚cup‚rer cette routine que tardivement, et il manquait
// un fichier de d‚clarations. Par ailleur, j'ai supprim‚ les fonctions
// de lecture d'une image, ainsi que toutes les proc‚dures ayant un
// rapport avec la vid‚o (gestion des BGI, etc...)
//
int sauvegarde_image_PICT24(char*,int,int,int,int);


//
// Ces 3 fonctions permettent respectivement de changer la palette de 256
// couleurs, de lire le mode graphique courant, et de changer celui ci.
// Il est inutile d'utiliser la ®Borland Graphics Library¯. Cela permet
// de gagner de la m‚moire, ‚vite que le programme soit diffus‚ sans le
// BGI n‚cessaire … son fonctionnement... Et finalement, n'est pas plus
// ®non portable¯ que les BGI.
//
void change_palette(unsigned char* PalBuf);
char get_rez();
void change_rez(char rez);


//
// Impl‚mentation d'un type virgule fixe 32 bits.
// Donne un pr‚cision de 2^-16 tout en permettant d'effectuer
// les calculs uniquement sur des LONG et des INT.
// Toutes les routines bas‚es sur un algorytme de ®sens de l'erreur¯
// (Bresenham,etc...) peuvent ˆtre impl‚ment‚s de cette fa‡on et sont
// plus rapides. N‚anmoins, il faut se limiter au 320x200, sinon les
// erreurs d'arrondi deviennent g‚nantes.
// Il y a plusieurs impl‚mentations possibles, pour passer de la
// repr‚sentation INT->ETENDU ou de ETENDU->INT:
//
// 1/ En effectuant des d‚callages … gauche et … droite. Cette m‚thode
// suppose que le type INT fait 16 bits, que le type LONG fait 32 bits.
// Mais globalement, cela reste portable.
//   entier(x)=int(x>>16)
//   etendu(x)=(long(x)<<16)
//
// 2/ En effectuant des multiplications et des divisions. Le r‚sultat est
// le mˆme, mais le ralentissement occasionn‚ est COLOSSAL. La portabilit‚
// du code est garantie.
//   entier(x)=int(x/65536)
//   etendu(x)=(long(x)*65536)
//
// 3/ En effectuant des manipulations de pointeur. Cette m‚thode est plus
// rapide que la (1), mais n'est pas franchement compr‚hensible … lire:
// Cela consiste … d‚clarer un pointeur sur un INT, que l'on fait pointer
// sur l'adresse de la valeur LONG+1. Donc, on accŠde directement aux
// 16 bits sup‚rieurs du LONG. L'inconv‚nient de cette m‚thode est que
// l'impl‚mentation d‚pend directement du type de microprocesseur utilis‚.
// Ainsi, sur un INTEL 80x86, le nombre hexad‚cimal 0x12345678 est repr‚sent‚
// en m‚moire sous la forme:
//
// Adresse: x    x+1  x+2  x+3
// Contenu  [78] [56] [34] [12]
//                     -------
//
// Et sur les processeurs MOTOROLA 680x0 sous la forme:
// Adresse: x    x+1  x+2  x+3
// Contenu  [12] [34] [56] [78]
//           -------
//
// Nous cherchons … r‚cup‚rer la valeur 0x1234, qui repr‚sente donc les
// 16 bits sup‚rieurs de notre nombre, or il faut chercher … x+2 avec le
// format INTEL, et en X avec le format MOTOROLA.
//
// L'impl‚mentation choisie est la (1).
//
#define entier(x) int(x>>16)
#define etendu(x) (long(x)<<16)


//
// D‚clare de la place pour m‚moriser le Z-Buffer.
// Le tableau a ‚t‚ organis‚ dans le sens [Y][X], afin de permettre une
// recopie rapide du Buffer … l'‚cran. (Stockage lin‚aire des pixels dans
// le sens naturel de la m‚moire ‚cran)
//
unsigned int huge zbuf[200][320];  // Profondeur affect‚e au point
unsigned char far zcol[200][320];  // Couleur affect‚e


//
// Ces tableaux repr‚sente les cot‚s gauches et droits des polygones
// CONVEXES qui sont affich‚s.
// - DEBX/FINX contiennent respectivement le d‚but et la fin de chaque
//   segment horizontal qui doit ˆtre trac‚.
// - DEBZ/FINZ contiennent la profondeur respective des extremit‚s
//   gauches et droites de ce mˆme segment. (Pour le Z-Buffer)
// - DEBC/FINC contiennent les couleurs de ce mˆmes extr‚mit‚s. On
//   effectuera une interpolation lin‚aire de l'une … l'autre pour
//   obtenir le lissage de Gouraud.
//
int debx[200];
int finx[200];
int debz[200];
int finz[200];
int debc[200];
int finc[200];


//
// Ces tableaux contiennent respectivement les coordonn‚es des points
// situ‚s en d‚but et en fin d'un segment parcouru sur la texture.
// On parcourera tous les pixels de ce segment en effectuant une 
// interpolation en X et en Y.
//
unsigned int x1[200];
unsigned int y1[200];
unsigned int x2[200];
unsigned int y2[200];


//
// Liste des points de la scŠne aprŠs projection dans le repŠre ‚cran.
// La description initiale de la scŠne reste inchang‚e d'une image sur
// l'autre. C'est l'observateur qui tourne autour de la scŠne.
//
struct point3d p[nb_point];   // Coordonn‚es des points … l'‚cran.


//
// Coordonn‚es des angles des textures. Dans l'‚tat actuel du programme
// ce tableau n'a pas vraiment d'utilit‚, puisqu'il ne contient que 4
// valeurs, mais normalement, il devrait y avoir toutes les combinaisons
// utilis‚es dans la description de la scŠne (quadrilatŠres, mais aussi
// triangles, pentagones, xxxxgones...)
// La liste des points doit ˆtre effectu‚es dans le sens trigonom‚trique,
// en partant du point situ‚ le plus en haut … gauche.
// Pour un quadrilatŠre, on aurait la description suivante:
// {A(x1,y1),B(x2,y2),C(x3,y3),D(x4,y4)}, correspondant … la forme:
//
//  A  <--D
//  |     ^ 
//  v     |
//  B---->C
//
// Normalement, les murs ®tronqu‚s¯ situ‚s sous les toits auraient du ˆtre
// mod‚lis‚s par des textures triangulaires de la forme:
// {A(x1,y1),B(x2,y2),C(x3,y3)}
//
//     A            A
//    /  ^          |  \
//   /   |     et   v   \
//  B--->C          B--->C
//
// Ce qui aurait ‚vit‚ la d‚formation provoqu‚e par la r‚union des points
// A et D sur ces facettes. [--> Pas le temps]
//
struct point2d t[4];          // Coordonn‚es des points de la texture.






//
// Echange de deux variables de type INT.
// Abondamment utilis‚ dans les routines de calcul des segments.
//
void swap(int& a,int& b)
{
  int temp;
  temp=a;
  a=b;
  b=temp;
}


//
// NB: Dans les deux fonctions qui suivent, il est possible d'effectuer
// des effets d'apparitions en faisant une initialisation du Z-Buffer
// avant chaque affichage, en faisant reculer de plus en plus la valeur
// de profondeur. Toutes les points dont le Z est sup‚rieur … cette
// valeur seront affich‚, tandis que les autres seront dissimul‚s.
// -> Donne l'impression que les objets surgissent … travers une feuille
// de papier qu'ils d‚chirent par endroit.
//
// Initialise le Z-Buffer … une couleur donn‚e pour une profondeur
// donn‚e. Il serait possible d'utiliser la fonction MEMCOPY, qui
// permet de remplir une zone m‚moire avec une valeur quelconque.
//
void initialise_z_buffer(char couleur,int profondeur)
{
	int x,y;
	for (y=0;y<200;y++)
	{
		for (x=0;x<320;x++)
		{
			zbuf[y][x]=profondeur;
			zcol[y][x]=couleur;
		}
	}
}


//
// Initialise une fenˆtre dans Z-Buffer … une couleur donn‚e pour une
// profondeur donn‚e.
//
void initialise_fenetre(char couleur,int profondeur,struct clipping fenetre)
{
	int x,y;
	for (y=fenetre.haut;y<=fenetre.bas;y++)
		for (x=fenetre.gauche;x<=fenetre.droite;x++)
		{
			zbuf[y][x]=profondeur;
			zcol[y][x]=couleur;
		}
}


//
// On r‚affiche de fa‡on rapide un tableau lin‚aire de 64000
// octets correspondant aux 320x200 pixels du tableau ZCOL.
// [NB: Le warning sur le 64000 ®constant is long¯ n'a pu ˆtre
// ot‚. Le compilateur d‚fini cette valeur au format ®size_t¯
// quit n'est d‚fini nul part, et le compilateur n'accepte pas que
// l'on ®cast¯ en INT/UNSIGNED INT,LONG UNSIGNED LONG, ou encore
// 64000L. Ceci dit, le transfert de 64000 octets est correctement
// effectu‚. (Pas d'octets en trop)]
//
void affiche_z_buffer()
{
	movedata(FP_SEG(zcol),FP_OFF(zcol),0xa000,0,64000);
}





//
// Initialise les tableaux Min et Max … des valeurs extrˆmes qui seront
// modifi‚es par Calcule_Segment_xxxxx.
//
void initialise_polygone(struct clipping fenetre,int& ymin,int& ymax)
{
  int y;
  for (y=fenetre.haut;y<=fenetre.bas;y++)
  {
    debx[y]=32000;
    finx[y]=-32000;
	}
  ymin=fenetre.bas;
  ymax=fenetre.haut;
}




//
// Les 4 routines ci-dessous auraient-pu n'en faire que 2. Il aurait
// suffit de rajouter des tests, car leur structure est identique. Seul
// le nombre de variables it‚r‚es change. N‚anmoins, cela rendrait la
// structure beaucoup plus lourde, et qui plus est ralentirait ‚norm‚ment
// le calcul.
//

/////////////////////////////// Partie Gouraud

//
// Trace virtuellement une ligne dans le tableau minimax
// Calcule par interpolation lin‚aire.
//
// - p1,p2   Num‚ro des sommets du segment
//
// - x1,y1 Coordonn‚es du premier point du segment
// - z1,c1 Profondeur et Couleur de ce mˆme point
//
// - x2,y2,z2,c2 Idem, pour le second point
//
// - ix,iz,ic    Incr‚ments en X,Z,C, cod‚ en virgule fixe 32 bits
// - x,z,c       X,Z,C pour le point courant
//
// - hauteur     Nombre de pixels … tracer verticalement
//
void calcule_segment_gouraud(int p1,int p2,struct clipping fenetre,int& ymin,int& ymax)
{
	long ix,iz,ic;

	long x,z,c;

	int y;
	int x1,y1,z1,c1,x2,y2,z2,c2;
	int hauteur;
	int longueur;

	x1=p[p1].x; // Premier point de la ligne
	y1=p[p1].y;
	z1=p[p1].z;
	c1=p[p1].c;

	x2=p[p2].x; // Second point de la ligne
	y2=p[p2].y;
	z2=p[p2].z;
	c2=p[p2].c;

	//
	// Si le premier point est plus bas, on ‚change les points
	//
	if (y1>y2)
	{
		swap(x1,x2);
		swap(y1,y2);
		swap(z1,z2);
		swap(c1,c2);
	}

	//
	// Teste si la ligne apparait dans la fenˆtre de clipping
	//
	if (y2>fenetre.haut && y1<fenetre.bas)
	{
		//
		// Nombre de pixels constituant le segment AVANT CLIPPING
		//
		hauteur=y2-y1+1;

		//
		// Calcule les incr‚ments
		//
		ix=etendu(x2-x1)/hauteur;
		iz=etendu(z2-z1)/hauteur;
		ic=etendu(c2-c1)/hauteur;

		//
		// On part du premier point pour aller vers le deuxiŠme
		//
		x=etendu(x1);
		z=etendu(z1);
		c=etendu(c1);

		//
		// Si le point sup‚rieur d‚passe de la fenˆtre, on clippe
		// les extr‚mit‚s
		//
		if (y1<fenetre.haut)
		{
			//
			// Calcule le nombre de pixels qui d‚passent de la fenˆtre,
			// et remet … jour la position du pixel courant.
			//
			longueur=fenetre.haut-y1;
			x+=ix*longueur;
			z+=iz*longueur;
			c+=ic*longueur;
			y1=fenetre.haut;
		}

		//
		// On trace jusqu'… la fin de la ligne, ou bien jusqu'… la
		// limite inf‚rieure de la fenˆtre
		//
		if (y2>fenetre.bas)
			y2=fenetre.bas;


		//
		// M‚morise le plus haut point du polygone, et le plus bas.
		//
		if (y1<ymin)
			ymin=y1;
		if (y2>ymax)
			ymax=y2;


		//
		// ItŠre verticalement, et calcule pour chaque Y, la position X,
		// et la couleur … affecter au pixel
		//
		for (y=y1;y<=y2;y++)
		{
			//
			// V‚rifie si notre pixel est plus … gauche que le dernier trac‚
			// Si oui, enregistre son abscisse, sa profondeur, et sa couleur
			//
			if (entier(x)<debx[y])
			{
				debx[y]=entier(x);
				debz[y]=entier(z);
				debc[y]=entier(c);
			}
			//
			// Idem, mais … droite
			//
			if (entier(x)>finx[y])
			{
				finx[y]=entier(x);
				finz[y]=entier(z);
				finc[y]=entier(c);
			}
			x+=ix;
			z+=iz;
			c+=ic;
		}
	}
}



//
// Routine qui effectue le tra‡age r‚el du polygone, de haut en bas.
//
void calcule_polygone_gouraud(struct clipping fenetre,const int& ymin,const int& ymax)
{
	int x,y;
	int xdeb,xfin,zdeb,zfin,cdeb,cfin;
	int largeur_dest;
	long iz,ic;
	int longueur;

	long zz,cc;

	//
	// On trace verticalement, en partant du plus haut point du polygone,
	// jusqu'… son plus bas point.
	//
	for (y=ymin;y<=ymax;y++)
	{
		//
		// R‚cupŠre les coordonn‚es gauches et droite du segment … tracer.
		//
		xdeb=debx[y];
		xfin=finx[y];

		//
		// Si le segment est au moins partiellement dans la fenetre alors,
		// on l'affiche, sinon on passe au suivant.
		//
		if ((xfin>=fenetre.gauche) && (xdeb<=fenetre.droite))
		{

			//
			// R‚cupŠre la profondeur et la couleur des extr‚mit‚s du segment.
			//
			zdeb=debz[y];
			zfin=finz[y];
			cdeb=debc[y];
			cfin=finc[y];

			//
			// Calcule la largeur du segment AVANT CLIPPING, et en d‚duit les
			// incr‚ments sur Z et C.
			//
			largeur_dest=xfin-xdeb+1;
			iz=etendu(zfin-zdeb)/largeur_dest;
			ic=etendu(cfin-cdeb)/largeur_dest;
			zz=etendu(zdeb);
			cc=etendu(cdeb);

			//
			// Effectue le clipping … gauche
			//
			if (xdeb<fenetre.gauche)
			{
				longueur=fenetre.gauche-xdeb;
				zz+=iz*longueur;
				cc+=ic*longueur;
				xdeb=fenetre.gauche;
			}

			//
			// Effectue le clipping … droite
			//
			if (xfin>fenetre.droite)
				xfin=fenetre.droite;


			//
			// Trace horizontalement le segment, de gauche … droite.
			//
			for (x=xdeb;x<=xfin;x++)
			{

				//
				// Si notre point est plus prŠs que le pr‚c‚dent, alors
				// il remplace l'ancien.
				//
				if (entier(zz)>zbuf[y][x])
				{
					zcol[y][x]=entier(cc);
					zbuf[y][x]=entier(zz);
				}
				zz+=iz;
				cc+=ic;
			}
		}
	}
}




/////////////////////////////// Partie textur‚e

//
// Trace virtuellement une ligne dans le tableau minimax
// Calcule par interpolation lin‚aire.
//
// - p1,p2       Num‚ro des sommets du segment
//
// - dx1,dy1,dz1 Coordonn‚es du premier point du segment destination (buffer)
// - dx2,dy2,dz2 Idem, pour le second point
//
// - sx1,sy1     Coordonn‚es du premier point du segment source (texture)
// - sx2,sy2     Idem pour le second point
//
// - idx,idz     Incr‚ments en X,Z pour la destination
// - isx,isz     Idem pour la source
//
// - dx,dz       X,Z pour le point courant sur la destination
// - sx,sz       Idem pour la source
//
// - hauteur     Nombre de pixels … tracer verticalement
//

void calcule_segment_texture(int p1,int p2,int pt,struct clipping fenetre,int& ymin,int& ymax)
{
	long isx,isy,idx,idz;
	long sx,sy,dx,dz;
	int dy;
	int sx1,sy1,sx2,sy2;
	int dx1,dy1,dz1,dx2,dy2,dz2;
	int d_hauteur;
	int longueur;

	sx1=t[pt %4].x;       // Premier point du segment source
	sy1=t[pt %4].y;

	sx2=t[(pt+1) %4].x;   // Second point du segment source
	sy2=t[(pt+1) %4].y;

	dx1=p[p1].x;          // Premier point de la destination
	dy1=p[p1].y;
	dz1=p[p1].z;

	dx2=p[p2].x;          // Second point du segment destination
	dy2=p[p2].y;
	dz2=p[p2].z;

	//
	// Si le premier point est plus bas, on ‚change les points
	//
	if (dy1>dy2)
	{
		swap(dx1,dx2);
		swap(dy1,dy2);
		swap(dz1,dz2);
		swap(sx1,sx2);
		swap(sy1,sy2);
	}

	//
	// Teste si la ligne apparait dans la fenˆtre de clipping
	//
	if (dy2>fenetre.haut && dy1<fenetre.bas)
	{

		//
		// Nombre de pixels constituant le segment AVANT CLIPPING
		// [NB:On travaille de la destination vers la source]
		//
		d_hauteur=dy2-dy1+1;

		//
		// Calcule les incr‚ments
		//
		idx=etendu(dx2-dx1)/d_hauteur;
		idz=etendu(dz2-dz1)/d_hauteur;
		isx=etendu(sx2-sx1)/d_hauteur;
		isy=etendu(sy2-sy1)/d_hauteur;

		//
		// On part du premier point pour aller vers le deuxiŠme
		//
		dx=etendu(dx1);
		dz=etendu(dz1);
		sx=etendu(sx1);
		sy=etendu(sy1);

		//
		// Si le point sup‚rieur d‚passe de la fenˆtre, on clippe
		// les extr‚mit‚s
		//
		if (dy1<fenetre.haut)
		{
			//
			// Calcule le nombre de pixels qui d‚passent de la fenˆtre,
			// et remet … jour la position du pixel courant.
			//
			longueur=fenetre.haut-dy1;
			dx+=idx*longueur;
			dz+=idz*longueur;
			sx+=isx*longueur;
			sy+=isy*longueur;
			dy1=fenetre.haut;
		}

		//
		// On trace jusqu'… la fin de la ligne, ou bien jusqu'… la
		// limite inf‚rieure de la fenˆtre
		//
		if (dy2>fenetre.bas)
			dy2=fenetre.bas;

		//
		// M‚morise le plus haut point du polygone, et le plus bas.
		//
		if (dy1<ymin)
			ymin=dy1;
		if (dy2>ymax)
			ymax=dy2;

		//
		// ItŠre verticalement, et calcule pour chaque Y, la position X,
		// et la couleur … affecter au pixel
		//
		for (dy=dy1;dy<=dy2;dy++)
		{
			//
			// V‚rifie si notre pixel est plus … gauche que le dernier trac‚
			// Si oui, enregistre son abscisse, sa profondeur,
			// ainsi que les coordonn‚es du point correspondant sur le segment
			// source (texture)
			//
			if (entier(dx)<debx[dy])
			{
				debx[dy]=entier(dx);
				debz[dy]=entier(dz);
				x1[dy]=entier(sx);
				y1[dy]=entier(sy);
			}
			//
			// V‚rifie si notre pixel est plus … droite que le dernier trac‚
			// Si oui, enregistre son abscisse, sa profondeur,
			// ainsi que les coordonn‚es du point correspondant sur le segment
			// source (texture)
			//
			if (entier(dx)>finx[dy])
			{
				finx[dy]=entier(dx);
				finz[dy]=entier(dz);
				x2[dy]=entier(sx);
				y2[dy]=entier(sy);
			}
			dx+=idx;
			dz+=idz;
			sx+=isx;
			sy+=isy;
		}
	}
}




//
// Routine qui effectue le tra‡age r‚el du polygone, de haut en bas.
//
void calcule_polygone_texture(unsigned char* texture,struct clipping fenetre,int ombrage,const int& ymin,const int& ymax)
{
	int x,y;
	int xdeb,xfin,zdeb,zfin;
	int xx1,yy1,xx2,yy2;
	int largeur_dest;
	long isx,isy,idz;
	long xx,yy,zz;
	int longueur;

	//
	// On trace verticalement, en partant du plus haut point du polygone,
	// jusqu'… son plus bas point.
	//
	for (y=ymin;y<=ymax;y++)
	{
		//
		// R‚cupŠre les coordonn‚es gauches et droite du segment … tracer.
		//
		xdeb=debx[y];
		xfin=finx[y];

		//
		// Si le segment est au moins partiellement dans la fenetre alors,
		// on l'affiche, sinon on passe au suivant.
		//
		if ((xfin>=fenetre.gauche) && (xdeb<=fenetre.droite))
		{
			//
			// R‚cupŠre la profondeur du segment de destination,
			// et les coordonn‚esa des extr‚mit‚s du segment source.
			//
			zdeb=debz[y];
			zfin=finz[y];
			xx1=x1[y];
			yy1=y1[y];
			xx2=x2[y];
			yy2=y2[y];
			//
			// Calcule la largeur du segment AVANT CLIPPING, et en d‚duit les
			// incr‚ments sur Z (destination), et X,Y (source)
			//
			largeur_dest=xfin-xdeb+1;
			isx=etendu(xx2-xx1)/largeur_dest;
			isy=etendu(yy2-yy1)/largeur_dest;
			idz=etendu(zfin-zdeb)/largeur_dest;
			xx=etendu(xx1);
			yy=etendu(yy1);
			zz=etendu(zdeb);

			//
			// Effectue le clipping … gauche
			//
			if (xdeb<fenetre.gauche)
			{
				longueur=fenetre.gauche-xdeb;
				xx+=isx*longueur;
				yy+=isy*longueur;
				zz+=idz*longueur;
				xdeb=fenetre.gauche;
			}

			//
			// Effectue le clipping … droite
			//
			if (xfin>fenetre.droite)
				xfin=fenetre.droite;

			//
			// Trace horizontalement le segment, de gauche … droite.
			//
			for (x=xdeb;x<=xfin;x++)
			{
				//
				// Si notre point est plus prŠs que le pr‚c‚dent, alors
				// il remplace l'ancien.
				//
				if (entier(zz)>zbuf[y][x])
				{
					zcol[y][x]=colors[ombrage][(unsigned char)texture[entier(xx)+entier(yy)*80]];
					zbuf[y][x]=entier(zz);
				}
				xx+=isx;
				yy+=isy;
				zz+=idz;
			}
		}
	}
}





//
// Algorithme de tra‡age des polygones avec Z-Buffer
// - Polygone en ombrage de gouraud
// - Polygone mapp‚ avec une texture
//
// En paramŠtre:
// - num_face    Num‚ro de la facette a afficher
// - fenetre     Fenˆtre de clipping
// - ymin,ymax   Minima et Maxima pour le polygone, en Y.
//
void trace_facette(int num_face,struct clipping fenetre)
{
	int point,ymin,ymax;
	initialise_polygone(fenetre,ymin,ymax);
	switch (f[num_face].type)
	{
	case 1:
		//
		// Le polygone est affich‚ en ombrage de gouraud
		//
		// Trace virtuellement chaque segment
		//
		for (point=0;point<4;point++)
			calcule_segment_gouraud(f[num_face].p[point],f[num_face].p[(point+1) % 4],fenetre,ymin,ymax);
		//
		// Trace verticalement le polygone
		//
		calcule_polygone_gouraud(fenetre,ymin,ymax);
		break;
	case 2:
		//
		// Le polygone est mapp‚ avec une texture
		//
		// Trace virtuellement chaque segment
		//
		for (point=0;point<4;point++)
			calcule_segment_texture(f[num_face].p[point],f[num_face].p[(point+1) % 4],point,fenetre,ymin,ymax);
		//
		// Trace verticalement le polygone
		//
		calcule_polygone_texture(f[num_face].texture,fenetre,f[num_face].motif,ymin,ymax);
		break;
	}
}


//
// Attend que l'on rentre 'o/O' ou 'n/N'
// -> 0 si N/n
// -> 1 si O/o
//
char confirme()
{
	char c;
	printf("R‚pondez par (O)ui ou (N)on\n");
	do
	{
		c=getch();
	}
	while (!((c=='o') || (c=='O') || (c=='n') || (c=='N')));
	if ((c=='o') || (c=='O'))
		return (1);
	else
		return (0);
}


void affiche_infos()
{
	clrscr();
	printf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿\n");
	printf("³ Projet d'informatique graphique. Ann‚e 1993-1994.    ³\n");
	printf("³ Bin“me DUBIEF ArlŠne & POINTIER Micka‰l              ³\n");
	printf("³ ®Z-Buffer + Placage de texture + Ombrage de Gouraud¯ ³\n");
	printf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ\n\n");
	printf(" R‚alis‚ en Borland C++ 3.1 (DOS) \n");
	printf(" Tout le calcul est effectu‚ en ®temps r‚el¯\n\n");
	printf("Vous pouvez agir sur l'animation avec les touches:\n\n");
	printf("[GAUCHE/DROITE] Changer l'incr‚ment/d‚cr‚ment sur l'angle THETA.\n");
	printf("[HAUT/BAS]      Changer l'incr‚ment/d‚cr‚ment sur l'angle PHI.\n");
	printf("[F1...F12]      Changer la configuration de la scŠne.\n");
	printf("[0...9]         Changer la distance de la cam‚ra\n");
	printf("[ESCAPE/Q/q]    Quitter le programme.\n\n");
	printf("[*/-]           Activer ou non la sauvegarde automatique des images\n");
	printf("[/]             Sauver l'image courante\n\n");
	printf("[ESPACE]        Cette page d'informations\n\n");
}


void affiche_message(char &flag_sauvegarde,char* chemin)
{
	affiche_infos();
	printf("D‚sirez vous utiliser la sauvegarde ?\n");
	if (confirme())
	{
		flag_sauvegarde=1;
		clrscr();
		printf("Saisissez le chemin d'accŠs pour la sauvegarde:\n");
		printf("Exemple: E:\\DIVERS\\\IMAGES\\ \n\n");
		gets(chemin); //scanf("%80",chemin);
	}
	clrscr();
}


void sauve(int& num_image,char* chemin,char& flag_sauvegarde,struct clipping fenetre,char& old_rez)
{
	char chemin_image[101];
	char nom_image[9];
	char *extension_image=".P24";
	//
	// Cr‚ation du chemin complet avec le nom du type: "E:\DIVERS\1.P24"
	//
	itoa(num_image,nom_image,10);
	strcpy(chemin_image,chemin);
	strcat(chemin_image,nom_image);
	strcat(chemin_image,extension_image);
	//
	// Sauve l'image. Si le r‚sultat est diff‚rent de 0, c'est qu'il y a
	// eu une erreur. Dans ce cas, on interromp l'affichage.
	//
	if (sauvegarde_image_PICT24(chemin_image,fenetre.gauche,fenetre.haut,fenetre.droite,fenetre.bas)!=0)
	{
		flag_sauvegarde=0;
		change_rez(old_rez);
		clrscr();
		printf("Erreur durant la sauvegarde.\n");
		printf("La sauvegarde n'est plus possible.\n");
		printf("<< Appuyez sur une touche pour continuer >>");
		getch();
		change_rez(19);
		change_palette(palette);
	}
	num_image++;
}

//
// Demande une confirmation avant de quitter
//
char confirme_quitter(char& old_rez)
{
	char c;
	change_rez(old_rez);
	clrscr();
	printf("D‚sirez-vous quitter ???\n");
	c=confirme();
	change_rez(19);
	change_palette(palette);
	return(c);
}

//
// R‚capitule la liste des touches
//
void affiche_commandes(char& old_rez)
{
	change_rez(old_rez);
	affiche_infos();
	getch();
	change_rez(19);
	change_palette(palette);
}


//
// Le main. Tres gros.
//
void main(void)
{
	int face;
	int cx,cy,cz;	// Milieu de l'‚cran
	float st,sp,ct,cp;
	int the,phi,rho;
	int ithe,iphi;
	char c;
	float cs,ss,zz,sc;
	int d;
	float x,y,z,zr;
	int point;
	char old_rez;

	char chemin[101];

	char flag_sauvegarde=0;
	char flag_enregistre=0;
	char fin_programme=0;

	int num_image=0;

	struct clipping fenetre;

	fenetre.gauche=0;
	fenetre.droite=319;
	fenetre.haut=0;
	fenetre.bas=199;

	t[0].x=0;
	t[0].y=0;

	t[1].x=0;
	t[1].y=84;

	t[2].x=79;
	t[2].y=84;

	t[3].x=79;
	t[3].y=0;

	the =0;
	phi =180+25;
	ithe=8;
	iphi=0;

	affiche_message(flag_sauvegarde,chemin);

	old_rez=get_rez();
	change_rez(19);

	//
	// Change la palette de couleur courante
	//
	change_palette(palette);

	//
	// Efface tout le Z-Buffer avant de commencer
	//
	initialise_z_buffer(23,0);

	cx=160;
	cy=100;
	cz=2500;
	d=200  ;100;   //*COSQ(rot3&)
	rho=40;
	do
	{
		//
		// R‚initialisation de la fenˆtre de clipping (nettoyage...)
		// Avec un fonds noir.
		//
		initialise_fenetre(0,0,fenetre);

		//
		// Transformations 3D
		//

		st=sin(the*M_PI/180);
		sp=sin(phi*M_PI/180);
		ct=cos(the*M_PI/180);
		cp=cos(phi*M_PI/180);

		the =(the+ithe) % 360;
		phi =(phi+iphi) % 360;

		cs=ct*sp;
		ss=st*sp;
		zz=ct*cp;
		sc=st*cp;

		for (point=0;point<nb_point;point++)
		{
			x=-ps[point].x;
			y=ps[point].y;
			z=ps[point].z;
			zr=x*zz-y*sc-z*sp+rho;
			p[point].x=cx+(d*(x*st+y*ct))/zr;
			p[point].y=cy-(d*(x*cs-y*ss+z*cp))/zr;
			p[point].z=cz-zr;
			p[point].c=ps[point].c;
		}

		//
		// Tra‡age de tous les polygones dans le Z-Buffer
		//

		for (face=0;face<nb_face;face++)
			trace_facette(face,fenetre);
		//
		// affiche le r‚sultat … l'‚cran
		//
		affiche_z_buffer();

		//
		// Sauve si n‚c‚ssaire l'image sur disk.
		//
		if (flag_enregistre)
			sauve(num_image,chemin,flag_sauvegarde,fenetre,old_rez);

		if (kbhit())
		{
			c=getch();
			if (c!=0)
			{
				switch (c)
				{
				case 'q':
				case 'Q':
				case 27:
					if (confirme_quitter(old_rez))
						fin_programme=1;
					break;
				case '/':
					if (flag_sauvegarde)
						sauve(num_image,chemin,flag_sauvegarde,fenetre,old_rez);
					break;
				case '*':
					if (flag_sauvegarde)
						flag_enregistre=1;
					break;
				case '-':
					flag_enregistre=0;
					break;
				case '0':
					d=100;
					break;
				case '1':
					d=120;
					break;
				case '2':
					d=140;
					break;
				case '3':
					d=160;
					break;
				case '4':
					d=200;
					break;
				case '5':
					d=200;
					break;
				case '6':
					d=210;
					break;
				case '7':
					d=220;
					break;
				case '8':
					d=230;
					break;
				case '9':
					d=240;
					break;
				case ' ':
					affiche_commandes(old_rez);
				}
			}
			else
			{
				c=getch();
				switch (c)
				{
				case 75:    // <-
					ithe-=1;
					break;
				case 77:    // ->
					ithe+=1;
					break;
				case 72:    // ^
					iphi-=1;
					break;
				case 80:    // v
					iphi+=1;
					break;
				case 59:    // F1
					the=0;
					phi=205;
					ithe=8;
					iphi=0;
					break;
				case 60:    // F2
					the=0;
					phi=25;
					ithe=8;
					iphi=0;
					break;
				case 61:    // F3
					the=0;
					phi=0;
					ithe=5;
					iphi=4;
					break;
				case 62:    // F4
					the=0;
					phi=0;
					ithe=2;
					iphi=5;
					break;
				}
			}
		}
	}
	while (!fin_programme);
	change_rez(old_rez);
}


