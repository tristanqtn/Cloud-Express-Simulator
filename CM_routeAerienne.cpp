#include "CH_routeAerienne.h"

using namespace std;



    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur surchargé
RouteAerienne::RouteAerienne(Aeroport aeroport1, Aeroport aeroport2, int longueur)
    : m_longueur(longueur)
{
    //Insertion du premier aéroport
    m_aeroportsRelies.push_back(new Aeroport);
    m_aeroportsRelies[0] = &aeroport1;

    //Insertion du deuxième aéroport
    m_aeroportsRelies.push_back(new Aeroport);
    m_aeroportsRelies[1] = &aeroport2;
}

//Constructeur par défaut
RouteAerienne::RouteAerienne()
    : m_longueur(-1)
{

}

//Destructeur
RouteAerienne::~RouteAerienne()
{

}



    ////////////////
    // ACCESSEURS //
    ////////////////

//Getter d'un des deux aéroports
Aeroport* RouteAerienne::getAeroport(int indiceAeroport)
{
    return m_aeroportsRelies[indiceAeroport];
}

//Setter de la longueur
void RouteAerienne::setLongueur(int longueur)
{
    m_longueur = longueur;
}


    //////////////
    // METHODES //
    //////////////

//Méthode d'initialisation des aéroports
void RouteAerienne::initAeroports(Aeroport &aeroport1, Aeroport &aeroport2)
{
    //Insertion du premier aéroport
    m_aeroportsRelies.push_back(&aeroport1);

    //Insertion du deuxième aéroport
    m_aeroportsRelies.push_back(&aeroport2);
}

//Méthode d'ajout d'un avion sur la route
void RouteAerienne::ajoutAvion(Avion &avionAAjouter)
{

}


//Méthode d'affichage des informations
void RouteAerienne::afficherInfos()
{
    cout << "Relie " << m_aeroportsRelies[0]->get_nom() << " a " << m_aeroportsRelies[1]->get_nom() << " avec une distance de " << m_longueur << endl;
}







