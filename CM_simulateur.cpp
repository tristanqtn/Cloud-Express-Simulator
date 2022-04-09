#include "CH_simulateur.h"
#include <math.h>

using namespace std;


//////////////////////////////////
// CONSTRUCTEURS ET DESTRUCTEUR //
//////////////////////////////////


//Constructeur par défaut
Simulateur::Simulateur()
    : m_modeSimulation(-1), m_envergureSimulation(-1)
{

}


//Destructeur
Simulateur::~Simulateur()
{
    /*
    for(int i=0; i<m_envergureSimulation; i++)
    {
        delete m_matrice_adjacence[i];
    }
    delete m_matrice_adjacence;

    cout << "destruction";
    */
}



////////////////
// ACCESSEURS //
////////////////

//Getter de l'indice d'un aéroport
int Simulateur::getIndiceAeroport(string nomAeroport)
{
    //Boucle de recherche de l'aéroport
    for(int i=0 ; i<int(m_aeroports.size()) ; i++)
    {
        if(m_aeroports[i].get_nom() == nomAeroport) //On trouve l'aéroport
        {
            return i; //On return son indice
        }
    }

    return -1;
}



//////////////
// METHODES //
//////////////


//////////////////////////////////////
// FONCTIONS DIRECTES DU SIMULATEUR //
//////////////////////////////////////

//Méthode d'initialisation du simulateur
void Simulateur::initSimulateur(int modeSimulation, int envergureSimulation)
{
    //RouteAerienne routeTempo; //Contient temporairement une route aérienne
    bool indicRoute = false; //Permet de savoir si une route est déjà référencée

    //Initialisation du mode et de l'envergure de la simulation
    m_modeSimulation = modeSimulation;
    m_envergureSimulation = envergureSimulation;

    //Processus de réinitialisation de l'ensemble des vecteurs du simulateur en cas de relance de la simulation sans fermeture du programme
    reinitialisationSimulateur();

    //SI l'envergure choisie est NORMALE
    if(envergureSimulation == 7)
    {
        charger_carte_NORMALE();
    }

    //SINON, l'envergure choisie est EXTREME
    else
    {
        charger_carte_EXTREME();
    }

    //Coloration du graphe
    algoWelsh();

    //Création des routes aériennes entre les aéroports
    for(int i=0 ; i<int(m_aeroports.size()) ; i++) //Boucle de parcours du vecteur d'aéroports
    {

        //Boucle de parcours des aéroports connectés à celui-ci
        for(int j=0 ; j<int(m_aeroports[i].get_distance_aeroports().size()) ; j++)
        {
            //Réinitialisation de l'indicateur d'existence de la route
            indicRoute = false;

            //SI l'aéroport est connecté
            if(m_aeroports[i].get_distance_aeroports()[j].second != -1)
            {

                //Boucle de parcours évitant les doublons de routes aériennes
                for(int k=0 ; k<int(m_ensembleRoutes.size()) ; k++)
                {
                    //SI la route est déjà référencée
                    if((m_ensembleRoutes[k]->getAeroport(0)->get_nom() == m_aeroports[i].get_nom() && m_ensembleRoutes[k]->getAeroport(1)->get_nom() == m_aeroports[i].get_distance_aeroports()[j].first) || (m_ensembleRoutes[k]->getAeroport(0)->get_nom() == m_aeroports[i].get_distance_aeroports()[j].first && m_ensembleRoutes[k]->getAeroport(1)->get_nom() == m_aeroports[i].get_nom()))
                    {
                        indicRoute = true; //On l'indique
                    }
                }

                //SI la route n'est pas déjà référencée, on le fait
                if(indicRoute == false)
                {

                    RouteAerienne* nouvelleRoute = new RouteAerienne();
                    m_ensembleRoutes.push_back(nouvelleRoute);

                    //On crée la nouvelle route
                    m_ensembleRoutes.back()->initAeroports(m_aeroports[i], m_aeroports[getIndiceAeroport(m_aeroports[i].get_distance_aeroports()[j].first)]); //Ajout du lien entre les deux aéroports
                    m_ensembleRoutes.back()->setLongueur(m_aeroports[i].get_distance_aeroports()[j].second); //Ajout de la distance

                    //Initialisation du double vecteur d'entiers de la route
                    m_ensembleRoutes.back()->initTabEtats();

                    //On initialise les coefficients de la droite
                    m_ensembleRoutes.back()->setARoute((m_ensembleRoutes.back()->getAeroport(1)->get_position().get_coord_y() - m_ensembleRoutes.back()->getAeroport(0)->get_position().get_coord_y()) / (m_ensembleRoutes.back()->getAeroport(1)->get_position().get_coord_x() - m_ensembleRoutes.back()->getAeroport(0)->get_position().get_coord_x()));
                    m_ensembleRoutes.back()->setBRoute(m_ensembleRoutes.back()->getAeroport(1)->get_position().get_coord_y() - (m_ensembleRoutes.back()->getARoute() * m_ensembleRoutes.back()->getAeroport(1)->get_position().get_coord_x()));
                }
            }
        }
    }

    //Chargement des modeles d'avions
    Avion court_courrier ("data/avion/court.txt");
    m_infos_types_avions.push_back(court_courrier);
    Avion moyen_courrier ("data/avion/moyen.txt");
    m_infos_types_avions.push_back(moyen_courrier);
    Avion long_courrier ("data/avion/long.txt");
    m_infos_types_avions.push_back(long_courrier);

    //Création d'une horloge
    Horloge new_date;

    m_horlogeGMT = new_date;

    deduire_matrice_adjacence();
}


//Méthode de réinitialisation du simulateur
void Simulateur::reinitialisationSimulateur()
{
    //Réinitialisation du vecteur d'aéroports
    while(int(m_aeroports.size()) > 0)
    {
        m_aeroports.pop_back();
    }

    //Réinitialisation du vecteur de la flotte d'avions
    while(int(m_flotte_avions.size()) > 0)
    {
        m_flotte_avions.back() = nullptr;
        m_flotte_avions.pop_back();
    }

    //Réinitialisation du vecteur contenant les informations des différents types d'avion
    while(int(m_infos_types_avions.size()) > 0)
    {
        m_infos_types_avions.pop_back();
    }

    //Réinitialisation du vecteur contenant les routes aériennes
    while(int(m_ensembleRoutes.size()) > 0)
    {
        m_ensembleRoutes.back() = nullptr;
        m_ensembleRoutes.pop_back();
    }
}


//Méthode qui permet le chargement de la carte NORMALE
void Simulateur::charger_carte_NORMALE()
{
    int nombre_aeroport; //Contient le nombre d'aéroports de la simulation

    //Ouverture du fichier
    ifstream fichier_nombre_aeroport{"data/aeroport/carte_1/NB.txt"};
    if (!fichier_nombre_aeroport)
        throw runtime_error( "Impossible d'ouvrir en lecture data/aeroport/carte_1/NB.txt"); //Erreur de lecture

    //Lecture du nom de l'aéroport
    fichier_nombre_aeroport >> nombre_aeroport;
    if (fichier_nombre_aeroport.fail() )
        throw runtime_error("Probleme lecture nombre aeroport"); //Erreur de lecture

    //Chargement de la carte de base
    Aeroport BKK ("data/aeroport/carte_1/BKK.txt", nombre_aeroport);
    m_aeroports.push_back(BKK);
    Aeroport CDG ("data/aeroport/carte_1/CDG.txt", nombre_aeroport);
    m_aeroports.push_back(CDG);
    Aeroport CPT ("data/aeroport/carte_1/CPT.txt", nombre_aeroport);
    m_aeroports.push_back(CPT);
    Aeroport JFK ("data/aeroport/carte_1/JFK.txt", nombre_aeroport);
    m_aeroports.push_back(JFK);
    Aeroport LPB ("data/aeroport/carte_1/LPB.txt", nombre_aeroport);
    m_aeroports.push_back(LPB);
    Aeroport PAP ("data/aeroport/carte_1/PAP.txt", nombre_aeroport);
    m_aeroports.push_back(PAP);
    Aeroport PER ("data/aeroport/carte_1/PER.txt", nombre_aeroport);
    m_aeroports.push_back(PER);
}


//Méthode qui permet le chargement de la carte EXTREME
void Simulateur::charger_carte_EXTREME()
{
    int nombre_aeroport; //Contient le nombre d'aéroports de la simulation

    //Ouverture du fichier
    ifstream fichier_nombre_aeroport{"data/aeroport/carte_2/NB.txt"};
    if (!fichier_nombre_aeroport)
        throw runtime_error( "Impossible d'ouvrir en lecture data/aeroport/carte_2/NB.txt"); //Erreur de lecture

    //Lecture du nom de l'aéroport
    fichier_nombre_aeroport >> nombre_aeroport;
    if (fichier_nombre_aeroport.fail() )
        throw runtime_error("Probleme lecture nombre aeroport"); //Erreur de lecture

    //Chargement de la carte extreme
    Aeroport BKK ("data/aeroport/carte_2/BKK.txt", nombre_aeroport);
    m_aeroports.push_back(BKK);
    Aeroport CDG ("data/aeroport/carte_2/CDG.txt", nombre_aeroport);
    m_aeroports.push_back(CDG);
    Aeroport DEL ("data/aeroport/carte_2/DEL.txt", nombre_aeroport);
    m_aeroports.push_back(DEL);
    Aeroport DFA ("data/aeroport/carte_2/DFA.txt", nombre_aeroport);
    m_aeroports.push_back(DFA);
    Aeroport HND ("data/aeroport/carte_2/HND.txt", nombre_aeroport);
    m_aeroports.push_back(HND);
    Aeroport HNL ("data/aeroport/carte_2/HNL.txt", nombre_aeroport);
    m_aeroports.push_back(HNL);
    Aeroport JFK ("data/aeroport/carte_2/JFK.txt", nombre_aeroport);
    m_aeroports.push_back(JFK);
    Aeroport JNB ("data/aeroport/carte_2/JNB.txt", nombre_aeroport);
    m_aeroports.push_back(JNB);
    Aeroport MEX ("data/aeroport/carte_2/MEX.txt", nombre_aeroport);
    m_aeroports.push_back(MEX);
    Aeroport MOW ("data/aeroport/carte_2/MOW.txt", nombre_aeroport);
    m_aeroports.push_back(MOW);
    Aeroport SCL ("data/aeroport/carte_2/SCL.txt", nombre_aeroport);
    m_aeroports.push_back(SCL);
    Aeroport STO ("data/aeroport/carte_2/STO.txt", nombre_aeroport);
    m_aeroports.push_back(STO);
    Aeroport UAK ("data/aeroport/carte_2/UAK.txt", nombre_aeroport);
    m_aeroports.push_back(UAK);
    Aeroport WLG ("data/aeroport/carte_2/WLG.txt", nombre_aeroport);
    m_aeroports.push_back(WLG);
    Aeroport YUL ("data/aeroport/carte_2/YUL.txt", nombre_aeroport);
    m_aeroports.push_back(YUL);
}


//Méthode de déroulement général de la simulation
void Simulateur::deroulementGlobal(Ressources &motherShip, bool &indicClic, bool &done)
{
    bool finDeroulement = false; //Indicateur de fin de déroulement de la simulation
    bool indicEchap = false; //Indicateur permettant de savoir quand la touche ESCAPE est pressée
    bool indicEspace = false; //Indicateur permettant de savoir quand la touche ESPACE est pressée
    bool indicF = false; //Indicateur permettant de savoir quand la touche F est pressée
    bool pause = false; //Indicateur de pause
    int compteur = 0; //Permet d'actualiser à temps réguliers les avions et l'horloge
    int modeVitesse = 0; //Permet de déterminer la vitesse d'actualisation
    int coorXRectangleDroite = 1165; //Contient la position en X du rectangle de droite
    int etatMenu = 1; //Permet de suivre l'évolution du menu à droite du simulateur

    //Processus d'initialisation des cartes affichées en fond
    initCartesFond(motherShip);

    //Boucle de déroulement général de la partie
    while(!finDeroulement)
    {
        //Réinitialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //SI aucune pause n'a lieu
        if(!pause)
        {
            compteur++; //Incrémentation du compteur permettant les actualisations régulières

            //SI une actualisation doit avoir lieu
            if((modeVitesse == 0 && compteur == 120) || (modeVitesse == 1 && compteur == 60) || (modeVitesse == 2 && compteur == 30))
            {
                compteur = 0; //Réinitialisation du compteur

                //Réinitialisation de la vitesse des avions
                for(int i = 0 ; i < int(m_flotte_avions.size()) ; i ++)
                {
                    if(m_flotte_avions[i]->get_type_vol() == "court")
                    {
                        m_flotte_avions[i]->set_vitesse(m_infos_types_avions[0].get_vitesse());
                    }
                    else if(m_flotte_avions[i]->get_type_vol() == "moyen")
                    {
                        m_flotte_avions[i]->set_vitesse(m_infos_types_avions[1].get_vitesse());
                    }
                    else if(m_flotte_avions[i]->get_type_vol() == "long")
                    {
                        m_flotte_avions[i]->set_vitesse(m_infos_types_avions[2].get_vitesse());
                    }
                }

                //Boucle d'actualisation des intempéries
                for(int i=0 ; i<int(m_listeIntemperies.size()) ; i++)
                {
                    //Actualisation de l'intempérie
                    m_listeIntemperies[i]->actualisationIntemperie();

                    //SI l'intempérie est terminée
                    if(m_listeIntemperies[i]->getDureeIntemperie() == 0)
                    {
                        //On la supprime du vecteur de pointeurs sur Intempérie
                        m_listeIntemperies.erase(m_listeIntemperies.begin() + i);
                    }
                }

                //ACTUALISATION DES CRASH
                for(size_t t=0; t<m_coord_crash.size(); t++)
                {
                    m_duree_affichage_crash[t]--;
                    if(m_duree_affichage_crash[t] == 0)
                    {
                        m_duree_affichage_crash.erase(m_duree_affichage_crash.begin()+t);
                        m_coord_crash.erase(m_coord_crash.begin()+t);
                    }
                }

                //Analyse du placement des intempéries
                rechercheIntemperie(motherShip);

                //ACTUALISATION DE L'HORLOGE
                m_horlogeGMT.actualiser_heure();

                //ACTUALISATION DES AEROPORTS
                for(int i=0 ; i<int(m_aeroports.size()) ; i++)
                {
                    m_aeroports[i].actualisationAeroport(m_aeroports, m_ensembleRoutes, i, m_matrice_adjacence, m_horlogeGMT);

                }

                //ACTUALISATION DES AVIONS
                for(int i=0 ; i<int(m_flotte_avions.size()) ; i++)
                {
                    ///////////////////
                    bool crash = m_flotte_avions[i]->actualiser_action_avion();
                    if(crash == true)
                    {
                        nouveau_crash((m_flotte_avions[i]));
                    }
                    ///////////////////
                }
            }
        }

        //cout << mouse_x << " et " << mouse_y << endl;

        //SI l'heure est comprise entre 7h et 20h, c'est le JOUR
        if(m_horlogeGMT.get_heure().first > 6 && m_horlogeGMT.get_heure().first < 20) //SI il s'agit du JOUR
        {
            blit(motherShip.getBIT(6), motherShip.getBIT(0), 0, 0, 0, 0, motherShip.getBIT(6)->w, motherShip.getBIT(6)->h); //Affichage de la map monde JOUR
        }
        //SINON, c'est la NUIT
        else
        {
            blit(motherShip.getBIT(7), motherShip.getBIT(0), 0, 0, 0, 0, motherShip.getBIT(7)->w, motherShip.getBIT(7)->h); //Affichage de la map monde NUIT
        }

        //SI l'utilisateur appuie sur C
        if(key[KEY_C])
        {
            //On affiche le quadrillage
            affichageQuadrillage(motherShip);
        }
        if(key[KEY_F] && indicF == false)
        {
                ouvrir_liste_crash();
                indicF = true;
        }


        //Affichage de l'horloge
        m_horlogeGMT.afficher_heure_date(motherShip.getBIT(0), motherShip.getFONT(6), motherShip.getFONT(5));

        //Boucle d'affichage des intempéries
        for(int i=0 ; i<int(m_listeIntemperies.size()) ; i++)
        {
            //On affiche l'intempérie
            m_listeIntemperies[i]->afficherIntemperie(motherShip);
        }

        //Affichage du rectangle de droite
        rectfill(motherShip.getBIT(0), coorXRectangleDroite, 0, SCREEN_W, SCREEN_H, makecol(41, 49, 51));

        //Affichage du menu de droite actuel
        switch(etatMenu)
        {
        case 1 : //Menu principal : CHOIX
            menuPrincipal(motherShip, indicClic, etatMenu);
            break;

        case 2 : //Menu secondaire : AJOUT D'UN AVION
            menuAjoutAvion(motherShip, indicClic, etatMenu);
            break;

        case 3 : //Menu secondaire : SUPPRESSION D'UN AVION
            menuSupprimerAvion(motherShip, indicClic, etatMenu);
            break;

        case 4 : //Menu secondaire : AJOUT D'UNE INTEMPERIE
            menuAjoutIntemperie(motherShip, indicClic, etatMenu);
            break;
        }

        //Affichage des boutons de déroulement temporel de la simulation
        masked_stretch_blit(motherShip.getBIT(10), motherShip.getBIT(0), 0, 0, motherShip.getBIT(10)->w, motherShip.getBIT(10)->h, coorXRectangleDroite+20, 670, motherShip.getBIT(10)->w/7, motherShip.getBIT(10)->h/7);
        masked_stretch_blit(motherShip.getBIT(12), motherShip.getBIT(0), 0, 0, motherShip.getBIT(12)->w, motherShip.getBIT(12)->h, coorXRectangleDroite+80, 668, motherShip.getBIT(12)->w/6, motherShip.getBIT(12)->h/6);
        masked_stretch_blit(motherShip.getBIT(14), motherShip.getBIT(0), 0, 0, motherShip.getBIT(14)->w, motherShip.getBIT(14)->h, coorXRectangleDroite+140, 670, motherShip.getBIT(14)->w/4-5, motherShip.getBIT(14)->h/4-5);
        masked_stretch_blit(motherShip.getBIT(16), motherShip.getBIT(0), 0, 0, motherShip.getBIT(16)->w, motherShip.getBIT(16)->h, coorXRectangleDroite+220, 670, motherShip.getBIT(16)->w/3-9, motherShip.getBIT(16)->h/3-9);

        //SI la souris passe sur le bouton PAUSE
        if(mouse_x >= 1188 && mouse_x <= 1224 && mouse_y >= 670 && mouse_y <= 720)
        {
            //SI clic gauche
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true;
                pause = true; //Indication qu'il s'agit de la pause
            }
        }

        //SINON SI la souris passe sur le bouton PLAY
        else if(mouse_x >= 1250 && mouse_x <= 1286 && mouse_y >= 670 && mouse_y <= 720)
        {
            //SI clic gauche
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true;
                pause = false; //Indication qu'il ne s'agit plus de la pause
                modeVitesse = 0; //Indication qu'il s'agit du mode LENT
                compteur = 0; //Réinitialisation du compteur pour éviter les bugs
            }
        }

        //SINON SI la souris passe sur le bouton Speed 1
        else if(mouse_x >= 1309 && mouse_x <= 1369 && mouse_y >= 670 && mouse_y <= 720)
        {
            //SI clic gauche
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true;
                pause = false; //Indication qu'il ne s'agit plus de la pause
                modeVitesse = 1; //Indication qu'il s'agit du mode MOYEN
                compteur = 0; //Réinitialisation du compteur pour éviter les bugs
            }
        }

        //SINON SI la souris passe sur le bouton Speed 2
        else if(mouse_x >= 1386 && mouse_x <= 1472 && mouse_y >= 670 && mouse_y <= 720)
        {
            //SI clic gauche
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true;
                pause = false; //Indication qu'il ne s'agit plus de la pause
                modeVitesse = 2; //Indication qu'il s'agit du mode RAPIDE
                compteur = 0; //Réinitialisation du compteur pour éviter les bugs
            }
        }

        //Affichage du bouton actif
        if(pause == true) //PAUSE
        {
            masked_stretch_blit(motherShip.getBIT(11), motherShip.getBIT(0), 0, 0, motherShip.getBIT(11)->w, motherShip.getBIT(11)->h, coorXRectangleDroite+20, 670, motherShip.getBIT(11)->w/7, motherShip.getBIT(11)->h/7);
        }
        else
        {
            if(modeVitesse == 0) //PLAY
            {
                masked_stretch_blit(motherShip.getBIT(13), motherShip.getBIT(0), 0, 0, motherShip.getBIT(13)->w, motherShip.getBIT(13)->h, coorXRectangleDroite+80, 668, motherShip.getBIT(13)->w/6, motherShip.getBIT(13)->h/6);
            }
            else if(modeVitesse == 1) //SPEED 1
            {
                masked_stretch_blit(motherShip.getBIT(15), motherShip.getBIT(0), 0, 0, motherShip.getBIT(15)->w, motherShip.getBIT(15)->h, coorXRectangleDroite+140, 670, motherShip.getBIT(15)->w/4-5, motherShip.getBIT(15)->h/4-5);
            }
            else //SPEED 2
            {
                masked_stretch_blit(motherShip.getBIT(17), motherShip.getBIT(0), 0, 0, motherShip.getBIT(17)->w, motherShip.getBIT(17)->h, coorXRectangleDroite+220, 670, motherShip.getBIT(17)->w/3-9, motherShip.getBIT(17)->h/3-9);
            }
        }

        //Parcours de l'ensemble des aéroports pour voir si la souris est au dessus de l'un d'eux
        for(int i=0 ; i<int(m_aeroports.size()) ; i++)
        {
            m_aeroports[i].actualisationPointeurLocalisation(motherShip, m_aeroports, indicClic, indicEchap);
        }

        //Parcours de l'ensemble des routes aériennes pour voir si la souris est au dessus de l'une d'elles
        for(int i=0 ; i<int(m_ensembleRoutes.size()) ; i++)
        {
            m_ensembleRoutes[i]->actualisationSurbrillanceRoute(motherShip, indicClic, indicEchap);
        }
        //Parcours de l'ensemble des crash afin de les afficher
        for(int i=0 ; i<int(m_coord_crash.size()) ; i++)
        {
            masked_blit(motherShip.getBIT(37), motherShip.getBIT(0), 0, 0, m_coord_crash[i].get_coord_x(), m_coord_crash[i].get_coord_y()-motherShip.getBIT(37)->h/2, motherShip.getBIT(37)->w,motherShip.getBIT(37)->h);
        }

        //Parcours de l'ensemble des avions afin de les afficher
        for(int i=0 ; i<int(m_flotte_avions.size()) ; i++)
        {
            m_flotte_avions[i]->affichageAvionCarte(motherShip.getBIT(0), motherShip.getBIT(18), motherShip.getBIT(19), motherShip.getBIT(20));
        }

        //Parcours de l'ensemble des avions afin de voir si l'utilsiateur souhaite afficher leur overlay
        for(int i=0 ; i<int(m_flotte_avions.size()) ; i++)
        {
            bool suppression = m_flotte_avions[i]->actualisationSurbrillanceAvion(m_aeroports, true, m_flotte_avions[i]->get_coord().get_coord_x(), m_flotte_avions[i]->get_coord().get_coord_y(),motherShip.getBIT(0), motherShip.getBIT(26), motherShip.getBIT(30), motherShip.getBIT(27), motherShip.getBIT(28), motherShip.getBIT(29), motherShip.getFONT(7), motherShip.getFONT(2), motherShip.getFONT(10));
            if(suppression == true && m_flotte_avions[i]->get_action_en_cours() == 3)
            {
                    for(size_t t=0; t<m_ensembleRoutes.size(); t++)
                    {
                        m_ensembleRoutes[t]->supprimer_avion(m_flotte_avions[i]->get_immatriculation());
                    }
                    supprimer_avion_on_click(m_flotte_avions[i]->get_immatriculation());
            }
        }


        //SI la touche ESPACE est pressée, on inverse l'état de pause de la simulation
        if(key[KEY_SPACE] && indicEspace == false)
        {
            indicEspace = true;
            pause = !pause;
        }

        //SI l'utilisateur appuie sur ESC, lance le menu de sortie du jeu
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true; //On indique que la touche ESC est pressée
            menuESC(done, finDeroulement, indicClic, indicEchap, motherShip.getBIT(0), motherShip.getBIT(3), motherShip.getFONT(3), motherShip.getFONT(1));
        }

        //SI le clic gauche n'est plus pressé, on l'indique
        if(!(mouse_b & 1) && indicClic == true)
        {
            indicClic = false;
        }

        //SI la touche ESCAPE n'est plus pressée
        if(!key[KEY_ESC] && indicEchap == true)
        {
            indicEchap = false; //On l'indique
        }

        //SI la touche ESPACE n'est plus pressée
        if(!key[KEY_SPACE] && indicEspace == true)
        {
            indicEspace = false; //On l'indique
        }
        if(!key[KEY_F] && indicF== true)
        {
               indicF = false;
        }

        //Affichage du curseur
        affichageCurseur(motherShip.getBIT(3), motherShip.getBIT(0));

        //Affichage du double buffer
        blit(motherShip.getBIT(0), screen, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);
    }

    //Sauvegarde de l'heure
    m_horlogeGMT.sauvegarder_heure();
}


//Méthode d'ajout d'un avion dans un parking
void Simulateur::nouvelAvionDansParking(Avion *nouvelAvion)
{
    vector<int> listeAeroportsDispo; //Contient la liste des indices des aéroports disponibles
    int indiceAeroportAleatoire; //Contient l'indice aléatoire de l'aéroport accueillant le nouvel avion

    //Boucle de parcours de l'ensemble des aéroports afin de référencer ceux disponibles
    for(int i=0 ; i<int(m_aeroports.size()) ; i++)
    {
        //SI l'aéroport est disponible
        if(int(m_aeroports[i].getNbreAvionsParking()) < m_aeroports[i].get_nombre_places_sol())
        {
            if(verifier_distance_chemin_sim(nouvelAvion->get_type_vol(), m_aeroports[i].get_distance_aeroports())==true)
                //On l'ajoute dans la liste des aéroports disponibles
             {

                listeAeroportsDispo.push_back(i);
             }
        }
    }

    //On tire aléatoirement un des indices parmi ceux disponibles
    indiceAeroportAleatoire = rand()%(int(listeAeroportsDispo.size()));
    cout << endl << "AEROPORT DE DEPART : " << m_aeroports[listeAeroportsDispo[indiceAeroportAleatoire]].get_nom() << endl << endl;

    //On ajoute l'avion à l'aéroport désigné
    m_aeroports[listeAeroportsDispo[indiceAeroportAleatoire]].ajoutAvionParking(nouvelAvion);
}

bool Simulateur::verifier_distance_chemin_sim (string type_vol, vector<pair<string, int>> aeroports_connectes)
{
    int distance_min = distance_minimale_sim(aeroports_connectes);

    if(distance_min <= 3000 && type_vol == "court")
    {
        return true;
    }
    else if(distance_min > 3000 && distance_min <= 8000 && type_vol == "moyen")
    {
        return true;
    }
    else if(distance_min > 8000 && type_vol == "long")
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Simulateur::distance_minimale_sim(vector<pair<string, int>> aeroports_connectes)
{
    int mini = INT_MAX;
    for(size_t t=0; t<aeroports_connectes.size(); t++)
    {
        if(aeroports_connectes[t].second < mini && aeroports_connectes[t].second!=-1)
        {
            mini = aeroports_connectes[t].second;
        }
    }
    return mini;
}


//Méthode de positionnement des intempéries sur les routes

/*
parcours du tableau d'intempéries :
m_listeIntemperies[i]->get_coord_x() par exemple

parcours du tableau de routes
m_ensembleRoute[i]->getARoute()


*/



///////////////////////////////////////
// FONCTIONS PRATIQUES DU SIMULATEUR //
///////////////////////////////////////

//Méthode d'affichage de l'ensemble des aéroports
void Simulateur::afficher_aeroports()
{
    //Boucle de parcours des aéroports
    for(size_t t=0; t<m_aeroports.size(); t++)
    {
        //Affichage de l'aéroport
        m_aeroports[t].afficher_caracteristique(m_envergureSimulation);
    }
}


//Méthode de création d'un avion
void Simulateur::creer_avion(string type_de_vol)
{
    int modele_aleatoire = rand()%3; //Génération alétoire d'un entier entre 0 et 2 pour modèle
    bool autorisation = true; //Booléen d'autorisation pour l'immatriculation

    //SI le mode de création est ALEATOIRE
    if(type_de_vol == "aleatoire")
    {
        cout << " ----- CREATION ALEATOIRE D'UN AVION" << endl;

        //Génération d'un avion aléatoire
        int choix_aleatoire = rand()%3; //Génération alétoire d'un entier entre 0 et 2 pour type de vol

        m_flotte_avions.push_back(new Avion(m_infos_types_avions[choix_aleatoire]));

        //Choix du modèle aléatoirement
        if(m_flotte_avions.back()->get_type_vol() == "court") //Court courrier
        {
            if(modele_aleatoire == 0)
            {
                m_flotte_avions.back()->set_modele("BOMBARDIER CRJ 200");
            }
            else if(modele_aleatoire == 1)
            {
                m_flotte_avions.back()->set_modele("AIRBUS A319");
            }
            else if(modele_aleatoire == 2)
            {
                m_flotte_avions.back()->set_modele("BOEING 737");
            }
        }
        else if(m_flotte_avions.back()->get_type_vol() == "moyen") //Moyen courrier
        {
            if(modele_aleatoire == 0)
            {
                m_flotte_avions.back()->set_modele("AIRBUS A220");
            }
            else if(modele_aleatoire == 1)
            {
                m_flotte_avions.back()->set_modele("BOEING 747");
            }
            else if(modele_aleatoire == 2)
            {
                m_flotte_avions.back()->set_modele("BOEING 757");
            }
        }
        else if (m_flotte_avions.back()->get_type_vol() == "long") //Long courrier
        {
            if(modele_aleatoire == 0)
            {
                m_flotte_avions.back()->set_modele("AIRBUS A830");
            }
            else if(modele_aleatoire == 1)
            {
                m_flotte_avions.back()->set_modele("AIRBUS A850");
            }
            else if(modele_aleatoire == 2)
            {
                m_flotte_avions.back()->set_modele("BOIENG 777");
            }
        }

        //Génération aléatoire de l'immatriculation
        do
        {
            m_flotte_avions.back()->set_immatriculation(chaine_aleatoire(5)); //Génération de l'immatriculation

            //Parcours de l'ensemble des avions pour vérifier qu'il ne s'agit pas d'un doublon
            for(size_t t=0; t<m_flotte_avions.size()-1; t++)
            {
                //SI l'immatriculation est déjà prise
                if(m_flotte_avions.back()->get_immatriculation() == m_flotte_avions[t]->get_immatriculation())
                {
                    autorisation = false; //Autorisation fausse => re-générer une immatriculation
                }
            }
        }
        while(autorisation == false); //Génération blindée

        //Ajout du nouvel avion dans un aéroport aléatoire
        nouvelAvionDansParking(m_flotte_avions.back());
    }

    //SINON SI le mode de création est AVION COURT COURRIER
    else if(type_de_vol == "court")
    {
        m_flotte_avions.push_back(new Avion(m_infos_types_avions[0]));

        //Choix aléatoire du modèle
        if(modele_aleatoire == 0)
        {
            m_flotte_avions.back()->set_modele("BOMBARDIER CRJ 200");
        }
        else if(modele_aleatoire == 1)
        {
            m_flotte_avions.back()->set_modele("AIRBUS A319");
        }
        else if(modele_aleatoire == 2)
        {
            m_flotte_avions.back()->set_modele("BOEING 737");
        }

        //Génération aléatoire de l'immatriculation
        do
        {
            m_flotte_avions.back()->set_immatriculation(chaine_aleatoire(5)); //Génération de l'immatriculation

            //Parcours de l'ensemble des avions pour vérifier qu'il ne s'agit pas d'un doublon
            for(size_t t=0; t<m_flotte_avions.size()-1; t++)
            {
                //SI l'immatriculation est déjà prise
                if(m_flotte_avions.back()->get_immatriculation() == m_flotte_avions[t]->get_immatriculation())
                {
                    autorisation = false; //Autorisation fausse => re-générer une immatriculation
                }
            }
        }
        while(autorisation == false); //Génération blindée

        //Ajout du nouvel avion dans un aéroport aléatoire
        nouvelAvionDansParking(m_flotte_avions.back());
    }

    if(type_de_vol == "moyen")
    {
        //génération d'un vol moyen

        m_flotte_avions.push_back(new Avion(m_infos_types_avions[1]));

        //Choix du modèle aléatoirement
        if(modele_aleatoire == 0)
        {
            //choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("AIRBUS A220");
        }
        else if(modele_aleatoire == 1)
        {
            //choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("BOEING 747");
        }
        else if(modele_aleatoire == 2)
        {
            //choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("BOEING 757");
        }

        //Génération aléatoire de l'immatriculation
        do
        {
            m_flotte_avions.back()->set_immatriculation(chaine_aleatoire(5)); //Génération de l'immatriculation

            //Parcours de l'ensemble des avions pour vérifier qu'il ne s'agit pas d'un doublon
            for(size_t t=0; t<m_flotte_avions.size()-1; t++)
            {
                //SI l'immatriculation est déjà prise
                if(m_flotte_avions.back()->get_immatriculation() == m_flotte_avions[t]->get_immatriculation())
                {
                    autorisation = false; //Autorisation fausse => re-générer une immatriculation
                }
            }
        }
        while(autorisation == false); //Génération blindée

        //Ajout du nouvel avion dans un aéroport aléatoire
        nouvelAvionDansParking(m_flotte_avions.back());
    }

    if(type_de_vol == "long")
    {
        //génération d'un vol long

        m_flotte_avions.push_back(new Avion(m_infos_types_avions[2]));

        //Choix du modèle aléatoirement
        if(modele_aleatoire == 0)
        {
            //choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("AIRBUS A830");
        }
        else if(modele_aleatoire == 1)
        {
            //choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("AIRBUS A850");
        }
        else if(modele_aleatoire == 2)
        {
            //choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("BOIENG 777");
        }

        //Génération aléatoire de l'immatriculation
        do
        {
            m_flotte_avions.back()->set_immatriculation(chaine_aleatoire(5)); //Génération de l'immatriculation

            //Parcours de l'ensemble des avions pour vérifier qu'il ne s'agit pas d'un doublon
            for(size_t t=0; t<m_flotte_avions.size()-1; t++)
            {
                //SI l'immatriculation est déjà prise
                if(m_flotte_avions.back()->get_immatriculation() == m_flotte_avions[t]->get_immatriculation())
                {
                    autorisation = false; //Autorisation fausse => re-générer une immatriculation
                }
            }
        }
        while(autorisation == false); //Génération blindée

        //Ajout du nouvel avion dans un aéroport aléatoire
        nouvelAvionDansParking(m_flotte_avions.back());
    }
}


//Méthode de suppression aléatoire d'un avion
void Simulateur::supprimer_avion_aleatoire()
{
    int index_suppression = (rand()%m_flotte_avions.size());
    m_flotte_avions.erase(m_flotte_avions.begin()+index_suppression);
}


//Méthode de suppression d'un avion ciblé par son nom
void Simulateur::supprimer_avion_on_click(string immatriculation)
{
    //Parcours du vecteur d'avions
    for(size_t t=0; t<m_flotte_avions.size(); t++)
    {
        //On a trouvé l'avion à supprimer
        if(m_flotte_avions[t]->get_immatriculation() == immatriculation)
        {
            m_flotte_avions.erase(m_flotte_avions.begin()+t); //On le supprime
        }
    }
}



/////////////////////////////////////
// FONCTIONS DE MENU DU SIMULATEUR //
/////////////////////////////////////

//Méthode du menu déclenché lors de la pression de la touche ESCAPE
void Simulateur::menuESC(bool &done, bool &finDeroulement, bool &indicClic, bool &indicEchap, BITMAP* doubleBuffer, BITMAP* curseur, FONT* policeTitre, FONT* policeChoix)
{
    bool finMenuESC = false; //Indicateur de fin du menu ESC
    BITMAP* doubleBufferProvisoire = create_bitmap(SCREEN_W, SCREEN_H); //Permet d'avoir le fond du menu
    int ecartBords = 100;

    //Impression / conservation du fond derrière le menu ESCAPE
    blit(doubleBuffer, doubleBufferProvisoire, 0, 0, 0, 0, doubleBuffer->w, doubleBuffer->h);

    //Boucle de déroulement du menu ESC
    while(!finMenuESC)
    {
        //Réinitialisation du double buffer
        clear_bitmap(doubleBuffer);

        //Affichage du double buffer contenant le fond du menu
        blit(doubleBufferProvisoire, doubleBuffer, 0, 0, 0, 0, doubleBufferProvisoire->w, doubleBufferProvisoire->h);

        //Affichage des rectangles entourant le menu
        rectfill(doubleBuffer, ecartBords, ecartBords, SCREEN_W-ecartBords, SCREEN_H-ecartBords, makecol(127, 127, 127));
        rectfill(doubleBuffer, ecartBords+30, ecartBords+30, SCREEN_W-ecartBords-30, SCREEN_H-ecartBords-30, makecol(204, 204, 204));

        //Affichage du nom du menu
        textprintf_centre_ex(doubleBuffer, policeTitre, SCREEN_W/2, ecartBords+60, makecol(255, 255, 255), -1, "MENU");

        //Affichage des choix
        textprintf_centre_ex(doubleBuffer, policeChoix, SCREEN_W/2, 320, makecol(255,255,255), -1, "Reprendre la simulation");
        textprintf_centre_ex(doubleBuffer, policeChoix, SCREEN_W/2, 410, makecol(255,255,255), -1, "Revenir au menu principal");
        textprintf_centre_ex(doubleBuffer, policeChoix, SCREEN_W/2, 500, makecol(255,255,255), -1, "Quitter le simulateur");

        //SI l'utilisateur passe sa souris sur "Reprendre la simulation"
        if((mouse_x >= 483 && mouse_x <= 1017) && (mouse_y >=321 && mouse_y <= 371)) //Reprendre la simulation
        {
            textprintf_centre_ex(doubleBuffer, policeChoix, SCREEN_W/2, 320, makecol(250,202,48), -1, "Reprendre la simulation");

            if(mouse_b & 1 && indicClic == false) //L'utilisateur souhaite reprendre la simulation
            {
                indicClic = true; //Indication que le clic gauche est maintenu
                finMenuESC = true;
            }
        }

        //SINON SI l'utilisateur passe sa souris sur "Revenir au menu principal"
        else if((mouse_x >= 447 && mouse_x <= 1054) && (mouse_y >=413 && mouse_y <= 451)) //Revenir au menu principal
        {
            textprintf_centre_ex(doubleBuffer, policeChoix, SCREEN_W/2, 410, makecol(250, 202, 48), -1, "Revenir au menu principal");

            if(mouse_b & 1 && indicClic == false) //L'utilisateur souhaite reprendre la simulation
            {
                indicClic = true; //Indication que le clic gauche est maintenu
                finMenuESC = true;
                finDeroulement = true;

                for(size_t t=0; t<m_flotte_avions.size(); t++)
                {
                    m_flotte_avions[t]->enregistrer_informations();
                }
            }
        }

        //SINON SI l'utilisateur passe sa souris sur "Quitter le simulateur"
        else if((mouse_x >= 521 && mouse_x <= 984) && (mouse_y >= 501 && mouse_y <= 540)) //Quitter le simulateur
        {
            textprintf_centre_ex(doubleBuffer, policeChoix, SCREEN_W/2, 500, makecol(250, 0, 0), -1, "Quitter le simulateur");

            if(mouse_b & 1 && indicClic == false) //L'utilisateur souhaite reprendre la simulation
            {
                indicClic = true; //Indication que le clic gauche est maintenu
                finMenuESC = true;
                finDeroulement = true;
                done = true;

                for(size_t t=0; t<m_flotte_avions.size(); t++)
                {
                    m_flotte_avions[t]->enregistrer_informations();
                }
            }
        }

        //Affichage du curseur
        affichageCurseur(curseur, doubleBuffer);

        //Affichage du double buffer
        blit(doubleBuffer, screen, 0, 0, 0, 0, doubleBuffer->w, doubleBuffer->h);

        //SI la touche ESC est pressée de nouveau
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true;
            finMenuESC = true;
        }

        //SI la touche ESC n'est plus pressée
        if(!key[KEY_ESC])
        {
            indicEchap = false; //On l'indique
        }

        //SI le clic gauche de la souris n'est pas pressé
        if(!(mouse_b & 1))
        {
            indicClic = false; //On l'indique
        }
    }
}


//Méthode du menu principal de choix pendant la simulation
void Simulateur::menuPrincipal(Ressources &motherShip, bool &indicClic, int &etat)
{
    //Affichage du menu
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(3), 1250, 30, makecol(255,255,255),-1, "Choix");
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 150, makecol(255,255,255),-1, "Ajouter un avion");
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 200, makecol(255,255,255),-1, "Supprimer un avion");
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 250, makecol(255,255,255),-1, "Ajouter intemperie");

    //SI la simulation dispose d'avions
    if(int(m_flotte_avions.size()) > 0)
    {
        //On propose le scénario de la duite de réservoir
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 300, makecol(255, 255, 255), -1, "Fuite de reservoir");

        //On propose également le scénario de détournement d'avion
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 350, makecol(255, 255, 255), -1, "Detournement d'avion");
    }


     //cout << mouse_x << " et " << mouse_y << endl;
    //SI la souris passe sur "Ajouter un avion"
    if(mouse_x > 1190 && mouse_x < 1422 && mouse_y > 150 && mouse_y < 180)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 150, makecol(25,255,52),-1, "Ajouter un avion"); //Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé
            etat = 2; //On passe au menu d'ajout d'un avion
        }
    }

    //SINON SI la souris passe sur "Supprimer un avion"
    else if(mouse_x > 1190 && mouse_x < 1471 && mouse_y > 200 && mouse_y < 230)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 200, makecol(25,255,52),-1, "Supprimer un avion"); //Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé
            etat = 3; //On passe au menu de suppression d'un avion
        }
    }

    //SINON SI la souris passe sur "Ajouter une intempérie"
    else if(mouse_x > 1190 && mouse_x < 1422 && mouse_y > 250 && mouse_y < 280)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 250, makecol(25,255,52),-1, "Ajouter intemperie");//Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé
            etat = 4; //On passe au menu d'ajout d'une intempérie
        }
    }

    //SINON SI la souris passe sur "Fuite de reservoir"
    else if(mouse_x > 1190 && mouse_x < 1457 && mouse_y > 300 && mouse_y < 330 && int(m_flotte_avions.size()) > 0)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 300, makecol(25,255,52),-1, "Fuite de reservoir");//Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé
            bool ok = false;
            do
            {
                int indice_hasard = rand()%m_flotte_avions.size();
                if(m_flotte_avions[indice_hasard]->get_action_en_cours() != 0)
                {
                    m_flotte_avions[indice_hasard]->set_etat_reservoir(true);
                    ok = true;
                }
            }while(ok == false);
            // FUITE DE RESERVOIR ICI
            // PTET RAJOUTER UN ATTRIBUT A AVION HISTOIRE DE POUVOIR LAFFICHER DIFFEREMMENT
        }
    }

    //SINON SI la souris passe sur "Détournement d'avion"
    else if(mouse_x > 1190 && mouse_x < 1485 && mouse_y > 350 && mouse_y < 380 && int(m_flotte_avions.size()) > 0)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1190, 350, makecol(25,255,52),-1, "Detournement d'avion");//Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé

            //int indice_hasard = rand()%m_flotte_avions.size();
            //m_flotte_avions[indice_hasard]->set_etat_reservoir(true);
            // DETOURNEMENT ICI
            // PTET RAJOUTER UN ATTRIBUT A AVION HISTOIRE DE POUVOIR LAFFICHER DIFFEREMMENT
        }
    }
}


//Méthode du menu d'ajout d'un avion
void Simulateur::menuAjoutAvion(Ressources &motherShip, bool &indicClic, int &etat)
{
    //Affichage du titre
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(3),1200, 30, makecol(255,255,255), -1, "Ajouter");
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(3),1200, 110, makecol(255,255,255), -1, "un avion");

    //Affichage du bouton de retour
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1187, 610, makecol(255,255,255),-1, "Retour");

    //SI il reste des places dans les aéroports
    if(aeroportDisponible())
    {
        //Affichage du menu
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 250, makecol(255,255,255),-1, "Court courrier");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 300, makecol(255,255,255),-1, "Moyen courrier");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 350, makecol(255,255,255),-1, "Long courrier");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 400, makecol(255,255,255),-1, "Aleatoire");


        //SI la souris passe sur "Court courrier"
        if(mouse_x > 1200 && mouse_x < 1405 && mouse_y > 250 && mouse_y < 275)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 250, makecol(25,255,52),-1, "Court courrier"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion court courrier
                creer_avion("court");
            }
        }
        //SINON SI la souris passe sur "Moyen courrier"
        else if(mouse_x > 1200 && mouse_x < 1415 && mouse_y > 300 && mouse_y < 325)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 300, makecol(25,255,52),-1, "Moyen courrier"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion moyen courrier
                creer_avion("moyen");
            }
        }
        //SINON SI la souris passe sur "Long courrier"
        else if(mouse_x > 1200 && mouse_x < 1395 && mouse_y > 350 && mouse_y < 375)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 350, makecol(25,255,52),-1, "Long courrier"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion long courrier
                creer_avion("long");
            }
        }
        //SINON SI la souris passe sur "Aléatoire"
        else if(mouse_x > 1200 && mouse_x < 1315 && mouse_y > 400 && mouse_y < 425)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 400, makecol(25,255,52),-1, "Aleatoire"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion aléatoire
                creer_avion("aleatoire");
            }
        }
    }

    //SINON, plus aucun aéroport ne peut accueillir de nouvel avion et on l'indique
    else
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 400, makecol(255,255,255),-1, "Plus de places..."); //Changement de couleur du texte
    }

    //SI la souris passe sur "Retour"
    if(mouse_x > 1187 && mouse_x < 1315 && mouse_y > 610 && mouse_y < 635)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1187, 610, makecol(25,255,52),-1, "Retour"); //Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé
            etat = 1; //Retour au menu précédent
        }
    }
}


//Méthode du menu de suppression d'un avion
void Simulateur::menuSupprimerAvion(Ressources &motherShip, bool &indicClic, int &etat)
{
    //Affichage du titre
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(9),1200, 30, makecol(255,255,255), -1, "Supprimer");
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(3),1200, 110, makecol(255,255,255), -1, "un avion");

    //Affichage du bouton retour
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1187, 610, makecol(255,255,255),-1, "Retour");

    //SI il reste des avions, on peut encore en supprimer
    if(encoreDesAvions())
    {
        //Affichage du menu
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 250, makecol(255,255,255),-1, "Court courrier");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 300, makecol(255,255,255),-1, "Moyen courrier");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 350, makecol(255,255,255),-1, "Long courrier");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 400, makecol(255,255,255),-1, "Aleatoire");


        //SI la souris passe sur "Court courrier"
        if(mouse_x > 1200 && mouse_x < 1405 && mouse_y > 250 && mouse_y < 275)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 250, makecol(25,255,52),-1, "Court courrier"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion court courrier
            }
        }
        //SINON SI la souris passe sur "Moyen courrier"
        else if(mouse_x > 1200 && mouse_x < 1415 && mouse_y > 300 && mouse_y < 325)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 300, makecol(25,255,52),-1, "Moyen courrier"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion moyen courrier
            }
        }
        //SINON SI la souris passe sur "Long courrier"
        else if(mouse_x > 1200 && mouse_x < 1395 && mouse_y > 350 && mouse_y < 375)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 350, makecol(25,255,52),-1, "Long courrier"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion long courrier
            }
        }
        //SINON SI la souris passe sur "Aléatoire"
        else if(mouse_x > 1200 && mouse_x < 1315 && mouse_y > 400 && mouse_y < 425)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 400, makecol(25,255,52),-1, "Aleatoire"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Ajout d'un avion aléatoire
            }
        }
    }
    //SINON, on ne peut plus supprimer d'avion
    else
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 400, makecol(255,255,255),-1, "Plus d'avions..."); //On l'indique
    }

    //SI la souris passe sur "Retour"
    if(mouse_x > 1187 && mouse_x < 1315 && mouse_y > 610 && mouse_y < 635)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1187, 610, makecol(25,255,52),-1, "Retour"); //Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé
            etat = 1; //Retour au menu précédent
        }
    }
}


void Simulateur::menuAjoutIntemperie(Ressources &motherShip, bool &indicClic, int &etat)
{
    //Affichage du titre
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(3),1200, 30, makecol(255,255,255), -1, "Ajouter");
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(7),1220, 110, makecol(255,255,255), -1, "intemperie");

    //Affichage du bouton de retour
    textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 610, makecol(255,255,255),-1, "Retour");

    //SI il est encore possible d'ajouter des intempéries
    if(int(m_listeIntemperies.size()) < MAX_INTEMPERIES)
    {
        //Affichage des choix disponibles
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 250, makecol(255,255,255),-1, "Aleatoire");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 300, makecol(255,255,255),-1, "Pluie");
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 350, makecol(255,255,255),-1, "Vents violents");


        //SI la souris passe sur "Aleatoire"
        if(mouse_x > 1200 && mouse_x < 1405 && mouse_y > 250 && mouse_y < 275)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 250, makecol(25,255,52),-1, "Aleatoire"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Création de l'intempérie ALEATOIRE
                m_listeIntemperies.push_back(new Intemperie("aleatoire"));
            }
        }
        //SINON SI la souris passe sur "Pluie"
        else if(mouse_x > 1200 && mouse_x < 1415 && mouse_y > 300 && mouse_y < 325)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 300, makecol(25,255,52),-1, "Pluie"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Création de l'intempérie PLUIE
                m_listeIntemperies.push_back(new Intemperie("pluie"));
            }
        }
        //SINON SI la souris passe sur "Vents violents"
        else if(mouse_x > 1200 && mouse_x < 1395 && mouse_y > 350 && mouse_y < 375)
        {
            textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 350, makecol(25,255,52),-1, "Vents violents"); //Changement de couleur du texte

            //SI clic
            if(mouse_b & 1 && indicClic == false)
            {
                indicClic = true; //Indication que le clic gauche est pressé

                //Création de l'intempérie VENT
                m_listeIntemperies.push_back(new Intemperie("vent"));
            }
        }
    }
    //SINON, il n'est plus possible d'en rajouter
    else
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1180, 400, makecol(255,255,255),-1, "Trop d'intemperies..."); //On l'indique
    }

    //SI la souris passe sur "Retour"
    if(mouse_x > 1187 && mouse_x < 1315 && mouse_y > 610 && mouse_y < 635)
    {
        textprintf_ex(motherShip.getBIT(0), motherShip.getFONT(8), 1200, 610, makecol(25,255,52),-1, "Retour"); //Changement de couleur du texte

        //SI clic
        if(mouse_b & 1 && indicClic == false)
        {
            indicClic = true; //Indication que le clic gauche est pressé
            etat = 1; //Retour au menu précédent
        }
    }
}



/////////////////
// INDICATEURS //
/////////////////

//Méthode permettant de savoir si il reste des aéroports disponibles pour ajouter des avions
bool Simulateur::aeroportDisponible()
{
    //Boucle de parcours de la liste des aéroports de la simulation
    for(int i=0 ; i<int(m_aeroports.size()) ; i++)
    {
        //SI aéroport dispose encore de place dans son parking
        if(int(m_aeroports[i].getNbreAvionsParking()) < m_aeroports[i].get_nombre_places_sol())
        {
            //On indique qu'il est donc encore possible d'ajouter des avions
            return true;
        }
    }

    //SINON, on indique que ce n'est pas possible d'ajouter un nouvel avion
    return false;
}


//Méthode permettant de savoir s'il reste des avions dans la simulation
bool Simulateur::encoreDesAvions()
{
    //SI il reste des avions
    if(int(m_flotte_avions.size()) > 0)
    {
        //On l'indique
        return true;
    }
    //SINON, il n'y a plus d'avion
    else
    {
        //On l'indique
        return false;
    }
}



//////////////
// REGLAGES //
//////////////

//Méthode d'initialisation des cartes mondiales en fonction des aéroports
void Simulateur::initCartesFond(Ressources &motherShip)
{
    int coorXRectangleDroite = 1165; //Contient la position en X du rectangle de droite

    //Impression des cartes vierges
    blit(motherShip.getBIT(4), motherShip.getBIT(6), 0, 0, 0, 0, motherShip.getBIT(4)->w, motherShip.getBIT(4)->h);
    blit(motherShip.getBIT(5), motherShip.getBIT(7), 0, 0, 0, 0, motherShip.getBIT(5)->w, motherShip.getBIT(5)->h);

    //Affichage de l'ensemble des routes aériennes
    for(int i=0 ; i<int(m_ensembleRoutes.size()) ; i++)
    {

            //Routes aériennes de JOUR
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
            line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));

            //Routes aériennes de NUIT
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
            line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
    }

    //Affichage du rectangle de droite
    rectfill(motherShip.getBIT(6), coorXRectangleDroite, 0, SCREEN_W, SCREEN_H, makecol(41, 49, 51));

    //Impression de l'ensemble des aéroports
    for(int i=0 ; i<int(m_aeroports.size()) ; i++)
    {
        masked_stretch_blit(motherShip.getBIT(8), motherShip.getBIT(6), 0, 0, motherShip.getBIT(8)->w, motherShip.getBIT(8)->h, m_aeroports[i].get_position().get_coord_x() - motherShip.getBIT(8)->w/4, m_aeroports[i].get_position().get_coord_y() - motherShip.getBIT(8)->h/2, motherShip.getBIT(8)->w/2, motherShip.getBIT(8)->h/2);
        masked_stretch_blit(motherShip.getBIT(8), motherShip.getBIT(7), 0, 0, motherShip.getBIT(8)->w, motherShip.getBIT(8)->h, m_aeroports[i].get_position().get_coord_x() - motherShip.getBIT(8)->w/4, m_aeroports[i].get_position().get_coord_y() - motherShip.getBIT(8)->h/2, motherShip.getBIT(8)->w/2, motherShip.getBIT(8)->h/2);
    }
}


//Méthode de création de la matrice d'adjacence
void Simulateur::deduire_matrice_adjacence ()
{
    //Alloc de la matrice [ordre]x[ordre]
    m_matrice_adjacence= new int*[m_envergureSimulation]; //alloc d'un tableau de sommet * de la taille de l'ordre

    for(int i = 0; i < m_envergureSimulation; ++i) //pour chaque case du tableau de pointeur de sommet
        m_matrice_adjacence[i] = new int[m_envergureSimulation]; //alloc du tableai de sommet

    //mise à 0 de toutes les cases de la matrice
    for(int i=0; i<m_envergureSimulation; i++)
    {
        //Colonnes
        for(int j=0; j<m_envergureSimulation; j++)
        {
            //Lignes
            m_matrice_adjacence[i][j] = 0; //RAZ
        }
    }

    for(int i=0; i<m_envergureSimulation; i++)
    {
        //Pour chaque aéroport du simulateur
        for(size_t t=0; t<m_aeroports[i].get_distance_aeroports().size(); t++)
        {
            //Pour chaque aéroports connectés à l'aéroport étudié
            if(m_aeroports[i].get_successeur_precis(t).second != -1)
            {
                //Si les aéroports sont bel et bien connecté (poid != -1)
                //Stockage du poid dans la colonne de l'aeroport étudié et ligne de l'aéroport connecté
                m_matrice_adjacence[getIndiceAeroport(m_aeroports[i].get_nom())][getIndiceAeroport(m_aeroports[i].get_successeur_precis(t).first)] = m_aeroports[i].get_successeur_precis(t).second;
            }
        }
    }
}



void Simulateur::nouveau_crash(Avion * crash)
{
    std::ofstream fichier;
    fichier.open("data/historique_crash/historique_crash.txt", std::ofstream::app);

    if(!fichier)
    {}
    else
    {
        fichier << "Le " << crash->get_modele() << " du vol " << crash->get_immatriculation() << " (" << crash->get_type_vol() << " courrier) au depart de " << crash->getNomAeroportD() << " a destination de " << crash->getNomAeroportD() << " le " << m_horlogeGMT.get_jour() << "/" << m_horlogeGMT.get_mois()<< "/"  << m_horlogeGMT.get_annee()<< " a " << m_horlogeGMT.get_heure().first<< "h" << m_horlogeGMT.get_heure().second<< "\n";
        fichier.close();
    }


    m_coord_crash.push_back(crash->get_coord());
    m_duree_affichage_crash.push_back(20);

    for(size_t t=0; t<m_ensembleRoutes.size(); t++)
    {
        m_ensembleRoutes[t]->supprimer_avion(crash->get_immatriculation());
    }
    supprimer_avion_on_click(crash->get_immatriculation());
}


void Simulateur::ouvrir_liste_crash()
{
    system("start notepad \"data\\historique_crash\\historique_crash.txt\"");
}


//Méthode de coloration du graphe
void Simulateur::algoWelsh()
{
    int compteur = 0; //Permet de calculer le degré des sommets
    vector<int> tabDegre; //Contient les degrés des aéroports
    vector<int> tabIndicesAeroport; //Contient les indices des aéroports par rapport à m_aeroports
    vector<int> tabColoration; //Contient l'ensemble des colorations affectées aux indices
    int stockage1; //Permet les inversions de valeur lors du tri à bulle
    int stockage2; //Permet les inversions de valeur lors du tri à bulle
    int couleurProbable = 0; //Contient la couleur probable affectée à un sommet
    bool finColoration = true; //Indique lorsque le processus de coloration est terminé
    bool couleurDejaRefCommePrise = false; //Permet d'indiquer lorsqu'une couleur a déjà été référencée comme prise
    vector<int> couleurDejaPrise; //Permet de déterminer quelles couleurs ont été prises dans les sommets adjacents à un autre


    //Détermination de l'ordre décroissant des sommets
    //Boucle de parcours des aéroports afin de les trier selon degré décroissant
    for(int i = 0 ; i < int(m_aeroports.size()) ; i++)
    {
        //Parcours de l'ensemble des adjacences de l'aéroport
        for(int j = 0 ; j < int(m_aeroports[i].get_distance_aeroports().size()); j++)
        {
            //SI il s'agit d'un aéroport adjacent
            if(m_aeroports[i].get_distance_aeroports()[j].second != -1)
            {
                //On incrémente le degré de l'aéroport initial
                compteur++;
            }
        }

        //On enregistre le degré calculé
        tabDegre.push_back(compteur);

        //On enregistre l'indice de l'aéroport équivalent
        tabIndicesAeroport.push_back(i);

        //Réinitialisation du compteur
        compteur = 0;
    }

    //Tri à bulle décroissant
    //Boucle de tri du tableau de Degrés en même temps que celui d'Indice
    for (int i=0;i<=int(m_aeroports.size())-1;i++)
    {
        for(int j=0;j<=int(m_aeroports.size())-2;j++)
        {
            //SI les deux valeurs doivent être inversées
            if (tabDegre[j]<tabDegre[j+1])
            {
                //On inverse pour les deux tableaux
                stockage1=tabDegre[j];
                stockage2=tabIndicesAeroport[j];

                tabDegre[j]=tabDegre[j+1];
                tabIndicesAeroport[j]=tabIndicesAeroport[j+1];

                tabDegre[j+1]=stockage1;
                tabIndicesAeroport[j+1]=stockage2;
            }
        }
    }


    //Boucle d'initialisation du tableau de couleurs
    for(int i = 0 ; i < int(tabDegre.size()) ; i++)
    {
        tabColoration.push_back(0);
    }

    //Attribution de la première couleur
    tabColoration[0] = 1;


    //Boucle de coloration
    do
    {
        //Réinitialisation de la variable à risque
        finColoration = true;

        //Boucle de parcours des aéroports ordonnés
        for(int i=0 ; i<int(tabIndicesAeroport.size()) ; i++)
        {
            //Réinitialisation du vecteur contenant les couleurs prises par les sommets adjacents à l'aéroport
            for(int i=0 ; i<int(couleurDejaPrise.size()) ; i++)
            {
                couleurDejaPrise.pop_back();
            }

            //SI le sommet n'a toujours pas été coloré
            if(tabColoration[i] == 0)
            {

                //On parcourt ces voisins
                for(int j = 0 ; j < int(m_aeroports[tabIndicesAeroport[i]].get_distance_aeroports().size()) ; j++)
                {
                    //SI le voisin en est un
                    if(m_aeroports[tabIndicesAeroport[i]].get_distance_aeroports()[j].second != -1)
                    {
                        //On essaye alors de trouver la couleur de son voisin en reparcourant le vecteur ordonné
                        for(int k = 0 ; k < int(tabIndicesAeroport.size()) ; k++)
                        {
                            //SI l'indice référencé correspond au sommet voisin
                            if(tabIndicesAeroport[k] == getIndiceAeroport(m_aeroports[tabIndicesAeroport[i]].get_distance_aeroports()[j].first))
                            {
                                //Réinitialisation (position arbitraire) de l'indicateur permettant d'éviter les doublons dans le vecteur de couleurs déjà prises
                                couleurDejaRefCommePrise = false;

                                //SI ce voisin est coloré
                                if(tabColoration[k] != 0)
                                {
                                    //On cherche à savoir si la coloration n'est pas déjà référencée
                                    for(int m=0 ; m<int(couleurDejaPrise.size()) ; m++)
                                    {
                                        //SI la coloration est déjà référencée
                                        if(couleurDejaPrise[m] == tabColoration[k])
                                        {
                                            //On l'indique
                                            couleurDejaRefCommePrise = true;
                                        }
                                    }

                                    //SI la coloration n'est pas référencée
                                    if(couleurDejaRefCommePrise == false)
                                    {
                                        //On ajoute la couleur au vecteur de couleurs déjà prises
                                        couleurDejaPrise.push_back(tabColoration[k]);
                                    }
                                }
                            }
                        }
                    }
                }

                //Tri à bulle décroissant des couleurs déjà prises
                for (int j=0;j<int(couleurDejaPrise.size());j++)
                {
                    for(int k=0;k<int(couleurDejaPrise.size())-j-1;k++)
                    {
                        //Les deux valeurs doivent être inversées
                        if (couleurDejaPrise[k]<couleurDejaPrise[k+1])
                        {
                            stockage1=couleurDejaPrise[k];

                            couleurDejaPrise[k]=couleurDejaPrise[k+1];

                            couleurDejaPrise[k+1]=stockage1;
                        }
                    }
                }

                //Affectation de la couleur probable par défaut
                couleurProbable = 1;

                //SI il y a des couleurs déjà prises, on ne peut pas simplement affecter 1
                if(int(couleurDejaPrise.size()) > 1)
                {
                    //On parcourt le tableau trié de couleurs déjà référencées
                    for(int j=0 ; j<int(couleurDejaPrise.size())-1 ; j++)
                    {
                        //SI on trouve une valeur inférieure
                        if(couleurDejaPrise[j] > couleurDejaPrise[j+1]+1)
                        {
                            //On l'affecte
                            couleurProbable = couleurDejaPrise[j+1]+1;
                        }
                        //SINON
                        else
                        {
                            bool indicBon = true; //Permet d'indiquer que la couleur n'existe pas déjà dans le tableau

                            //Boucle de parcours des couleurs
                            for(int m=0 ; m<int(couleurDejaPrise.size()) ; m++)
                            {
                                //SI la couleur est déjà référencée
                                if(m != j && couleurDejaPrise[m] == couleurDejaPrise[j]+1)
                                {
                                    //On indique que la situation n'est pas bonne
                                    indicBon = false;
                                }
                            }

                            //SI la couleur n'est pas déjà référencée
                            if(indicBon == true)
                            {
                                //On l'affecte
                                couleurProbable = couleurDejaPrise[j] + 1;
                            }
                        }
                    }

                    bool indicVaut1 = true; //Permet d'indiquer si la couleur vaut, en réalité, 1

                    //Boucle permettant de savoir si la couleur vaut, en réalité, 1
                    for(int j=0 ; j<int(couleurDejaPrise.size()) ; j++)
                    {
                        //SI 1 est déjà pris
                        if(couleurDejaPrise[j] == 1)
                        {
                            //On l'indique
                            indicVaut1 = false;
                        }
                    }

                    //SI 1 n'est pas déjà pris
                    if(indicVaut1 == true)
                    {
                        //On l'affecte
                        couleurProbable = 1;
                    }
                }
                //SINON SI il y a une seule couleur déjà prise
                else if(int(couleurDejaPrise.size()) == 1)
                {
                    //SI il s'agit d'un 1
                    if(couleurDejaPrise[0] == 1)
                    {
                        //On affecte 2
                        couleurProbable = 2;
                    }
                    //SINON
                    else
                    {
                        //On affecte 1
                        couleurProbable = 1;
                    }
                }
                //SINON SI il n'y a pas de couleurs déjà prises
                else if(int(couleurDejaPrise.size()) == 0)
                {
                    //On affecte 1
                    couleurProbable = 1;
                }

                //On ajoute la couleur au tableau de coloration
                tabColoration[i] = couleurProbable;
            }
        }

        //Boucle permettant de vérifier si la coloration est terminée
        for(int i=0 ; i<int(tabColoration.size()) ; i++)
        {
            //SI la coloration n'est pas terminée
            if(tabColoration[i] == 0)
            {
                //On l'indique
                finColoration = false;
            }
        }
    }
    while(finColoration == false); //Tant que la coloration n'est pas terminée

    /*

    for(int i=0 ; i<int(tabIndicesAeroport.size()) ; i++)
    {
        cout << "Indice : " << tabIndicesAeroport[i] << endl;
        cout << "   Coloration : " << tabColoration[i] << endl;
        cout << endl;
    }
*/
    int compteurCouleur=0;
    int plageAltitude;
    int espacement;


    for(int i = 0 ; i < int(tabColoration.size());i++)
    {
        if(tabColoration[i] > compteurCouleur)
        {
            compteurCouleur = tabColoration[i];
        }
    }

    //Pour les vols courts courries
    plageAltitude = 3000;

    espacement = plageAltitude/compteurCouleur;


    for(int i = 1; i <= compteurCouleur ; i ++)
    {
        for(int j = 0 ; j < int(tabColoration.size()) ; j ++)
        {
            if(tabColoration[j] == i)
            {
                m_aeroports[tabIndicesAeroport[j]].setAltitudeAvions(5000+espacement*i) ;
                m_aeroports[tabIndicesAeroport[j]].setAltitudeAvions(8000+espacement*i) ;
                m_aeroports[tabIndicesAeroport[j]].setAltitudeAvions(11000+espacement*i) ;
            }
        }
    }

    for(int i = 0 ; i < int(m_aeroports.size());i++)
    {
        cout << m_aeroports[i].get_nom() << endl;

        for(int j = 0 ; j < 3 ; j++)
        {
            cout<< m_aeroports[i].getAltitudesAvions(j) <<endl;
        }

        cout << endl;
    }
}

void Simulateur::rechercheIntemperie(Ressources& motherShip)
{
    int indicCoteEntree = 0;
    //int indicCoteSortie = 0;

    /*
    A        B
    D        C
    */

    Coord A;
    Coord B;
    Coord C;
    Coord D;

    Coord ptIntersectionEntree;
    Coord ptIntersectionSortie;

    //Parcours des intempéries
    for(int i = 0 ; i < int (m_listeIntemperies.size()) ; i++)
    {
        //Mise en place descoordonnées du rectangle
        A.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX());
        A.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY());

        B.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX()+motherShip.getBIT(25)->w/7);
        B.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY());

        C.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX()+motherShip.getBIT(25)->w/7);
        C.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY()+motherShip.getBIT(25)->h/7);

        D.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX());
        D.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY()+motherShip.getBIT(25)->h/7);

        //Parcours de l'ensemble des routes afin de tester si l'une d'elles est touchée par l'intempérie
        for(int j = 0 ; j < int(m_ensembleRoutes.size()) ; j++)
        {
            indicCoteEntree = 0;
            //indicCoteSortie = 0;
            ptIntersectionEntree.set_coord_x(0);
            ptIntersectionEntree.set_coord_y(0);
            ptIntersectionSortie.set_coord_x(0);
            ptIntersectionSortie.set_coord_y(0);

            if((m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x() < m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x() && A.get_coord_x() >= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x() - motherShip.getBIT(25)->w/7 && A.get_coord_x() <= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x())
                    || (m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x() < m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x() && A.get_coord_x() >= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x() - motherShip.getBIT(25)->w/7 && A.get_coord_x() <= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x()))
            {
                if((m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y() < m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y() && A.get_coord_y() >= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y() - motherShip.getBIT(25)->h/7 && A.get_coord_y() <= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y())
                        || (m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y() < m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y() && A.get_coord_y() >= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y() - motherShip.getBIT(25)->h/7 && A.get_coord_y() <= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y()))
                {
                    //Entree dans coté A-D
                    if(m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute() <= D.get_coord_y() && m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute() > A.get_coord_y() )
                    {
                        if(indicCoteEntree == 0)
                        {
                            indicCoteEntree = 1;

                            ptIntersectionEntree.set_coord_x(A.get_coord_x());
                            ptIntersectionEntree.set_coord_y(m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
                        }
                        else
                        {
                            //indicCoteSortie = 1;

                            ptIntersectionSortie.set_coord_x(A.get_coord_x());
                            ptIntersectionSortie.set_coord_y(m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
                        }
                    }

                    //Entree entre B-C
                    if(m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute() < C.get_coord_y() && m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute()> B.get_coord_y() )
                    {
                        if(indicCoteEntree == 0)
                        {
                            indicCoteEntree = 2;

                            ptIntersectionEntree.set_coord_x(B.get_coord_x());
                            ptIntersectionEntree.set_coord_y(m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
                        }
                        else
                        {
                            //indicCoteSortie = 2;

                            ptIntersectionSortie.set_coord_x(B.get_coord_x());
                            ptIntersectionSortie.set_coord_y(m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
                        }

                    }

                    //Entree dans C-D
                    if((A.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() < C.get_coord_x() && (A.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() > D.get_coord_x())
                    {
                        if(indicCoteEntree == 0)
                        {

                            indicCoteEntree = 3;

                            ptIntersectionEntree.set_coord_y(C.get_coord_y());
                            ptIntersectionEntree.set_coord_x((C.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
                        }
                        else
                        {
                            //indicCoteSortie = 3;

                            ptIntersectionSortie.set_coord_y(C.get_coord_y());
                            ptIntersectionSortie.set_coord_x((C.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
                        }

                    }

                    //Entree dans B-A
                    if((D.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() < B.get_coord_x() && (D.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() > A.get_coord_x())
                    {
                        if(indicCoteEntree == 0)
                        {
                            indicCoteEntree = 4;


                            ptIntersectionEntree.set_coord_y(B.get_coord_y());
                            ptIntersectionEntree.set_coord_x((B.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
                        }
                        else
                        {
                            //indicCoteSortie = 4;

                            ptIntersectionSortie.set_coord_y(B.get_coord_y());
                            ptIntersectionSortie.set_coord_x((B.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
                        }
                    }

                    float xA = ptIntersectionEntree.get_coord_x();
                    float xB = ptIntersectionSortie.get_coord_x();

                    float yA = ptIntersectionEntree.get_coord_y();
                    float yB = ptIntersectionSortie.get_coord_y();


                    float distanceEntreDeuxPointsIntemp = sqrt((xA-xB)*(xA-xB)+(yA-yB)*(yA-yB));

                    float xC = m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x();
                    float yC = m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y();

                    float distanceDepIntemp = sqrt((xA-xC)*(xA-xC)+(yA-yC)*(yA-yC));

                    distanceDepIntemp = distanceDepIntemp*4000/273.61;
                    distanceEntreDeuxPointsIntemp = distanceEntreDeuxPointsIntemp*4000/273.61;

                    if(distanceDepIntemp > 0 && distanceEntreDeuxPointsIntemp > 0)
                    {
                        //On parcourt l'ensemble des avions contenus dans la route
                        for(int m = 0 ; m < int(m_ensembleRoutes[j]->getAvionsPresents().size()) ; m++)
                        {
                            //SI l'avion va dans le bon sens
                            if(m_ensembleRoutes[j]->getAvionsPresents()[m]->getNomAeroportD() == m_ensembleRoutes[j]->getAeroport(0)->get_nom())
                            {
                                //SI l'avion est dans l'intempérie
                                if(m_ensembleRoutes[j]->getAvionsPresents()[m]->getDistanceParcourue() > distanceDepIntemp && m_ensembleRoutes[j]->getAvionsPresents()[m]->getDistanceParcourue() < distanceDepIntemp+distanceEntreDeuxPointsIntemp)//SI la distance parcourue par l'avion est dans l'intempérie
                                {
                                    //SI on a de la pluie
                                    if(m_listeIntemperies[i]->get_type() == 1)
                                    {
                                        cout << "altitude avion : " << m_ensembleRoutes[j]->getAvionsPresents()[m]->get_altitude() << endl;
                                        cout << "altitude pluie : " <<m_listeIntemperies[i]->getAltitudes() << endl;
                                        //SI L'avion est en dessous du nuage
                                        if(m_ensembleRoutes[j]->getAvionsPresents()[m]->get_altitude() < m_listeIntemperies[i]->getAltitudes())
                                        {
                                            cout << "OK"<<endl;
                                            m_ensembleRoutes[j]->getAvionsPresents()[m]->changementVitesse(1);
                                        }
                                    }
                                    //SINON, on a du vent
                                    else
                                    {
                                        //SI L'avion est dans la plage d'altitude du vent
                                        if(m_ensembleRoutes[j]->getAvionsPresents()[m]->get_altitude() == m_listeIntemperies[i]->getAltitudes())
                                        {
                                            m_ensembleRoutes[j]->getAvionsPresents()[m]->changementVitesse(2);
                                        }
                                    }
                                }
                            }
                            //SINON, l'avion va dans le sens inverse
                            else
                            {
                                //SI l'avion est dans l'intempérie
                                if((m_ensembleRoutes[j]->getLongueur() - m_ensembleRoutes[j]->getAvionsPresents()[m]->getDistanceParcourue()) > distanceDepIntemp && (m_ensembleRoutes[j]->getLongueur() - m_ensembleRoutes[j]->getAvionsPresents()[m]->getDistanceParcourue()) < distanceDepIntemp+distanceEntreDeuxPointsIntemp)//SI la distance parcourue par l'avion est dans l'intempérie
                                {
                                    //SI on a de la pluie
                                    if(m_listeIntemperies[i]->get_type() == 1)
                                    {
                                        cout << "altitude avion : " << m_ensembleRoutes[j]->getAvionsPresents()[m]->get_altitude() << endl;
                                        cout << "altitude pluie : " <<m_listeIntemperies[i]->getAltitudes() << endl;
                                        //SI L'avion est en dessous du nuage
                                        if(m_ensembleRoutes[j]->getAvionsPresents()[m]->get_altitude() < m_listeIntemperies[i]->getAltitudes())
                                        {
                                            cout << "OK"<<endl;
                                            m_ensembleRoutes[j]->getAvionsPresents()[m]->changementVitesse(1);
                                        }
                                    }
                                    //SI on a du vent
                                    else
                                    {
                                        //SI L'avion est dans la plage d'altitude du vent
                                        if(m_ensembleRoutes[j]->getAvionsPresents()[m]->get_altitude() == m_listeIntemperies[i]->getAltitudes())
                                        {
                                            m_ensembleRoutes[j]->getAvionsPresents()[m]->changementVitesse(2);
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if(distanceDepIntemp > 0 && distanceEntreDeuxPointsIntemp > 0)
                    {
                        //On adapte les valeurs de départ et de longueur de l'intempérie
                        distanceDepIntemp = int(distanceDepIntemp - (int(distanceDepIntemp)%100));
                        distanceEntreDeuxPointsIntemp = int(distanceEntreDeuxPointsIntemp - (int(distanceEntreDeuxPointsIntemp)%100));



                        //On parcourt le tableau de cases de la route
                        for(int k=0 ; k<int(m_ensembleRoutes[j]->getTabEtats().size()) ; k++)
                        {
                            for(int m=0 ; m<12 ; m++)
                            {
                                //SI la case n'a pas encore de coloration
                                if(m_ensembleRoutes[j]->getTabEtats()[k][m] == 0)
                                {
                                    //SI la case est dans la plage de valeur de l'intempérie
                                    cout <<  "altitude pluie :: " << m_listeIntemperies[i]->getAltitudes()<<endl;
                                    cout << " valeur :  " <<  5750+m*750 <<endl;
                                    if(k*100 >= distanceDepIntemp && k*100 <= distanceDepIntemp + distanceEntreDeuxPointsIntemp && 5750+m*750 == m_listeIntemperies[i]->getAltitudes())
                                    {
                                        //SI il s'agit du vent
                                        if(m_listeIntemperies[i]->get_type() == 2)
                                        {
                                            //On remplit la case
                                            m_ensembleRoutes[j]->actualiserCase(k, m, 2);
                                        }
                                        //SINON il s'agit de pluie
                                        else
                                        {
                                            //On applique l'intempérie à toutes les cases en dessous
                                            for(int n=m ; n<12 ; n++)
                                            {
                                                m_ensembleRoutes[j]->actualiserCase(k, n, 1);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
        }
    }
}


//void Simulateur::rechercheIntemperie(Ressources& motherShip)
//{
//
//    int indicCoteEntree = 0;
//    int indicCoteSortie = 0;
//
//    Coord A;
//    Coord B;
//    Coord C;
//    Coord D;
//
//    Coord ptIntersectionEntree;
//    Coord ptIntersectionSortie;
//
//    if(int(m_listeIntemperies.size()) > 0)
//    {
//        //Boucle de réinitialisation de l'ensemble des cases de la route
//        for(int i=0 ; i<int(m_ensembleRoutes.size()) ; i++)
//        {
//            for(int j=0 ; j<int(m_ensembleRoutes[i]->getTabEtats().size()) ; j++)
//            {
//                for(int k=0 ; k<12 ; k++)
//                {
//                    m_ensembleRoutes[i]->actualiserCase(j, k, 0);
//                }
//            }
//        }
//    }
//
//    //Parcours des intempéries
//    for(int i = 0 ; i < int (m_listeIntemperies.size()) ; i++)
//    {
//        A.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX());
//        A.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY());
//
//        B.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX()+motherShip.getBIT(25)->w/7);
//        B.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY());
//
//        C.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX()+motherShip.getBIT(25)->w/7);
//        C.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY()+motherShip.getBIT(25)->h/7);
//
//        D.set_coord_x(m_listeIntemperies[i]->get_coordIntemperieX());
//        D.set_coord_y(m_listeIntemperies[i]->get_coordIntemperieY()+motherShip.getBIT(25)->h/7);
//
//        //Parcours de l'ensemble des routes afin de tester si l'une d'elles est touchée par l'intempérie
//        for(int j = 0 ; j < int(m_ensembleRoutes.size()) ; j++)
//        {
//
//            indicCoteEntree = 0;
//            indicCoteSortie = 0;
//            ptIntersectionEntree.set_coord_x(0);
//            ptIntersectionEntree.set_coord_y(0);
//            ptIntersectionSortie.set_coord_x(0);
//            ptIntersectionSortie.set_coord_y(0);
//
//            if((m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x() < m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x() && A.get_coord_x() >= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x() - motherShip.getBIT(25)->w/7 && A.get_coord_x() <= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x())
//                    || (m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x() < m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x() && A.get_coord_x() >= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_x() - motherShip.getBIT(25)->w/7 && A.get_coord_x() <= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x()))
//            {
//                if((m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y() < m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y() && A.get_coord_y() >= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y() - motherShip.getBIT(25)->h/7 && A.get_coord_y() <= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y())
//                        || (m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y() < m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y() && A.get_coord_y() >= m_ensembleRoutes[j]->getAeroport(1)->get_position().get_coord_y() - motherShip.getBIT(25)->h/7 && A.get_coord_y() <= m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y()))
//                {
//                    //Entree dans coté A-D
//                    if(m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute() <= D.get_coord_y() && m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute() > A.get_coord_y() )
//                    {
//                        if(indicCoteEntree == 0)
//                        {
//                            indicCoteEntree = 1;
//
//                            ptIntersectionEntree.set_coord_x(A.get_coord_x());
//                            ptIntersectionEntree.set_coord_y(m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
//                        }
//                        else
//                        {
//                            indicCoteSortie = 1;
//
//                            ptIntersectionSortie.set_coord_x(A.get_coord_x());
//                            ptIntersectionSortie.set_coord_y(m_ensembleRoutes[j]->getARoute()*A.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
//                        }
//
//                    }
//
//                    //Entree entre B-C
//                    if(m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute() < C.get_coord_y() && m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute()> B.get_coord_y() )
//                    {
//                        if(indicCoteEntree == 0)
//                        {
//                            indicCoteEntree = 2;
//
//                            ptIntersectionEntree.set_coord_x(B.get_coord_x());
//                            ptIntersectionEntree.set_coord_y(m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
//                        }
//                        else
//                        {
//                            indicCoteSortie = 2;
//
//                            ptIntersectionSortie.set_coord_x(B.get_coord_x());
//                            ptIntersectionSortie.set_coord_y(m_ensembleRoutes[j]->getARoute()*B.get_coord_x()+m_ensembleRoutes[j]->getBRoute());
//                        }
//
//                    }
//
//                    //Entree dans C-D
//                    if((A.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() < C.get_coord_x() && (A.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() > D.get_coord_x())
//                    {
//                        if(indicCoteEntree == 0)
//                        {
//
//                            indicCoteEntree = 3;
//
//                            ptIntersectionEntree.set_coord_y(C.get_coord_y());
//                            ptIntersectionEntree.set_coord_x((C.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
//                        }
//                        else
//                        {
//                            indicCoteSortie = 3;
//
//                            ptIntersectionSortie.set_coord_y(C.get_coord_y());
//                            ptIntersectionSortie.set_coord_x((C.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
//                        }
//
//                    }
//
//                    //Entree dans B-A
//                    if((D.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() < B.get_coord_x() && (D.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute() > A.get_coord_x())
//                    {
//                        if(indicCoteEntree == 0)
//                        {
//                            indicCoteEntree = 4;
//
//
//                            ptIntersectionEntree.set_coord_y(B.get_coord_y());
//                            ptIntersectionEntree.set_coord_x((B.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
//                        }
//                        else
//                        {
//                            indicCoteSortie = 4;
//
//                            ptIntersectionSortie.set_coord_y(B.get_coord_y());
//                            ptIntersectionSortie.set_coord_x((B.get_coord_y()-m_ensembleRoutes[j]->getBRoute())/m_ensembleRoutes[j]->getARoute());
//                        }
//                    }
//
//                    //cout << m_ensembleRoutes[j]->getAeroport(0)->get_nom() << " a " << m_ensembleRoutes[j]->getAeroport(1)->get_nom() << endl;
//                    //cout << indicCoteEntree << " et " << indicCoteSortie << endl << endl;
//
//
//                    float xA = ptIntersectionEntree.get_coord_x();
//                    float xB = ptIntersectionSortie.get_coord_x();
//
//                    float yA = ptIntersectionEntree.get_coord_y();
//                    float yB = ptIntersectionSortie.get_coord_y();
//
//
//                    float distanceEntreDeuxPointsIntemp = sqrt((xA-xB)*(xA-xB)+(yA-yB)*(yA-yB));
//                    //cout << distanceEntreDeuxPointsIntemp << endl;
//
//                    float xC = m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_x();
//                    float yC = m_ensembleRoutes[j]->getAeroport(0)->get_position().get_coord_y();
//
//                    float distanceDepIntemp = sqrt((xA-xC)*(xA-xC)+(yA-yC)*(yA-yC));
//
//                    distanceDepIntemp = distanceDepIntemp*4000/273.61;
//                    distanceEntreDeuxPointsIntemp = distanceEntreDeuxPointsIntemp*4000/273.61;
//
//                    //cout << "Distance dep - intemp " << distanceDepIntemp << "km" << endl;
//                    //cout << "Distance intemp - intemp " << distanceEntreDeuxPointsIntemp << "km"<< endl;
//
//
//
//
//                    // PROGRAMME VICTOIRE
//
//
//
//
//                    if(distanceDepIntemp > 0 && distanceEntreDeuxPointsIntemp > 0)
//                    {
//                        //On adapte les valeurs de départ et de longueur de l'intempérie
//                        distanceDepIntemp = int(distanceDepIntemp - (int(distanceDepIntemp)%100));
//                        distanceEntreDeuxPointsIntemp = int(distanceEntreDeuxPointsIntemp - (int(distanceEntreDeuxPointsIntemp)%100));
//
//
//
//                        //On parcourt le tableau de cases de la route
//                        for(int k=0 ; k<int(m_ensembleRoutes[j]->getTabEtats().size()) ; k++)
//                        {
//                            for(int m=0 ; m<12 ; m++)
//                            {
//                                //SI la case n'a pas encore de coloration
//                                if(m_ensembleRoutes[j]->getTabEtats()[k][m] == 0)
//                                {
//                                    //SI la case est dans la plage de valeur de l'intempérie
//                                    if(k*100 >= distanceDepIntemp && k*100 <= distanceDepIntemp + distanceEntreDeuxPointsIntemp && 5750+m*750 == m_listeIntemperies[i]->getAltitudes())
//                                    {
//                                        //SI il s'agit du vent
//                                        if(m_listeIntemperies[i]->get_type() == 2)
//                                        {
//                                            //On remplit la case
//                                            m_ensembleRoutes[j]->actualiserCase(k, m, 2);
//                                        }
//                                        //SINON il s'agit de pluie
//                                        else
//                                        {
//                                            //On applique l'intempérie à toutes les cases en dessous
//                                            for(int n=m ; n<12 ; n++)
//                                            {
//                                                m_ensembleRoutes[j]->actualiserCase(k, n, 1);
//                                            }
//                                        }
//                                    }
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//}

//Méthode d'affichage du quadrillage de la map
void Simulateur::affichageQuadrillage(Ressources &motherShip)
{
    //Boucle d'affichage des colonnes
    for(int i = 0 ; i < 1500 ; i+=8)
    {
        rect(motherShip.getBIT(0), i,0,i+8,750,makecol(0,0,0));
    }

    //Boucle d'affichage des lignes
    for(int i = 0 ; i < 750 ; i+=8)
    {
        rect(motherShip.getBIT(0), 0,i,1500,i+8,makecol(0,0,0));
    }
}
