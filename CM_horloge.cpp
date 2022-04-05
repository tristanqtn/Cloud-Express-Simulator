#include "CH_horloge.h"

using namespace std;



    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur par d�faut de l'horloge
Horloge::Horloge()
{
    //ouverture du fichier
    ifstream fichier_horloge{"data/horloge/horloge.txt"};
    if (!fichier_horloge)
        throw runtime_error( "Impossible d'ouvrir en lecture data/horloge/horloge.txt"); //erreur de lecture

    //lecture de l'ann�e
    fichier_horloge >> m_annee;
    if ( fichier_horloge.fail() )
        throw runtime_error("Probleme lecture de l'annee"); //erreur de lecture

    //lecture du mois
    fichier_horloge >> m_mois;
    if ( fichier_horloge.fail() )
        throw runtime_error("Probleme lecture du mois"); //erreur de lecture

    //lecture du jour
    fichier_horloge >> m_jour;
    if ( fichier_horloge.fail() )
        throw runtime_error("Probleme lecture du jour"); //erreur de lecture

    //lecture de l'heure
    fichier_horloge >> m_heure.first >> m_heure.second;
    if ( fichier_horloge.fail() )
        throw runtime_error("Probleme lecture de l'heure"); //erreur de lecture
    fichier_horloge.close();
}

//Destructeur d'horloge
Horloge::~Horloge()
{

}



    ////////////////
    // ACCESSEURS //
    ////////////////

//Getter de l'ann�e
int Horloge::get_annee()
{
    return m_annee;
}

//Getter du mois
int Horloge::get_mois()
{
    return m_mois;
}

//Getter du jour
int Horloge::get_jour()
{
    return m_jour;
}

//Getter de l'heure
pair <int, int> Horloge::get_heure()
{
    return m_heure;
}



    //////////////
    // METHODES //
    //////////////

//M�thode d'affichage de l'heure fictive
void Horloge::afficher_heure_date(BITMAP* doubleBuffer, FONT* policeHeure, FONT* policeDate)
{
    string chaineCarHeure = ""; //Cha�ne de caract�res contenant l'heure GMT
    string chaineCarDate = ""; //Cha�ne de caract�res contenant la date GMT

    //Cr�ation de la cha�ne correspondant � l'heure
    if(m_heure.first < 10) //SI heure < 10 alors on rajoute un 0
    {
        chaineCarHeure += "0";
    }
    chaineCarHeure += to_string(m_heure.first); //Ajout de l'heure
    chaineCarHeure += ":";
    if(m_heure.second < 10) //SI minute < 10, alors on rajoute un 0
    {
        chaineCarHeure += "0";
    }
    chaineCarHeure += to_string(m_heure.second); //Ajout des minutes

    //Cr�ation de la cha�ne correspondant � la date
    if(m_jour < 10) //SI le jour < 10, on rajoute un 0
    {
        chaineCarDate += "0";
    }
    chaineCarDate += to_string(m_jour); //Ajout du jour
    chaineCarDate += "/";
    if(m_mois < 10)
    {
        chaineCarDate += "0";
    }
    chaineCarDate += to_string(m_mois); //Ajout du mois
    chaineCarDate += "/";
    if(m_annee < 10)
    {
        chaineCarDate += "0";
    }
    chaineCarDate += to_string(m_annee); //Ajout de l'ann�e

    //Mise en page Allegro
    rectfill(doubleBuffer, SCREEN_W/2-200-160, SCREEN_H-90, SCREEN_W/2+200-160, SCREEN_H, makecol(41, 49, 51));
    textprintf_ex(doubleBuffer, policeHeure, SCREEN_W/2-200-150, SCREEN_H-80, makecol(25, 255, 52), -1, convertisseurStringChar(chaineCarHeure));
    textprintf_ex(doubleBuffer, policeDate, SCREEN_W/2-200+15, SCREEN_H-58, makecol(255, 0, 0), -1, convertisseurStringChar(chaineCarDate));
}


//M�thode qui actualise l'heure apr�s un tour de boucle
void Horloge::actualiser_heure()
{
    //Ajout d'une minute
    m_heure.second += 10;

    //SI 1 heure
    if(m_heure.second == 60)
    {
        m_heure.second = 0; //R�initialisation de minute
        m_heure.first++; ////Incr�mentation de l'heure

        //SI 24 heures
        if(m_heure.first == 24)
        {
            m_heure.first = 0; //R�initialisation de l'heure
            m_jour++; //Incr�mentation du jour

            //SI mois de f�vrier termin� et ann�e non bissextile ou bissextile
            if((m_jour == 28 && m_mois == 2 && m_annee%4 != 0) || (m_jour == 29 && m_mois == 2 && m_annee%4 == 0))
            {
                m_jour = 0; //R�initialisation du jour
                m_mois++; //Incr�mentation des mois

                //SI ann�e finie
                if(m_mois == 12)
                {
                    m_mois = 0; //R�initialisation des mois
                    m_annee++; //Incr�mentation de l'ann�e
                }
            }

            //SINON SI mois de 30 jours termin�
            else if(m_jour == 30 && (m_mois == 4 || m_mois == 6 || m_mois == 9 || m_mois == 11))
            {
                m_jour = 0; //R�initialisation du jour
                m_mois++; //Incr�mentation des mois

                //SI ann�e finie
                if(m_mois == 12)
                {
                    m_mois = 0; //R�initialisation des mois
                    m_annee++; //Incr�mentation de l'ann�e
                }
            }

            //SINON SI mois de 31 jours termin�
            else if(m_jour == 31 && (m_mois == 1 || m_mois == 3 || m_mois == 5 || m_mois == 7 || m_mois == 8 || m_mois == 10 || m_mois == 12))
            {
                m_jour = 0; //R�initialisation du jour
                m_mois++; //Incr�mentation des mois

                //SI ann�e finie
                if(m_mois == 12)
                {
                    m_mois = 0; //R�initialisation des mois
                    m_annee++; //Incr�mentation de l'ann�e
                }
            }
        }
    }
}


//M�thode de sauvegarde de l'heure dans le fichier d�di�
void Horloge::sauvegarder_heure()
{
    ofstream sauvegarde_horloge;

    cout << " ----- SAUVEGARDE DE L'HORLOGE ----- " << endl;

    //Ouverture du fichier d�di� � l'heure
    cout << "     ----- Ouverture du fichier de l'horloge" << endl;
    sauvegarde_horloge.open("data/horloge/horloge.txt");
    if (sauvegarde_horloge.fail() )
        throw runtime_error("Probleme ouverture en ecriture de data/horloge/horloge.txt"); //Erreur de lecture

    //Sauvegarde de l'ensemble des informations de l'horloge
    cout << "     ----- Sauvegarde des informations" << endl;
    sauvegarde_horloge << m_annee << "\n";
    sauvegarde_horloge << m_mois << "\n";
    sauvegarde_horloge << m_jour << "\n";
    sauvegarde_horloge << m_heure.first << "\n";
    sauvegarde_horloge << m_heure.second << "\n";

    //Fermeture du fichier
    cout << "     ----- Fermeture du fichier" << endl;
    sauvegarde_horloge.close();

    cout << " ----- SAUVEGARDE TERMINEE ----- " << endl << endl;
}
