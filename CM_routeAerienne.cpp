#include "CH_routeAerienne.h"

using namespace std;



    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur surcharg�
RouteAerienne::RouteAerienne(Aeroport aeroport1, Aeroport aeroport2, int longueur)
    : m_longueur(longueur)
{
    //Insertion du premier a�roport
    m_aeroportsRelies.push_back(new Aeroport);
    m_aeroportsRelies[0] = &aeroport1;

    //Insertion du deuxi�me a�roport
    m_aeroportsRelies.push_back(new Aeroport);
    m_aeroportsRelies[1] = &aeroport2;
}

//Constructeur par d�faut
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

//Getter d'un des deux a�roports
Aeroport* RouteAerienne::getAeroport(int indiceAeroport)
{
    return m_aeroportsRelies[indiceAeroport];
}

//Getter du vecteur d'avions pr�sents
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

//M�thode d'initialisation des a�roports
void RouteAerienne::initAeroports(Aeroport &aeroport1, Aeroport &aeroport2)
{
    //Insertion du premier a�roport
    m_aeroportsRelies.push_back(&aeroport1);

    //Insertion du deuxi�me a�roport
    m_aeroportsRelies.push_back(&aeroport2);
}

//M�thode d'ajout d'un avion sur la route
void RouteAerienne::ajoutAvion(Avion *avionAAjouter)
{
    cout << "OUI" << endl;
    m_avionsPresents.push_back(avionAAjouter);
}

//M�thode de suppression d'un avion de la route
void RouteAerienne::suppressionAvionRoute(int indiceAvion)
{
    m_avionsPresents[indiceAvion] = nullptr;
    m_avionsPresents.erase(m_avionsPresents.begin() + indiceAvion);
}


//M�thode d'affichage des informations
void RouteAerienne::afficherInfos()
{
    cout << "Relie " << m_aeroportsRelies[0]->get_nom() << " a " << m_aeroportsRelies[1]->get_nom() << " avec une distance de " << m_longueur << endl;
}


//M�thode d'affichage d'une surbrillance de la route si la souris passe dessus
void RouteAerienne::actualisationSurbrillanceRoute(Ressources &motherShip, bool &indicClic, bool &indicEchap)
{
    float a = 0; //Coefficient directeur de la route a�rienne
    float b = 0; //Constante de la route a�rienne
    int intervalle = 8; //Intervalle de pr�cision n�cessaire pour cliquer sur l'ar�te
    int coorXRectangleDroite = 1165;

    //Calcul des constantes de la fonction de la route
    a = ((float)getAeroport(1)->get_position().get_coord_y() - (float)getAeroport(0)->get_position().get_coord_y())/((float)getAeroport(1)->get_position().get_coord_x()-(float)getAeroport(0)->get_position().get_coord_x());
    b = (float)getAeroport(0)->get_position().get_coord_y() - (a * (float)getAeroport(0)->get_position().get_coord_x());

    //SI la coordonn�e en Y de la souris correspond � la fonction de l'ar�te
    if(mouse_y >= (a * mouse_x + b) - intervalle && mouse_y <= (a * mouse_x + b) + intervalle)
    {
        if((getAeroport(0)->get_position().get_coord_x() < getAeroport(1)->get_position().get_coord_x() && mouse_x >= getAeroport(0)->get_position().get_coord_x() && mouse_x <= getAeroport(1)->get_position().get_coord_x())
            || (getAeroport(0)->get_position().get_coord_x() > getAeroport(1)->get_position().get_coord_x() && mouse_x >= getAeroport(1)->get_position().get_coord_x() && mouse_x <= getAeroport(0)->get_position().get_coord_x()))
        {
            //SI la route passe d'un bord de l'�cran � l'autre
            if((getAeroport(0)->get_nom() == "HNL" && getAeroport(1)->get_nom() == "HND")
                || (getAeroport(0)->get_nom() == "HND" && getAeroport(1)->get_nom() == "HNL")
                || (getAeroport(0)->get_nom() == "HNL" && getAeroport(1)->get_nom() == "WLG")
                || (getAeroport(0)->get_nom() == "WLG" && getAeroport(1)->get_nom() == "HNL"))
            {
                //SI l'a�roport de "d�part" est � gauche de l'�cran et que son arriv�e est � droite
                if(getAeroport(0)->get_position().get_coord_x() < getAeroport(1)->get_position().get_coord_x())
                {
                    //Routes a�riennes de JOUR
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


            //SI clic sur l'ar�te, on va lancer l'affichage du menu de l'ar�te
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication qu'un clic a lieu
                menuRouteAerienne(motherShip, indicEchap);
            }

        }
    }
}


//M�thode d'affichage du menu de l'ar�te
void RouteAerienne::menuRouteAerienne(Ressources &motherShip, bool &indicEchap)
{
    bool finMenuRouteAerienne = false; //Indicateur de l'�tat du menu

    //Boucle de d�roulement du menu Route A�rienne
    while(!finMenuRouteAerienne)
    {
        //R�initialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //Affichage du fond du menu
        blit(motherShip.getBIT(21), motherShip.getBIT(0), 0, 0, 0, 0, motherShip.getBIT(21)->w, motherShip.getBIT(21)->h);

        //FAIRE LE TRAITEMENT ICI

        //Affichage du double buffer sur l'�cran
        blit(motherShip.getBIT(0), screen, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);

        //SI la touche ESCAPE est press�e
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true; //On indique que la touche est press�e
            finMenuRouteAerienne = true; //On indique qu'on peut sortir du menu
        }

        //SI la touche ESCAPE n'est plus press�e
        if(!key[KEY_ESC] && indicEchap == true)
        {
            indicEchap = false; //On l'indique
        }
    }
}





