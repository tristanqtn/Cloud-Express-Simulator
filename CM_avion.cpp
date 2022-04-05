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
    fichier_aeroport >> m_quantite_kerosene;
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
}
