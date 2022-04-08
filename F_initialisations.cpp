#include "H_header.h"
#include "CH_ressources.h"
#include <iostream>

using namespace std;




//Sous-programme d'initialisation du vecteur de BITMAPS
void initTabBIT(vector<BITMAP*> &tabBIT)
{
    cout << "     ----- Chargement des BITMAPS" << endl;

    //Boucle de remplissage du vecteur de BITMAPS
    for(int i=0 ; i<NBRE_BITMAPS ; i++)
    {
        tabBIT.push_back(new BITMAP); //Ajout d'une nouvelle BITMAP

        switch(i)
        {
            case 0 : //Double buffer
                tabBIT[0] = create_bitmap(SCREEN_W, SCREEN_H);
                break;

            case 1 : //Fond du menu principal
                tabBIT[1] = load_bitmap("images/menus/fondMenuPrincipal.bmp", NULL);
                break;

            case 2 : //Fond écran du menu des complexités
                tabBIT[2] = load_bitmap("images/menus/fondMenuEnvergure.bmp", NULL);
                break;

            case 3 : //Curseur
                tabBIT[3] = load_bitmap("images/systeme/curseur.bmp", NULL);
                break;

            case 4 : //Interface de la carte du monde JOUR
                tabBIT[4] = load_bitmap("images/interface/interfaceCarteMondeJour.bmp", NULL);
                break;

            case 5 : //Interface de la carte du monde NUIT
                tabBIT[5] = load_bitmap("images/interface/interfaceCarteMondeNuit.bmp", NULL);
                break;

            case 6 : //Interface de la carte du monde JOUR VERSION AFFICHEE
                tabBIT[6] = create_bitmap(SCREEN_W, SCREEN_H);
                break;

            case 7 : //Interface de la carte du monde NUIT VERSION AFFICHEE
                tabBIT[7] = create_bitmap(SCREEN_W, SCREEN_H);
                break;

            case 8 : //Pointeur de localisation
                tabBIT[8] = load_bitmap("images/interface/pointeurLocalisation.bmp", NULL);
                break;

            case 9 : //Pointeur de localisation actif
                tabBIT[9] = load_bitmap("images/interface/pointeurLocalisationActif.bmp", NULL);
                break;

            case 10 : //Bouton Pause
                tabBIT[10] = load_bitmap("images/interface/boutonPause.bmp", NULL);
                break;

            case 11 : //Bouton Pause Actif
                tabBIT[11] = load_bitmap("images/interface/boutonPauseActif.bmp", NULL);
                break;

            case 12 : //Bouton Play
                tabBIT[12] = load_bitmap("images/interface/boutonPlay.bmp", NULL);
                break;

            case 13 : //Bouton Play Actif
                tabBIT[13] = load_bitmap("images/interface/boutonPlayActif.bmp", NULL);
                break;

            case 14 : //Bouton Speed 1
                tabBIT[14] = load_bitmap("images/interface/boutonSpeed1.bmp", NULL);
                break;

            case 15 : //Bouton Speed 1 Actif
                tabBIT[15] = load_bitmap("images/interface/boutonSpeed1Actif.bmp", NULL);
                break;

            case 16 : //Bouton Speed 2
                tabBIT[16] = load_bitmap("images/interface/boutonSpeed2.bmp", NULL);
                break;

            case 17 : //Bouton Speed 2 Actif
                tabBIT[17] = load_bitmap("images/interface/boutonSpeed2Actif.bmp", NULL);
                break;

            case 18 : //Avion Court
                tabBIT[18] = load_bitmap("images/avions/avionJaune/avionJauneDessus.bmp", NULL);
                break;

            case 19 : //Avion Moyen
                tabBIT[19] = load_bitmap("images/avions/avionRouge/avionRougeDessus.bmp", NULL);
                break;

            case 20 : //Avion Long
                tabBIT[20] = load_bitmap("images/avions/avionViolet/avionVioletDessus.bmp", NULL);
                break;

            case 21 : //Fond du menu des routes aériennes
                tabBIT[21] = load_bitmap("images/interface/interfaceRouteAerienne.bmp", NULL);
                break;
        }

        //Si le tableau est toujours vide, c'est qu'il y a eu une erreur
        if(tabBIT[i] == 0)
        {
            cout << "Erreur de chargement de l'image " << i << endl;
        }
    }
}

// Référence du tableau de BITMAPS
/*
0 : Double buffer
1 : Menu principal
2 : Menu des complexités
3 : Curseur
4 : Carte monde JOUR
5 : Carte monde NUIT
6 : Carte monde JOUR AFFICHEE
7 : Carte monde NUIT AFFICHEE
8 : Pointeur de localisation
9 : Pointeur de localisation ACTIF
10 : Bouton Pause
11 : Bouton Pause ACTIF
12 : Bouton Play
13 : Bouton Play ACTIF
14 : Bouton Speed 1
15 : Bouton Speed 1 ACTIF
16 : Bouton Speed 2
17 : Bouton Speed 2 ACTIF
18 : Avion court
19 : Avion moyen
20 : Avion long
21 : Fond du menu des routes aériennes
*/


//Sous-programme d'initialisation du vecteur de FONTS
void initTabFONT(vector<FONT*> &tabFONT)
{
    cout << "     ----- Chargement des FONTS" << endl;

    //Boucle de remplissage du vecteur de FONTS
    for(int i=0 ; i<NBRE_POLICES ; i++)
    {
        tabFONT.push_back(new FONT); // Ajout d'une nouvelle FONT

        switch(i)
        {
            case 0 :
                tabFONT[0] = load_font("polices/pixellari/pixellari_38.pcx", NULL, NULL);
                break;

            case 1 :
                tabFONT[1] = load_font("polices/pixellari/pixellari_40.pcx", NULL, NULL);
                break;

            case 2 :
                tabFONT[2] = load_font("polices/pixellari/pixellari_20.pcx", NULL, NULL);
                break;

            case 3 :
                tabFONT[3] = load_font("polices/TOP GUN/TOP GUN_50.pcx", NULL, NULL);
                break;

            case 4 :
                tabFONT[4] = load_font("polices/Seven Segment/Seven Segment_45.pcx", NULL, NULL);
                break;

            case 5 :
                tabFONT[5] = load_font("polices/Seven Segment/Seven Segment_35.pcx", NULL, NULL);
                break;

            case 6 :
                tabFONT[6] = load_font("polices/Seven Segment/Seven Segment_55.pcx", NULL, NULL);
                break;

            case 7 :
                tabFONT[7] = load_font("polices/TOP GUN/TOP GUN_30.pcx", NULL, NULL);
                break;

            case 8 :
                tabFONT[8] = load_font("polices/pixellari/pixellari_25.pcx", NULL, NULL);
                break;

            case 9 :
                tabFONT[9] = load_font("polices/TOP GUN/TOP GUN_40.pcx", NULL, NULL);
                break;
        }

        //Si le tableau est toujours vide, c'est qu'il y a eu une erreur
        if(tabFONT[i] == 0)
        {
            cout << "Erreur de chargement de la FONT " << i << endl;
        }
    }
}

// Références du tableau de FONTS
/*
Pixellari_20 : 2
Pixellari_25 : 8
Pixellari_38 : 0
Pixellari_40 : 1

TOP GUN_30 : 7
TOP GUN_40 : 9
TOP GUN_50 : 3

Seven Segment_35 : 5
Seven Segment_45 : 4
Seven Segment_55 : 6
*/


//Sous-programme d'initialisation des BITMAPS de l'animation de l'AVION
void initAnimAvion(vector<BITMAP*> &animAvion)
{
    string nomFichierAvion; //Permet de plus facilement récupérer les BITMAPS dans les fichiers

    cout << "     ----- Chargement de l'animation de l'avion" << endl;

    for(int i=0 ; i<11 ; i++) //Boucle de remplissage du vecteur de BITMAPS de l'animation
    {
        animAvion.push_back(new BITMAP);

        //Actualisation du nom
        nomFichierAvion = "images/animations/avionDemarrageSimulation/animAvion";

        //Ajout du digit dans le nom
        nomFichierAvion += to_string(i+1);

        //Ajout de l'extension du fichier
        nomFichierAvion += ".bmp";

        //Chargement de la BITMAP
        animAvion[i] = load_bitmap(convertisseurStringChar(nomFichierAvion), NULL);
    }
}
