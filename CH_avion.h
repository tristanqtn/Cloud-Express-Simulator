#ifndef CH_AVION_H_INCLUDED
#define CH_AVION_H_INCLUDED

#include <ctime>
#include <string>
#include <fstream>
#include <iostream>

#include "CH_coordonnees.h"

// Définition de la classe Avion
class Avion
{
    private :

        Coord m_coord;

        std::string m_modele;
        std::string m_type_vol;
        std::string m_immatriculation;

        int m_quantite_kerosene;
        int m_consommation;
        int m_vitesse;

    public :

        Avion(std::string nom_fichier);
        Avion(const Avion & copie);
        ~Avion();

        Coord get_coord();

        std::string get_modele();
        std::string get_type_vol();
        std::string get_immatriculation();

        int get_quantite_kerosene();
        int get_consommation();
        int get_vitesse();

        void set_modele (std::string _modele);
        void set_immatriculation (std::string _immatriculation);

        void afficher_caracteristique();
};

#endif // CH_AVION_H_INCLUDED
