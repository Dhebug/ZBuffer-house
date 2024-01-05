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

//#include <stdio.h>  // Juste pour le d‚bogage

//#include <stdlib.h>
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
int get_rez();
void change_rez(int rez);


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
//
//
int ymin,ymax;

void swap(int& a,int& b)
{
	int temp;
	temp=a;
	a=b;
	b=temp;
}



//
// Initialise le Z-Buffer … une couleur donn‚e pour une profondeur
// donn‚e.
//

void initialise_z_buffer(char couleur,struct clipping fenetre)
{
	int x,y;
	for (y=0;y<200;y++)
	{
		for (x=0;x<320;x++)
		{
			zbuf[y][x]=0;
			zcol[y][x]=couleur;
		}
	}
	for (y=fenetre.haut;y<=fenetre.bas;y++)
		for (x=fenetre.gauche;x<=fenetre.droite;x++)
			zcol[y][x]=0;
}



//
// On r‚affiche de fa‡on rapide un tableau lin‚aire de 64000
// octets correspondant aux 320x200 pixels du tableau ZCOL
//
void affiche_z_buffer()
{
	movedata(FP_SEG(zcol),FP_OFF(zcol),0xa000,0,64000);
}





//
// Initialise les tableaux Min et Max … des valeurs extrˆmes qui seront
// modifi‚es par Calcule_Segment_xxxxx.
//
void initialise_polygone(struct clipping fenetre)
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



/////////////////////////////// Partie Gouraud

//
// Trace virtuellement une ligne dans le tableau minimax
// Calcule par interpolation lin‚aire.
//
void calcule_segment_gouraud(int p1,int p2,struct clipping fenetre)
{
	long idx,idz,idc;

	long dx,dz,dc;

	int dy;
	int dx1,dy1,dz1,dc1,dx2,dy2,dz2,dc2;
	int d_hauteur;
	int longueur;

	dx1=p[p1].x; // Premier point de la ligne
	dy1=p[p1].y;
	dz1=p[p1].z;
	dc1=p[p1].c;

	dx2=p[p2].x; // Second point de la ligne
	dy2=p[p2].y;
	dz2=p[p2].z;
	dc2=p[p2].c;

	//
	// Si le premier point est plus bas, on ‚change les points
	//
	if (dy1>dy2)
	{
		swap(dx1,dx2);
		swap(dy1,dy2);
		swap(dz1,dz2);
		swap(dc1,dc2);
	}

	//
	// Teste si la ligne apparait dans la fenˆtre de clipping
	//
	if (dy2>fenetre.haut && dy1<fenetre.bas)
	{
		d_hauteur=dy2-dy1+1;

		idx=etendu(dx2-dx1)/d_hauteur;
		idz=etendu(dz2-dz1)/d_hauteur;
		idc=etendu(dc2-dc1)/d_hauteur;

		dx=etendu(dx1);
		dz=etendu(dz1);
		dc=etendu(dc1);

		//
		// Si le point sup‚rieur d‚passe de la fenˆtre, on clippe
		// les extr‚mit‚s
		//
		if (dy1<fenetre.haut)
		{
			longueur=fenetre.haut-dy1;
			dx+=idx*longueur;
			dz+=idz*longueur;
			dc+=idc*longueur;
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
			if (entier(dx)<debx[dy])
			{
				debx[dy]=entier(dx);
				debz[dy]=entier(dz);
				debc[dy]=entier(dc);
			}
			if (entier(dx)>finx[dy])
			{
				finx[dy]=entier(dx);
				finz[dy]=entier(dz);
				finc[dy]=entier(dc);
			}
			dx+=idx;
			dz+=idz;
			dc+=idc;
		}
	}
}




void calcule_polygone_gouraud(struct clipping fenetre)
{
	int x,y;
	int xdeb,xfin,zdeb,zfin,cdeb,cfin;
	int largeur_dest;
	long idz,idc;
	int longueur;

	long zz,cc;

	for (y=ymin;y<=ymax;y++)
	{
		xdeb=debx[y];
		xfin=finx[y];
		if ((xfin>=fenetre.gauche) && (xdeb<=fenetre.droite))
		{
			zdeb=debz[y];
			zfin=finz[y];
			cdeb=debc[y];
			cfin=finc[y];
			largeur_dest=xfin-xdeb+1;
			idz=etendu(zfin-zdeb)/largeur_dest;
			idc=etendu(cfin-cdeb)/largeur_dest;
			zz=etendu(zdeb);
			cc=etendu(cdeb);

			if (xdeb<fenetre.gauche)
			{
				longueur=fenetre.gauche-xdeb;
				zz+=idz*longueur;
				cc+=idc*longueur;
				xdeb=fenetre.gauche;
			}
			if (xfin>fenetre.droite)
				xfin=fenetre.droite;

			for (x=xdeb;x<=xfin;x++)
			{
				if (entier(zz)>zbuf[y][x])
				{
					zcol[y][x]=entier(cc);
					zbuf[y][x]=entier(zz);
				}
				zz+=idz;
				cc+=idc;
			}
		}
	}
}




/////////////////////////////// Partie textur‚e

void calcule_segment_texture(int p1,int p2,int pt,struct clipping fenetre)
{
	long isx,isy,idx,idz;
	long sx,sy,dx,dz;
	int dy;
	int sx1,sy1,sx2,sy2;
	int dx1,dy1,dz1,dx2,dy2,dz2;
	int d_hauteur;
	int longueur;

	sx1=t[pt %4].x;
	sy1=t[pt %4].y;

	sx2=t[(pt+1) %4].x;
	sy2=t[(pt+1) %4].y;

	dx1=p[p1].x;
	dy1=p[p1].y;
	dz1=p[p1].z;

	dx2=p[p2].x;
	dy2=p[p2].y;
	dz2=p[p2].z;

	if (dy1>dy2)
	{
		swap(dx1,dx2);
		swap(dy1,dy2);
		swap(dz1,dz2);
		swap(sx1,sx2);
		swap(sy1,sy2);
	}

	if (dy2>fenetre.haut && dy1<fenetre.bas)
	{
		d_hauteur=dy2-dy1+1;

		idx=etendu(dx2-dx1)/d_hauteur;
		idz=etendu(dz2-dz1)/d_hauteur;
		isx=etendu(sx2-sx1)/d_hauteur;
		isy=etendu(sy2-sy1)/d_hauteur;

		dx=etendu(dx1);
		dz=etendu(dz1);
		sx=etendu(sx1);
		sy=etendu(sy1);

		if (dy1<fenetre.haut)
		{
			longueur=fenetre.haut-dy1;
			dx+=idx*longueur;
			dz+=idz*longueur;
			sx+=isx*longueur;
			sy+=isy*longueur;
			dy1=fenetre.haut;
		}

		if (dy2>fenetre.bas)
			dy2=fenetre.bas;

		if (dy1<ymin)
			ymin=dy1;
		if (dy2>ymax)
			ymax=dy2;

		for (dy=dy1;dy<=dy2;dy++)
		{
			if (entier(dx)<debx[dy])
			{
				debx[dy]=entier(dx);
				debz[dy]=entier(dz);
				x1[dy]=entier(sx);
				y1[dy]=entier(sy);
			}
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
// Pr‚voir en paramŠtre le num‚ro de la texture ???
//
void calcule_polygone_texture(unsigned char* texture,struct clipping fenetre,int ombrage)
{
	int x,y;
	int xdeb,xfin,zdeb,zfin;
	int xx1,yy1,xx2,yy2;
	int largeur_dest;
	long isx,isy,idz;
	long xx,yy,zz;
	int longueur;

	for (y=ymin;y<=ymax;y++)
	{
		xdeb=debx[y];
		xfin=finx[y];
		if ((xfin>=fenetre.gauche) && (xdeb<=fenetre.droite))
		{
			zdeb=debz[y];
			zfin=finz[y];
			xx1=x1[y];
			yy1=y1[y];
			xx2=x2[y];
			yy2=y2[y];
			largeur_dest=xfin-xdeb+1;
			isx=etendu(xx2-xx1)/largeur_dest;
			isy=etendu(yy2-yy1)/largeur_dest;
			idz=etendu(zfin-zdeb)/largeur_dest;
			xx=etendu(xx1);
			yy=etendu(yy1);
			zz=etendu(zdeb);

			if (xdeb<fenetre.gauche)
			{
				longueur=fenetre.gauche-xdeb;
				xx+=isx*longueur;
				yy+=isy*longueur;
				zz+=idz*longueur;
				xdeb=fenetre.gauche;
			}
			if (xfin>fenetre.droite)
				xfin=fenetre.droite;

			for (x=xdeb;x<=xfin;x++)
			{
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
// - Polygone uni
// - Polygone en ombrage de gouraud
// - Polygone mapp‚ avec une texture
//
// En paramŠtre:
// - num_face    Num‚ro de la facette a afficher
// - fenetre     Fenˆtre de clipping
//
void trace_facette(int num_face,struct clipping fenetre)
{
	int point;
	initialise_polygone(fenetre);
	switch (f[num_face].type)
	{
	case 1:
		//
		// Le polygone est affich‚ en ombrage de gouraud
		//
		for (point=0;point<4;point++)
			calcule_segment_gouraud(f[num_face].p[point],f[num_face].p[(point+1) % 4],fenetre);
		calcule_polygone_gouraud(fenetre);
		break;
	case 2:
		//
		// Le polygone est mapp‚ avec une texture
		//
		for (point=0;point<4;point++)
			calcule_segment_texture(f[num_face].p[point],f[num_face].p[(point+1) % 4],point,fenetre);
		calcule_polygone_texture(f[num_face].texture,fenetre,f[num_face].motif);
		break;
	}
}





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
	int old_rez;

	char* nom_image="E:\ZBUFFER\IMAGE.P24";

	char fin_programme=0;

	struct clipping fenetre;


	old_rez=get_rez();
	change_rez(19);

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

	change_palette(palette);
	do
	{
		//
		// Mise … z‚ro du Z-Buffer
		//

		initialise_z_buffer(23,fenetre);

		//
		// Transformations 3D
		//
		cx=160;
		cy=100;
		cz=2500;
		d=200  ;100;   //*COSQ(rot3&)
		rho=40;

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
					fin_programme=1;
					break;
				case 's':
				case 'S':
					sauvegarde_image_PICT24(nom_image,fenetre.gauche,fenetre.haut,fenetre.droite,fenetre.bas);
					break;
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


