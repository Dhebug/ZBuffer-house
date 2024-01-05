#include <graphics.h>

#include <conio.h>
#include <math.h>

struct coord_2D {
	int                 x,y ; } ;

struct polygone {
	int                 n ;
	struct coord_2D     p[20] ; } ;

typedef double matrice[4][4] ;

void     affiche_lisse(struct coord_2D *p,int n,matrice m,int c) ;

/* ******************************************************************* */

void affiche_lisse(p,n,m,c)
	struct coord_2D           *p ;
	int                       n,c ;
	matrice                   m ;
	{ int                       i,j,k ;
		double                    t,tt[4],ttt[4],x,y ;

		for ( i = 0 ; i < n ; i++ ) {
			t =(double) i/(n-1) ;

			for ( j = 0 ; j < 4 ; j++ )
//				tt[j] = ( t || 3-j ) ? pow(t,(double) 3-j) : 1 ;
				if (t!=3-j)
					tt[j] =pow(t,(double)3-j);
				else
					tt[j]=1;

			for ( j = 0 ; j < 4 ; j++ )
				for ( k = 0,ttt[j] = 0 ; k < 4 ; k++ )
					ttt[j] += tt[k] * m[k][j] ;

//z=0;
			for ( j = 0,x = 0,y = 0 ; j < 4 ; j++ ) {
				x += ttt[j] * p[j].x ;
				y += ttt[j] * p[j].y ;
//z+=ttt[j]*p[j].z;
			}

			x += 0.5 ;
			y += 0.5 ;
			putpixel((int) x,(int) y,c) ; }
	}
/* ------------------------------------------------------------------- */
void main()
	{ struct polygone   p = { 11,{{50,50},{50,50},{50,50},{300,70},{550,270},{220,330},{50,100},{30,290},{330,250},{330,250},{330,250}}} ;
		struct polygone   p3 = { 9,{{50,50},{50,50},{300,70},{550,270},{220,330},{50,100},{30,290},{330,250},{330,250}}} ;
		struct polygone   p2 = { 7,{{50,50},{300,70},{550,270},{220,330},{50,100},{30,290},{330,250}}} ;
		int               j,k ;
    matrice           bezier  = {{-1, 3,-3, 1},
                                 { 3,-6, 3, 0},
                                 {-3, 3, 0, 0},
                                 { 1, 0, 0, 0}} ;
    matrice           bspline = {{-1, 3,-3, 1},
                                 { 3,-6, 3, 0},
																 {-3, 0, 3, 0},
																 { 1, 4, 1, 0}} ;
		matrice           cr      = {{-1, 3,-3, 1},
																 { 2,-5, 4,-1},
																 {-1, 0, 1, 0},
																 { 0, 2, 0, 0}} ;
		int                   gd=VGA,gm=VGAMED ;

		initgraph(&gd,&gm,"c:\\borlandc\\bgi") ;
		cleardevice() ;
		setcolor(15) ;
		drawpoly(p.n,(int *) &p.p) ;

		for ( j = 0 ; j < 4 ; j++ )
			for ( k = 0 ; k < 4 ; k++ )
				bspline[j][k] *= 0.166666666 ;
		for ( j = 0 ; j < 4 ; j++ )
			for ( k = 0 ; k < 4 ; k++ )
				cr[j][k] *= 0.5 ;

		for ( j = 0 ; j < p.n-3 ; j++ )
			affiche_lisse(&p.p[j],1000,bspline,2) ;
		for ( j = 0 ; j < p3.n-3 ; j++ )
			affiche_lisse(&p3.p[j],1000,cr,4) ;

		while(!kbhit()) ;
		getch() ;
    closegraph() ;
	}

/* ******************************************************************* */
