#ifndef H_HEADER_H_INCLUDED
#define H_HEADER_H_INCLUDED


    ////////////////////////////////
    // DECLARATION DES CONSTANTES //
    ////////////////////////////////

#define NBRE_BITMAPS 38
#define NBRE_POLICES 11
#define UT 10
#define MAX_INTEMPERIES 20
#define COORD_X_RECTANGLE_DROITE 1165


    ////////////////////////////////
    // INCLUSION DES BIBLIOTHQUES //
    ////////////////////////////////

#include "CH_ressources.h"
#include <allegro.h>
#include <vector>
#include <iostream>



    ////////////////////////////////
    // DECLARATION DES PROROTYPES //
    ////////////////////////////////

//Sous-programmes de REGLAGES SYSTEME
void initAllegro();

//Sous-programmes d'initialisation
void initTabBIT(std::vector<BITMAP*> &tabBIT);
void initTabFONT(std::vector<FONT*> &tabFONT);
void initAnimAvion(std::vector<BITMAP*> &animAvion);

//Sous-programmes de menu
bool menuPrincipal(Ressources &motherShip, bool &indicClic, int &modeSimulation, int &envergureSimulation);
void menuRegles(bool &indicClic, BITMAP *doubleBuffer, BITMAP *regles, BITMAP *flecheHautBlanche, BITMAP *flecheHautBleue, BITMAP *flecheBasBlanche, BITMAP *flecheBasBleue, BITMAP* curseur, FONT* policeRetour);
void menuSelectionEnvergure(bool &indicClic, bool &done, int &envergureSimulation, BITMAP* doubleBuffer, BITMAP* fondEcranComplexite, BITMAP* curseur, FONT* policeTitre, FONT* policeEnvergures, FONT* policeNbreAeroports, FONT* policeRetour);

//Sous-programmes d'affichages
void affichageCurseur(BITMAP* curseur, BITMAP* doubleBuffer);

//Sous-programmes pratiques
std::string chaine_aleatoire(int longueur);
const char* convertisseurStringChar(std::string motAConvertir);

#endif // H_HEADER_H_INCLUDED
