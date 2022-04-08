#include "CH_simulateur.h"

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
    bool pause = false; //Indicateur de pause
    int compteur = 0; //Permet d'actualiser à temps réguliers les avions et l'horloge
    int modeVitesse = 0; //Permet de déterminer la vitesse d'actualisation
    int coorXRectangleDroite = 1165; //Contient la position en X du rectangle de droite
    int etatMenu = 1; //Permet de suivre l'évolution du menu à droite du simulateur

    //Processus d'initialisation des cartes affichées en fond
    initCartesFond(motherShip);

    creer_avion("aleatoire");
    m_flotte_avions[0]->parametrer_nouveau_vol(m_aeroports[getIndiceAeroport("CDG")], m_aeroports[getIndiceAeroport("JFK")]);

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

                //ACTUALISATION DES AEROPORTS
                for(int i=0 ; i<int(m_aeroports.size()) ; i++)
                {
                    m_aeroports[i].actualisationAeroport(m_aeroports, m_ensembleRoutes, i, m_matrice_adjacence);
                }

                //ACTUALISATION DES AVIONS
                for(int i=0 ; i<int(m_flotte_avions.size()) ; i++)
                {
                    m_flotte_avions[i]->actualiser_action_avion();
                }

                //ACTUALISATION DE L'HORLOGE
                m_horlogeGMT.actualiser_heure();
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

        //Affichage de l'horloge
        m_horlogeGMT.afficher_heure_date(motherShip.getBIT(0), motherShip.getFONT(6), motherShip.getFONT(5));

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
            m_aeroports[i].actualisationPointeurLocalisation(motherShip, indicClic, indicEchap);
        }

        //Parcours de l'ensemble des routes aériennes pour voir si la souris est au dessus de l'une d'elles
        for(int i=0 ; i<int(m_ensembleRoutes.size()) ; i++)
        {
            m_ensembleRoutes[i]->actualisationSurbrillanceRoute(motherShip, indicClic, indicEchap);
        }

        //Parcours de l'ensemble des avions afin de les afficher
        for(int i=0 ; i<int(m_flotte_avions.size()) ; i++)
        {
            m_flotte_avions[i]->affichageAvionCarte(motherShip.getBIT(0), motherShip.getBIT(18), motherShip.getBIT(19), motherShip.getBIT(20));
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
            //On l'ajoute dans la liste des aéroports disponibles
            listeAeroportsDispo.push_back(i);
        }
    }

    //On tire aléatoirement un des indices parmi ceux disponibles
    indiceAeroportAleatoire = rand()%(int(listeAeroportsDispo.size()));

    cout << endl << "AEROPORT DE DEPART : " << m_aeroports[indiceAeroportAleatoire].get_nom() << endl << endl;

    //On ajoute l'avion à l'aéroport désigné
    m_aeroports[indiceAeroportAleatoire].ajoutAvionParking(nouvelAvion);
}



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
    {//génération d'un vol moyen

        m_flotte_avions.push_back(new Avion(m_infos_types_avions[1]));

        //Choix du modèle aléatoirement
        if(modele_aleatoire == 0)
        {//choix aléatoire selon la valeur de modele_aleatoire
           m_flotte_avions.back()->set_modele("AIRBUS A220");
        }
        else if(modele_aleatoire == 1)
        {//choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("BOEING 747");
        }
        else if(modele_aleatoire == 2)
        {//choix aléatoire selon la valeur de modele_aleatoire
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
    {//génération d'un vol long

        m_flotte_avions.push_back(new Avion(m_infos_types_avions[2]));

        //Choix du modèle aléatoirement
        if(modele_aleatoire == 0)
        {//choix aléatoire selon la valeur de modele_aleatoire
           m_flotte_avions.back()->set_modele("AIRBUS A830");
        }
        else if(modele_aleatoire == 1)
        {//choix aléatoire selon la valeur de modele_aleatoire
            m_flotte_avions.back()->set_modele("AIRBUS A850");
        }
        else if(modele_aleatoire == 2)
        {//choix aléatoire selon la valeur de modele_aleatoire
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
        //SI la route passe d'un bord de l'écran à l'autre
        if((m_ensembleRoutes[i]->getAeroport(0)->get_nom() == "HNL" && m_ensembleRoutes[i]->getAeroport(1)->get_nom() == "HND")
            || (m_ensembleRoutes[i]->getAeroport(0)->get_nom() == "HND" && m_ensembleRoutes[i]->getAeroport(1)->get_nom() == "HNL")
            || (m_ensembleRoutes[i]->getAeroport(0)->get_nom() == "HNL" && m_ensembleRoutes[i]->getAeroport(1)->get_nom() == "WLG")
            || (m_ensembleRoutes[i]->getAeroport(0)->get_nom() == "WLG" && m_ensembleRoutes[i]->getAeroport(1)->get_nom() == "HNL"))
        {
            //SI l'aéroport de "départ" est à gauche de l'écran et que son arrivée est à droite
            if(m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x() < m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())
            {
                //Routes aériennes de JOUR
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));

                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));

                //Routes aériennes de NUIT
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));

                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
            }

            //SINON SI l'aéaroport de "départ" est à droite de l'écran et que son arrivée est à gauche
            else if(m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x() > m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x())
            {
                //Routes aériennes de JOUR
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));

                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(6), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));

                //Routes aériennes de NUIT
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, coorXRectangleDroite + m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, makecol(0, 0, 0));

                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()+1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())+1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x(), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x()), m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()+1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()+1, makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y(), -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y(), makecol(0, 0, 0));
                line(motherShip.getBIT(7), m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_x()-1, m_ensembleRoutes[i]->getAeroport(1)->get_position().get_coord_y()-1, -(coorXRectangleDroite - m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_x())-1, m_ensembleRoutes[i]->getAeroport(0)->get_position().get_coord_y()-1, makecol(0, 0, 0));

            }
        }

        //SINON la route peut être normalement représentée
        else
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
    {//Colonnes
        for(int j=0; j<m_envergureSimulation; j++)
        {//Lignes
            m_matrice_adjacence[i][j] = 0; //RAZ
        }
    }

    for(int i=0; i<m_envergureSimulation; i++)
    {//Pour chaque aéroport du simulateur
        for(size_t t=0; t<m_aeroports[i].get_distance_aeroports().size(); t++)
        {//Pour chaque aéroports connectés à l'aéroport étudié
            if(m_aeroports[i].get_successeur_precis(t).second != -1)
            {//Si les aéroports sont bel et bien connecté (poid != -1)
                //Stockage du poid dans la colonne de l'aeroport étudié et ligne de l'aéroport connecté
                m_matrice_adjacence[getIndiceAeroport(m_aeroports[i].get_nom())][getIndiceAeroport(m_aeroports[i].get_successeur_precis(t).first)] = m_aeroports[i].get_successeur_precis(t).second;
            }
        }
    }
}



