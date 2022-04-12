#include "H_header.h"
#include "CH_avion&aeroport&route.h"
#include "CH_coordonnees.h"
#include <algorithm>

using namespace std;



//////////////////////////////////
// CONSTRUCTEURS ET DESTRUCTEUR //
//////////////////////////////////

// Constructeur surchargé
Aeroport::Aeroport(string nom_fichier, int nombre_aeroport)
{

    //Ouverture du fichier
    ifstream fichier_aeroport{nom_fichier};
    if (!fichier_aeroport)
        throw runtime_error( "Impossible d'ouvrir en lecture " + nom_fichier ); //Erreur de lecture

    //Lecture du nom de l'aéroport
    fichier_aeroport >> m_nom;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture nom de l'aeroport"); //Erreur de lecture

    //Lecture du nom de la ville
    fichier_aeroport >> m_ville;
    replace(m_ville.begin(), m_ville.end(), '_', ' ');
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture nom de la ville"); //Erreur de lecture

    //Lecture de la position de l'aéroport
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

    //Initialisation des pistes
    vector<Avion*> vecteurProvisoire;
    for(int i=0 ; i<m_nombre_pistes ; i++)
    {
        //Création du double vecteur de pointeurs sur Avion
        vecteurProvisoire.push_back(new Avion);
        vecteurProvisoire.back() = nullptr;
        m_pistes.push_back(vecteurProvisoire);

        vecteurProvisoire.pop_back();
    }

    //Lecture du nombre de places au sol
    fichier_aeroport >> m_nombre_places_sol;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture nombres de places de stationnement"); //Erreur de lecture

    //Lecture du délai d'attente au sol
    fichier_aeroport >> m_delai_attente_sol;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture delai attente au sol"); //Erreur de lecture

    //Lecture du temps d'acces aux pistes
    fichier_aeroport >> m_temps_acces_pistes;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture delai acces pistes"); //Erreur de lecture

    //Lecture du temps de décollage et d'atterissgae
    fichier_aeroport >> m_temps_decollage_atterissage;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture temps decollage/atterissage"); //Erreur de lecture

    //Lecture du délai anticollisisions
    fichier_aeroport >> m_delai_anticollisions;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture delai anticollisions"); //Erreur de lecture

    //Lecture de la boucle d'attente en vol
    fichier_aeroport >> m_duree_boucle_attente_vol;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture temps boucle en vol"); //Erreur de lecture

    //Lecture de l'ensemble des aéroports connectés à celui-ci
    for(int i=0; i<(nombre_aeroport-1); i++)
    {
        string aeroport_connecte; //Contient le nom de l'aéroport à connecter
        int distance; //Contient la distance à cet aéroport

        fichier_aeroport >> aeroport_connecte>> distance; //Lecture de l'aéroport à atteindre et de la distance les séparant
        if (fichier_aeroport.fail() )
            throw runtime_error("Probleme lecture sommet / successeur du graphe");

        //Appel du processus d'ajout de l'aéroport
        ajouter_aeroport_connecte(aeroport_connecte, distance);
    }

    //Fermeture du fichier
    fichier_aeroport.close();
}


//Constructeur par défaur
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

//Getter du délai d'attente au sol
int Aeroport::get_delai_attente_sol()
{
    return m_delai_attente_sol;
}

//Getter du temps d'accès aux pistes
int Aeroport::get_temps_acces_pistes()
{
    return m_temps_acces_pistes;
}

//Getter du délai anticollisions
int Aeroport::get_delai_anticollisions()
{
    return m_delai_anticollisions;
}

//Getter du temps de décollage et d'atterissage
int Aeroport::get_temps_decollage_atterissage()
{
    return m_temps_decollage_atterissage;
}

//Getter de la durée de la boucle attente vol
int Aeroport::get_duree_boucle_attente_vol()
{
    return m_duree_boucle_attente_vol;
}

//Getter du nom de l'aéroport
string Aeroport::get_nom()
{
    return m_nom;
}

//Getter de la distance à un autre aéroport précis
int Aeroport::get_distance_aeroports_preci(string nom_aeroport)
{
    int temporary = -1;

    //Boucle de parcours du vecteur d'aéroports voisins
    for(size_t t=0; t<m_distance_aeroports.size(); t++)
    {
        //L'aéroport correspond à celui recherché
        if(nom_aeroport == m_distance_aeroports[t].first)
        {
            //On récupère la distance entre les deux aéroports
            temporary = m_distance_aeroports[t].second;
        }
    }

    return temporary;
}

//Getter de la distance à tous les aéroports
vector < pair<string, int> > Aeroport::get_distance_aeroports()
{
    return m_distance_aeroports;
}

//Getter des coordonnées de l'aéroport
Coord Aeroport::get_position()
{
    return m_position;
}

//Getter du nombre d'avions dans le parking de l'aéroport
int Aeroport::getNbreAvionsParking()
{
    return int(m_avions_parking.size());
}

//Getter des pistes
vector<vector<Avion*>> Aeroport::getPistes()
{
    return m_pistes;
}

//Getter d'un successeur précis de l'aéroport
pair<string, int> Aeroport::get_successeur_precis(int i)
{
    return m_distance_aeroports[i];
}

int Aeroport::getAltitudesAvions(int i)
{
    return m_altitudesAvions[i];
}



void Aeroport::setAltitudeAvions(int valeur)
{
    m_altitudesAvions.push_back(valeur);
}


    //////////////
    // METHODES //
    //////////////


        ////////////////
        // AFFICHAGES //
        ////////////////

// Méthode d'affichage des caractéristiques d'un aéroport
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


//Méthode d'actualisation du pointeur de localisation de l'aéroport
void Aeroport::actualisationPointeurLocalisation(Ressources &motherShip, vector<Aeroport> m_aeroports, bool &indicClic, bool &indicEchap)
{
    //SI la souris passe sur le pointeur
    if(mouse_x >= get_position().get_coord_x() - motherShip.getBIT(9)->w/4 && mouse_x <= get_position().get_coord_x() + motherShip.getBIT(9)->w/4 && mouse_y >= get_position().get_coord_y() - motherShip.getBIT(9)->h/2 && mouse_y <= get_position().get_coord_y())
    {
        //Affichage du pointeur actif
        masked_stretch_blit(motherShip.getBIT(9), motherShip.getBIT(0), 0, 0, motherShip.getBIT(9)->w, motherShip.getBIT(9)->h, get_position().get_coord_x() - motherShip.getBIT(9)->w/4, get_position().get_coord_y() - motherShip.getBIT(9)->h/2, motherShip.getBIT(9)->w/2, motherShip.getBIT(9)->h/2);

        //SI clic sur l'aéroport, on va lancer le menu de l'aéroport
        if(mouse_b & 1 && indicClic == false)
        {
            //Lancement du menu Aéroport
            menuAeroport(motherShip, m_aeroports, indicClic, indicEchap);
        }
    }
}


//Méthode du menu de l'aéroport
void Aeroport::menuAeroport(Ressources &motherShip, vector<Aeroport> m_aeroports, bool &indicClic, bool &indicEchap)
{
    int ecartBords = 100;
    int ecartInfosAvions = 75;
    int decalageAvions = 40;
    bool finMenuAeroport = false; //Indicateur de fin du menu Aéroport
    BITMAP* doubleBufferProvisoire = create_bitmap(SCREEN_W, SCREEN_H); //Permet d'avoir le fond du menu

    //Impression du double buffer précédent dans le double buffer provisoire
    blit(motherShip.getBIT(0), doubleBufferProvisoire, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);

    while(!finMenuAeroport)
    {
        //Réinitialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //Impression du fond derrière le menu
        blit(doubleBufferProvisoire, motherShip.getBIT(0), 0, 0, 0, 0, doubleBufferProvisoire->w, doubleBufferProvisoire->h);

        //Affichage des rectangles entourant le menu
        rectfill(motherShip.getBIT(0), ecartBords, ecartBords, SCREEN_W-ecartBords, SCREEN_H-ecartBords, makecol(127, 127, 127));
        rectfill(motherShip.getBIT(0), ecartBords+30, ecartBords+30, SCREEN_W-ecartBords-30, SCREEN_H-ecartBords-30, makecol(204, 204, 204));

        //Affichage du nom de l'aéroport
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(3), ecartBords+60, ecartBords+60, makecol(255, 255, 255), -1, convertisseurStringChar(m_nom));
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(7), SCREEN_W/2-420, ecartBords+85, makecol(255, 255, 255), -1, convertisseurStringChar(m_ville));

        //Affichage des informations à gauche
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+180, makecol(255, 255, 255), -1, "Nombre de pistes : %d", m_nombre_pistes);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+230, makecol(255, 255, 255), -1, "Nombre de places au sol : %d / %d", m_nombre_places_sol-int(m_avions_parking.size()), m_nombre_places_sol);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+280, makecol(255, 255, 255), -1, "Delai obligatoire d'attente");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+315, makecol(255, 255, 255), -1, "au sol : %d UT", m_delai_attente_sol);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+365, makecol(255, 255, 255), -1, "Temps d'acces aux pistes : %d UT", m_temps_acces_pistes);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+415, makecol(255, 255, 255), -1, "Duree d'atterrissage / decollage : %d UT", m_temps_decollage_atterissage);
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+60, ecartBords+465, makecol(255, 255, 255), -1, "Attente a l'atterrissage : %d UT", m_duree_boucle_attente_vol);

        //Affichage des informations du parking
        if(int(m_avions_parking.size()) > 0) //SI le parking n'est pas vide
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+700, ecartBords+80, makecol(255, 255, 255), -1, "Parking : ");
        }
        else //SINON, le parking est vide
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+700, ecartBords+80, makecol(255, 255, 255), -1, "Parking : Vide");
        }

        //Parcours de l'ensemble des pistes afin d'afficher leurs informations
        for(int i=0 ; i<int(m_pistes.size()) ; i++)
        {
            //SI la piste contient des avions
            if(int(m_pistes[i].size()) >= 1 && m_pistes[i][0] != nullptr)
            {
                textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+700, ecartBords+80 + (i+1)*ecartInfosAvions, makecol(255, 255, 255), -1, "Piste %d : ", i+1);
            }
            //SINON SI la piste ne contient pas d'avions
            else if(int(m_pistes[i].size()) == 1 && m_pistes[i][0] == nullptr)
            {
                textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), ecartBords+700, ecartBords+80 + (i+1)*ecartInfosAvions, makecol(255, 255, 255), -1, "Piste %d : Vide", i+1);
            }
        }

        //Boucle de parcours des avions dans le parking afin de les afficher
        for(int i=0 ; i<int(m_avions_parking.size()) ; i++)
        {
            //SI l'avion est un court courrier
            if(m_avions_parking[i]->get_type_vol() == "court")
            {
                rotate_scaled_sprite(motherShip.getBIT(0), motherShip.getBIT(18), ecartBords+850 + i*decalageAvions, ecartBords+75,ftofix(0),ftofix(0.5));
            }
            //SINON SI il s'agit d'un moyen courrier
            else if(m_avions_parking[i]->get_type_vol() == "moyen")
            {
                rotate_scaled_sprite(motherShip.getBIT(0), motherShip.getBIT(19), ecartBords+850 + i*decalageAvions, ecartBords+75,ftofix(0),ftofix(0.5));
            }
            //SINON SI il s'agit d'un long courrier
            else if(m_avions_parking[i]->get_type_vol() == "long")
            {
                rotate_scaled_sprite(motherShip.getBIT(0), motherShip.getBIT(20), ecartBords+850 + i*decalageAvions, ecartBords+75,ftofix(0),ftofix(0.5));
            }

            //Affichage des informations de l'avion
            m_avions_parking[i]->actualisationSurbrillanceAvion(m_aeroports, false, ecartBords+850 + i*decalageAvions + 21, ecartBords+75+21,motherShip.getBIT(0), motherShip.getBIT(26), motherShip.getBIT(30), motherShip.getBIT(27), motherShip.getBIT(28), motherShip.getBIT(29), motherShip.getFONT(7), motherShip.getFONT(2), motherShip.getFONT(10));
        }

        //Boucle de parcours de l'ensemble des pistes afin d'afficher les informations des avions
        for(int i=0 ; i<int(m_pistes.size()) ; i++)
        {
            //SI la piste contient des avions
            if(int(m_pistes[i].size()) >= 1 && m_pistes[i][0] != nullptr)
            {
                for(int j=0 ; j<int(m_pistes[i].size()) ; j++)
                {
                    //SI l'avion est un court courrier
                    if(m_pistes[i][j]->get_type_vol() == "court")
                    {
                        rotate_scaled_sprite(motherShip.getBIT(0), motherShip.getBIT(18), ecartBords+850 + j*decalageAvions, ecartBords+75+(i+1)*ecartInfosAvions,ftofix(0),ftofix(0.5));
                    }
                    //SINON SI il s'agit d'un moyen courrier
                    else if(m_pistes[i][j]->get_type_vol() == "moyen")
                    {
                        rotate_scaled_sprite(motherShip.getBIT(0), motherShip.getBIT(19), ecartBords+850 + j*decalageAvions, ecartBords+75+(i+1)*ecartInfosAvions,ftofix(0),ftofix(0.5));
                    }
                    //SINON SI il s'agit d'un long courrier
                    else if(m_pistes[i][j]->get_type_vol() == "long")
                    {
                        rotate_scaled_sprite(motherShip.getBIT(0), motherShip.getBIT(20), ecartBords+850 + j*decalageAvions, ecartBords+75+(i+1)*ecartInfosAvions,ftofix(0),ftofix(0.5));
                    }

                    //Affichage des informations de l'avion
                    m_pistes[i][j]->actualisationSurbrillanceAvion(m_aeroports, false, ecartBords+850 + j*decalageAvions + 21, ecartBords+75+(i+1)*ecartInfosAvions+21,motherShip.getBIT(0), motherShip.getBIT(26), motherShip.getBIT(30), motherShip.getBIT(27), motherShip.getBIT(28), motherShip.getBIT(29), motherShip.getFONT(7), motherShip.getFONT(2), motherShip.getFONT(10));
                }
            }
        }

        //Boucle de parcours de l'ensemble des pistes afin d'afficher les informations des avions
        for(int i=0 ; i<int(m_pistes.size()) ; i++)
        {
            //SI la piste contient des avions
            if(int(m_pistes[i].size()) >= 1 && m_pistes[i][0] != nullptr)
            {
                for(int j=0 ; j<int(m_pistes[i].size()) ; j++)
                {
                    //Affichage des informations de l'avion
                    m_pistes[i][j]->actualisationSurbrillanceAvion(m_aeroports, false, ecartBords+850 + j*decalageAvions + 21, ecartBords+75+(i+1)*ecartInfosAvions+21,motherShip.getBIT(0), motherShip.getBIT(26), motherShip.getBIT(30), motherShip.getBIT(27), motherShip.getBIT(28), motherShip.getBIT(29), motherShip.getFONT(7), motherShip.getFONT(2), motherShip.getFONT(10));
                }
            }
        }

        //Affichage du curseur
        affichageCurseur(motherShip.getBIT(3), motherShip.getBIT(0));

        //Affichage du double buffer
        blit(motherShip.getBIT(0), screen, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);

        //SI la touche ESCAPE est pressée, on l'indique et on sort du menu
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true;
            finMenuAeroport = true;
        }

        //SI le clic gauche n'est plus pressé, on l'indique
        if(!(mouse_b & 1) && indicClic == true)
        {
            indicClic = false;
        }

        //SI le bouton ESCAPE n'est plus pressé, on l'indique
        if(!key[KEY_ESC] && indicEchap == true)
        {
            indicEchap = false;
        }
    }
}



///////////////////////////
// GESTION DE L'AEROPORT //
///////////////////////////

// Méthode d'ajout d'un aéroport dans la base de connexion d'un aéroport source
void Aeroport::ajouter_aeroport_connecte(string nom_aeroport, int distance)
{
    pair <string, int> tempo;  //Contient temporairement la paire de données

    //Récupération des données et affectation dans la paire
    tempo.first = nom_aeroport;
    tempo.second = distance;

    //Affectation de la paire dans le conteneur d'aéroports reliés
    m_distance_aeroports.push_back(tempo);
}



////////////////////////
// GESTION DES AVIONS //
////////////////////////

//Méthode d'actualisation de l'ensemble des files d'avions de l'aéroport
void Aeroport::actualisationAeroport(vector<Aeroport> m_aeroports, vector<RouteAerienne*> &m_ensembleRoutes, int indiceAeroport, int** m_matrice_adjacence, Horloge heureActuelle)
{
    //Actualisation des pistes
    actualisationPistes(m_ensembleRoutes);

    //Actualisation des accès aux pistes
    actualisationAccesPistes();

    //Actualisation des sorties de pistes
    actualisationSortiePistes();

    //Actualisation des entrées et sorties dans la boucle d'attente
    actualisationBoucleAttente(m_ensembleRoutes);

    //Actualisation du parking
    actualisationParking(m_aeroports, indiceAeroport, m_matrice_adjacence, heureActuelle);
}


//Méthode d'actualisation des entrées et sorties dans la boucle d'attente de l'aéroport
void Aeroport::actualisationBoucleAttente(vector<RouteAerienne*> &m_ensembleRoute)
{
    int indicePisteDisponible = -1; //Contient l'indice d'une possible piste pouvant accueillir un avion sortant de la boucle d'attente
    int indiceAvionATransferer = -1; //Contient l'indice de l'avion à faire passer en phase d'atterrissage
    bool indicNouvelleRef = true; //Indique qu'un nouvel avion a été prix comme référence
    bool indicEntreePossible = false; //Contient l'indicateur permettant de savoir si un avion peut rentrer dans la boucle d'attente


    //ACTUALISATION DES SORTIES DE LA BOUCLE D'ATTENTE

    //SI la boucle d'attente n'est pas vide
    if(int(m_queue_boucle_attente.size()) != 0)
    {
        //On parcourt l'ensemble des avions de la boucle d'attente
        for(int i=0 ; i<int(m_queue_boucle_attente.size()) ; i++)
        {
            //SI l'avion a fini son délai d'attente
            if(m_queue_boucle_attente[i]->get_action_en_cours() == 4 && m_queue_boucle_attente[i]->get_duree_boucle_attente() == 0)
            {

                //SI il s'agit du premier avion pouvant potentiellement atterrir
                if(indicNouvelleRef == true)
                {
                    //On sauvegarde son indice
                    indiceAvionATransferer = i;
                }
                //SINON, il ne s'agit pas du premier avion pouvant potentiellement atterrir
                else
                {
                    //SI le potentiel nouvel référentiel a un réservoir en seuil critique (pire que la précédente référence)
                    if(m_queue_boucle_attente[i]->get_quantite_kerosene() <= (get_temps_decollage_atterissage() + get_temps_acces_pistes() + 1) && m_queue_boucle_attente[i]->get_quantite_kerosene() < m_queue_boucle_attente[indiceAvionATransferer]->get_quantite_kerosene())
                    {
                        //On sauvegarde son indice
                        indiceAvionATransferer = i;
                    }
                }
            }
        }

        //Parcours des pistes à la recherche d'une piste vide
        for(int j=0 ; j<m_nombre_pistes ; j++)
        {
            //SI la piste est vide
            if(int(m_pistes[j].size()) == 1 && m_pistes[j][0] == nullptr)
            {
                //On récupère son indice
                indicePisteDisponible = j;
            }
        }

        //Une piste disponible a été trouvée et un avion peut être transféré
        if(indicePisteDisponible != -1 && indiceAvionATransferer != -1)
        {
            //Parcours des pistes à la recherche d'une piste disponible (des avions y sont mais ont le même objectif : décoller)
            for(int j=0 ; j<m_nombre_pistes ; j++)
            {
                //Parcours des avions présents sur la piste
                for(int k=0 ; k<int(m_pistes[j].size()) ; k++)
                {
                    //SI un avion existe
                    if(m_pistes[j][k] != nullptr)
                    {
                        //SI cet avion décolle et a un décalage de 1 UT par rapport à celui que l'on veut injecter
                        if(m_pistes[j][k]->get_action_en_cours() == 5 && m_pistes[j][k]->get_duree_atterrissage() <= m_queue_boucle_attente[indiceAvionATransferer]->get_duree_decollage()-2)
                        {
                            //On récupère l'indice de la piste
                            indicePisteDisponible = j;
                        }
                    }
                }
            }

            //L'avion rentre dans la phase d'accès aux pistes
            m_queue_boucle_attente[indiceAvionATransferer]->set_action_en_cours(m_queue_boucle_attente[indiceAvionATransferer]->get_action_en_cours() + 1);

            //On l'ajoute à la piste en question
            if(int(m_pistes[indicePisteDisponible].size()) == 1 && m_pistes[indicePisteDisponible][0] == nullptr)
            {
                m_pistes[indicePisteDisponible][0] = m_queue_boucle_attente[indiceAvionATransferer];
            }
            else if(int(m_pistes[indicePisteDisponible].size()) > 1 || m_pistes[indicePisteDisponible][0] != nullptr)
            {
                m_pistes[indicePisteDisponible].push_back(new Avion);
                m_pistes[indicePisteDisponible].back() = m_queue_boucle_attente[indiceAvionATransferer];
            }

            //On supprime l'avion de la file d'accès aux pistes
            m_queue_boucle_attente[indiceAvionATransferer] = nullptr;
            m_queue_boucle_attente.erase(m_queue_boucle_attente.begin() + indiceAvionATransferer);
        }
    }



    //ACTUALISATION DES ENTREES EN BOUCLE D'ATTENTE

    //Parcours des routes aériennes pour trouver celles arrivant à cet aéroport
    for(int k=0 ; k<int(m_ensembleRoute.size()) ; k++)
    {
        //SI on la trouve (la route
        if(m_ensembleRoute[k]->getAeroport(0)->get_nom() == get_nom() || m_ensembleRoute[k]->getAeroport(1)->get_nom() ==get_nom())
        {
            //Parcours de l'ensemble des avions contenus dans la route
            for(int i=0 ; i<int(m_ensembleRoute[k]->getAvionsPresents().size()) ; i++)
            {
                //SI l'avion a pour destination l'aéroport en question et qu'il a fini son vol
                if(m_ensembleRoute[k]->getAvionsPresents()[i]->getNomAeroportA() == get_nom() && m_ensembleRoute[k]->getAvionsPresents()[i]->get_duree_vol() == 0)
                {
                    //SI la boucle d'attente n'est pas vide
                    if(int(m_queue_boucle_attente.size()) != 0)
                    {
                        //SI le dernier avion dans la boucle a bien un UT de décalage par rapport au potentiel nouvel avion
                        if(m_queue_boucle_attente.back()->get_duree_boucle_attente() <= m_ensembleRoute[k]->getAvionsPresents()[i]->get_duree_boucle_attente()-2)
                        {
                            //On indique que l'avion peut rentrer
                            indicEntreePossible = true;
                        }
                    }
                    //SINON, la file est vide
                    else
                    {
                        //On indique que l'avion peut donc rentrer
                        indicEntreePossible = true;
                    }

                    //SI l'avion peut rentrer dans la boucle d'attente
                    if(indicEntreePossible == true)
                    {

                        //L'avion rentre dans la phase de boucle d'attente
                        m_ensembleRoute[k]->getAvionsPresents()[i]->set_action_en_cours(m_ensembleRoute[k]->getAvionsPresents()[i]->get_action_en_cours() + 1);

                        //On l'ajoute à la boucle d'attente
                        m_queue_boucle_attente.push_back(new Avion);
                        m_queue_boucle_attente.back() = m_ensembleRoute[k]->getAvionsPresents()[i];

                        //On le supprime de la route aérienne
                        m_ensembleRoute[k]->suppressionAvionRoute(i);
                    }
                }
            }
        }
    }
}


//Méthode d'actualisation de la file de sortie des pistes
void Aeroport::actualisationSortiePistes()
{
    bool peutParking = false; //Permet de savoir si l'avion peut rentrer dans le parking

    //Boucle de parcours des avions dans la file de sortie des pistes
    for(int i=0 ; i<int(m_queue_sortie_piste.size()) ; i++)
    {
        //Réinitialisation de la permission de rentrer dans le parking
        peutParking = false;

        //SI l'avion peut rentrer dans le parking
        if(m_queue_sortie_piste[i]->get_action_en_cours() == 6 && m_queue_sortie_piste[i]->get_duree_sortie_piste() == 0)
        {
            //SI le parking contient des avions
            if(int(m_avions_parking.size()) != 0)
            {
                //SI le parking n'est pas plein et que le dernier avion dedans a un décalage temporel suffisant par rapport au possible prochain avion
                //cout << "ACTUALISATION PARKING : " << m_avions_parking.back()->get_duree_prepraration() << " et " << get_delai_attente_sol()-2 << endl;
                if(m_avions_parking.back()->get_duree_prepraration() <= get_delai_attente_sol()-2 && int(m_avions_parking.size()) < m_nombre_places_sol)
                {
                    //On indique que l'avion va pouvoir rentrer dans le parking
                    peutParking = true;
                }
            }
            //SINON, le parking est vide
            else
            {
                //L'avion peut donc rentrer dans le parking
                peutParking = true;
            }

            //SI l'avion peut rentrer dans le parking
            if(peutParking == true)
            {
                //L'avion rentre dans la phase de préparation (état initial)
                m_queue_sortie_piste[i]->set_action_en_cours(0);
                m_queue_sortie_piste[i]->recuperationInfosAeroportD(this);

                //On l'ajoute au parking
                m_avions_parking.push_back(new Avion);
                m_avions_parking.back() = m_queue_sortie_piste[i];

                //On le supprime de la piste d'atterrissage
                m_queue_sortie_piste[i] = nullptr;
                m_queue_sortie_piste.erase(m_queue_sortie_piste.begin() + i);
            }
        }
    }
}


//Méthode d'actualisation des pistes de l'aéroport
void Aeroport::actualisationPistes(vector<RouteAerienne*> &m_ensembleRoutes)
{
    bool peutDecoller = false; //Permet de savoir si l'avion peut décoller
    bool peutAtterrir = false; //Permet de savoir si l'avion peut atterrir (passer à la sortie des pistes)
    int indiceRoute = -1; //Contient l'indice de la route dans laquelle ajouter l'avion

    //Boucle de parcours de l'ensemble des pistes
    for(int i=0 ; i<int(m_pistes.size()) ; i++)
    {
        //Boucle de parcours des possibles avions contenus dans la piste
        for(int j=0 ; j<int(m_pistes[i].size()) ; j++)
        {
            //On réinitialise l'indicateur de possibilité de décollage et d'atterrissage
            peutDecoller = true;
            peutAtterrir = true;

            //SI il y a un avion
            if(m_pistes[i][j] != nullptr)
            {
                //SI il peut décoller
                if(m_pistes[i][j]->get_action_en_cours() == 2 && m_pistes[i][j]->get_duree_decollage() == 0)
                {

                    //Parcours de l'ensemble des routes aériennes afin de trouver celle concernée par l'avion
                    for(int k=0 ; k<int(m_ensembleRoutes.size()) ; k++)
                    {
                        //SI on la trouve (la route)
                        if((m_ensembleRoutes[k]->getAeroport(0)->get_nom() == m_pistes[i][j]->getNomAeroportD() && m_ensembleRoutes[k]->getAeroport(1)->get_nom() == m_pistes[i][j]->getNomAeroportA())
                                || (m_ensembleRoutes[k]->getAeroport(0)->get_nom() == m_pistes[i][j]->getNomAeroportA() && m_ensembleRoutes[k]->getAeroport(1)->get_nom() == m_pistes[i][j]->getNomAeroportD()))
                        {
                            //Actualisation de l'indice de la route correpondant au trajet de l'avion
                            indiceRoute = k;

                            //SI la route possède des avions
                            if(int(m_ensembleRoutes[k]->getAvionsPresents().size()) != 0)
                            {
                                //On parcourt la liste de ses avions à la recherche de ceux ayant le même trajet
                                for(int m=0 ; m<int(m_ensembleRoutes[k]->getAvionsPresents().size()) ; m++)
                                {
                                    //SI on en trouve un qui a le même trajet, même modèle et dont le temps rend impossible le départ
                                    if(m_ensembleRoutes[k]->getAvionsPresents()[m]->getNomAeroportD() == m_pistes[i][j]->getNomAeroportD() && m_ensembleRoutes[k]->getAvionsPresents()[m]->getNomAeroportA() == m_pistes[i][j]->getNomAeroportA() && m_ensembleRoutes[k]->getAvionsPresents()[m]->get_modele() == m_pistes[i][j]->get_modele() && m_ensembleRoutes[k]->getAvionsPresents()[m]->get_duree_vol() > m_pistes[i][j]->get_duree_vol()-2)
                                    {
                                        //On indique que le décollage est impossible
                                        peutDecoller = false;
                                    }
                                }
                            }
                        }
                    }

                    //SI l'avion peut décoller
                    if(peutDecoller == true)
                    {
                        //L'avion rentre dans la phase d'accès aux pistes
                        m_pistes[i][j]->set_action_en_cours(m_pistes[i][j]->get_action_en_cours() + 1);

                        //Ajout de l'avion à la route aérienne
                        m_ensembleRoutes[indiceRoute]->ajoutAvion(m_pistes[i][j]);

                        //SI la piste ne contient qu'un seul avion
                        if(int(m_pistes[i].size()) == 1)
                        {
                            //On réinitialise le pointeur de l'avion
                            m_pistes[i][0] = nullptr;
                        }
                        //SINON, la piste contient plusieurs avions, on peut donc faire décoller celui en front
                        else
                        {
                            //On supprime l'avion de la file d'accès aux pistes
                            m_pistes[i][j] = nullptr;
                            m_pistes[i].erase(m_pistes[i].begin() + j);
                        }

                    }
                }

                //SINON SI il a atterri
                else if(m_pistes[i][j]->get_action_en_cours() == 5 && m_pistes[i][j]->get_duree_atterrissage() == 0)
                {

                    //SI la file de sortie de piste contient des avions
                    if(int(m_queue_sortie_piste.size()) != 0)
                    {
                        //Si le dernier avion à être rentré dedans a un decalage de 1 UT par rapport au nouveau
                        if(m_queue_sortie_piste.back()->get_duree_sortie_piste() <= m_pistes[i][j]->get_duree_sortie_piste()-2)
                        {
                            //On indique que l'avion peut y aller
                            peutAtterrir = true;
                        }
                    }
                    //SINON, la file est vide
                    else
                    {
                        //On indique que l'avion peut y aller
                        peutAtterrir = true;
                    }

                    //SI l'avion peut aller dans la file de sortie des pistes
                    if(peutAtterrir == true)
                    {
                        //L'avion rentre dans la phase de sortie de piste
                        m_pistes[i][j]->set_action_en_cours(m_pistes[i][j]->get_action_en_cours() + 1);

                        //On l'ajoute à la file de sortie de piste
                        m_queue_sortie_piste.push_back(new Avion);
                        m_queue_sortie_piste.back() = m_pistes[i][j];

                        //SI la piste ne contient qu'un seul avion
                        if(int(m_pistes[i].size()) == 1)
                        {
                            //On réinitialise le pointeur de l'avion
                            m_pistes[i][0] = nullptr;
                        }
                        //SINON, la piste contient plusieurs avions, on peut donc faire décoller celui en front
                        else
                        {
                            //On supprime l'avion de la file d'accès aux pistes
                            m_pistes[i][j] = nullptr;
                            m_pistes[i].erase(m_pistes[i].begin() + j);
                        }
                    }
                    m_pistes[i][j] = nullptr;
                }
            }
        }
    }
}


//Méthode d'actualisation des accès aux pistes de l'aéroport
void Aeroport::actualisationAccesPistes()
{
    int indicePisteDisponible = -1; //Contient l'indice de la piste disponible sur laquelle aller
    bool transitionPossible = true;

    //SI la file d'accès aux pistes dispose d'avions
    if(int(m_queue_acces_piste.size()) != 0)
    {
        //On fait tourner l'algorithme tant que l'avion en front de la file d'accès aux pistes peut potentiellement y accéder
        while(int(m_queue_acces_piste.size()) != 0 && m_queue_acces_piste.front()->get_action_en_cours() == 1 && m_queue_acces_piste.front()->get_duree_roulage() == 0 && transitionPossible == true)
        {
            //Parcours des pistes à la recherche d'une piste vide
            for(int j=0 ; j<m_nombre_pistes ; j++)
            {
                //SI la piste est vide
                if((int(m_pistes[j].size()) == 1 && m_pistes[j][0] == nullptr) || m_pistes[j].size() == 0)
                {
                    //On récupère son indice
                    indicePisteDisponible = j;
                }
            }

            //Parcours des pistes à la recherche d'une piste disponible (des avions y sont mais ont le même objectif : décoller)
            for(int j=0 ; j<m_nombre_pistes ; j++)
            {
                //Parcours des avions présents sur la piste
                for(int k=0 ; k<int(m_pistes[j].size()) ; k++)
                {
                    //SI un avion existe
                    if(m_pistes[j][k] != nullptr)
                    {
                        //SI cet avion décolle et a un décalage de 1 UT par rapport à celui que l'on veut injecter
                        if(m_pistes[j][k]->get_action_en_cours() == 2 && m_pistes[j][k]->get_duree_decollage() <= m_queue_acces_piste.front()->get_duree_decollage()-2)
                        {
                            //On récupère l'indice de la piste
                            indicePisteDisponible = j;
                        }
                    }
                }
            }

            //cout << indicePisteDisponible << endl;

            //SI une piste a été trouvée
            if(indicePisteDisponible != -1)
            {
                //On indique que c'est visiblement encore possible d'inclure des avions
                transitionPossible = true;

                //L'avion rentre dans la phase d'accès aux pistes
                m_queue_acces_piste.front()->set_action_en_cours(m_queue_acces_piste.front()->get_action_en_cours() + 1);

                //On l'ajoute à la piste en question
                if(int(m_pistes[indicePisteDisponible].size()) == 1 && m_pistes[indicePisteDisponible][0] == nullptr)
                {
                    m_pistes[indicePisteDisponible][0] = m_queue_acces_piste.front();
                }
                else if(int(m_pistes[indicePisteDisponible].size()) > 1 || m_pistes[indicePisteDisponible][0] != nullptr)
                {
                    m_pistes[indicePisteDisponible].push_back(new Avion);
                    m_pistes[indicePisteDisponible].back() = m_queue_acces_piste.front();
                }

                //On supprime l'avion de la file d'accès aux pistes
                m_queue_acces_piste.front() = nullptr;
                m_queue_acces_piste.erase(m_queue_acces_piste.begin());
            }
            //SINON
            else
            {
                //On indique que ça ne sert plus à rien d'essayer d'inclure des avions
                transitionPossible = false;
            }
        }
    }
}


//Méthode d'actualisation du parking
void Aeroport::actualisationParking(vector<Aeroport> m_aeroports, int indiceAeroport, int** m_matrice_adjacence, Horloge heureActuelle)
{
    vector<int> vecteur_escales;
    bool transitionPossible = true;
    //int indiceProchaineDestination = -1; //Contient l'indice de la prochaine destination (aéroport)
    string prochaineDestination = ""; //Va contenir la prochaine destination
    int indiceDepart = -1; //Contient l'indice de l'aéroport de départ
    int indiceArrivee = -1; //Contient l'indice de l'aéroport d'arrivée du prochain sous-trajet
    int distance = -1;
    bool recommencer = false;
    //On fait tourner l'algorithme tant que l'avion en front de la file du parking peut potentiellement accéder aux accès aux pistes
    while(int(m_avions_parking.size()) != 0 && m_avions_parking.front()->get_action_en_cours() == 0 && m_avions_parking.front()->get_duree_prepraration() == 0 && transitionPossible == true)
    {
        //SI la file d'accès aux pistes est vide ou que le dernier avion référencé dedans a un décalage de 1 UT avec le potentiel nouveau
        if(int(m_queue_acces_piste.size()) == 0 || m_queue_acces_piste[int(m_queue_acces_piste.size()) - 1]->get_duree_roulage() <= m_avions_parking.front()->get_duree_roulage() - 2)
        {
            transitionPossible = true; //On indique que la transition est possible

            //L'avion rentre dans la phase d'accès aux pistes
            m_avions_parking.front()->set_action_en_cours(m_avions_parking.front()->get_action_en_cours() + 1);

            //SI l'avion a fini son trajet précédent, DIJKSTRA
            if(int(m_avions_parking.front()->getTousIndicesEscales().size()) <= 1)
            {
                do
                {
                    //Récupération d'un aéroport de destination
                    prochaineDestination = piocheAeroportAleatoire();

                    //cout << endl << "DEPART : " << get_nom() << endl;
                    //cout << "NOUVELLE DESTINATION : " << prochaineDestination << endl;

                    //On parcourt les aéroports afin de trouver l'indice de l'aéroport de départ ainsi que celui d'arrivée
                    for(int i=0 ; i<int(m_aeroports.size()) ; i++)
                    {
                        //SI le nom de l'aéroport est celui de l'aéroport de départ
                        if(m_aeroports[i].get_nom() == get_nom())
                        {
                            //On récupère son indice
                            indiceDepart = i;
                        }
                        //SINON SI le nomde l'aéroport est celui de l'aéroport d'arrivée
                        else if(m_aeroports[i].get_nom() == prochaineDestination)
                        {
                            //On récupère son indice
                            indiceArrivee = i;
                        }
                    }

                    //Actualisation de l'aéroport initial et final du trajet
                    m_avions_parking.front()->setNomAeroportInitial(get_nom());
                    m_avions_parking.front()->setNomAeroportFinal(prochaineDestination);

                    //On détermine le trajet le plus court grâce à DIJKSTRA
                    vecteur_escales = dijkstra(m_matrice_adjacence, int(m_aeroports.size()), indiceDepart, indiceArrivee);

                    if(validite_dijkstra(m_aeroports[indiceDepart].get_distance_aeroports_preci(m_aeroports[vecteur_escales[0]].get_nom()), m_avions_parking.front()->get_type_vol()))
                    {
                        for(size_t s=0; s<vecteur_escales.size()-1;s++)
                        {//Parcours du vecteur d'escales
                            if(validite_dijkstra(m_aeroports[s].get_distance_aeroports_preci(m_aeroports[s+1].get_nom()), m_avions_parking.front()->get_type_vol())==false)
                            {
                              recommencer = true;
                            }


                        }
                    }
                    else
                    {
                        recommencer = true;
                    }
                    m_avions_parking.front()->setListeEscales(vecteur_escales);

                    //On actualise le prochain aéroport
                    m_avions_parking.front()->setNomAeroportA(m_aeroports[m_avions_parking.front()->getIndicePrecisEscale(0)].get_nom());

                    //On actualise l'heure de départ du vol
                    m_avions_parking.front()->setHeureDepart(heureActuelle);

                    if(int (vecteur_escales.size() )== 1)

                    {
                        for(int i=0; i< int (get_distance_aeroports().size()); i++)
                        {
                            if(get_distance_aeroports()[i].first == prochaineDestination)
                            {
                                distance = get_distance_aeroports()[i].second;
                            }
                        }
                    }
                //cout << verifier_distance_chemin(m_avions_parking.front(), vecteur_escales, m_matrice_adjacence, distance) << endl;
                }
                while(verifier_distance_chemin(m_avions_parking.front(), vecteur_escales, m_matrice_adjacence, distance)==false && recommencer == true );  //blindage distance / type de vol
                m_avions_parking.front()->set_liste_escales(get_nom()); //Ajout de l'aéroport de départ
                for(size_t t=0; t<vecteur_escales.size(); t++)
                {
                    m_avions_parking.front()->set_liste_escales(m_aeroports[vecteur_escales[t]].get_nom());
                }
            }
            //SINON, l'avion n'a pas fini son trajet, il continue alors ses escales
            else
            {
                //On pop l'aéroport dans lequel on se trouve (front du vecteur)
                m_avions_parking.front()->passageAEscaleSuivante();

                //On actualise le prochaine aéroport
                m_avions_parking.front()->setNomAeroportA(m_aeroports[m_avions_parking.front()->getIndicePrecisEscale(0)].get_nom());
            }

            //Récupération des informations de l'aéroport d'arrivée
            m_avions_parking.front()->recuperationInfosAeroportA(m_aeroports[m_avions_parking.front()->getIndicePrecisEscale(0)]);

            //On actualise le trajet allant être effectué
            m_avions_parking.front()->parametrer_nouveau_vol(m_aeroports[indiceAeroport].get_position().get_coord_x(),m_aeroports[indiceAeroport].get_position().get_coord_y(), m_aeroports[m_avions_parking.front()->getIndicePrecisEscale(0)].get_position().get_coord_x(),m_aeroports[m_avions_parking.front()->getIndicePrecisEscale(0)].get_position().get_coord_y());

            //On ajoute l'avion dans la file d'accès aux pistes
            m_queue_acces_piste.push_back(new Avion);
            m_queue_acces_piste.back() = m_avions_parking.front();

            //On supprime l'avion du parking
            m_avions_parking.front() = nullptr;
            m_avions_parking.erase(m_avions_parking.begin());
        }
        //SINON impossible de faire transitionner l'avion
        else
        {
            transitionPossible = false;
        }
    }

    //Boucle d'actualisation de l'altitude de l'avion à celui de l'aéroport
    for(int i=0 ; i<int(m_avions_parking.size()) ; i++)
    {
        //SI c'est un court courrier
        if(m_avions_parking[i]->get_type_vol() == "court")
        {
            m_avions_parking[i]->set_altitude(getAltitudesAvions(0));
        }
        //SINON SI c'est un moyen courrier
        else if(m_avions_parking[i]->get_type_vol() == "moyen")
        {
            m_avions_parking[i]->set_altitude(getAltitudesAvions(1));
        }
        //SINON SI c'est un long courrier
        else if(m_avions_parking[i]->get_type_vol() == "long")
        {
            m_avions_parking[i]->set_altitude(getAltitudesAvions(2));
        }
    }
}


bool Aeroport::validite_dijkstra(int distance, string type_vol)
{
    if(distance>0 && distance<= 3000 && type_vol == "court")
    {
        return true;
    }
    else if(distance > 3000 && distance <= 8000 && type_vol == "moyen")
    {
        return true;
    }
    else if(distance > 8000 && type_vol == "long")
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Aeroport::verifier_distance_chemin (Avion * avion_utlise, vector<int> vecteur_escales, int **m_matrice_adjacence, int distance)
{
    int distance_max = distance_maximale(vecteur_escales, m_matrice_adjacence, distance);
    if(distance_max>0 && distance_max<= 3000 && avion_utlise->get_type_vol() == "court")
    {
        return true;
    }
    else if(distance_max > 3000 && distance_max <= 8000 && avion_utlise->get_type_vol() == "moyen")
    {
        return true;
    }
    else if(distance_max > 8000 && avion_utlise->get_type_vol() == "long")
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Aeroport::distance_maximale( vector<int> vecteur_escales, int **m_matrice_adjacence, int distance)
{
    if(int (vecteur_escales.size()) != 1)
    {
        int maxi =0;

        for(int t=0; t<int(vecteur_escales.size()-1); t++)
        {
            if(m_matrice_adjacence[vecteur_escales[t]][vecteur_escales[t+1]] > maxi)
            {

                maxi = m_matrice_adjacence[vecteur_escales[t]][vecteur_escales[t+1]];
            }
        }
        return maxi;
    }
    else
    {
        return distance;
    }


}

//Méthode d'ajout d'un avion dans le parking de l'aéroport
void Aeroport::ajoutAvionParking(Avion *nouvelAvion)
{
    //SI le parking de l'aéroport dispose encore de places libres
    if(int(m_avions_parking.size()) < m_nombre_places_sol)
    {
        //Ajout de l'avion dans le parking de l'aéroport
        m_avions_parking.push_back(new Avion);
        m_avions_parking.back() = nouvelAvion;
        m_avions_parking.back()->recuperationInfosAeroportD(this);
    }
}



////////////////////////////////////
// RECHERCHE DU PLUS COURT CHEMIN //
////////////////////////////////////

//Méthode de pioche aléatoire d'un nom d'aéroport
string Aeroport::piocheAeroportAleatoire()
{
    int valeur_aleatoire = 0;

    //do
    //{
        valeur_aleatoire = rand()%(int(m_distance_aeroports.size()));
    //}while(m_distance_aeroports[valeur_aleatoire].second == -1);

    return m_distance_aeroports[valeur_aleatoire].first;
}



//Algo de recherche du chemin le plus court
vector <int> Aeroport::dijkstra(int** m_matrice_adjacence, int m_envergureSimulation, int depart, int arrivee)
{
    //VAR
    int * distance = new int [m_envergureSimulation]; //Distance entre les sommets
    int * predecesseur = new int [m_envergureSimulation]; //Vecteur de prédécesseurs
    bool * verif_arrivee = new bool [m_envergureSimulation]; //Vecteur de bool pour confirmer la vitesse

    //INITIALISATION
    for(int i=0; i<m_envergureSimulation; i++)
    {
        predecesseur[i] = -1; //Vecteur de predecesseur à -1
        distance[i] = INT_MAX; //Distance infinie
        verif_arrivee[i] = false; //Verification de l'arrivee fausse
    }

    distance[depart] = 0; //Distance du point de depart = 0

    //BOUCLE D'ALGO
    for(int i=0; i<m_envergureSimulation; i++)
    {
        //Pour chaque sommet
        int sommet_proche = distance_minimale(m_envergureSimulation, distance, verif_arrivee); //Distance la plus proche
        verif_arrivee[sommet_proche] = true; //Sommet actuel visité

        for(int j=0; j<m_envergureSimulation; j++)
        {
            //Pour chaque sommet
            if(!verif_arrivee[j] && m_matrice_adjacence[sommet_proche][j] && distance[sommet_proche] != INT_MAX && distance[sommet_proche] + m_matrice_adjacence[sommet_proche][j] < distance[j])
            {
                //Sommet non visiter & le sommet le plus proche dans la matrice == 0 & distance du sommet le plus proche != infini & distance du sommet le plus proche + distance jusqu'au prochain sommet < à distance precedemment calculé
                //MISE A JOUR CAR CHEMIN PLUS COURT TROUVE
                distance[j] = distance[sommet_proche] + m_matrice_adjacence[sommet_proche][j]; //Mise à jour de la distance
                predecesseur[j] = sommet_proche; //Mise à jour du predecesseur
            }
        }
    }
    vector <int> tempo = resultat(distance, verif_arrivee, predecesseur, depart, arrivee);
//    delete distance;
//    delete predecesseur;
//    delete verif_arrivee;
    return (tempo); //Rendu du vecteur d'escales
}


//Méthode pour connaitre la plus petite distance à un sommet
int Aeroport::distance_minimale(int m_envergureSimulation, int * dist, bool * verif)
{
    //Var
    int mini = INT_MAX, index = 0;

    for (int i = 0; i < m_envergureSimulation; i++)
    {
        //Pour chaque sommet
        if (verif[i] == false && dist[i] <= mini)
        {
            //Si la case n'est pas explorée & distance inferieur à l'infinie
            //MISE A JOUR DE LA DISTANCE
            mini = dist[i]; //Changement distance par rapport au sommet
            index = i; //Enregistrement de l'index
        }
    }
    return index; //Rendu de l'index
}


//Programme récursif permettant de connaitre l'ordre des escales à réaliser
void Aeroport::afficher_chemin(int * predecesseur, int j, std::vector <int> &correspondance)
{
    if (predecesseur[j] == - 1)
    {
        //Si le sommet n'a pas de predecesseur
    }

    else
    {
        afficher_chemin(predecesseur, predecesseur[j], correspondance); //Appel recursif
        if(predecesseur[j] != -1)//Si le sommet a un predecesseur
        {
            //std::cout << m_aeroports[j].get_nom() << "  "; //affichage du predecesseur
            correspondance.push_back(j);//Ajout du sommet dans le vecteur d'escale
        }
    }
}




//Exploitation des résultats de l'agorythme de dijkstra
vector <int> Aeroport::resultat(int * dist, bool * verif, int * predecesseur, int source, int arrivee)
{
    vector <int> correspondances;

    if(verif[arrivee] == true)
    {
        //Si le sommet est rallié
        //Chemin de predecesseurs
        //std::cout << std::endl << "Chemin : " << m_aeroports[source].get_nom()<< "  ";
        afficher_chemin(predecesseur, arrivee, correspondances); //Appel de la fonction traçant le chemin à réaliser

        //std::cout << std::endl << std::endl;
    }
    else//Sommet non rallié
        std::cout << "Il est impossible de relier ces deux sommet avec un chemin, veuillez faire attenion au nom des aeroports !" << std::endl;

    return correspondances; //Rendu du vecteur de correspondances
}
