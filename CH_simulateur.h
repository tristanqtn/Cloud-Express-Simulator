#ifndef HEADER_SIMULATEUR_H_INCLUDED
#define HEADER_SIMULATEUR_H_INCLUDED


//#include <cstdlib>

#include "H_header.h"
#include "CH_avion.h"
#include "CH_horloge.h"
#include "CH_aeroport.h"
#include "CH_routeAerienne.h"



//Définition de la classe Simulateur
class Simulateur
{
    private :

        std::vector <Aeroport> m_aeroports;

        std::vector <Avion> m_flotte_avions;
        std::vector <Avion> m_infos_types_avions;

        std::vector <RouteAerienne*> m_ensembleRoutes;

        int m_modeSimulation;
        int m_envergureSimulation;

        Horloge m_horlogeGMT;


    public :

        Simulateur();
        ~Simulateur();

        int getIndiceAeroport(std::string nomAeroport);

        void initSimulateur(int modeSimulation, int envergureSimulation);
        void reinitialisationSimulateur();
        void afficher_aeroports();
        Avion creer_avion_aleatoire();
        void charger_carte_NORMALE();
        void charger_carte_EXTREME();
        void deroulementGlobal(Ressources &motherShip, bool &indicClic, bool &done);
        void menuESC(bool &done, bool &finDeroulement, bool &indicClic, bool &indicEchap, BITMAP* doubleBuffer, BITMAP* curseur, FONT* policeTitre, FONT* policeChoix);
        void initCartesFond(Ressources &motherShip);

        void ajouter_crash(std::string modele_avion, std::string immatriculation, std::string depart, std::string arrivee, int anne, int mois, int jour, int heure, int minute);
        void ouvrir_liste_crash() const;

};

#endif // HEADER_SIMULATEUR_H_INCLUDED
