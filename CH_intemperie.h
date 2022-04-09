#ifndef CH_INTEMPERIE_H_INCLUDED
#define CH_INTEMPERIE_H_INCLUDED

#include "CH_coordonnees.h"
#include "CH_ressources.h"
#include <iostream>


// Définition de la classe Intempérie
class Intemperie
{
    private :

        Coord m_coord;
        int m_typeIntemperie;
        int m_dureeIntemperie;
        int m_altitudesIntemperies;


    public :

        //Constructeurs et destructeur
        Intemperie(std::string typeCrea);
        ~Intemperie();

        //Accesseurs
        Coord get_coordIntemperie();
        float get_coordIntemperieX();
        float get_coordIntemperieY();
        int get_type();
        int getDureeIntemperie();
        int getAltitudes();
        void set_coord(float x, float y);
        void setDureeIntemperie(int nouvelleDuree);
        void setAltitudeIntemperie(int altitudeIntemperie);

        //Méthodes
        void actualisationIntemperie();
        void afficherIntemperie(Ressources &motherShip);

};

#endif // CH_INTEMPERIE_H_INCLUDED
