#include "H_header.h"
#include "CH_aeroport.h"
#include "CH_coordonnees.h"
#include <algorithm>

using namespace std;


    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

// Constructeur surcharg�
Aeroport::Aeroport(string nom_fichier, int nombre_aeroport)
{

    //Ouverture du fichier
    ifstream fichier_aeroport{nom_fichier};
    if (!fichier_aeroport)
        throw runtime_error( "Impossible d'ouvrir en lecture " + nom_fichier ); //Erreur de lecture

    //Lecture du nom de l'a�roport
    fichier_aeroport >> m_nom;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture nom de l'aeroport"); //Erreur de lecture

    //Lecture du nom de la ville
    fichier_aeroport >> m_ville;
    replace(m_ville.begin(), m_ville.end(), '_', ' ');
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture nom de la ville"); //Erreur de lecture

    //Lecture de la position de l'a�roport
    int coord_x, coord_y;
    fichier_aeroport >> coord_x >> coord_y; //Nombre de sommets
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture position de l'aeroport"); //Erreur de lecture
    m_position.set_coord_x(coord_x);
    m_position.set_coord_y(coord_y);

    //Lecture du nombre de pistes d'atterrissage
    fichier_aeroport >> m_nombre_pistes;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture pistes d'atterissages"); //Erreur de lecture

    //Lecture du nombre de places au sol
    fichier_aeroport >> m_nombre_places_sol;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture nombres de places de stationnement"); //Erreur de lecture

    //Lecture du d�lai d'attente au sol
    fichier_aeroport >> m_delai_attente_sol;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture delai attente au sol"); //Erreur de lecture

    //Lecture du temps d'acces aux pistes
    fichier_aeroport >> m_temps_acces_pistes;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture delai acces pistes"); //Erreur de lecture

    //Lecture du temps de d�collage et d'atterissgae
    fichier_aeroport >> m_temps_decollage_atterissage;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture temps decollage/atterissage"); //Erreur de lecture

    //Lecture du d�lai anticollisisions
    fichier_aeroport >> m_delai_anticollisions;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture delai anticollisions"); //Erreur de lecture

    //Lecture de la boucle d'attente en vol
    fichier_aeroport >> m_duree_boucle_attente_vol;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture temps boucle en vol"); //Erreur de lecture

    //Lecture de l'ensemble des a�roports connect�s � celui-ci
    for(int i=0; i<(nombre_aeroport-1); i++)
    {
        string aeroport_connecte; //Contient le nom de l'a�roport � connecter
        int distance; //Contient la distance � cet a�roport

        fichier_aeroport >> aeroport_connecte>> distance; //Lecture de l'a�roport � atteindre et de la distance les s�parant
        if (fichier_aeroport.fail() )
            throw runtime_error("Probleme lecture sommet / successeur du graphe");

        //Appel du processus d'ajout de l'a�roport
        ajouter_aeroport_connecte(aeroport_connecte, distance);
    }

    //Fermeture du fichier
    fichier_aeroport.close();
}


//Constructeur par d�faur
Aeroport::Aeroport()
    : m_nom(""), m_nombre_pistes(-1), m_nombre_places_sol(-1), m_delai_attente_sol(-1), m_temps_acces_pistes(-1), m_delai_anticollisions(-1), m_temps_decollage_atterissage(-1), m_duree_boucle_attente_vol(-1)
{

}


// Destructeur
Aeroport::~Aeroport()
{

}



    ////////////////
    // ACCESSEURS //
    ////////////////


//Getter du nombre de pistes
int Aeroport::get_nombre_pistes()
{
    return m_nombre_pistes;
}

//Getter du nombre de places au sol
int Aeroport::get_nombre_places_sol()
{
    return m_nombre_places_sol;
}

//Getter du d�lai d'attente au sol
int Aeroport::get_delai_attente_sol()
{
    return m_delai_attente_sol;
}

//Getter du temps d'acc�s aux pistes
int Aeroport::get_temps_acces_pistes()
{
    return m_temps_acces_pistes;
}

//Getter du d�lai anticollisions
int Aeroport::get_delai_anticollisions()
{
    return m_delai_anticollisions;
}

//Getter du temps de d�collage et d'atterissage
int Aeroport::get_temps_decollage_atterissage()
{
    return m_temps_decollage_atterissage;
}

//Getter de la dur�e de la boucle attente vol
int Aeroport::get_duree_boucle_attente_vol()
{
    return m_duree_boucle_attente_vol;
}

//Getter du nom de l'a�roport
string Aeroport::get_nom()
{
    return m_nom;
}

//Getter de la distance � un autre a�roport pr�cis
int Aeroport::get_distance_aeroports_preci(string nom_aeroport)
{
    int temporary = -1;

    //Boucle de parcours du vecteur d'a�roports voisins
    for(size_t t=0; t<m_distance_aeroports.size(); t++)
    {
        //L'a�roport correspond � celui recherch�
        if(nom_aeroport == m_distance_aeroports[t].first)
        {
            //On r�cup�re la distance entre les deux a�roports
            temporary = m_distance_aeroports[t].second;
        }
    }

    return temporary;
}

//Getter de la distance � tous les a�roports
vector < pair<string, int> > Aeroport::get_distance_aeroports()
{
    return m_distance_aeroports;
}

//Getter des coordonn�es de l'a�roport
Coord Aeroport::get_position()
{
    return m_position;
}



    //////////////
    // METHODES //
    //////////////

// M�thode d'ajout d'un a�roport dans la base de connexion d'un a�roport source
void Aeroport::ajouter_aeroport_connecte(string nom_aeroport, int distance)
{
    pair <string , int> tempo; //Contient temporairement la paire de donn�es

    //R�cup�ration des donn�es et affectation dans la paire
    tempo.first = nom_aeroport;
    tempo.second = distance;

    //Affectation de la paire dans le conteneur d'a�roports reli�s
    m_distance_aeroports.push_back(tempo);
}


// M�thode d'affichage des caract�ristiques d'un a�roport
void Aeroport::afficher_caracteristique(int nombre_aeroport)
{
    cout << " ========== " << m_nom << endl;

    m_position.afficher_coord();
    cout << endl << "Nombre de pistes : " << m_nombre_pistes << endl;
    cout << "Nombre de places au sol : " << m_nombre_places_sol << endl;
    cout << endl << "Attente au sol : " << m_delai_attente_sol << " UT" << endl;
    cout << "Acces aux pistes : " << m_temps_acces_pistes << " UT" << endl;
    cout << "Duree atterissage / decollage au sol : " << m_temps_decollage_atterissage << " UT" << endl;
    cout << "Duree anticollision : " << m_delai_anticollisions << " UT" << endl;
    cout << "Attente au sol : " << m_duree_boucle_attente_vol << " UT" << endl;

    cout << endl;

    for(int i=0; i<(nombre_aeroport-1); i++)
    {
        if(m_distance_aeroports[i].second != -1)
            cout << m_distance_aeroports[i].first << " distance : " <<  m_distance_aeroports[i].second << endl;
        else
            cout <<  m_distance_aeroports[i].first << " non desservi par " << m_nom << endl;
    }

    cout << endl << " ========== " << endl;
}


//M�thode d'actualisation du pointeur de localisation de l'a�roport
void Aeroport::actualisationPointeurLocalisation(Ressources &motherShip, bool &indicClic, bool &indicEchap)
{
    //SI la souris passe sur le pointeur
    if(mouse_x >= get_position().get_coord_x() - motherShip.getBIT(9)->w/4 && mouse_x <= get_position().get_coord_x() + motherShip.getBIT(9)->w/4 && mouse_y >= get_position().get_coord_y() - motherShip.getBIT(9)->h/2 && mouse_y <= get_position().get_coord_y())
    {
        //Affichage du pointeur actif
        masked_stretch_blit(motherShip.getBIT(9), motherShip.getBIT(0), 0, 0, motherShip.getBIT(9)->w, motherShip.getBIT(9)->h, get_position().get_coord_x() - motherShip.getBIT(9)->w/4, get_position().get_coord_y() - motherShip.getBIT(9)->h/2, motherShip.getBIT(9)->w/2, motherShip.getBIT(9)->h/2);

        //SI clic sur l'a�roport, on va lancer le menu de l'a�roport
        if(mouse_b & 1 && indicClic == false)
        {
            //Lancement du menu A�roport
            menuAeroport(motherShip, indicClic, indicEchap);
        }
    }
}


//M�thode du menu de l'a�roport
void Aeroport::menuAeroport(Ressources &motherShip, bool &indicClic, bool &indicEchap)
{
    int ecartBords = 100;
    bool finMenuAeroport = false; //Indicateur de fin du menu A�roport
    BITMAP* doubleBufferProvisoire = create_bitmap(SCREEN_W, SCREEN_H); //Permet d'avoir le fond du menu

    //Impression du double buffer pr�c�dent dans le double buffer provisoire
    blit(motherShip.getBIT(0), doubleBufferProvisoire, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);

    while(!finMenuAeroport)
    {
        //R�initialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //Impression du fond derri�re le menu
        blit(doubleBufferProvisoire, motherShip.getBIT(0), 0, 0, 0, 0, doubleBufferProvisoire->w, doubleBufferProvisoire->h);

        //Affichage des rectangles entourant le menu
        rectfill(motherShip.getBIT(0), ecartBords, ecartBords, SCREEN_W-ecartBords, SCREEN_H-ecartBords, makecol(127, 127, 127));
        rectfill(motherShip.getBIT(0), ecartBords+30, ecartBords+30, SCREEN_W-ecartBords-30, SCREEN_H-ecartBords-30, makecol(204, 204, 204));

        //Affichage du nom de l'a�roport
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(3), ecartBords+60, ecartBords+60, makecol(255, 255, 255), -1, convertisseurStringChar(m_nom));
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(7), SCREEN_W/2-420, ecartBords+85, makecol(255, 255, 255), -1, convertisseurStringChar(m_ville));

        //Affichage des informations � gauche
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+180, makecol(255, 255, 255), -1, "Nombre de pistes : %d", m_nombre_pistes);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+230, makecol(255, 255, 255), -1, "Nombre de places au sol : %d / %d", m_nombre_places_sol, m_nombre_places_sol);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+280, makecol(255, 255, 255), -1, "Delai obligatoire d'attente");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+315, makecol(255, 255, 255), -1, "au sol : %d UT", m_delai_attente_sol);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+365, makecol(255, 255, 255), -1, "Temps d'acces aux pistes : %d UT", m_temps_acces_pistes);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+415, makecol(255, 255, 255), -1, "Duree d'atterrissage / decollage : %d UT", m_temps_decollage_atterissage);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+465, makecol(255, 255, 255), -1, "Attente a l'atterrissage : %d UT", m_duree_boucle_attente_vol);
        //Affichage du curseur
        affichageCurseur(motherShip.getBIT(3), motherShip.getBIT(0));

        //Affichage du double buffer
        blit(motherShip.getBIT(0), screen, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);

        //SI la touche ESCAPE est press�e, on l'indique et on sort du menu
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true;
            finMenuAeroport = true;
        }

        //SI le clic gauche n'est plus press�, on l'indique
        if(!(mouse_b & 1) && indicClic == true)
        {
            indicClic = false;
        }

        //SI le bouton ESCAPE n'est plus press�, on l'indique
        if(!key[KEY_ESC] && indicEchap == true)
        {
            indicEchap = false;
        }
    }
}
