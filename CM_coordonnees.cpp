#include "CH_coordonnees.h"
#include <iostream>

using namespace std;


    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur par défaut
Coord::Coord()
    : m_coord_x(0), m_coord_y(0)
{

}


//Constructeur surchargé
Coord::Coord(int _coord_x, int _coord_y)
    : m_coord_x(_coord_x), m_coord_y(_coord_y)
{

}


//Destructeur
Coord::~Coord()
{

}



    ////////////////
    // ACCESSEURS //
    ////////////////

//Getter de la coordonnée en X
int Coord::get_coord_x()
{
    return m_coord_x;
}

//Getter de la coordonnée en Y
int Coord::get_coord_y()
{
    return m_coord_y;
}

//Setter de la coordonnée en X
void Coord::set_coord_x(int _x)
{
    m_coord_x = _x;
}

//Setter de la coordonnée en Y
void Coord::set_coord_y(int _y)
{
    m_coord_y = _y;
}



    //////////////
    // METHODES //
    //////////////

//Méthode d'affichage des coordonnées
void Coord::afficher_coord()
{
    cout << endl << "Position x : " << m_coord_x << endl << "Position Y : " << m_coord_y << endl;
}
