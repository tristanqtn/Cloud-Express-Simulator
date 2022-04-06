#ifndef HEADER_AEROPORT_H_INCLUDED
#define HEADER_AEROPORT_H_INCLUDED

#include "CH_coordonnees.h"
#include "CH_ressources.h"

#include <vector>
#include <fstream>
#include <iostream>

// Acronymes des aéroports
// BKK : Bangkok
// CDG : Paris
// LPB : La Paz
// JFK : New York
// PER : Perth
// CPT : Cape Town
// PAP : Haiti

//YUL : Montréal
//MEX : Mexico
//HND : Tokyo
//JNB : Johanesburg
//HNL : Honolulu
//SCL : Santiago
//STO : Stockholm
//DFA : Abu Dhabi
//DEL : New Delhi
//UAK : Groenland
//CAI : Le Caire


// Définition de la classe Aéroport

class Aeroport
{
    private :

        std::string m_nom;
        std::string m_ville;

        int m_nombre_pistes;
        int m_nombre_places_sol;
        int m_delai_attente_sol;
        int m_temps_acces_pistes;
        int m_delai_anticollisions;
        int m_temps_decollage_atterissage;
        int m_duree_boucle_attente_vol;

        std::vector < std::pair<std::string, int> > m_distance_aeroports;

        Coord m_position;


    public :

        Aeroport(std::string nom_fichier, int nombre_aeroport);
        Aeroport();
        ~Aeroport();

        std::string get_nom();

        int get_nombre_pistes();
        int get_nombre_places_sol();
        int get_delai_attente_sol();
        int get_temps_acces_pistes();
        int get_delai_anticollisions();
        int get_temps_decollage_atterissage();
        int get_duree_boucle_attente_vol();

        std::vector < std::pair<std::string, int> > get_distance_aeroports();
        int get_distance_aeroports_preci(std::string nom_aeroport);

        Coord get_position();

        void ajouter_aeroport_connecte(std::string nom_aeroport, int distance);
        void afficher_caracteristique(int nombre_aeroport);
        void actualisationPointeurLocalisation(Ressources &motherShip, bool &indicClic, bool &indicEchap);
        void menuAeroport(Ressources &motherShip, bool &indicClic, bool &indicEchap);
};


#endif // HEADER_AEROPORT_H_INCLUDED
