// Module MAIN.C
// Julien Bourgeois
// Arläne Dubief
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
	//on passe en paramätre la chapelle initialisÇe (c) et celle qui sera
	//affichÇe apräs les calculs (c1)
	trace_catmull_rom(c,c1);
	closegraph() ;
}
