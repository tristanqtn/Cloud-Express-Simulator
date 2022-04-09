#include "CH_avion&aeroport&route.h"

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

//Getter du A de la droite
float RouteAerienne::getARoute()
{
    return m_a_route;
}

//Getter du B de la droite
float RouteAerienne::getBRoute()
{
    return m_b_route;
}

//Setter du A de la droite
void RouteAerienne::setARoute(float nouveauA)
{
    m_a_route = nouveauA;
}

//Setter du B de la droite
void RouteAerienne::setBRoute(float nouveauB)
{
    m_b_route = nouveauB;
}

//Setter de la longueur
void RouteAerienne::setLongueur(int longueur)
{
    m_longueur = longueur;
}

//Getter de la longueur de la route
int RouteAerienne::getLongueur()
{
    return m_longueur;
}

void RouteAerienne::initTabEtats()
{
    vector<int> vectTempo; //Permet l'implémentation du double vecteur d'entiers de la route aérienne

    //Boucle d'initialisation du vecteur d'état de la route
    for(int k=0 ; k<getLongueur()/100 ; k++)
    {
        //Boucle de remplissage du vecteur temporaire
        for(int m=0 ; m<12 ; m++)
        {
            vectTempo.push_back(0);
        }

        //Insertion du vecteur temporaire
        m_etatRoute.push_back(vectTempo);

        //On vide le vecteur temporaire
        while(int(vectTempo.size()) > 0)
        {
            vectTempo.pop_back();
        }
    }
}


void RouteAerienne::actualiserCase(int indiceX, int indiceY, int valeur)
{
    m_etatRoute[indiceX][indiceY] = valeur;
}

vector<vector<int>> RouteAerienne::getTabEtats()
{
    return m_etatRoute;
}


//////////////
// METHODES //
//////////////


////////////////////////
// GESTION DES AVIONS //
////////////////////////

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
    m_avionsPresents.push_back(avionAAjouter);
}


//Méthode de suppression d'un avion de la route
void RouteAerienne::suppressionAvionRoute(int indiceAvion)
{
    m_avionsPresents[indiceAvion] = nullptr;
    m_avionsPresents.erase(m_avionsPresents.begin() + indiceAvion);
}



////////////////
// AFFICHAGES //
////////////////

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


    //Calcul des constantes de la fonction de la route
    a = ((float)getAeroport(1)->get_position().get_coord_y() - (float)getAeroport(0)->get_position().get_coord_y())/((float)getAeroport(1)->get_position().get_coord_x()-(float)getAeroport(0)->get_position().get_coord_x());
    b = (float)getAeroport(0)->get_position().get_coord_y() - (a * (float)getAeroport(0)->get_position().get_coord_x());

    //SI la coordonnée en Y de la souris correspond à la fonction de l'arête
    if(mouse_y >= (a * mouse_x + b) - intervalle && mouse_y <= (a * mouse_x + b) + intervalle)
    {
        if((getAeroport(0)->get_position().get_coord_x() < getAeroport(1)->get_position().get_coord_x() && mouse_x >= getAeroport(0)->get_position().get_coord_x() && mouse_x <= getAeroport(1)->get_position().get_coord_x())
                || (getAeroport(0)->get_position().get_coord_x() > getAeroport(1)->get_position().get_coord_x() && mouse_x >= getAeroport(1)->get_position().get_coord_x() && mouse_x <= getAeroport(0)->get_position().get_coord_x()))
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

            //SI clic sur l'arête, on va lancer l'affichage du menu de l'arête
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication qu'un clic a lieu
                menuRouteAerienne(motherShip, indicEchap);
            }

        }
    }
}



///////////
// MENUS //
///////////

//Méthode d'affichage du menu de l'arête
void RouteAerienne::menuRouteAerienne(Ressources &motherShip, bool &indicEchap)
{
    bool finMenuRouteAerienne = false; //Indicateur de l'état du menu

    int tailleEcran = 1000;
    int distanceColonne = 100;
    float tailleColonne;
    float nbColonnes;
    int distance = 0;
    int compteur = 0;
    int nbreLigne = 12;

    //Boucle de déroulement du menu Route Aérienne
    while(!finMenuRouteAerienne)
    {
        //Réinitialisation des variables à risque
        distance = 0;
        compteur = 0;

        //Réinitialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //Affichage du fond du menu
        rectfill(motherShip.getBIT(0),0,0, 1500,750, makecol(41,49,51));
        stretch_blit(motherShip.getBIT(21), motherShip.getBIT(0), 0,0, motherShip.getBIT(21)->w, motherShip.getBIT(21)->h,250,125,1000,500);

        //Affichage du nom de l'arête
        textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(3), SCREEN_W/2, 30, makecol(255,255,255),-1,"Arete %s - %s", convertisseurStringChar(getAeroport(0)->get_nom()), convertisseurStringChar(getAeroport(1)->get_nom()));

        //Calcul du nombre de colonnes à afficher et de la taille (d'affichage) de chacune d'elles
        nbColonnes = (float)m_longueur/(float)distanceColonne;
        tailleColonne = (float)tailleEcran/nbColonnes;

        //Boucle d'affichage des informations d'intempéries
        for(int i=0 ; i<int(m_etatRoute.size()) ; i++)
        {
            //On parcourt les intempéries
            for(int j=0 ; j<12 ; j++)
            {
                //Affichage adapté à l'état de la case
                switch(m_etatRoute[i][j])
                {
                case 0 : //Aucune anomalie

                    break;

                case 1 : //Pluie
                    rectfill(motherShip.getBIT(0), 250+i*tailleColonne, 125+j*(500.0/12.0), 250+i*tailleColonne+tailleColonne, 125+j*(500.0/12.0)+(500.0/12.0), makecol(255, 0, 0));
                    break;

                case 2 : //Vents violents
                    rectfill(motherShip.getBIT(0), 250+i*tailleColonne, 125+j*(500.0/12.0), 250+i*tailleColonne+tailleColonne, 125+j*(500.0/12.0)+(500.0/12.0), makecol(255, 255, 0));
                    break;

                default : //Par défaut
                    break;
                }
            }
        }

        //Affichage des colonnes
        for(float i = 250.0 ; i < 1250.0 ; i += tailleColonne)
        {
            rect(motherShip.getBIT(0), i, 125,i+tailleColonne, 625,makecol(41,49,51));
        }

        //Affichage des lignes
        for(float i = 125 ; i <= 525+500.0/12.0 ; i += 500.0/12.0)
        {
            rect(motherShip.getBIT(0), 250, i,1250, i+500.0/12.0,makecol(41,49,51));
            //textprintf_right_ex(motherShip.getBIT(0), motherShip.getFONT(8), 200, 720-(i+9), makecol(255,255,255), -1, "%d", int(5000+((i-125)/(500.0/12.0))*750));
        }


        //Boucle de parcours de l'ensemble des avions présents sur la route aérienne
        for(int i=0 ; i<int(m_avionsPresents.size()) ; i++)
        {
            //On cherche le sens de l'avion

            //SI l'avion est orienté dans le sens normal
            if(m_avionsPresents[i]->getNomAeroportD() == getAeroport(0)->get_nom())
            {
                //SI il s'agit d'un avion court courrier
                if(m_avionsPresents[i]->get_type_vol() == "court")
                {
                    masked_stretch_blit(motherShip.getBIT(32), motherShip.getBIT(0), 0, 0, motherShip.getBIT(32)->w, motherShip.getBIT(32)->h, 250+(m_avionsPresents[i]->getDistanceParcourue()/distanceColonne)*tailleColonne, 625-(m_avionsPresents[i]->get_altitude()-5000)/(9000/nbreLigne)*(500.0/12.0)+4, motherShip.getBIT(32)->w/5, motherShip.getBIT(32)->h/5);
                }
                //SINON SI il s'agit d'un avion moyen courrier
                else if(m_avionsPresents[i]->get_type_vol() == "moyen")
                {
                    masked_stretch_blit(motherShip.getBIT(34), motherShip.getBIT(0), 0, 0, motherShip.getBIT(34)->w, motherShip.getBIT(34)->h, 250+(m_avionsPresents[i]->getDistanceParcourue()/distanceColonne)*tailleColonne, 625-(m_avionsPresents[i]->get_altitude()-5000)/(9000/nbreLigne)*(500.0/12.0)+4, motherShip.getBIT(34)->w/5, motherShip.getBIT(34)->h/5);
                }
                //SINON SI il s'agit d'un avion long courrier
                else if(m_avionsPresents[i]->get_type_vol() == "long")
                {
                    masked_stretch_blit(motherShip.getBIT(36), motherShip.getBIT(0), 0, 0, motherShip.getBIT(36)->w, motherShip.getBIT(36)->h, 250+(m_avionsPresents[i]->getDistanceParcourue()/distanceColonne)*tailleColonne, 625-(m_avionsPresents[i]->get_altitude()-5000)/(9000/nbreLigne)*(500.0/12.0)+4, motherShip.getBIT(36)->w/5, motherShip.getBIT(36)->h/5);
                }
            }
            //SINON l'avion est orienté dans le sens inverse
            else
            {

                //SI il s'agit d'un avion court courrier
                if(m_avionsPresents[i]->get_type_vol() == "court")
                {
                    masked_stretch_blit(motherShip.getBIT(31), motherShip.getBIT(0), 0, 0, motherShip.getBIT(31)->w, motherShip.getBIT(31)->h, 1250-(m_avionsPresents[i]->getDistanceParcourue()/distanceColonne)*tailleColonne-motherShip.getBIT(31)->h/3.6, 625-(m_avionsPresents[i]->get_altitude()-5000)/(9000/nbreLigne)*(500.0/12.0)+4, motherShip.getBIT(31)->w/5, motherShip.getBIT(31)->h/5);
                }
                //SINON SI il s'agit d'un avion moyen courrier
                else if(m_avionsPresents[i]->get_type_vol() == "moyen")
                {
                    masked_stretch_blit(motherShip.getBIT(33), motherShip.getBIT(0), 0, 0, motherShip.getBIT(33)->w, motherShip.getBIT(33)->h, 1250-(m_avionsPresents[i]->getDistanceParcourue()/distanceColonne)*tailleColonne-motherShip.getBIT(33)->h/3.6, 625-(m_avionsPresents[i]->get_altitude()-5000)/(9000/nbreLigne)*(500.0/12.0)+4, motherShip.getBIT(33)->w/5, motherShip.getBIT(33)->h/5);
                }
                //SINON SI il s'agit d'un avion long courrier
                else if(m_avionsPresents[i]->get_type_vol() == "long")
                {
                    masked_stretch_blit(motherShip.getBIT(35), motherShip.getBIT(0), 0, 0, motherShip.getBIT(35)->w, motherShip.getBIT(35)->h, 1250-(m_avionsPresents[i]->getDistanceParcourue()/distanceColonne)*tailleColonne-motherShip.getBIT(35)->h/3.6, 625-(m_avionsPresents[i]->get_altitude()-5000)/(9000/nbreLigne)*(500.0/12.0)+4, motherShip.getBIT(35)->w/5, motherShip.getBIT(35)->h/5);
                }
            }
        }

        //Affichage de rectangles sur les côtés afin d'éviter les trucs moches possibles et imaginables
        rectfill(motherShip.getBIT(0), 0, 0, 250, SCREEN_H, makecol(41,49,51));
        rectfill(motherShip.getBIT(0), 1250, 0, SCREEN_W, SCREEN_H, makecol(41,49,51));


        //Affichage des lignes
        for(float i = 125 ; i <= 525+2*500.0/12.0 ; i += 500.0/12.0)
        {
            textprintf_right_ex(motherShip.getBIT(0), motherShip.getFONT(8), 200, 720-(i+9), makecol(255,255,255), -1, "%d", int(5750+((i-125)/(500.0/12.0))*750));
        }

        //Affichage des distances
        for(int i = 250 ; i <= 1250 ; i += tailleColonne)
        {
            if(compteur%((int)nbColonnes/5)== 0)
            {
                compteur = 0 ;
                textprintf_right_ex(motherShip.getBIT(0), motherShip.getFONT(8), i,640, makecol(255,255,255), -1, "%d",distance);
            }
            distance += distanceColonne;
            compteur ++;
        }



        //blit(motherShip.getBIT(31), motherShip.getBIT(0), 0, 0, 10, 100, motherShip.getBIT(31)->w, motherShip.getBIT(31)->h);

        textprintf_centre_ex(motherShip.getBIT(0), motherShip.getFONT(7), SCREEN_W/2, 680, makecol(255,255,255),-1,"%s                                                                   %s", convertisseurStringChar(getAeroport(0)->get_nom()), convertisseurStringChar(getAeroport(1)->get_nom()));

        //Affichage du curseur
        affichageCurseur(motherShip.getBIT(3), motherShip.getBIT(0));

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


void RouteAerienne::supprimer_avion(std::string immatriculation)
{
    for(size_t t=0; t<m_avionsPresents.size(); t++)
    {
        if(m_avionsPresents[t]->get_immatriculation() == immatriculation)
        {
            delete m_avionsPresents[t];
            m_avionsPresents.erase(m_avionsPresents.begin()+t);
        }
    }
}


