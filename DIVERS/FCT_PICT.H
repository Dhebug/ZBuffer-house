/* FCT_PICT.H */

#include "types.h"

/* Initialisation de la librairie graphique */
void InitGraphMode(void (*)(), void (*)(), Color32 (*)(int,int));
/* Commutation en mode graphique */
void Ecran_Graphique(void);
/* Commutation en mode texte */
void Ecran_Texte(void);
/* Sauvegarde d'une image dans un format PICT 24 bits */
int  sauvegarde_image_PICT24(char *, int, int, int, int);
