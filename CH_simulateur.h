#ifndef HEADER_SIMULATEUR_H_INCLUDED
#define HEADER_SIMULATEUR_H_INCLUDED


#include "H_header.h"
#include "CH_avion&aeroport&route.h"
#include "CH_horloge.h"
#include "CH_intemperie.h"



//Définition de la classe Simulateur
class Simulateur
{
    private :

        int m_modeSimulation;
        int m_envergureSimulation;

        std::vector <Aeroport> m_aeroports;
        std::vector <Avion*> m_flotte_avions;
        std::vector <RouteAerienne*> m_ensembleRoutes;

        std::vector <Avion> m_infos_types_avions;

        Horloge m_horlogeGMT;

        int ** m_matrice_adjacence;

        std::vector<Intemperie*> m_listeIntemperies;

        std::vector <Coord> m_coord_crash;
        std::vector <int> m_duree_affichage_crash;


    public :

        //Constructeurs et destructeur
        Simulateur();
        ~Simulateur();

        //Accesseurs
        int getIndiceAeroport(std::string nomAeroport);


        //Méthodes

        //Fonctions directes du simulateur
        void initSimulateur(int modeSimulation, int envergureSimulation);
        void reinitialisationSimulateur();
        void charger_carte_NORMALE();
        void charger_carte_EXTREME();
        void deroulementGlobal(Ressources &motherShip, bool &indicClic, bool &done);
        void nouvelAvionDansParking(Avion *nouvelAvion);

        //Fonctions pratiques du simulateur
        void afficher_aeroports();
        void creer_avion(std::string type_de_vol);
        void supprimer_avion_aleatoire(std::string type_vol);
        void supprimer_avion_on_click(std::string immatriculation);

        void nouveau_crash(Avion * crash);
        void ouvrir_liste_crash();

        //Fonctions de menu du simulateur
        void menuESC(bool &done, bool &finDeroulement, bool &indicClic, bool &indicEchap, BITMAP* doubleBuffer, BITMAP* curseur, FONT* policeTitre, FONT* policeChoix);
        void menuPrincipal(Ressources &motherShip, bool &indicClic, int &etat);
        void menuAjoutAvion(Ressources &motherShip, bool &indicClic, int &etat);
        void menuSupprimerAvion(Ressources &motherShip, bool &indicClic, int &etat);
        void menuAjoutIntemperie(Ressources &motherShip, bool &indicClic, int &etat);

        //Indicateurs
        bool aeroportDisponible();
        bool encoreDesAvions();

        //Réglages
        void initCartesFond(Ressources &motherShip);

        void deduire_matrice_adjacence();
        void enregistrer_crash();
        bool verifier_distance_chemin_sim (std::string type_vol, std::vector<std::pair<std::string, int>> aeroports_connectes);
        int distance_minimale_sim(std::vector<std::pair<std::string, int>> aeroports_connectes);

        void algoWelsh();

        void rechercheIntemperie(Ressources& motherShip);
        void affichageQuadrillage(Ressources &motherShip);
};

#endif // HEADER_SIMULATEUR_H_INCLUDED
