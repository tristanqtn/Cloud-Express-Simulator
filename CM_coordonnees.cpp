#include "CH_coordonnees.h"
#include <iostream>

using namespace std;


    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur par d�faut
Coord::Coord()
    : m_coord_x(0), m_coord_y(0)
{

}


//Constructeur surcharg�
Coord::Coord(float _coord_x, float _coord_y)
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

//Getter de la coordonn�e en X
float Coord::get_coord_x()
{
    return m_coord_x;
}

//Getter de la coordonn�e en Y
float Coord::get_coord_y()
{
    return m_coord_y;
}

//Setter de la coordonn�e en X
void Coord::set_coord_x(float _x)
{
    m_coord_x = _x;
}

//Setter de la coordonn�e en Y
void Coord::set_coord_y(float _y)
{
    m_coord_y = _y;
}



    //////////////
    // METHODES //
    //////////////

//M�thode d'affichage des coordonn�es
void Coord::afficher_coord()
{
    cout << endl << "Position x : " << m_coord_x << endl << "Position Y : " << m_coord_y << endl;
}
