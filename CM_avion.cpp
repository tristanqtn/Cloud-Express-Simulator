#include "H_header.h"
#include "CH_avion&aeroport.h"
#include <cmath>

using namespace std;

    //////////////////////////////////
    // CONSTRUCTEURS ET DESTRUCTEUR //
    //////////////////////////////////

//Constructeur d'un objet avion en utilisant un des 3 types d'avion
Avion::Avion(string nom_fichier)
{
    //ouverture du fichier
    ifstream fichier_aeroport{nom_fichier};
    if (!fichier_aeroport)
        throw runtime_error( "Impossible d'ouvrir en lecture " + nom_fichier ); //erreur de lecture

    //lecture du type de vol
    fichier_aeroport >> m_type_vol;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture type de vol"); //erreur de lecture

    //lecture de la vitesse
    fichier_aeroport >> m_vitesse;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture vitesse"); //erreur de lecture

    //lecture de la quantite de kerosene
    fichier_aeroport >> m_quantite_kerosene;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture quantite de kerosene"); //erreur de lecture

    //lecture de la consommation
    fichier_aeroport >> m_consommation;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture de la consommation"); //erreur de lecture

    //initialisation des coordonn�es nulle
    m_coord.set_coord_x(0);
    m_coord.set_coord_y(0);

    m_modele = "";
    m_immatriculation = "";
}

//Constructeur par copie
Avion::Avion(const Avion & copie)
{
    m_coord = copie.m_coord;
    m_modele = copie.m_modele;
    m_vitesse = copie.m_vitesse;
    m_type_vol = copie.m_type_vol;
    m_consommation = copie.m_consommation;
    m_immatriculation = copie.m_immatriculation;
    m_quantite_kerosene= copie.m_quantite_kerosene;
    m_quantite_max_kerosene = copie.m_quantite_max_kerosene;
    m_altitude = copie.m_altitude;
    m_duree_roulage = 0;
}

//Constructeur par d�faut pour les pointeurs
Avion::Avion()
{

}

//Destructeur
Avion::~Avion()
{

}



    ////////////////
    // ACCESSEURS //
    ////////////////

//Getter des coordonn�es
Coord Avion::get_coord()
{
    return m_coord;
};

//Getter du mod�le
string Avion::get_modele()
{
    return m_modele;
}

//Getter du type de vol
string Avion::get_type_vol()
{
    return m_type_vol;
}

//Getter de l'immatriculation
string Avion::get_immatriculation()
{
    return m_immatriculation;
}

//Getter de la quantit� de kerosen
int Avion::get_quantite_kerosene()
{
    return m_quantite_kerosene;
}

//Getter de la quantit� maximale de kerosen
int Avion::get_quantite_max_kerosene()
{
    return m_quantite_max_kerosene;
}

//Getter de la consommation
int Avion::get_consommation()
{
    return m_consommation;
}

//Getter de la vitesse
int Avion::get_vitesse()
{
    return m_vitesse;
}

//Getter de l'altitude
int Avion::get_altitude()
{
    return m_altitude;
}

//Getter de l'action en cours
int Avion::get_action_en_cours()
{
    return m_action_en_cours;
}

//Getter de la dur�e de pr�paration
int Avion::get_duree_prepraration()
{
    return m_duree_prepraration;
}

//Getter de la dur�e de roulage
int Avion::get_duree_roulage()
{
    return m_duree_roulage;
}

//Getter de la dur�e de d�collage
int Avion::get_duree_decollage()
{
    return m_duree_decollage;
}

//Getter de la dur�e de vol
int Avion::get_duree_vol()
{
    return m_duree_vol;
}

//Getter de la dur�e d'atterrissage
int Avion::get_duree_atterrissage()
{
    return m_duree_atterrissage;
}

//Getter de la dur�e de sortie de piste
int Avion::get_duree_sortie_piste()
{
    return m_duree_sortie_piste;
}

//Getter de la dur�e de boucle d'attente (avant atterrissage)
int Avion::get_duree_boucle_attente()
{
    return m_duree_boucle_attente;
}

//Setter de l'immatriculation
void Avion::set_immatriculation(string _immatriculation)
{
    m_immatriculation = _immatriculation;
}

//Setter du mod�le
void Avion::set_modele(string _modele)
{
    m_modele = _modele;
}

void Avion::set_action_en_cours(int valeur) {m_action_en_cours = valeur;}
void Avion::set_duree_prepraration(int valeur) {m_duree_prepraration = valeur;}
void Avion::set_duree_roulage(int valeur) {m_duree_roulage = valeur;}
void Avion::set_duree_decollage(int valeur) {m_duree_decollage = valeur;}
void Avion::set_duree_vol(int valeur) {m_duree_vol = valeur;}
void Avion::set_duree_atterrissage(int valeur) {m_duree_atterrissage = valeur;}
void Avion::set_duree_sortie_piste(int valeur) {m_duree_sortie_piste = valeur;}
void Avion::set_duree_boucle_attente(int valeur) {m_duree_boucle_attente = valeur;}

//Setter des coordonn�es de l'avion
void Avion::set_coord(float x, float y)
{
    m_coord.set_coord_x(x);
    m_coord.set_coord_y(y);
}

//Getter du nom de l'a�roport de D�part
string Avion::getNomAeroportD()
{
    return m_aeroportD;
}

//Getter du nom de l'a�roport d'Arriv�e
string Avion::getNomAeroportA()
{
    return m_aeroportA;
}

//Setter du nom de l'a�roport de D�part
void Avion::setNomAeroportD(string nom)
{
    m_aeroportD = nom;
}

//Setter du nom de l'a�roport d'Arriv�e
void Avion::setNomAeroportA(string nom)
{
    m_aeroportA = nom;
}

//Getter d'un successeur pr�cis de l'a�roport
pair<string, int> Aeroport::get_successeur_precis(int i)
{
    return m_distance_aeroports[i];
}

//Getter de l'indice pr�cis d'une escale
int Avion::getIndicePrecisEscale(int indice)
{
    return m_indiceEscales[indice];
}

//Getter du vecteur d'indices des escales
vector<int> Avion::getTousIndicesEscales()
{
    return m_indiceEscales;
}

//Setter du vecteur d'escales
void Avion::setListeEscales(vector<int> nouvelleListe)
{
    m_indiceEscales = nouvelleListe;
}



    //////////////
    // METHODES //
    //////////////

//Affichage des caract�ristique de l'avion
void Avion::afficher_caracteristique()
{
    cout << "Modele : " << m_modele << " \t Immatriculation : " << m_immatriculation << endl;
    cout << "Type de vol : " << m_type_vol << " courrier" << endl;
    cout << "Consommation : " << m_consommation << " l/100km" << endl;
    cout << "Vitesse : " << m_vitesse << " km/h" << endl;
    cout << "Quantite de kerosene : " << m_quantite_kerosene << " l" << endl;
    cout << "Quantite de kerosene MAX : " << m_quantite_max_kerosene<< " l" << endl;

}

//Affichage des caract�ristique de l'avion
void Avion::afficher_caracteristique2()
{
    cout << "Duree preparation : " << m_duree_prepraration << endl;
    cout << "Duree roulage : " << m_duree_roulage << endl;
    cout << "Duree decollage : " << m_duree_decollage<< endl;
    cout << "Duree Atterrissage : " << m_duree_atterrissage << endl;
    cout << "Duree sortie de piste : " << m_duree_sortie_piste << endl;
    cout << "Duree boucle attente : " << m_duree_boucle_attente << endl;
    cout << "Duree vol : " << m_duree_vol << endl;
}

//Passage � l'escale suivante
void Avion::passageAEscaleSuivante()
{
    m_indiceEscales.erase(m_indiceEscales.begin());
}

//M�thode d'actualisation de l'avion en fonction de son �tat
void Avion::actualiser_action_avion()
{
    //Traitement adapt� � l'�tat
    switch(m_action_en_cours)
    {
        case 0: //Preparation au vol
            m_quantite_kerosene = m_quantite_max_kerosene; //Remplissage du reservoir

            //SI la dur�e de pr�paration n'est toujours pas �coul�e, on la diminue
            if(m_duree_prepraration > 0)
            {
                m_duree_prepraration--; //D�cr�mentation de la phase de pr�paration
            }

        break;


        case 1: //Roulage jusqu'� la piste

            //SI la dur�e de roulage n'est toujours pas �coul�e, on la diminue
            if(m_duree_roulage > 0)
            {
                m_duree_roulage--; //D�cr�mentation de la phase de roulage
            }

        break;


        case 2: //Decollage

            //SI la dur�e de d�collage n'est toujours pas �coul�e, on la diminue
            if(m_duree_decollage > 0)
            {
                m_duree_decollage--; //D�cr�mentation de la phase de d�collage
            }

            //cout << "Decollage" << endl;

        break;


        case 3: //Vol

            //SI la dur�e de vol n'est toujours pas �coull�e, on la diminue
            if(m_duree_vol > 0)
            {
                m_duree_vol--; //D�cr�mentation de la phase de vol
                actualisationPositionVol();
            }

            //cout << "Vol" << endl;

        break;


        case 4: //Boucle d'attente

            //SI la dur�e de la boucle d'attente n'est toujours pas �coul�e, on la diminue
            if(m_duree_boucle_attente > 0)
            {
                m_duree_boucle_attente--; //D�cr�mentation de la phase de boucle d'attente
            }

            //cout << "Boucle d'attente" << endl;
        break;

        case 5: //Atterissage

            //SI la dur�e d'atterrissage n'est toujours pas �coul�e, on la diminue
            if(m_duree_atterrissage > 0)
            {
                m_duree_atterrissage--; //D�cr�mentation de la phase d'atterrissage
            }

            //cout << "Aterrissage" << endl;
        break;

        case 6: //Degager la piste

            //SI la dur�e de sortie de piste n'est toujours pas �coul�e, on la diminue
            if(m_duree_sortie_piste > 0)
            {
                m_duree_sortie_piste--; //D�cr�mentation de la phase de sortie de piste
            }

            //cout << "Degagement de la piste" << endl;
        break;

    }

}


//M�thode de r�cup�ration des informations de l'a�roport de d�part
void Avion::recuperationInfosAeroportD(Aeroport *aeroportDepart)
{
    //R�cup�ration de l'ensemble des informations concernant les UT et la localisation de l'a�roport de d�part du futur trajet
    m_duree_prepraration = aeroportDepart->get_delai_attente_sol();
    m_duree_roulage = aeroportDepart->get_temps_acces_pistes();
    m_duree_decollage = aeroportDepart->get_temps_decollage_atterissage();
    m_coordDepart.first = aeroportDepart->get_position().get_coord_x();
    m_coordDepart.second = aeroportDepart->get_position().get_coord_y();
    m_coord.set_coord_x(aeroportDepart->get_position().get_coord_x());
    m_coord.set_coord_y(aeroportDepart->get_position().get_coord_y());

    //R�initialisation de l'action en cours
    m_action_en_cours = 0;

    m_aeroportD = aeroportDepart->get_nom();
}

//M�thode d'actualisation de la position de l'avion en vol
void Avion::actualisationPositionVol()
{
    //Actualisation des coordonn�es
    set_coord(get_coord().get_coord_x() + m_espacementAffichageX*1.05, get_coord().get_coord_y() + m_espacementAffichageY*1.15);
}

//M�thode d'enregistrement des informations des a�roports de d�part et d'arriv�e
void Avion::parametrer_nouveau_vol(Aeroport depart, Aeroport arrivee)
{
    int xAngleDroit = 0; //Position en X de l'angle droit
    int yAngleDroit = 0; //Position en Y de l'angle droit
    int DAngleDroit = 0; //Distance entre le D�part et l'angle droit
    int AAngleDroit = 0; //Distance entre l'arriv�e et l'angle droit
    float tanDepart = 0; //Valeur de la tangente en D�part
    float angle; //Valeur de l'angle en D�part
    float hypothenuse = 0; //Valeur de l'hypoth�nuse du triangle rectangle
    float x = 0;
    float y = 0;

    /*
    //R�cup�ration des informations de d�part
    m_duree_prepraration = depart.get_delai_attente_sol();
    m_duree_roulage = depart.get_temps_acces_pistes();
    m_duree_decollage = depart.get_temps_decollage_atterissage();
    m_coordDepart.first = depart.get_position().get_coord_x();
    m_coordDepart.second = depart.get_position().get_coord_y();
    m_coord.set_coord_x(depart.get_position().get_coord_x());
    m_coord.set_coord_y(depart.get_position().get_coord_y());
*/
    //R�cup�ration des informations d'arriv�e
    m_duree_atterrissage = arrivee.get_temps_decollage_atterissage();
    m_duree_sortie_piste = arrivee.get_temps_acces_pistes();
    m_duree_boucle_attente = arrivee.get_duree_boucle_attente_vol();
    m_coordArrivee.first = arrivee.get_position().get_coord_x();
    m_coordArrivee.second = arrivee.get_position().get_coord_y();

    //Affectation de la dur�e de vol
    for(int i=0 ; i<int(depart.get_distance_aeroports().size()) ; i++) //Boucle de parcours de l'ensemble des a�roports voisins � celui de d�part
    {
        //SI il s'agit de l'a�roport d'arriv�e
        if(depart.get_distance_aeroports()[i].first == arrivee.get_nom())
        {
            //On calcule et on affecte la dur�e du vol en UT
            m_duree_vol = ((depart.get_distance_aeroports()[i].second / m_vitesse) * 60) / UT;
            break;
        }
    }



    //CALCUL DE L'ANGLE DE L'AVION

    //Actualsiation de la position de l'angle droit
    xAngleDroit = m_coordDepart.first;
    yAngleDroit = m_coordArrivee.second;

    //SI l'arriv�e est au dessus du point de d�part
    if(yAngleDroit < m_coordDepart.second)
    {
        DAngleDroit = m_coordDepart.second - yAngleDroit;
    }
    else //SINON, l'arriv�e est en dessous
    {
        DAngleDroit = yAngleDroit - m_coordDepart.second;
    }

    //SI le d�part est � gauche de l'arriv�e
    if(xAngleDroit < m_coordArrivee.first)
    {
        AAngleDroit = m_coordArrivee.first - xAngleDroit;
    }
    else //SINON, le d�part est � droite
    {
        AAngleDroit = xAngleDroit - m_coordArrivee.first;
    }

    //Calcul de l'angle D
    tanDepart = (float)AAngleDroit/(float)DAngleDroit;
    angle = atan(tanDepart);
    angle = angle*57.29;

    //Quatre cas de figures d'orientation du triangle rectangle
    if(xAngleDroit < m_coordArrivee.first && yAngleDroit > m_coordDepart.second)
    {
        /*
        D
        C   A
        */

        angle = 180 - angle;
    }
    else if(xAngleDroit > m_coordArrivee.first && yAngleDroit > m_coordDepart.second)
    {
        /*
            D
        A   C
        */

        angle = 180 + angle;
    }
    else if (xAngleDroit < m_coordArrivee.first && yAngleDroit < m_coordDepart.second)
    {
        /*
        C   A
        D
        */

        angle = angle;
    }
    else if(xAngleDroit > m_coordArrivee.first && yAngleDroit < m_coordDepart.second)
    {
        /*
        A   C
            D
        */

        angle = 180 + (180 - angle);
    }

    angle = angle*255/360; //Produit en croix car l'angle doit �tre entre 0 et 255
    m_angle = angle; //R�cup�ration de l'angle comme attribut de l'avion

    //Calcul de la progression en X et en Y � chaque UT
    hypothenuse = sqrt(DAngleDroit * DAngleDroit + AAngleDroit * AAngleDroit) / (float)m_duree_vol; //Calcul de l'hytpoth�nuse du triangle rectangle
    angle = angle * 360.0 / 255.0; //Calcul de l'angle
    y = cos(angle/57.29) * hypothenuse; //Calcul du d�calage en Y � chaque UT
    x = sqrt(hypothenuse * hypothenuse - y * y); //Calcul du d�calage en X � chaque UT

    //Quatre cas de figures d'orientation du triangle rectangle r�galnt le signe de d�calage en X et en Y
    if(xAngleDroit < m_coordArrivee.first && yAngleDroit > m_coordDepart.second)
    {
        /*
        D
        C   A
        */

        x = x;
        y = -y;
    }
    else if(xAngleDroit > m_coordArrivee.first && yAngleDroit > m_coordDepart.second)
    {
        /*
            D
        A   C
        */

        x = -x;
        y = -y;
    }
    else if (xAngleDroit < m_coordArrivee.first && yAngleDroit < m_coordDepart.second)
    {
        /*
        C   A
        D
        */

        x = x;
        y = -y;
    }
    else if(xAngleDroit > m_coordArrivee.first && yAngleDroit < m_coordDepart.second)
    {
        /*
        A   C
            D
        */

        x = -x;
        y = -y;
    }

    //Affectation des d�calages en X et en Y actualis�s
    m_espacementAffichageX = x;
    m_espacementAffichageY = y;
}


//M�thode d'affichage de l'avion
void Avion::affichageAvionCarte(BITMAP* doubleBuffer, BITMAP* avionCourt, BITMAP* avionMoyen, BITMAP* avionLong)
{
    //SI l'avion vole, on l'affiche
    if(m_action_en_cours == 3)
    {
        //SI l'avion est court courrier
        if(m_type_vol == "court")
        {
            rotate_scaled_sprite(doubleBuffer, avionCourt, get_coord().get_coord_x()-(avionCourt->w/4), get_coord().get_coord_y()-(avionCourt->h/4),ftofix(m_angle),ftofix(0.5));
        }
        //SINON SI l'avion est moyen courrier
        else if(m_type_vol == "moyen")
        {
            rotate_scaled_sprite(doubleBuffer, avionMoyen, get_coord().get_coord_x()-(avionMoyen->w/4), get_coord().get_coord_y()-(avionMoyen->h/4),ftofix(m_angle),ftofix(0.5));
        }
        //SINON SI l'avion est long courrier
        else if(m_type_vol == "long")
        {
            rotate_scaled_sprite(doubleBuffer, avionLong, get_coord().get_coord_x()-(avionLong->w/4), get_coord().get_coord_y()-(avionLong->h/4),ftofix(m_angle),ftofix(0.5));
        }
    }
}
