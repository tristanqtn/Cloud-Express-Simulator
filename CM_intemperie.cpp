#include "CH_intemperie.h"

using namespace std;



    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur surcharg�
Intemperie::Intemperie(string typeCrea)
{

    vector<int> altitudesTempo;

    //Cr�ation diff�rente en fonction de la volont� de l'utilisateur

    //SI choix d'une int�mp�rie al�atoire
    if(typeCrea == "aleatoire")
    {
        //G�n�ration al�atoire du type de l'intemp�rie
        m_typeIntemperie = rand() % (2-1 + 1) + 1 ;
    }
    //SINON SI choix de la pluie
    else if(typeCrea == "pluie")
    {
        //G�n�ration du type pluie
        m_typeIntemperie = 1;
    }
    //SINON SI choix du vent
    else if(typeCrea == "vent")
    {
        //G�n�ration du type vent
        m_typeIntemperie = 2;
    }

    //Initialisation al�atoire des coordonn�es de l'intemp�rie
    m_coord.set_coord_x(rand()%(1100 + 1));
    m_coord.set_coord_y(rand()%(400 + 1));

    //Initialisation al�atoire de la dur�e de l'intemp�rie
    m_dureeIntemperie = rand()%(30 - 10 + 1) + 10;

    //R�cup�ration d'un indice al�atoire
    int indice = rand()%(11 - 0 + 1) + 0;

    //Cr�ation du vecteur contenant l'ensemble des altitudes possibles
    altitudesTempo.push_back(5750);
    altitudesTempo.push_back(6500);
    altitudesTempo.push_back(7250);
    altitudesTempo.push_back(8000);
    altitudesTempo.push_back(8750);
    altitudesTempo.push_back(9500);
    altitudesTempo.push_back(10250);
    altitudesTempo.push_back(11000);
    altitudesTempo.push_back(11750);
    altitudesTempo.push_back(12500);
    altitudesTempo.push_back(13250);
    altitudesTempo.push_back(14000);

    //On pioche dans le vecteur � l'aide de l'indice
    m_altitudesIntemperies = altitudesTempo[indice];
}


//Destructeur
Intemperie::~Intemperie()
{

}


////////////////
// ACCESSEURS //
////////////////

//Getter du couple de coordonn�es de l'intemp�rie
Coord Intemperie::get_coordIntemperie()
{
    return m_coord;
}

//Getter de la coordonn�e en X de l'intemp�rie
float Intemperie::get_coordIntemperieX()
{
    return m_coord.get_coord_x();
}

//Getter de la coordonn�e en Y de l'intemp�rie
float Intemperie::get_coordIntemperieY()
{
    return m_coord.get_coord_y();
}

//Getter du type de l'intemp�rie
int Intemperie::get_type()
{
    return m_typeIntemperie;
}

//Getter de la dur�e de l'intemp�rie
int Intemperie::getDureeIntemperie()
{
    return m_dureeIntemperie;
}

//Getter de l'altitude de l'intemp�rie
int Intemperie::getAltitudes()
{
    return m_altitudesIntemperies;
}

//Setter des coordonn�es de l'intemp�rie
void Intemperie::set_coord(float x, float y)
{
    m_coord.set_coord_x(x);
    m_coord.set_coord_y(y);
}

//Setter de la dur�e de l'intemp�rie
void Intemperie::setDureeIntemperie(int nouvelleDuree)
{
    m_dureeIntemperie = nouvelleDuree;
}

//Setter de l'altitude de l'intemp�rie
void Intemperie::setAltitudeIntemperie(int altitudeIntemperie)
{
    m_altitudesIntemperies = altitudeIntemperie;
}



    //////////////
    // METHODES //
    //////////////

//M�thode d'actualisation al�atoire des coordonn�es de l'intemp�rie
void Intemperie::actualisationIntemperie()
{
    int intervalle = 5;

    //Actualisation al�atoire des coordonn�es de l'intemp�rie
    set_coord(m_coord.get_coord_x() + (rand()%(intervalle + intervalle + 1) - intervalle), m_coord.get_coord_y() + (rand()%(intervalle + intervalle + 1) - intervalle));

    //SI l'intemp�rie n'est pas termin�e
    if(getDureeIntemperie() > 0)
    {
        //D�cr�mentation de la dur�e de l'intemp�rie
        setDureeIntemperie(getDureeIntemperie() - 1);
    }
}


//M�thode d'affichage de l'intemp�rie en fonction de son type
void Intemperie :: afficherIntemperie(Ressources &motherShip)
{
    //Affichage diff�rent en fonction du type de l'intemp�rie

    //SI il s'agit de la pluie
    if(m_typeIntemperie == 1)
    {
        //On afiche la pluie
        masked_stretch_blit(motherShip.getBIT(24),motherShip.getBIT(0), 0,0, motherShip.getBIT(24)->w, motherShip.getBIT(24)->h, m_coord.get_coord_x(), m_coord.get_coord_y(), motherShip.getBIT(24)->w/7,motherShip.getBIT(24)->h/7);
    }
    //SINON SI il s'agit du vent
    else if(m_typeIntemperie == 2)
    {
        //On affiche le vent
        masked_stretch_blit( motherShip.getBIT(25),motherShip.getBIT(0), 0,0, motherShip.getBIT(25)->w, motherShip.getBIT(25)->h, m_coord.get_coord_x(), m_coord.get_coord_y(), motherShip.getBIT(25)->w/7,motherShip.getBIT(25)->h/7);
    }
}


