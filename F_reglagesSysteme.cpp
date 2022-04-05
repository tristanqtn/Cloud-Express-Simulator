#include "H_header.h"


//Initialisation d'Allegro
void initAllegro()
{
    //Initialisation d'Allegro
    allegro_init();

    //Définition du mode graphique
    set_color_depth(desktop_color_depth());
    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,1500,750,0,0))!=0) //Erreur dans la mise en place du mode graphique
    {
        allegro_message("Probleme de mode graphique") ;
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    //Installation du clavier
    install_keyboard();

    //Installation de la souris
    install_mouse();
}
