#ifndef CH_AVION_H_INCLUDED
#define CH_AVION_H_INCLUDED


//Inclusion des biblioth�ques externes
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


//Inclusion des biblioth�ques internes
#include "H_header.h"
#include "CH_coordonnees.h"
#include "CH_avion&aeroport.h"
#include "CH_ressources.h"



//D�claration anticip�e de la classe A�roport
class Aeroport;
class RouteAerienne;


// D�finition de la classe Avion
class Avion
{
    private :

        Coord m_coord;

        std::string m_modele;
        std::string m_type_vol;
        std::string m_immatriculation;

        std::pair<float, float> m_coordDepart;
        std::pair<float, float> m_coordArrivee;

        std::string m_aeroportD;
        std::string m_aeroportA;

        int m_quantite_max_kerosene;
        int m_quantite_kerosene;
        int m_consommation;
        int m_altitude;
        int m_vitesse;
        float m_angle;
        int m_espacementAffichageX;
        int m_espacementAffichageY;

        int m_action_en_cours;
        int m_duree_prepraration;
        int m_duree_roulage;
        int m_duree_decollage;
        int m_duree_vol;
        int m_duree_atterrissage;
        int m_duree_sortie_piste;
        int m_duree_boucle_attente;

        std::vector<int> m_indiceEscales;

    public :

        //Constructeurs et destructeur
        Avion(std::string nom_fichier);
        Avion(const Avion & copie);
        Avion();
        ~Avion();

        //Accesseurs
        Coord get_coord();

        std::string get_modele();
        std::string get_type_vol();
        std::string get_immatriculation();

        int get_quantite_max_kerosene();
        int get_quantite_kerosene();
        int get_consommation();
        int get_vitesse();
        int get_altitude();

        int get_action_en_cours();
        int get_duree_prepraration();
        int get_duree_roulage();
        int get_duree_decollage();
        int get_duree_vol();
        int get_duree_atterrissage();
        int get_duree_sortie_piste();
        int get_duree_boucle_attente();

        void set_action_en_cours(int valeur);
        void set_duree_prepraration(int valeur);
        void set_duree_roulage(int valeur);
        void set_duree_decollage(int valeur);
        void set_duree_vol(int valeur);
        void set_duree_atterrissage(int valeur);
        void set_duree_sortie_piste(int valeur);
        void set_duree_boucle_attente(int valeur);

        void set_modele (std::string _modele);
        void set_immatriculation (std::string _immatriculation);
        void set_coord(float x, float y);

        int getIndicePrecisEscale(int indice);
        std::vector<int> getTousIndicesEscales();
        void setListeEscales(std::vector<int> nouvelleListe);

        std::string getNomAeroportD();
        std::string getNomAeroportA();
        void setNomAeroportD(std::string nom);
        void setNomAeroportA(std::string nom);

        //M�thodes
        void passageAEscaleSuivante();
        void recuperationInfosAeroportD(Aeroport *aeroportDepart);
        void afficher_caracteristique();
        void afficher_caracteristique2();
        void actualiser_action_avion();
        void actualisationPositionVol();
        void parametrer_nouveau_vol(Aeroport depart, Aeroport arrivee);
        void affichageAvionCarte(BITMAP* doubleBuffer, BITMAP* avionCourt, BITMAP* avionMoyen, BITMAP* avionLong);
};


// Acronymes des a�roports
// BKK : Bangkok
// CDG : Paris
// LPB : La Paz
// JFK : New York
// PER : Perth
// CPT : Cape Town
// PAP : Haiti

//YUL : Montr�al
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


// D�finition de la classe A�roport

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
        std::vector <Avion *> m_queue_boucle_attente;

        std::vector <std::vector <Avion*>> m_pistes;

    public :

        //Constructeurs et destructeur
        Aeroport(std::string nom_fichier, int nombre_aeroport);
        Aeroport();
        ~Aeroport();

        //Accesseurs
        int get_nombre_pistes();
        int get_nombre_places_sol();
        int get_delai_attente_sol();
        int get_temps_acces_pistes();
        int get_delai_anticollisions();
        int get_temps_decollage_atterissage();
        int get_duree_boucle_attente_vol();
        std::string get_nom();
        int get_distance_aeroports_preci(std::string nom_aeroport);
        std::vector < std::pair<std::string, int> > get_distance_aeroports();
        Coord get_position();
        int getNbreAvionsParking();
        std::pair<std::string, int> get_successeur_precis(int i);
        std::vector<std::vector<Avion*>> getPistes();


        //M�thodes

        //Affichages
        void afficher_caracteristique(int nombre_aeroport);
        void actualisationPointeurLocalisation(Ressources &motherShip, bool &indicClic, bool &indicEchap);
        void menuAeroport(Ressources &motherShip, bool &indicClic, bool &indicEchap);

        //Gestion de l'a�roport
        void ajouter_aeroport_connecte(std::string nom_aeroport, int distance);

        //Gestion des avions
        void actualisationAeroport(std::vector<Aeroport> m_aeroports, std::vector<RouteAerienne*> &m_ensembleRoutes, int indiceAeroport, int** m_matrice_adjacence);
        void actualisationBoucleAttente(std::vector<RouteAerienne*> &m_ensembleRoute);
        void actualisationPistes(std::vector<RouteAerienne*> &m_ensembleRoutes);
        void actualisationAccesPistes();
        void actualisationSortiePistes();
        void actualisationParking(std::vector<Aeroport> m_aeroports, int indiceAeroport, int** m_matrice_adjacence);
        void ajoutAvionParking(Avion *nouvelAvion);

        std::string piocheAeroportAleatoire();

        std::vector <int> resultat(int * dist, bool * verif, int * predecesseur, int source, int arrivee);
        std::vector <int> dijkstra(int** m_matrice_adjacence, int m_envergureSimulation, int depart, int arrivee);
        void afficher_chemin(int * predecesseur, int j, std::vector <int> &correspondance);
        int distance_minimale(int m_envergureSimulation, int * dist, bool * verif);
};


//D�fintion de la classe Route A�rienne
class RouteAerienne
{
    private :

        std::vector <std::vector <int>> m_etatRoute;

        std::vector <Aeroport*> m_aeroportsRelies;
        std::vector <Avion*> m_avionsPresents;

        int m_longueur;

    public :
        RouteAerienne(Aeroport aeroport1, Aeroport aeroport2, int longueur);
        RouteAerienne();
        ~RouteAerienne();

        Aeroport* getAeroport(int indiceAeroport);
        std::vector <Avion*> getAvionsPresents();
        void setLongueur(int longueur);
        void suppressionAvionRoute(int indiceAvion);

        void initAeroports(Aeroport &aeroport1, Aeroport &aeroport2);
        void ajoutAvion(Avion *avionAAjouter);
        void afficherInfos();
        void actualisationSurbrillanceRoute(Ressources &motherShip, bool &indicClic, bool &indicEchap);
        void menuRouteAerienne(Ressources &motherShip, bool &indicEchap);
};

#endif // CH_AVION_H_INCLUDED

