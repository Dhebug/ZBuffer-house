// Module MAIN.C
// Julien Bourgeois
// Arl�ne Dubief
// Groupe 3A

#include "chapelle.h"
#include <graphics.h>
#include <stdlib.h>


void main(void)
{

	chapelle *c=malloc(sizeof(chapelle));
	chapelle *c1=malloc(sizeof(chapelle));
	init_chapelle(c);
	installegraph();
	//on passe en param�tre la chapelle initialis�e (c) et celle qui sera
	//affich�e apr�s les calculs (c1)
	trace_catmull_rom(c,c1);
	closegraph() ;
}
