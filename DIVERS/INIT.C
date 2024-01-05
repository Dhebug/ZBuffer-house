// Module INIT.C
// Julien Bourgeois
// ArlŠne Dubief

#include "chapelle.h"
#include <graphics.h>

// Initialisation du mode graphique en 640*480 16 couleurs
void installegraph()
{
	int gdriver,gmode;

	gdriver=VGA;
	gmode  =VGAHI;
	initgraph(&gdriver,&gmode,"c:\\borlandc\\bgi");
}

//initialisation des coordonn‚es de la chapelle
void init_chapelle(chapelle* c)
{
 int i;

	double liste_point[]=
	{0.0, 0.0, 0.0,
	 0.0, 10.0, 0.0,
	 3.0, 10.0, 0.0,
	 3.0, 0.0, 0.0,
	 0.0, 0.0, 3.0,
	 0.0, 10.0, 3.0,
	 3.0, 10.0, 3.0,
	 3.0, 0.0, 3.0,
	 0.0, 5.0, 3.0,
	 1.5, 7.5, 3.0,
	 3.0, 5.0, 3.0,
	 0.0, 0.0, 15.0,
	 0.0, 5.0, 15.0,
	 1.5, 7.5, 15.0,
	 3.0, 5.0, 15.0,
	 3.0, 0.0, 15.0,
	 1.5, 13.0, 1.5,
	 1.5, 15.0, 1.5,
	 1.0, 14.0, 1.5,
	 2.0, 14.0, 1.5
	 };
	int seg[]=
	{
	 0, 1,
	 1, 2,
	 2, 3,
	 3, 0,
	 4, 5,
	 5, 6,
	 6, 7,
	 8, 9,
	 9, 10,
	 11, 12,
	 12, 13,
	 13, 14,
	 14, 15,
	 1, 5,
	 2, 6,
	 0, 11,
	 3, 15,
	 8, 12,
	 9, 13,
	 10, 14,
	 1, 16,
	 2, 16,
	 5, 16,
	 6, 16,
	 15, 11,
	 16, 17,
	 18, 19
	};
	 for(i=0;i<nb_arretes;i++)
	 {
		// le repŠre utilis‚ est : x en abscisse
		//                         z en ordonn‚e
		//                         y en profondeur

		c->tab[i].p1.x=liste_point[seg[i*2]*3];
		c->tab[i].p1.z=liste_point[seg[i*2]*3+1];
		c->tab[i].p1.y=liste_point[seg[i*2]*3+2];
		c->tab[i].p1.d=1;
		c->tab[i].p2.x=liste_point[seg[(i*2)+1]*3];
		c->tab[i].p2.z=liste_point[seg[(i*2)+1]*3+1];
		c->tab[i].p2.y=liste_point[seg[(i*2)+1]*3+2];
		c->tab[i].p2.d=1;
	 }
}

//initialisation de la matrice … partir de la normale, de alpha,
//de la position de l'observateur
void init_matrice(point *normale,double alpha,point point_obs,
									double matrice[4][4])
{
 //matrice utilis‚e pour l'affichage de la chapelle en perspective

	 matrice[0][0] = alpha * normale->y;
	 matrice[0][1] = -alpha * normale->x;
	 matrice[0][2] = 0;
	 matrice[0][3] = (-alpha * normale->y * point_obs.x)
			 +(alpha * normale->x * point_obs.y);
	 matrice[1][0] = alpha*normale->x*normale->z;
	 matrice[1][1] = alpha*normale->y*normale->z;
	 matrice[1][2] = -1.0/alpha;
	 matrice[1][3] = -(alpha*point_obs.x * normale->x*normale->z)
			 -(alpha*point_obs.y * normale->y*normale->z)
			 +(point_obs.z /alpha);
	 matrice[2][0] = normale->x ;
	 matrice[2][1] = normale->y ;
	 matrice[2][2] = normale->z;
	 matrice[2][3] = -(point_obs.x * normale->x)
			 -(point_obs.y * normale->y )
			 -(point_obs.z*normale->z);
	 matrice[3][0] = normale->x/distance;
	 matrice[3][1] = normale->y/distance;
	 matrice[3][2] = normale->z/distance;
	 matrice[3][3] = (-point_obs.x*normale->x - point_obs.y*normale->y
				-point_obs.z*normale->z)/distance;
}


