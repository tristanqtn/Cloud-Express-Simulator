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

//Getter du vecteur d'avions présents
vector<Avion*> RouteAerienne::getAvionsPresents()
{
    return m_avionsPresents;
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
void RouteAerienne::ajoutAvion(Avion *avionAAjouter)
{
    cout << "OUI" << endl;
    m_avionsPresents.push_back(avionAAjouter);
}

//Méthode de suppression d'un avion de la route
void RouteAerienne::suppressionAvionRoute(int indiceAvion)
{
    m_avionsPresents[indiceAvion] = nullptr;
    m_avionsPresents.erase(m_avionsPresents.begin() + indiceAvion);
}


//Méthode d'affichage des informations
void RouteAerienne::afficherInfos()
{
    cout << "Relie " << m_aeroportsRelies[0]->get_nom() << " a " << m_aeroportsRelies[1]->get_nom() << " avec une distance de " << m_longueur << endl;
}


//Méthode d'affichage d'une surbrillance de la route si la souris passe dessus
void RouteAerienne::actualisationSurbrillanceRoute(Ressources &motherShip, bool &indicClic, bool &indicEchap)
{
    float a = 0; //Coefficient directeur de la route aérienne
    float b = 0; //Constante de la route aérienne
    int intervalle = 8; //Intervalle de précision nécessaire pour cliquer sur l'arête
    int coorXRectangleDroite = 1165;

    //Calcul des constantes de la fonction de la route
    a = ((float)getAeroport(1)->get_position().get_coord_y() - (float)getAeroport(0)->get_position().get_coord_y())/((float)getAeroport(1)->get_position().get_coord_x()-(float)getAeroport(0)->get_position().get_coord_x());
    b = (float)getAeroport(0)->get_position().get_coord_y() - (a * (float)getAeroport(0)->get_position().get_coord_x());

    //SI la coordonnée en Y de la souris correspond à la fonction de l'arête
    if(mouse_y >= (a * mouse_x + b) - intervalle && mouse_y <= (a * mouse_x + b) + intervalle)
    {
        if((getAeroport(0)->get_position().get_coord_x() < getAeroport(1)->get_position().get_coord_x() && mouse_x >= getAeroport(0)->get_position().get_coord_x() && mouse_x <= getAeroport(1)->get_position().get_coord_x())
            || (getAeroport(0)->get_position().get_coord_x() > getAeroport(1)->get_position().get_coord_x() && mouse_x >= getAeroport(1)->get_position().get_coord_x() && mouse_x <= getAeroport(0)->get_position().get_coord_x()))
        {
            //SI la route passe d'un bord de l'écran à l'autre
            if((getAeroport(0)->get_nom() == "HNL" && getAeroport(1)->get_nom() == "HND")
                || (getAeroport(0)->get_nom() == "HND" && getAeroport(1)->get_nom() == "HNL")
                || (getAeroport(0)->get_nom() == "HNL" && getAeroport(1)->get_nom() == "WLG")
                || (getAeroport(0)->get_nom() == "WLG" && getAeroport(1)->get_nom() == "HNL"))
            {
                //SI l'aéroport de "départ" est à gauche de l'écran et que son arrivée est à droite
                if(getAeroport(0)->get_position().get_coord_x() < getAeroport(1)->get_position().get_coord_x())
                {
                    //Routes aériennes de JOUR
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x()), getAeroport(1)->get_position().get_coord_y(), makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x()), getAeroport(1)->get_position().get_coord_y()-1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x())+1, getAeroport(1)->get_position().get_coord_y()-1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x())+1, getAeroport(1)->get_position().get_coord_y(), makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x())+1, getAeroport(1)->get_position().get_coord_y()+1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x()), getAeroport(1)->get_position().get_coord_y()+1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x())-1, getAeroport(1)->get_position().get_coord_y()+1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x())-1, getAeroport(1)->get_position().get_coord_y(), makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - getAeroport(1)->get_position().get_coord_x())-1, getAeroport(1)->get_position().get_coord_y()-1, makecol(255, 0, 0));

                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x(), getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y(), makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x(), getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y()-1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x()+1, getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y()-1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x()+1, getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y(), makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x()+1, getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y()+1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x(), getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y()+1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x()-1, getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y()+1, makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x()-1, getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y(), makecol(255, 0, 0));
                    line(motherShip.getBIT(0), getAeroport(1)->get_position().get_coord_x()-1, getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y()-1, makecol(255, 0, 0));
                }
            }
            else
            {
                //On la surligne
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y(), getAeroport(1)->get_position().get_coord_x(), getAeroport(1)->get_position().get_coord_y(), makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x(), getAeroport(0)->get_position().get_coord_y()-1, getAeroport(1)->get_position().get_coord_x(), getAeroport(1)->get_position().get_coord_y()-1, makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y()-1, getAeroport(1)->get_position().get_coord_x()+1, getAeroport(1)->get_position().get_coord_y()-1, makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y(), getAeroport(1)->get_position().get_coord_x()+1, getAeroport(1)->get_position().get_coord_y(), makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y()+1, getAeroport(1)->get_position().get_coord_x()+1, getAeroport(1)->get_position().get_coord_y()+1, makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()+1, getAeroport(0)->get_position().get_coord_y(), getAeroport(1)->get_position().get_coord_x()+1, getAeroport(1)->get_position().get_coord_y(), makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y()-1, getAeroport(1)->get_position().get_coord_x()-1, getAeroport(1)->get_position().get_coord_y()-1, makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y(), getAeroport(1)->get_position().get_coord_x()-1, getAeroport(1)->get_position().get_coord_y(), makecol(255,0,0));
                line(motherShip.getBIT(0), getAeroport(0)->get_position().get_coord_x()-1, getAeroport(0)->get_position().get_coord_y()-1, getAeroport(1)->get_position().get_coord_x()-1, getAeroport(1)->get_position().get_coord_y()-1, makecol(255,0,0));
            }


            //SI clic sur l'arête, on va lancer l'affichage du menu de l'arête
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication qu'un clic a lieu
                menuRouteAerienne(motherShip, indicEchap);
            }

        }
    }
}


//Méthode d'affichage du menu de l'arête
void RouteAerienne::menuRouteAerienne(Ressources &motherShip, bool &indicEchap)
{
    bool finMenuRouteAerienne = false; //Indicateur de l'état du menu

    //Boucle de déroulement du menu Route Aérienne
    while(!finMenuRouteAerienne)
    {
        //Réinitialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //Affichage du fond du menu
        blit(motherShip.getBIT(21), motherShip.getBIT(0), 0, 0, 0, 0, motherShip.getBIT(21)->w, motherShip.getBIT(21)->h);

        //FAIRE LE TRAITEMENT ICI

        //Affichage du double buffer sur l'écran
        blit(motherShip.getBIT(0), screen, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);

        //SI la touche ESCAPE est pressée
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true; //On indique que la touche est pressée
            finMenuRouteAerienne = true; //On indique qu'on peut sortir du menu
        }

        //SI la touche ESCAPE n'est plus pressée
        if(!key[KEY_ESC] && indicEchap == true)
        {
            indicEchap = false; //On l'indique
        }
    }
}





