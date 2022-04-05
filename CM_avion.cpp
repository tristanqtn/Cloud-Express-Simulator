#include "CH_avion.h"

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
    fichier_aeroport >> m_quantite_max_kerosene;
    m_quantite_kerosene = m_quantite_max_kerosene;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture quantite de kerosene"); //erreur de lecture

    //AJOUTER QUANTITE DE KEROSEN

    //lecture de la consommation
    fichier_aeroport >> m_consommation;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture de la consommation"); //erreur de lecture

    //initialisation des coordonnées nulle
    m_coord.set_coord_x(0);
    m_coord.set_coord_y(0);

    m_modele = "";
    m_immatriculation = "";
    m_altitude = 0;
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

}

//Destructeur
Avion::~Avion()
{

}



    ////////////////
    // ACCESSEURS //
    ////////////////

//Getter des coordonnées
Coord Avion::get_coord()
{
    return m_coord;
};

//Getter du modèle
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

//Getter de la quantité de kerosen
int Avion::get_quantite_kerosene()
{
    return m_quantite_kerosene;
}

//Getter de la quantité maximale de kerosen
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

int Avion::get_action_en_cours() {return m_action_en_cours;}
int Avion::get_duree_prepraration() {return m_duree_prepraration;}
int Avion::get_duree_roulage() {return m_duree_roulage;}
int Avion::get_duree_decollage() {return m_duree_decollage;}
int Avion::get_duree_vol() {return m_duree_vol;}
int Avion::get_duree_atterrissage() {return m_duree_atterrissage;}
int Avion::get_duree_sortie_piste() {return m_duree_sortie_piste;}
int Avion::get_duree_boucle_attente() {return m_duree_boucle_attente;}

//Setter de l'immatriculation
void Avion::set_immatriculation(string _immatriculation)
{
    m_immatriculation = _immatriculation;
}

//Setter du modèle
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

    //////////////
    // METHODES //
    //////////////

//Affichage des caractéristique de l'avion
void Avion::afficher_caracteristique()
{
    cout << "Modele : " << m_modele << " \t Immatriculation : " << m_immatriculation << endl;
    cout << "Type de vol : " << m_type_vol << " courrier" << endl;
    cout << "Consommation : " << m_consommation << " l/100km" << endl;
    cout << "Vitesse : " << m_vitesse << " km/h" << endl;
    cout << "Quantite de kerosene : " << m_quantite_kerosene << " l" << endl;
    cout << "Quantite de kerosene MAX : " << m_quantite_max_kerosene<< " l" << endl;
}


void Avion::actualiser_action_avion()
{
    switch(m_action_en_cours)
    {
        case 0: //Preparation au vol
            m_quantite_kerosene = m_quantite_max_kerosene; //Remplissage du reservoir

            m_duree_prepraration--;

            if(m_duree_prepraration == 0)
                m_action_en_cours = 1;
            cout << "Preparation du vol" << endl;
        break;


        case 1: //Roulage jusqu'à la piste
            m_duree_roulage--;

            if(m_duree_roulage == 0)
                m_action_en_cours = 2;
            cout << "Roulage vers piste" << endl;
        break;


        case 2: //Decollage
            m_duree_decollage--;
            if(m_duree_decollage == 0)
                m_action_en_cours = 3;
            cout << "Decollage" << endl;
        break;

        case 3: //Vol
            m_duree_vol--;

            if(m_duree_vol == 0)
                m_action_en_cours = 4;
            cout << "Vol" << endl;
        break;

        case 4: //Boucle attente
            if(m_duree_boucle_attente != 0)
            {
                m_duree_boucle_attente--;

                if(m_duree_boucle_attente == 0)
                    m_action_en_cours = 5;
                cout << "Boucle d'attente" << endl;
            }
            else
                m_action_en_cours = 5;
        break;

        case 5: //Atterissage
            m_duree_atterrissage--;

            if(m_duree_atterrissage == 0)
                m_action_en_cours = 6;
            cout << "Aterrissage" << endl;
        break;

        case 6: //Degager la piste
            m_duree_sortie_piste--;

            if(m_duree_sortie_piste == 0)
                m_action_en_cours = 0;
            cout << "Degagement de la piste" << endl;
        break;

    }

}


void Avion::parametrer_nouveau_vol(Aeroport depart, Aeroport arrivee)
{
    //Depart
    m_duree_prepraration = depart.get_delai_attente_sol();
    m_duree_roulage = depart.get_temps_acces_pistes();
    m_duree_decollage = depart.get_temps_decollage_atterissage();

    ///SET LE TEMPS DE VOL !

    //Arrive
    m_duree_atterrissage = arrivee.get_temps_decollage_atterissage();
    m_duree_sortie_piste = arrivee.get_temps_acces_pistes();
    m_duree_boucle_attente = arrivee.get_duree_boucle_attente_vol();
}
