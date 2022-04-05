#include "H_header.h"



//Sous-programme d'affichage du curseur personnalisé de la souris
void affichageCurseur(BITMAP* curseur, BITMAP* doubleBuffer)
{
    masked_stretch_blit(curseur, doubleBuffer, 0, 0, curseur->w, curseur->h, mouse_x, mouse_y, 30, 30);
}
