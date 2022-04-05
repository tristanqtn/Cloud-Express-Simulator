#include "CH_simulateur.h"

using namespace std;


    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////


//Constructeur par d�faut
Simulateur::Simulateur()
    : m_modeSimulation(-1), m_envergureSimulation(-1)
{

}


//Destructeur
Simulateur::~Simulateur()
{

}



    //////////////
    // METHODES //
    //////////////

//M�thode d'initialisation du simulateur
void Simulateur::initSimulateur(int modeSimulation, int envergureSimulation)
{
    //Initialisation du mode et de l'envergure de la simulation
    m_modeSimulation = modeSimulation;
    m_envergureSimulation = envergureSimulation;

    //Processus de r�initialisation de l'ensemble des vecteurs du simulateur en cas de relance de la simulation sans fermeture du programme
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

    //Chargement des modeles d'avions
    Avion court_courrier ("data/avion/court.txt");
    m_infos_types_avions.push_back(court_courrier);
    Avion moyen_courrier ("data/avion/moyen.txt");
    m_infos_types_avions.push_back(moyen_courrier);
    Avion long_courrier ("data/avion/long.txt");
    m_infos_types_avions.push_back(long_courrier);

    //Cr�ation d'une horloge
    Horloge new_date;

    m_horlogeGMT = new_date;
}


//M�thode de r�initialisation du simulateur
void Simulateur::reinitialisationSimulateur()
{
    //R�initialisation du vecteur d'a�roports
    while(int(m_aeroports.size()) > 0)
    {
        m_aeroports.pop_back();
    }

    //R�initialisation du vecteur de la flotte d'avions
    while(int(m_flotte_avions.size()) > 0)
    {
        m_flotte_avions.pop_back();
    }

    //R�initialisation du vecteur contenant les informations des diff�rents types d'avion
    while(int(m_infos_types_avions.size()) > 0)
    {
        m_infos_types_avions.pop_back();
    }
}


//M�thode d'affichage de l'ensemble des a�roports
void Simulateur::afficher_aeroports()
{
    for(size_t t=0; t<m_aeroports.size(); t++)
    {
        m_aeroports[t].afficher_caracteristique(m_envergureSimulation);
    }
}


//Cr�ation al�atoire d'un avion
Avion Simulateur::creer_avion_aleatoire()
{
    int choix_aleatoire = rand()%3; //G�n�ration al�atoire d'un entier entre 0 et 2 pour type de vol
    int modele_aleatoire = rand()%3; //G�n�ration al�toire d'un entier entre 0 et 2 pour mod�le

    bool autorisation = true; //Bool�en d'autorisation pour l'immatriculation

    Avion nouvel_avion(m_infos_types_avions[choix_aleatoire]); //Construction par copie d'un nouvelle avion

    //Choix du mod�le al�atoirement
    if(nouvel_avion.get_type_vol() == "court")
    {//court courrir
        if(modele_aleatoire == 0)
        {//choix al�atoire selon la valeur de modele_aleatoire
           nouvel_avion.set_modele("BOMBARDIER CRJ 200");
        }
        else if(modele_aleatoire == 1)
        {//choix al�atoire selon la valeur de modele_aleatoire
            nouvel_avion.set_modele("AIRBUS A319");
        }
        else if(modele_aleatoire == 2)
        {//choix al�atoire selon la valeur de modele_aleatoire
            nouvel_avion.set_modele("BOEING 737");
        }
    }
    else if(nouvel_avion.get_type_vol() == "moyen")
    {//moyen courrier
        if(modele_aleatoire == 0)
        {//choix al�atoire selon la valeur de modele_aleatoire
           nouvel_avion.set_modele("AIRBUS A220");
        }
        else if(modele_aleatoire == 1)
        {//choix al�atoire selon la valeur de modele_aleatoire
            nouvel_avion.set_modele("BOEING 747");
        }
        else if(modele_aleatoire == 2)
        {//choix al�atoire selon la valeur de modele_aleatoire
            nouvel_avion.set_modele("BOEING 757");
        }
    }
    else if (nouvel_avion.get_type_vol() == "long")
    {//long courrier
        if(modele_aleatoire == 0)
        {//choix al�atoire selon la valeur de modele_aleatoire
           nouvel_avion.set_modele("AIRBUS A830");
        }
        else if(modele_aleatoire == 1)
        {//choix al�atoire selon la valeur de modele_aleatoire
            nouvel_avion.set_modele("AIRBUS A850");
        }
        else if(modele_aleatoire == 2)
        {//choix al�atoire selon la valeur de modele_aleatoire
            nouvel_avion.set_modele("BOIENG 777");
        }
    }

    //G�n�ration al�atoire de l'immatriculation
    do
    {
        nouvel_avion.set_immatriculation(chaine_aleatoire(5)); //G�n�ration de l'immatriculation

        for(size_t t=0; t<m_flotte_avions.size(); t++)
        {//Parcours de la liste d'avion d�j� cr��s
            if(nouvel_avion.get_immatriculation() == m_flotte_avions[t].get_immatriculation())
            {//Meme immatriculation
                autorisation = false; //Autorisation fausse => re-g�n�rer une immatriculation
            }
        }
    }
    while(autorisation == false); //G�n�ration blind�e

    m_flotte_avions.push_back(nouvel_avion); //Ajout du nouvel avion au vecteur d'avions en services
    return nouvel_avion; //Rendu avion
}


//M�thode qui permet le chargement de la carte NORMALE
void Simulateur::charger_carte_NORMALE()
{
    int nombre_aeroport;

    //Ouverture du fichier
    ifstream fichier_nombre_aeroport{"data/aeroport/carte_1/NB.txt"};
    if (!fichier_nombre_aeroport)
        throw runtime_error( "Impossible d'ouvrir en lecture data/aeroport/carte_1/NB.txt"); //Erreur de lecture

    //Lecture du nom de l'a�roport
    fichier_nombre_aeroport >> nombre_aeroport;
    if (fichier_nombre_aeroport.fail() )
        throw runtime_error("Probleme lecture nombre aeroport"); //Erreur de lecture

    //Chargement de la carte de base
    Aeroport JFK ("data/aeroport/carte_1/JFK.txt", nombre_aeroport);
    m_aeroports.push_back(JFK);
    Aeroport PER ("data/aeroport/carte_1/PER.txt", nombre_aeroport);
    m_aeroports.push_back(PER);
    Aeroport CPT ("data/aeroport/carte_1/CPT.txt", nombre_aeroport);
    m_aeroports.push_back(CPT);
    Aeroport LPB ("data/aeroport/carte_1/LPB.txt", nombre_aeroport);
    m_aeroports.push_back(LPB);
    Aeroport CDG ("data/aeroport/carte_1/CDG.txt", nombre_aeroport);
    m_aeroports.push_back(CDG);
    Aeroport BKK ("data/aeroport/carte_1/BKK.txt", nombre_aeroport);
    m_aeroports.push_back(BKK);
    Aeroport PAP ("data/aeroport/carte_1/PAP.txt", nombre_aeroport);
    m_aeroports.push_back(PAP);
}


//M�thode qui permet le chargement de la carte EXTREME
void Simulateur::charger_carte_EXTREME()
{
    int nombre_aeroport;

    //Ouverture du fichier
    ifstream fichier_nombre_aeroport{"data/aeroport/carte_2/NB.txt"};
    if (!fichier_nombre_aeroport)
        throw runtime_error( "Impossible d'ouvrir en lecture data/aeroport/carte_2/NB.txt"); //Erreur de lecture

    //Lecture du nom de l'a�roport
    fichier_nombre_aeroport >> nombre_aeroport;
    if (fichier_nombre_aeroport.fail() )
        throw runtime_error("Probleme lecture nombre aeroport"); //Erreur de lecture

    //Chargement de la carte extreme
    Aeroport BKK ("data/aeroport/carte_2/BKK.txt", nombre_aeroport);
    m_aeroports.push_back(BKK);
    Aeroport JFK ("data/aeroport/carte_2/JFK.txt", nombre_aeroport);
    m_aeroports.push_back(JFK);
    Aeroport CDG ("data/aeroport/carte_2/CDG.txt", nombre_aeroport);
    m_aeroports.push_back(CDG);
    Aeroport WLG ("data/aeroport/carte_2/WLG.txt", nombre_aeroport);
    m_aeroports.push_back(WLG);
    Aeroport YUL ("data/aeroport/carte_2/YUL.txt", nombre_aeroport);
    m_aeroports.push_back(YUL);
    Aeroport MEX ("data/aeroport/carte_2/MEX.txt", nombre_aeroport);
    m_aeroports.push_back(MEX);
    Aeroport HND ("data/aeroport/carte_2/HND.txt", nombre_aeroport);
    m_aeroports.push_back(HND);
    Aeroport JNB ("data/aeroport/carte_2/JNB.txt", nombre_aeroport);
    m_aeroports.push_back(JNB);
    Aeroport HNL ("data/aeroport/carte_2/HNL.txt", nombre_aeroport);
    m_aeroports.push_back(HNL);
    Aeroport SCL ("data/aeroport/carte_2/SCL.txt", nombre_aeroport);
    m_aeroports.push_back(SCL);
    Aeroport STO ("data/aeroport/carte_2/STO.txt", nombre_aeroport);
    m_aeroports.push_back(STO);
    Aeroport DFA ("data/aeroport/carte_2/DFA.txt", nombre_aeroport);
    m_aeroports.push_back(DFA);
    Aeroport DEL ("data/aeroport/carte_2/DEL.txt", nombre_aeroport);
    m_aeroports.push_back(DEL);
    Aeroport UAK ("data/aeroport/carte_2/UAK.txt", nombre_aeroport);
    m_aeroports.push_back(UAK);
    Aeroport MOW ("data/aeroport/carte_2/MOW.txt", nombre_aeroport);
    m_aeroports.push_back(MOW);
}


//M�thode de d�roulement g�n�ral de la simulation
void Simulateur::deroulementGlobal(Ressources &motherShip, bool &indicClic, bool &done)
{
    bool finDeroulement = false;
    bool indicEchap = false;

    //Processus d'initialisation des cartes affich�es en fond
    initCartesFond(motherShip);

    //Boucle de d�roulement g�n�ral de la partie
    while(!finDeroulement)
    {
        //R�initialisation du double buffer
        clear_bitmap(motherShip.getBIT(0));

        //cout << mouse_x << " et " << mouse_y << endl;

        //Affichage du mode JOUR ou NUIT
        if(m_horlogeGMT.get_heure().first > 7 && m_horlogeGMT.get_heure().first < 20) //SI il s'agit du JOUR
        {
            blit(motherShip.getBIT(6), motherShip.getBIT(0), 0, 0, 0, 0, motherShip.getBIT(6)->w, motherShip.getBIT(6)->h); //Affichage de la map monde JOUR
        }
        else //SINON, il s'agit de la NUIT
        {
            blit(motherShip.getBIT(7), motherShip.getBIT(0), 0, 0, 0, 0, motherShip.getBIT(7)->w, motherShip.getBIT(7)->h); //Affichage de la map monde NUIT
        }

        //Affichage de l'horloge
        m_horlogeGMT.afficher_heure_date(motherShip.getBIT(0), motherShip.getFONT(6), motherShip.getFONT(5));

        //SI l'utilisateur appuie sur ESC, lance le menu de sortie du jeu
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true; //On indique que la touche ESC est press�e
            menuESC(done, finDeroulement, indicClic, indicEchap, motherShip.getBIT(0), motherShip.getBIT(3), motherShip.getFONT(3), motherShip.getFONT(1));
        }

        //SI la touche ESCAPE n'est plus press�e
        if(!key[KEY_ESC])
        {
            indicEchap = false; //On l'indique
        }

        //Affichage du curseur
        affichageCurseur(motherShip.getBIT(3), motherShip.getBIT(0));

        //Affichage du double buffer
        blit(motherShip.getBIT(0), screen, 0, 0, 0, 0, motherShip.getBIT(0)->w, motherShip.getBIT(0)->h);
    }
}


void Simulateur::menuESC(bool &done, bool &finDeroulement, bool &indicClic, bool &indicEchap, BITMAP* doubleBuffer, BITMAP* curseur, FONT* policeTitre, FONT* policeChoix)
{
    bool finMenuESC = false; //Indicateur de fin du menu ESC
    BITMAP* doubleBufferProvisoire = create_bitmap(SCREEN_W, SCREEN_H); //Permet d'avoir le fond du menu
    int ecartBords = 100;

    blit(doubleBuffer, doubleBufferProvisoire, 0, 0, 0, 0, doubleBuffer->w, doubleBuffer->h);

    //Boucle de d�roulement du menu ESC
    while(!finMenuESC)
    {
        //R�initialisation du double buffer
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

        //SI la touche ESC est press�e de nouveau
        if(key[KEY_ESC] && indicEchap == false)
        {
            indicEchap = true;
            finMenuESC = true;
        }

        //SI la touche ESC n'est plus press�e
        if(!key[KEY_ESC])
        {
            indicEchap = false; //On l'indique
        }

        //SI le clic gauche de la souris n'est pas press�
        if(!(mouse_b & 1))
        {
            indicClic = false; //On l'indique
        }
    }
}


//M�thode d'initialisation des cartes mondiales en fonction des a�roports
void Simulateur::initCartesFond(Ressources &motherShip)
{
    //Impression des cartes vierges
    blit(motherShip.getBIT(4), motherShip.getBIT(6), 0, 0, 0, 0, motherShip.getBIT(4)->w, motherShip.getBIT(4)->h);
    blit(motherShip.getBIT(5), motherShip.getBIT(7), 0, 0, 0, 0, motherShip.getBIT(5)->w, motherShip.getBIT(5)->h);

    //Impression de l'ensemble des a�roports
    for(int i=0 ; i<int(m_aeroports.size()) ; i++)
    {
        //cout << "NOM : " << m_aeroports[i].get_nom() << endl;
        //cout << m_aeroports[i].get_position().get_coord_x() << " et " << m_aeroports[i].get_position().get_coord_y() << endl;
        masked_stretch_blit(motherShip.getBIT(8), motherShip.getBIT(6), 0, 0, motherShip.getBIT(8)->w, motherShip.getBIT(8)->h, m_aeroports[i].get_position().get_coord_x() - motherShip.getBIT(8)->w/4, m_aeroports[i].get_position().get_coord_y() - motherShip.getBIT(8)->h/2, motherShip.getBIT(8)->w/2, motherShip.getBIT(8)->h/2);
        masked_stretch_blit(motherShip.getBIT(8), motherShip.getBIT(7), 0, 0, motherShip.getBIT(8)->w, motherShip.getBIT(8)->h, m_aeroports[i].get_position().get_coord_x() - motherShip.getBIT(8)->w/4, m_aeroports[i].get_position().get_coord_y() - motherShip.getBIT(8)->h/2, motherShip.getBIT(8)->w/2, motherShip.getBIT(8)->h/2);
    }
}
