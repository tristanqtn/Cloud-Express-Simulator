#ifndef HEADER_AEROPORT_H_INCLUDED
#define HEADER_AEROPORT_H_INCLUDED

#include "CH_coordonnees.h"
#include "CH_ressources.h"

#include <vector>
#include <fstream>
#include <iostream>

class Avion;

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

        std::vector <Avion *> m_queue_acces_piste;
        std::vector <Avion *> m_queue_sortie_piste;
        std::vector <Avion *> m_avions_parking;

        std::vector <std::vector <Avion*>> m_pistes;

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

        void ajouter_avion_parking(Avion &ajout_parking);
        void ajouter_avion_file_acces_piste(Avion &ajout_acces);
        void ajouter_avion_file_sortie_piste(Avion &ajout_sortie);

        Avion* sortir_avion_parking();
        Avion* sortir_avion_file_acces_piste();
        Avion* sortir_avion_file_sortie_piste();


        void actualisationAeroport();
        void actualisationPistes();
};


#endif // HEADER_AEROPORT_H_INCLUDED
