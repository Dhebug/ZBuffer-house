// Module CHAPELLE.C
// Julien Bourgeois
// ArlŠne Dubief

#include "chapelle.h"
#include <graphics.h>
#include <math.h>
#include <malloc.h>
#include <conio.h>


// Affichage de la chapelle en perspective mode file de fer
void trace_chapelle(chapelle *c1)
{
 int v=10;
 int i;
 int x1,y1,x2,y2;
 for(i=0;i<nb_arretes;i++)
	{
		x1=(int)((c1->tab[i].p1.x)/(c1->tab[i].p1.d));
		y1=(int)((c1->tab[i].p1.y)/(c1->tab[i].p1.d));
		x2=(int)((c1->tab[i].p2.x)/(c1->tab[i].p2.d));
		y2=(int)((c1->tab[i].p2.y)/(c1->tab[i].p2.d));

		//on modifie le repŠre, on se place au centre de l'‚cran
		line(320+x1*v,240+y1*v,320+x2*v,240+y2*v);
	}
}

// calcul des nouvelles coordonn‚es de la chapelle en perspective en fonction
// de l'observateur et du point vis‚
void calcul_chapelle(chapelle *c,point point_obs,point point_vise,
		     chapelle *c1)
{
	int i;
	point *normale=malloc(sizeof(point));
	double alpha;
	double variable;
	double matrice[4][4];

	 calcul_normale(point_obs,point_vise,normale);

	 // calcul de la valeur de alpha
	 variable=sqrt( 1.0 - normale->z*normale->z);
	 if (variable==0)
			alpha = 10000000; // valeur -> l'infini
	 else
			alpha = 1.0/variable;

	 init_matrice(normale,alpha,point_obs,matrice);

	 // calcul de c1 (nouvelle coordonn‚es de la chapelle)
	 for(i=0;i<nb_arretes;i++)
	 {
		produit_matrice(&(c->tab[i].p1),matrice,&(c1->tab[i].p1));
		produit_matrice(&(c->tab[i].p2),matrice,&(c1->tab[i].p2));
	 }
}

// calcul de la normale en fonction de la position de l'observateur et du
// point vis‚
void calcul_normale(point point_obs,point point_vise,point *normale)
{
 double variable=sqrt(((point_vise.x - point_obs.x)*(point_vise.x - point_obs.x))
				 +((point_vise.y - point_obs.y)*(point_vise.y - point_obs.y))
				 +((point_vise.z - point_obs.z)*(point_vise.z - point_obs.z)));

		normale->x = (point_vise.x - point_obs.x) / variable;
		normale->y = (point_vise.y - point_obs.y) / variable;
		normale->z = (point_vise.z - point_obs.z)/variable;
		normale->d=0;
}

// produit d'un point et d'une matrice (r‚sultat dans pr)
void produit_matrice(point *p,double matrice[4][4],point *pr)
{
	pr->x = (matrice[0][0] * p->x)
	 +(matrice[0][1] * p->y)
	 +(matrice[0][2] * p->z)
	 +(matrice[0][3] * 1.0);
	pr->y = (matrice[1][0] * p->x)
	 +(matrice[1][1] * p->y)
	 +(matrice[1][2] * p->z)
	 +(matrice[1][3] * 1.0);
	pr->z = (matrice[2][0] * p->x)
	 +(matrice[2][1] * p->y)
	 +(matrice[2][2] * p->z)
	 +(matrice[2][3] * 1.0);
	pr->d = (matrice[3][0] * p->x)
	 +(matrice[3][1] * p->y)
	 +(matrice[3][2] * p->z)
	 +(matrice[3][3] * 1.0);
}

//affichage de la chapelle en fonction d'une courbe lisse
//en utilisant Cattmul-Rom
void affiche_lisse(point *p,int n,double m[4][4],chapelle *c,chapelle *c1)
{
 int j,k ;
 double v,vecteur2[4],vecteur3[4];
 double x,y,z;
 point po,pv;

	for ( v = 0; v < 1; v+=1.0/n)
	{
		for ( j = 0 ; j < 4 ; j++ )
		{
		 if (v!=3-j)
			 vecteur2[j] =pow(v,(double)3-j);
		 else
			 vecteur2[j]=1;
		}

	 for ( j = 0 ; j < 4 ; j++ )
		for ( k = 0,vecteur3[j] = 0 ; k < 4 ; k++ )
			vecteur3[j] += vecteur2[k] * m[k][j] ;

	 x=0;
	 y=0;
	 z=0;
	 for ( j = 0; j < 4 ; j++ )
	 {
		x += vecteur3[j]*p[j].x ;    //le nouveau point calcul‚
		y += vecteur3[j]*p[j].y ;
		z += vecteur3[j]*p[j].z;
	 }

	 putpixel((int) x,(int) y,15) ;
	 //calcul puis affichage de la chapelle
	 //on affiche le point et la chapelle afin de v‚rifier … l'‚cran si le
	 //point et la chapelle se correspondent
	 po.x=(x-320)/10;
	 po.y=(y-240)/10;
	 po.z=z;
	 po.d=1;
	 pv.x=0;
	 pv.y=0;
	 pv.z=0;
	 pv.d=1;
	 calcul_chapelle(c,po,pv,c1);
	 clearviewport();
	 trace_chapelle(c1);
	}
}
//on obtient la chapelle c1 en utilisant la m‚thode de Cattmul-Rom
void trace_catmull_rom(chapelle *c,chapelle *c1)
{
 int j,k;
 //matrice pour la m‚thode Catmull Rom
 double catmull_rom[4][4]= {{-1, 3,-3, 1},
					{ 2,-5, 4,-1},
					{-1, 0, 1, 0},
					{ 0, 2, 0, 0}} ;
 //ligne polygonale … parcourir
 polygone p = { 9,{{50,50,20,1},{50,50,20,1},{300,70,30,1},{550,270,30,1},
 {220,330,20,1},{50,100,20,1},{30,290,30,1},{330,250,30,1},{330,250,30,1}}};

	for ( j = 0 ; j < 4 ; j++ )
	 for ( k = 0 ; k < 4 ; k++ )
		catmull_rom[j][k] *= 0.5 ;

	for ( j = 0 ; j < p.n-3 ; j++ )
	 affiche_lisse(&p.p[j],1000,catmull_rom,c,c1) ;
					 //n positions sur la courbe lisse
	while(!kbhit()) ;
	getch() ;
}

