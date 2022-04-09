#include "CH_intemperie.h"

using namespace std;



    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur surchargé
Intemperie::Intemperie(string typeCrea)
{

    vector<int> altitudesTempo;

    //Création différente en fonction de la volonté de l'utilisateur

    //SI choix d'une intémpérie aléatoire
    if(typeCrea == "aleatoire")
    {
        //Génération aléatoire du type de l'intempérie
        m_typeIntemperie = rand() % (2-1 + 1) + 1 ;
    }
    //SINON SI choix de la pluie
    else if(typeCrea == "pluie")
    {
        //Génération du type pluie
        m_typeIntemperie = 1;
    }
    //SINON SI choix du vent
    else if(typeCrea == "vent")
    {
        //Génération du type vent
        m_typeIntemperie = 2;
    }

    //Initialisation aléatoire des coordonnées de l'intempérie
    m_coord.set_coord_x(rand()%(1100 + 1));
    m_coord.set_coord_y(rand()%(400 + 1));

    //Initialisation aléatoire de la durée de l'intempérie
    m_dureeIntemperie = rand()%(30 - 10 + 1) + 10;

    //Récupération d'un indice aléatoire
    int indice = rand()%(11 - 0 + 1) + 0;

    //Création du vecteur contenant l'ensemble des altitudes possibles
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

    //On pioche dans le vecteur à l'aide de l'indice
    m_altitudesIntemperies = altitudesTempo[indice];
}


//Destructeur
Intemperie::~Intemperie()
{

}


////////////////
// ACCESSEURS //
////////////////

//Getter du couple de coordonnées de l'intempérie
Coord Intemperie::get_coordIntemperie()
{
    return m_coord;
}

//Getter de la coordonnée en X de l'intempérie
float Intemperie::get_coordIntemperieX()
{
    return m_coord.get_coord_x();
}

//Getter de la coordonnée en Y de l'intempérie
float Intemperie::get_coordIntemperieY()
{
    return m_coord.get_coord_y();
}

//Getter du type de l'intempérie
int Intemperie::get_type()
{
    return m_typeIntemperie;
}

//Getter de la durée de l'intempérie
int Intemperie::getDureeIntemperie()
{
    return m_dureeIntemperie;
}

//Getter de l'altitude de l'intempérie
int Intemperie::getAltitudes()
{
    return m_altitudesIntemperies;
}

//Setter des coordonnées de l'intempérie
void Intemperie::set_coord(float x, float y)
{
    m_coord.set_coord_x(x);
    m_coord.set_coord_y(y);
}

//Setter de la durée de l'intempérie
void Intemperie::setDureeIntemperie(int nouvelleDuree)
{
    m_dureeIntemperie = nouvelleDuree;
}

//Setter de l'altitude de l'intempérie
void Intemperie::setAltitudeIntemperie(int altitudeIntemperie)
{
    m_altitudesIntemperies = altitudeIntemperie;
}



    //////////////
    // METHODES //
    //////////////

//Méthode d'actualisation aléatoire des coordonnées de l'intempérie
void Intemperie::actualisationIntemperie()
{
    int intervalle = 5;

    //Actualisation aléatoire des coordonnées de l'intempérie
    set_coord(m_coord.get_coord_x() + (rand()%(intervalle + intervalle + 1) - intervalle), m_coord.get_coord_y() + (rand()%(intervalle + intervalle + 1) - intervalle));

    //SI l'intempérie n'est pas terminée
    if(getDureeIntemperie() > 0)
    {
        //Décrémentation de la durée de l'intempérie
        setDureeIntemperie(getDureeIntemperie() - 1);
    }
}


//Méthode d'affichage de l'intempérie en fonction de son type
void Intemperie :: afficherIntemperie(Ressources &motherShip)
{
    //Affichage différent en fonction du type de l'intempérie

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


