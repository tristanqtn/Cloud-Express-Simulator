#ifndef CH_AVION_H_INCLUDED
#define CH_AVION_H_INCLUDED

#include <ctime>
#include <string>
#include <fstream>
#include <iostream>

#include "CH_aeroport.h"
#include "CH_coordonnees.h"

// Définition de la classe Avion
class Avion
{
    private :

        Coord m_coord;

        std::string m_modele;
        std::string m_type_vol;
        std::string m_immatriculation;

        int m_quantite_max_kerosene;
        int m_quantite_kerosene;
        int m_consommation;
        int m_altitude;
        int m_vitesse;

        int m_action_en_cours;
        int m_duree_prepraration;
        int m_duree_roulage;
        int m_duree_decollage;
        int m_duree_vol;
        int m_duree_atterrissage;
        int m_duree_sortie_piste;
        int m_duree_boucle_attente;

    public :

        Avion(std::string nom_fichier);
        Avion(const Avion & copie);
        ~Avion();

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

        void afficher_caracteristique();
        void actualiser_action_avion();
        void parametrer_nouveau_vol(Aeroport depart, Aeroport arrivee);
};

#endif // CH_AVION_H_INCLUDED
