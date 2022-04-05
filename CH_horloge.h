#ifndef CH_HORLOGE_H_INCLUDED
#define CH_HORLOGE_H_INCLUDED

#include "H_header.h"
#include <fstream>
#include <utility>
#include <iostream>
#include <allegro.h>

class Horloge
{
    private :

        int m_jour;
        int m_mois;
        int m_annee;

        std::pair <int, int> m_heure; //Sous forme pair < heure, minute >

    public :

        Horloge();
        ~Horloge();

        void afficher_heure_date(BITMAP* doubleBuffer, FONT* policeHeure, FONT* policeDate);

        int get_jour();
        int get_mois();
        int get_annee();

        std::pair <int ,int> get_heure();

        void actualiser_heure();
        void sauvegarder_heure();
};

#endif // CH_HORLOGE_H_INCLUDED
