#ifndef CH_AVION_H_INCLUDED
#define CH_AVION_H_INCLUDED


//Inclusion des bibliothèques externes
#include <ctime>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


//Inclusion des bibliothèques internes
#include "H_header.h"
#include "CH_coordonnees.h"
#include "CH_avion&aeroport&route.h"
#include "CH_ressources.h"
#include "CH_horloge.h"



//Déclaration anticipée de la classe Aéroport
class Aeroport;
class RouteAerienne;


// Définition de la classe Avion
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

        std::string m_aeroportInitial;
        std::string m_aeroportFinal;

        int m_quantite_max_kerosene;
        int m_quantite_kerosene;
        int m_consommation;
        int m_altitude;
        int m_vitesse;
        float m_angle;
        float m_espacementAffichageX;
        float m_espacementAffichageY;

        int m_action_en_cours;
        int m_duree_prepraration;
        int m_duree_roulage;
        int m_duree_decollage;
        int m_duree_vol;
        int m_duree_atterrissage;
        int m_duree_sortie_piste;
        int m_duree_boucle_attente;

        std::vector<int> m_indiceEscales;
        std::vector <std::string> m_liste_escales;

        float m_a_arete;
        float m_b_arete;

        Horloge m_heureDepart;

        float m_distanceParcourue;

        bool m_etat_reservoir;

        int m_etatVol;
        int m_compteurDetournement;

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
        int get_etatVol();

        void set_etatVol(int nouvelEtat);

        void set_altitude(int nouvelleAltitude);

        int get_action_en_cours();
        int get_duree_prepraration();
        int get_duree_roulage();
        int get_duree_decollage();
        int get_duree_vol();
        int get_duree_atterrissage();
        int get_duree_sortie_piste();
        int get_duree_boucle_attente();
        bool get_etat_reservoir();

        void set_action_en_cours(int valeur);
        void set_duree_prepraration(int valeur);
        void set_duree_roulage(int valeur);
        void set_duree_decollage(int valeur);
        void set_duree_vol(int valeur);
        void set_duree_atterrissage(int valeur);
        void set_duree_sortie_piste(int valeur);
        void set_duree_boucle_attente(int valeur);
        void set_etat_reservoir(bool nouvel_etat);

        void set_modele (std::string _modele);
        void set_immatriculation (std::string _immatriculation);
        void set_coord(float x, float y);

        std::string getNomAeroportD();
        std::string getNomAeroportA();
        void setNomAeroportD(std::string nom);
        void setNomAeroportA(std::string nom);

        std::string getNomAeroportInitial();
        std::string getNomAeroportFinal();
        void setNomAeroportInitial(std::string nom);
        void setNomAeroportFinal(std::string nom);

        int getIndicePrecisEscale(int indice);
        std::vector<int> getTousIndicesEscales();
        void setListeEscales(std::vector<int> nouvelleListe);
        void set_liste_escales(std::string escale);

        Horloge getHeureDepart();
        void setHeureDepart(Horloge &heureRef);

        float getAArete();
        float getBArete();
        void setAArete(float nouveauA);
        void setBArete(float nouveauB);

        float getDistanceParcourue();
        void setDistanceParcourue(float nouvelleDistance);

        void enregistrer_informations();

        //Méthodes
        bool consommer_kerosene();

        //Affichages
        void afficher_caracteristique();
        void afficher_caracteristique2();
        void affichageAvionCarte(BITMAP* doubleBuffer, BITMAP* avionCourt, BITMAP* avionMoyen, BITMAP* avionLong);
        bool actualisationSurbrillanceAvion(std::vector<Aeroport> m_aeroports, bool typeAffichage, int x, int y, BITMAP* doubleBuffer, BITMAP* overlay, BITMAP* overlayInverse, BITMAP* avionCourtSurligne, BITMAP* avionMoyenSurligne, BITMAP* avionLongSurligne, FONT* titre, FONT* texte, FONT* policeHeure);

        //Informations
        void passageAEscaleSuivante();
        bool actualiser_action_avion();
        void recuperationInfosAeroportD(Aeroport *aeroportDepart);
        void actualisationPositionVol();
        void parametrer_nouveau_vol(float xD, float yD, float xA, float yA);
        void recuperationInfosAeroportA(Aeroport &aeroportArrivee);

        void changementVitesse(int typeIntemperie);

        void set_vitesse(int vitesse);

        void detournement_avion();
};


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
        std::vector <int> m_altitudesAvions;

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
        std::vector<std::vector<Avion*>> getPistes();
        std::pair<std::string, int> get_successeur_precis(int i);
        int getAltitudesAvions(int i);
        void setAltitudeAvions(int altitude);

        //Méthodes

        //Affichages
        void afficher_caracteristique(int nombre_aeroport);
        void actualisationPointeurLocalisation(Ressources &motherShip, std::vector<Aeroport> m_aeroports, bool &indicClic, bool &indicEchap);
        void menuAeroport(Ressources &motherShip, std::vector<Aeroport> m_aeroports, bool &indicClic, bool &indicEchap);

        //Gestion de l'aéroport
        void ajouter_aeroport_connecte(std::string nom_aeroport, int distance);

        //Gestion des avions
        void actualisationAeroport(std::vector<Aeroport> m_aeroports, std::vector<RouteAerienne*> &m_ensembleRoutes, int indiceAeroport, int** m_matrice_adjacence, Horloge heureActuelle);
        void actualisationBoucleAttente(std::vector<RouteAerienne*> &m_ensembleRoute);
        void actualisationSortiePistes();
        void actualisationPistes(std::vector<RouteAerienne*> &m_ensembleRoutes);
        void actualisationAccesPistes();
        void actualisationParking(std::vector<Aeroport> m_aeroports, int indiceAeroport, int** m_matrice_adjacence, Horloge heureActuelle);
        void ajoutAvionParking(Avion *nouvelAvion);

        //Recherche du plus court chemin
        std::string piocheAeroportAleatoire();
        std::vector <int> dijkstra(int** m_matrice_adjacence, int m_envergureSimulation, int depart, int arrivee);
        int distance_minimale(int m_envergureSimulation, int * dist, bool * verif);
        void afficher_chemin(int * predecesseur, int j, std::vector <int> &correspondance);
        std::vector <int> resultat(int * dist, bool * verif, int * predecesseur, int source, int arrivee);

        int distance_maximale(std::vector<int> vecteur_escales, int **m_matrice_adjacence,int distance);
        bool verifier_distance_chemin (Avion * avion_utlise, std::vector<int> vecteur_escales, int **m_matrice_adjacence,int distance);
                //Vérif
        bool validite_dijkstra(int distance, std::string type_vol);
};


//Défintion de la classe Route Aérienne
class RouteAerienne
{
    private :

        std::vector <std::vector <int>> m_etatRoute;

        std::vector <Aeroport*> m_aeroportsRelies;
        std::vector <Avion*> m_avionsPresents;

        int m_longueur;

        float m_a_route;
        float m_b_route;

    public :

        //Constructeurs et destructeur
        RouteAerienne(Aeroport aeroport1, Aeroport aeroport2, int longueur);
        RouteAerienne();
        ~RouteAerienne();

        //Accesseurs
        Aeroport* getAeroport(int indiceAeroport);
        std::vector <Avion*> getAvionsPresents();
        float getARoute();
        float getBRoute();
        int getLongueur();
        void setARoute(float nouveauA);
        void setBRoute(float nouveauB);
        void setLongueur(int longueur);

        void initTabEtats();
        std::vector<std::vector<int>> getTabEtats();
        void actualiserCase(int indiceX, int indiceY, int valeur);
        void supprimer_avion(std::string immatriculation);

        //Méthodes

        //Gestion des avions
        void initAeroports(Aeroport &aeroport1, Aeroport &aeroport2);
        void suppressionAvionRoute(int indiceAvion);
        void ajoutAvion(Avion *avionAAjouter);

        //Affichages
        void afficherInfos();
        void actualisationSurbrillanceRoute(Ressources &motherShip, bool &indicClic, bool &indicEchap);


        //Menus
        void menuRouteAerienne(Ressources &motherShip, bool &indicEchap);
};

#endif // CH_AVION_H_INCLUDED

