#include "H_header.h"
#include "CH_ressources.h"
#include <fstream>
#include <iostream>

using namespace std;


//Sous-programme mettant en jeu le menu principal
bool menuPrincipal(Ressources &motherShip, bool &indicClic, int &modeSimulation, int &envergureSimulation)
{
    bool done = false; //Indicateur de sortie du menu principal (et donc du programme)
    int espaceRetourLigne = 50; //Espace séparant les premières lignes des deuxième lignes de texte
    int centrageTexte = SCREEN_W/2 + 495; //Permet de centrer les textes cliquables
    bool quitter = false; //Indicateur de fin de programme

    while(!done)
    {

        //Réinitialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //Affichge du fond d'écran
        blit(motherShip.getBIT(1), motherShip.getBIT(0), 0, 0, 0, 0, motherShip.getBIT(1)->w, motherShip.getBIT(1)->h);

        //Affichage du menu de base
        textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 340, makecol(255,255,255), -1, "Pilote automatique");

        textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 435, makecol(255,255,255), -1, "Prendre les");
        textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 435+espaceRetourLigne, makecol(255,255,255), -1, "commandes");

        textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), 40, SCREEN_H-60, makecol(255,255,255), -1, "?");

        textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 635, makecol(255,255,255), -1, "Quitter");

        //Si l'utilisateur est sur l'un des choix, celui-ci s'affiche dans une autre couleur
        if((mouse_x >= 1060 && mouse_x <= 1435) && (mouse_y >=340 && mouse_y <= 380)) //Pilote automatique
        {
            textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 340, makecol(250,202,48), -1, "Pilote automatique");

            if(mouse_b & 1 && indicClic == false) // L'utilisateur clique sur la simulation automatique
            {
                indicClic = true; //Indication que le clic gauche est maintenu
                modeSimulation = 0; //Mode de simulation AUTOMATIQUE choisi

                menuSelectionEnvergure(indicClic, done, envergureSimulation, motherShip.getBIT(0), motherShip.getBIT(2), motherShip.getBIT(3), motherShip.getFONT(3), motherShip.getFONT(1), motherShip.getFONT(2), motherShip.getFONT(0));
            }
        }

        else if((mouse_x >= 1120 && mouse_x <= 1373) && (mouse_y >=440 && mouse_y <= 523)) //Prendre les commandes
        {
            textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 435, makecol(250,202,48), -1, "Prendre les");
            textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 435+espaceRetourLigne, makecol(250,202,48), -1, "commandes");

            if(mouse_b & 1 && indicClic == false) //L'utilisateur clique la simulation contrôlée
            {
                indicClic = true; //Indication que le clic gauche est maintenu
                modeSimulation = 1; //Mode de simulation CONTROLEE choisi

                menuSelectionEnvergure(indicClic, done, envergureSimulation, motherShip.getBIT(0), motherShip.getBIT(2), motherShip.getBIT(3), motherShip.getFONT(3), motherShip.getFONT(1), motherShip.getFONT(2), motherShip.getFONT(0));
            }
        }

        else if((mouse_x >= 33 && mouse_x <= 53) && (mouse_y >=695 && mouse_y <= 730)) //Règles
        {
            textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), 40, SCREEN_H-60, makecol(250,202,48), -1, "?");

            if(mouse_b & 1 && indicClic == false) // L'utilisateur clique sur les règles
            {
                indicClic = true; //Indication que le clic gauche est maintenu

                //LANCER LES REGLES

                //menuRegles(indicClic, motherShip.getBIT(0), motherShip.getBIT(61), motherShip.getBIT(59), motherShip.getBIT(60), motherShip.getBIT(57), motherShip.getBIT(58), motherShip.getBIT(9), motherShip.getFONT(6)); //Lancement du menu des règles
            }
        }

        else if((mouse_x >= 1180 && mouse_x <= 1315) && (mouse_y >=638 && mouse_y <= 674)) //Quitter
        {
            textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(0), centrageTexte, 635, makecol(255,0,0), -1, "Quitter");

            if(mouse_b & 1 && indicClic == false) //L'utilisateur clique sur "QUITTER"
            {
                indicClic = true; //Indication que le clic gauche est maintenu
                done = true; //Indication que la boucle du menu peut se terminer
                quitter = true; //Indication que l'on peut quitter le programme
            }
        }

        //Affichage du curseur peronnalisé
        affichageCurseur(motherShip.getBIT(3), motherShip.getBIT(0));

        //Affichage du double buffer
        blit(motherShip.getBIT(0), screen, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);

        //Test pour savoir si le joueur maintient le clic gauche ou pas
        if(!(mouse_b & 1))
        {
            indicClic = false; //Indicateur de pression réinitialisé si le joueur n'appuie pas
        }

        //Autre possibilité : l'utilisateur appuie sur ESC, le programme s'arrête
        if(key[KEY_ESC])
        {
            quitter = true;
            done = true;
        }

    }

    return quitter;
}


//Sous-programme du menu des règles
void menuRegles(bool &indicClic, BITMAP *doubleBuffer, BITMAP *regles, BITMAP *flecheHautBlanche, BITMAP *flecheHautBleue, BITMAP *flecheBasBlanche, BITMAP *flecheBasBleue, BITMAP* curseur, FONT* policeRetour)
{
    int screenY = 0; //Permet le décalage vertical

    //Boucle de déroulement tant que le joueur n'appuie pas sur SPACE
    while(!key[KEY_SPACE])
    {
        //Réinitialisation du double buffer
        clear_bitmap(doubleBuffer);

        //Affichage des règles
        blit(regles,doubleBuffer,0,screenY,0,0,SCREEN_W,SCREEN_H); //Image de fond
        rectfill(doubleBuffer,0,650,1500,750,makecol(21,21,21));

        //Affichage de la touche de RETOUR
        textprintf_ex(doubleBuffer, policeRetour, 50, 685, makecol(250, 202, 48), -1, "Appuyez sur ESPACE pour retourner sur le menu principal");

        // Flèches normales
        if(screenY > 0)
        {
            //Affichage de la flèche haute
            masked_stretch_blit(flecheHautBlanche,doubleBuffer,0,0,flecheHautBlanche->w,flecheHautBlanche->h,1400,30,100,100);
        }
        if(screenY < regles->h - SCREEN_H)
        {
            //Affichage de la flèche bas
            masked_stretch_blit(flecheBasBlanche,doubleBuffer,0,0,flecheBasBlanche->w,flecheBasBlanche->h,1400,620,100,100);
        }

        //Flèches lorsque l'utilisateur appuie dessus
        if((mouse_x>=1400 && mouse_x<=1500 && mouse_y>=620 && mouse_y<=720 && mouse_b & 1) || key[KEY_DOWN])
        {
            //Flèche hbas bleue et décalage
            screenY+=5;
            masked_stretch_blit(flecheBasBleue,doubleBuffer,0,0,flecheBasBleue->w,flecheBasBleue->h,1400,620,100,100);
        }

        if((mouse_x>=1400 && mouse_x<=1500 && mouse_y>=30 && mouse_y<=130 && mouse_b & 1) || key[KEY_UP])
        {
            //Flèche haut bleue et décalage
            screenY-=5;
            masked_stretch_blit(flecheHautBleue,doubleBuffer,0,0,flecheHautBleue->w,flecheHautBleue->h,1400,30,100,100);
        }

        //Disparition des flèches lorsque les limites verticales sont atteintes
        if ( screenY < 0 )
        {
            screenY = 0;
            rectfill(doubleBuffer,1400,30,1500,300,makecol(21,21,21));
        }
        if ( screenY > regles->h - SCREEN_H )
        {
            screenY = regles->h - SCREEN_H;
            rectfill(doubleBuffer,1400,620,1500,720,makecol(21,21,21));
        }

        //Affichage du curseur
        //affichageCurseur(curseur, doubleBuffer);

        //Affichage du double buffer
        blit(doubleBuffer, screen, 0, 0, 0, 0, doubleBuffer->w, doubleBuffer->h);

        //Test pour savoir si le joueur maintient le clic gauche ou pas
        if(!(mouse_b & 1))
        {
            indicClic = false; //Indicateur de pression réinitialisé si le joueur n'appuie pas
        }
    }
}


//Sous-programme du menu de sélection du nombre d'aéroports
void menuSelectionEnvergure(bool &indicClic, bool &done, int &envergureSimulation, BITMAP* doubleBuffer, BITMAP* fondEcranComplexite, BITMAP* curseur, FONT* policeTitre, FONT* policeEnvergures, FONT* policeNbreAeroports, FONT* policeRetour)
{
    bool finMenu = false; //Indicateur de fin du menu

    while(!finMenu)
    {
        //Réinitialisation du double buffer
        clear_bitmap(doubleBuffer);

        //Affichage du titre du menu
        blit(fondEcranComplexite, doubleBuffer, 0, 0, 0, 0, fondEcranComplexite->w, fondEcranComplexite->h);
        textprintf_centre_ex(doubleBuffer, policeTitre, SCREEN_W/2, 40, makecol(255, 255, 255), -1, "ENVERGURE DU RESEAU");

        //SI l'utilisateur positionne sa souris sur NORMALE
        if((mouse_x >= 235 && mouse_x <= 530) && (mouse_y >=270 && mouse_y <= 440))
        {
            circlefill(doubleBuffer, SCREEN_W/4, 365, 150, makecol(105, 184, 230)); //Affichage d'un cercle en fond

            if(mouse_b & 1 && indicClic == false) //L'utilisateur choisit l'envergure normale
            {
                indicClic = true; //Indication qu'un clic a lieu
                finMenu = true; //Indication que l'on peut sortir de ce menu
                done = true; //Indication que l'on peut sortir du menu principal afin de commencer le processus de création d'une simulation

                envergureSimulation = 7; //L'envergure choisie est NORMALE
            }
        }

        //SINON SI l'utilisateur positionne sa souris sur EXTREME
        else if((mouse_x >= 970 && mouse_x <= 1290) && (mouse_y >=270 && mouse_y <= 440))
        {
            circlefill(doubleBuffer, 3*SCREEN_W/4, 365, 150, makecol(14, 89, 132)); //Affichage d'un cercle en fond

            if(mouse_b & 1 && indicClic == false) //L'utilisateur choisit l'envergure extrême
            {

                indicClic = true; //Indication qu'un clic a lieu
                finMenu = true; //Indication que l'on peut sortir de ce menu
                done = true; //Indication que l'on peut sortir du menu principal afin de commencer le processus de création d'une simulation

                envergureSimulation = 15; //L'envergure choisie est EXTREME
            }
        }

        //Affichage du menu
        textprintf_centre_ex(doubleBuffer, policeEnvergures, SCREEN_W/4, 340, makecol(255,255,255), -1, "NORMALE");
        textprintf_centre_ex(doubleBuffer, policeNbreAeroports, SCREEN_W/4, 390, makecol(255,255,255), -1, "7 aeroports");
        textprintf_centre_ex(doubleBuffer, policeEnvergures, 3*SCREEN_W/4, 340, makecol(255,255,255), -1, "EXTREME");
        textprintf_centre_ex(doubleBuffer, policeNbreAeroports, 3*SCREEN_W/4, 390, makecol(255,255,255), -1, "15 aeroports");
        textprintf_centre_ex(doubleBuffer, policeRetour, SCREEN_W/2, 650, makecol(255,255,255), -1, "RETOUR");

        if((mouse_x >= 665 && mouse_x <= 840) && (mouse_y >=655 && mouse_y <= 690)) //Le joueur est sur le bouton "RETOUR"
        {
            textprintf_centre_ex(doubleBuffer, policeRetour, SCREEN_W/2, 650, makecol(255, 0, 0), -1, "RETOUR");

            if(mouse_b & 1 && indicClic == false) //L'utilisateur choisit de retourner sur le menu principal
            {
                indicClic = true;
                finMenu = true;
            }
        }

        //Affichage du curseur peronnalisé
        affichageCurseur(curseur, doubleBuffer);

        //Affichage du double buffer
        blit(doubleBuffer, screen, 0, 0, 0, 0, doubleBuffer->w, doubleBuffer->h);

        //Test pour savoir si le joueur maintient le clic gauche ou pas
        if(!(mouse_b & 1))
        {
            indicClic = false; //Indicateur de pression réinitialisé si le joueur n'appuie pas
        }
    }
}

