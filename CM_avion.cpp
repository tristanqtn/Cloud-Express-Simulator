#include "H_header.h"
#include "CH_avion&aeroport&route.h"
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
    fichier_aeroport >> m_quantite_max_kerosene;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture quantite de kerosene"); //erreur de lecture

    //lecture de la consommation
    fichier_aeroport >> m_consommation;
    if ( fichier_aeroport.fail() )
        throw runtime_error("Probleme lecture de la consommation"); //erreur de lecture

    //initialisation des coordonnées nulle
    m_coord.set_coord_x(0);
    m_coord.set_coord_y(0);

    m_modele = "";
    m_immatriculation = "";
    m_etat_reservoir = false;
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
    m_etat_reservoir = false;
}

//Constructeur par défaut pour les pointeurs
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

//Getter de la quantité maximale de kerosen
int Avion::get_quantite_max_kerosene()
{
    return m_quantite_max_kerosene;
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

//Getter de l'altitude
int Avion::get_altitude()
{
    return m_altitude;
}

//Setter de l'altitude
void Avion::set_altitude(int nouvelleAltitude)
{
    m_altitude = nouvelleAltitude;
}

//Getter de l'état du vol
int Avion::get_etatVol()
{
    return m_etatVol;
}

//Setter de l'état du vol
void Avion::set_etatVol(int nouvelEtat)
{
    m_etatVol = nouvelEtat;
}

//Getter de l'action en cours
int Avion::get_action_en_cours()
{
    return m_action_en_cours;
}

//Getter de la durée de préparation
int Avion::get_duree_prepraration()
{
    return m_duree_prepraration;
}

//Getter de la durée de roulage
int Avion::get_duree_roulage()
{
    return m_duree_roulage;
}

//Getter de la durée de décollage
int Avion::get_duree_decollage()
{
    return m_duree_decollage;
}

//Getter de la durée de vol
int Avion::get_duree_vol()
{
    return m_duree_vol;
}

//Getter de la durée d'atterrissage
int Avion::get_duree_atterrissage()
{
    return m_duree_atterrissage;
}

//Getter de la durée de sortie de piste
int Avion::get_duree_sortie_piste()
{
    return m_duree_sortie_piste;
}

//Getter de la durée de boucle d'attente (avant atterrissage)
int Avion::get_duree_boucle_attente()
{
    return m_duree_boucle_attente;
}

bool Avion::get_etat_reservoir()
{
    return m_etat_reservoir;
}

void Avion::set_action_en_cours(int valeur) {m_action_en_cours = valeur;}
void Avion::set_duree_prepraration(int valeur) {m_duree_prepraration = valeur;}
void Avion::set_duree_roulage(int valeur) {m_duree_roulage = valeur;}
void Avion::set_duree_decollage(int valeur) {m_duree_decollage = valeur;}
void Avion::set_duree_vol(int valeur) {m_duree_vol = valeur;}
void Avion::set_duree_atterrissage(int valeur) {m_duree_atterrissage = valeur;}
void Avion::set_duree_sortie_piste(int valeur) {m_duree_sortie_piste = valeur;}
void Avion::set_duree_boucle_attente(int valeur) {m_duree_boucle_attente = valeur;}
void Avion::set_etat_reservoir(bool nouvel_etat) {m_etat_reservoir = nouvel_etat;}

//Setter du modèle
void Avion::set_modele(string _modele)
{
    m_modele = _modele;
}

//Setter de l'immatriculation
void Avion::set_immatriculation(string _immatriculation)
{
    m_immatriculation = _immatriculation;
}

//Setter des coordonnées de l'avion
void Avion::set_coord(float x, float y)
{
    m_coord.set_coord_x(x);
    m_coord.set_coord_y(y);
}

//Getter du nom de l'aéroport de Départ
string Avion::getNomAeroportD()
{
    return m_aeroportD;
}

//Getter du nom de l'aéroport d'Arrivée
string Avion::getNomAeroportA()
{
    return m_aeroportA;
}

//Setter du nom de l'aéroport de Départ
void Avion::setNomAeroportD(string nom)
{
    m_aeroportD = nom;
}

//Setter du nom de l'aéroport d'Arrivée
void Avion::setNomAeroportA(string nom)
{
    m_aeroportA = nom;
}

//Getter du nom de l'aéroport Initial
string Avion::getNomAeroportInitial()
{
    return m_aeroportInitial;
}

//Getter du nom de l'aéroport Final
string Avion::getNomAeroportFinal()
{
    return m_aeroportFinal;
}

//Setter du nom de l'aéroport Initial
void Avion::setNomAeroportInitial(string nom)
{
    m_aeroportInitial = nom;
}

//Setter du nom de l'aéroport Final
void Avion::setNomAeroportFinal(string nom)
{
    m_aeroportFinal = nom;
}

//Getter de l'indice précis d'une escale
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

//Getter de l'heure de départ du vol
Horloge Avion::getHeureDepart()
{
    return m_heureDepart;
}

//Setter de l'heure de départ du vol
void Avion::setHeureDepart(Horloge &heureRef)
{
    //Copie de l'horloge à un instant T
    m_heureDepart.set_jour(heureRef.get_jour());
    m_heureDepart.set_mois(heureRef.get_mois());
    m_heureDepart.set_annee(heureRef.get_annee());
    m_heureDepart.set_heure(heureRef.get_heure().first, heureRef.get_heure().second);
}

//Getter du coefficient A de la trajectoire de l'avion
float Avion::getAArete()
{
    return m_a_arete;
}

//Getter de la constante B de la trajectoire de l'avion
float Avion::getBArete()
{
    return m_b_arete;
}

//Setter du coefficient A de la trajectoire de l'avion
void Avion::setAArete(float nouveauA)
{
    m_a_arete = nouveauA;
}

//Setter de la constante B de la trajectoire de l'avion
void Avion::setBArete(float nouveauB)
{
    m_b_arete = nouveauB;
}

//Getter de la distance parcourue
float Avion::getDistanceParcourue()
{
    return m_distanceParcourue;
}

//Setter de la distance parcourue
void Avion::setDistanceParcourue(float nouvelleDistance)
{
    m_distanceParcourue = nouvelleDistance;
}



    //////////////
    // METHODES //
    //////////////


        ////////////////
        // AFFICHAGES //
        ////////////////

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


//Affichage des caractéristique de l'avion
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


//Méthode d'affichage de l'avion
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


//Méthode de mise en surbrillance des avions
void Avion::actualisationSurbrillanceAvion(vector<Aeroport> m_aeroports, bool typeAffichage, int x, int y, BITMAP* doubleBuffer, BITMAP* overlay, BITMAP* overlayInverse, BITMAP* avionCourtSurligne, BITMAP* avionMoyenSurligne, BITMAP* avionLongSurligne, FONT* titre, FONT* texte, FONT* policeHeure)
{
    int indicSensAffichage = 0; //Contient l'information du sens d'affichage des informations de l'avion
    int coefficientReducteur = 2;
    int couleurTexte = makecol(255, 255, 255); //Contient la couleur des informations textuelles de l'overlay

    //SI la souris passe sur l'avion
    if(mouse_x >= x - (avionCourtSurligne->w/4) && mouse_x <= x + 20 && mouse_y > y -(avionCourtSurligne->h/4) && mouse_y < y + 40)
    {
        if((typeAffichage == true && get_action_en_cours() == 3) || (typeAffichage == false))
        {
            //SI il s'agit d'un court courrier
            if(m_type_vol == "court")
            {
                //On le surligne
                rotate_scaled_sprite(doubleBuffer, avionCourtSurligne, x-(avionCourtSurligne->w/4), y-(avionCourtSurligne->h/4), ftofix(typeAffichage * m_angle), ftofix(0.5));
            }
            //SINON SI il s'agit d'un moyen courrier
            else if(m_type_vol == "moyen")
            {
                //On le surligne
                rotate_scaled_sprite(doubleBuffer, avionMoyenSurligne, x-(avionMoyenSurligne->w/4), y-(avionMoyenSurligne->h/4), ftofix(typeAffichage * m_angle), ftofix(0.5));
            }
            //SINON SI il s'agit d'un long courrier
            else if(m_type_vol == "long")
            {
                //On le surligne
                rotate_scaled_sprite(doubleBuffer, avionLongSurligne, x-(avionLongSurligne->w/4), y-(avionLongSurligne->h/4), ftofix(typeAffichage * m_angle), ftofix(0.5));
            }

            //SI la souris est trop haute pour que l'overlay s'affiche entièrement
            if(y - overlay->h/coefficientReducteur < 0)
            {
                //L'overlay devrait être inversé
                indicSensAffichage = -1;
            }
            //SINON
            else
            {
                //L'overlay peut s'afficher normalement
                indicSensAffichage = 1;
            }
        }
    }

    //Affichage adapté à la place disponible pour l'overlay
    switch(indicSensAffichage)
    {
        case -1 : //Overlay inversé
            //Affichage de l'overlay inversé
            masked_stretch_blit(overlayInverse, doubleBuffer, 0, 0, overlayInverse->w, overlayInverse->h, x-overlayInverse->w/(coefficientReducteur-0.5)/2, y, overlayInverse->w/(coefficientReducteur-0.5), overlayInverse->h/coefficientReducteur);


            //Affichage du trajet

            //SI l'avion a une destination
            if(getNomAeroportFinal() != "")
            {
                textprintf_centre_ex(doubleBuffer,titre,x,y+80,makecol(255, 255, 255),-1,"%s - %s", convertisseurStringChar(getNomAeroportInitial()), convertisseurStringChar(getNomAeroportFinal()));
            }
            //SINON, l'avion n'a pas de destination
            else
            {
                textprintf_centre_ex(doubleBuffer,titre,x,y+80,makecol(255, 255, 255),-1,"%s - ...", convertisseurStringChar(getNomAeroportInitial()));
            }


            //Affichage de l'heure de départ du vol total
            getHeureDepart().affichageOverlayAvion(x,y+130, policeHeure, doubleBuffer);


            //Affichage du type de vol

            //SI l'avion est court courrier
            if(get_type_vol() == "court")
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+190,couleurTexte,-1,"Court courrier");
            }
            //SINON SI l'avion est moyen courrier
            else if(get_type_vol() == "moyen")
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+190,couleurTexte,-1,"Moyen courrier");
            }
            //SINON SI l'avion est long courrier
            else if(get_type_vol() == "long")
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+190,couleurTexte,-1,"Long courrier");
            }

            //Affichage de l'immatriculation
            textprintf_centre_ex(doubleBuffer,texte,x,y+220,couleurTexte,-1,"%s", convertisseurStringChar(get_immatriculation()));


            //Affichage de l'escale actuelle

            //SI l'avion fait actuellement une escale
            if(int(getTousIndicesEscales().size()) > 1)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+250,couleurTexte,-1,"Escale : %s", convertisseurStringChar(m_aeroports[getIndicePrecisEscale(0)].get_nom()));
            }
            //SINON SI l'avion n'effectue pas d'escales
            else if(int(getTousIndicesEscales().size()) == 1)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+250,couleurTexte,-1,"Aucune escale");
            }
            //SINON SI l'avion n'a pas encore de trajet
            else
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+250,couleurTexte,-1,"Escale : ...");
            }


            //Affichage de l'altitude de l'avion


            //SI l'avion n'a pas encore commencé son vol
            if(get_action_en_cours() != 3)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+280,couleurTexte,-1,"Altitude : 0m");
            }
            //SINON l'aéroport est en vol
            else
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+280,couleurTexte,-1,"Altitude : %dm", get_altitude());
            }


            //Affichage de la quantité de kerosen
            textprintf_centre_ex(doubleBuffer,texte,x,y+310,couleurTexte,-1,"%d / %dL", get_quantite_kerosene(), get_quantite_max_kerosene());


            //Affichage de la vitesse de l'avion
            //SI l'avion est à l'arrêt
            if(get_action_en_cours() == 0)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+340,couleurTexte,-1,"A l'arret");
            }
            //SINON SI l'avion est en vol
            else if(get_action_en_cours() == 3)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+340,couleurTexte,-1,"%dkm/h", get_vitesse());
            }
            //SINON SI l'avion est dans une phase de démarrage
            else
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+340,couleurTexte,-1,"Demarrrage");
            }

            //Affichage de l'état du vol
            if(get_action_en_cours() != 3)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y+370,couleurTexte,-1,"Normal");
            }
            //SINON l'avion est en vol, il peut donc lui arriver des misères et on les affiche
            else
            {
                //Affichage en fonction de l'état du vol
                switch(get_etatVol())
                {
                    case 0 : //Normal
                        textprintf_centre_ex(doubleBuffer,texte,x,y+370,couleurTexte,-1,"Normal");
                        break;

                    case 1 : //Fuite de résevoir
                        textprintf_centre_ex(doubleBuffer,texte,x,y+370,couleurTexte,-1,"Fuite reservoir");
                        break;

                    case 2 : //Détournement
                        textprintf_centre_ex(doubleBuffer,texte,x,y+370,couleurTexte,-1,"Detournement");
                        break;
                }
            }
            break;

        case 1 :
            //Affichage de l'overlay normal
            masked_stretch_blit(overlay, doubleBuffer, 0, 0, overlay->w, overlay->h, x-overlay->w/(coefficientReducteur-0.5)/2, y-overlay->h/coefficientReducteur, overlay->w/(coefficientReducteur-0.5), overlay->h/coefficientReducteur);


            //Affichage du trajet

            //SI l'avion a une destination
            if(getNomAeroportFinal() != "")
            {
                textprintf_centre_ex(doubleBuffer,titre,x,y-375,makecol(255, 255, 255),-1,"%s - %s", convertisseurStringChar(getNomAeroportInitial()), convertisseurStringChar(getNomAeroportFinal()));
            }
            //SINON, l'avion n'a pas de destination
            else
            {
                textprintf_centre_ex(doubleBuffer,titre,x,y-375,makecol(255, 255, 255),-1,"%s - ...", convertisseurStringChar(getNomAeroportInitial()));
            }


            //Affichage de l'heure de départ du vol total
            getHeureDepart().affichageOverlayAvion(x,y-325, policeHeure, doubleBuffer);


            //Affichage du type de vol

            //SI l'avion est court courrier
            if(get_type_vol() == "court")
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-265,couleurTexte,-1,"Court courrier");
            }
            //SINON SI l'avion est moyen courrier
            else if(get_type_vol() == "moyen")
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-265,couleurTexte,-1,"Moyen courrier");
            }
            //SINON SI l'avion est long courrier
            else if(get_type_vol() == "long")
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-265,couleurTexte,-1,"Long courrier");
            }

            //Affichage de l'immatriculation
            textprintf_centre_ex(doubleBuffer,texte,x,y-235,couleurTexte,-1,"%s", convertisseurStringChar(get_immatriculation()));


            //Affichage de l'escale actuelle

            //SI l'avion fait actuellement une escale
            if(int(getTousIndicesEscales().size()) > 1)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-205,couleurTexte,-1,"Escale : %s", convertisseurStringChar(m_aeroports[getIndicePrecisEscale(0)].get_nom()));
            }
            //SINON SI l'avion n'effectue pas d'escales
            else if(int(getTousIndicesEscales().size()) == 1)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-205,couleurTexte,-1,"Aucune escale");
            }
            //SINON SI l'avion n'a pas encore de trajet
            else
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-205,couleurTexte,-1,"Escale : ...");
            }


            //Affichage de l'altitude de l'avion


            //SI l'avion n'a pas encore commencé son vol
            if(get_action_en_cours() != 3)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-175,couleurTexte,-1,"Altitude : 0m");
            }
            //SINON l'aéroport est en vol
            else
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-175,couleurTexte,-1,"Altitude : %dm", get_altitude());
            }


            //Affichage de la quantité de kerosen
            textprintf_centre_ex(doubleBuffer,texte,x,y-145,couleurTexte,-1,"%d / %dL", get_quantite_kerosene(), get_quantite_max_kerosene());


            //Affichage de la vitesse de l'avion
            //SI l'avion est à l'arrêt
            if(get_action_en_cours() == 0)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-115,couleurTexte,-1,"A l'arret");
            }
            //SINON SI l'avion est en vol
            else if(get_action_en_cours() == 3)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-115,couleurTexte,-1,"%dkm/h", get_vitesse());
            }
            //SINON SI l'avion est dans une phase de démarrage
            else
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-115,couleurTexte,-1,"Demarrrage");
            }

            //Affichage de l'état du vol
            if(get_action_en_cours() != 3)
            {
                textprintf_centre_ex(doubleBuffer,texte,x,y-85,couleurTexte,-1,"Normal");
            }
            //SINON l'avion est en vol, il peut donc lui arriver des misères et on les affiche
            else
            {
                //Affichage en fonction de l'état du vol
                switch(get_etatVol())
                {
                    case 0 : //Normal
                        textprintf_centre_ex(doubleBuffer,texte,x,y-85,couleurTexte,-1,"Normal");
                        break;

                    case 1 : //Fuite de résevoir
                        textprintf_centre_ex(doubleBuffer,texte,x,y-85,couleurTexte,-1,"Fuite reservoir");
                        break;

                    case 2 : //Détournement
                        textprintf_centre_ex(doubleBuffer,texte,x,y-85,couleurTexte,-1,"Detournement");
                        break;
                }
            }
            break;

        default :
            break;
    }
}



    //////////////////
    // INFORMATIONS //
    //////////////////

//Passage à l'escale suivante
void Avion::passageAEscaleSuivante()
{
    m_indiceEscales.erase(m_indiceEscales.begin());
}




///////////////////////////////////////////////////////////////////
//Méthode d'actualisation de l'avion en fonction de son état
bool Avion::actualiser_action_avion()
{
    bool rendu = false;
    //Traitement adapté à l'état
    switch(m_action_en_cours)
    {
        case 0: //Preparation au vol
            m_quantite_kerosene = m_quantite_max_kerosene; //Remplissage du reservoir
            m_etat_reservoir = false;
            m_etatVol = 0;
            m_distanceParcourue = 0;

            //SI la durée de préparation n'est toujours pas écoulée, on la diminue
            if(m_duree_prepraration > 0)
            {
                m_duree_prepraration--; //Décrémentation de la phase de préparation
            }

        break;


        case 1: //Roulage jusqu'à la piste

            //SI la durée de roulage n'est toujours pas écoulée, on la diminue
            if(m_duree_roulage > 0)
            {
                m_duree_roulage--; //Décrémentation de la phase de roulage
            }

        break;


        case 2: //Decollage

            //SI la durée de décollage n'est toujours pas écoulée, on la diminue
            if(m_duree_decollage > 0)
            {
                m_duree_decollage--; //Décrémentation de la phase de décollage
            }

            //cout << "Decollage" << endl;

        break;


        case 3: //Vol

            //SI la durée de vol n'est toujours pas écoullée, on la diminue
            if(m_duree_vol > 0)
            {
                m_duree_vol--; //Décrémentation de la phase de vol
                actualisationPositionVol();
                rendu = consommer_kerosene();
                setDistanceParcourue(getDistanceParcourue() + (float)get_vitesse() * (float)UT/60.0);
            }

            //cout << "Vol" << endl;

        break;


        case 4: //Boucle d'attente

            //SI la durée de la boucle d'attente n'est toujours pas écoulée, on la diminue
            if(m_duree_boucle_attente > 0)
            {
                m_duree_boucle_attente--; //Décrémentation de la phase de boucle d'attente
            }

            //cout << "Boucle d'attente" << endl;
        break;

        case 5: //Atterissage

            //SI la durée d'atterrissage n'est toujours pas écoulée, on la diminue
            if(m_duree_atterrissage > 0)
            {
                m_duree_atterrissage--; //Décrémentation de la phase d'atterrissage
            }

            //cout << "Aterrissage" << endl;
        break;

        case 6: //Degager la piste

            //SI la durée de sortie de piste n'est toujours pas écoulée, on la diminue
            if(m_duree_sortie_piste > 0)
            {
                m_duree_sortie_piste--; //Décrémentation de la phase de sortie de piste
            }

            //cout << "Degagement de la piste" << endl;
        break;

    }
    return rendu;
}
/////////////////////////////////////////////////////////////////////////////



//Méthode de récupération des informations de l'aéroport de départ
void Avion::recuperationInfosAeroportD(Aeroport *aeroportDepart)
{
    //Récupération de l'ensemble des informations concernant les UT et la localisation de l'aéroport de départ du futur trajet
    m_duree_prepraration = aeroportDepart->get_delai_attente_sol();
    m_duree_roulage = aeroportDepart->get_temps_acces_pistes();
    m_duree_decollage = aeroportDepart->get_temps_decollage_atterissage();
    m_coordDepart.first = aeroportDepart->get_position().get_coord_x();
    m_coordDepart.second = aeroportDepart->get_position().get_coord_y();
    m_coord.set_coord_x(aeroportDepart->get_position().get_coord_x());
    m_coord.set_coord_y(aeroportDepart->get_position().get_coord_y());

    //Réinitialisation de l'action en cours
    m_action_en_cours = 0;

    //Obtention du nom de l'aéroport de départ
    m_aeroportD = aeroportDepart->get_nom();
}


//Méthode d'actualisation de la position de l'avion en vol
void Avion::actualisationPositionVol()
{
    //Actualisation des coordonnées
    set_coord(get_coord().get_coord_x() + m_espacementAffichageX*1.05, getAArete() * (get_coord().get_coord_x() + m_espacementAffichageX*1.05) + getBArete());
}


//Méthode d'enregistrement des informations des aéroports de départ et d'arrivée
void Avion::parametrer_nouveau_vol(float xD, float yD, float xA, float yA)
{
    double xAngleDroit = 0; //Position en X de l'angle droit
    double yAngleDroit = 0; //Position en Y de l'angle droit
    double DAngleDroit = 0; //Distance entre le Départ et l'angle droit
    double AAngleDroit = 0; //Distance entre l'arrivée et l'angle droit
    double tanDepart = 0; //Valeur de la tangente en Départ
    double angle; //Valeur de l'angle en Départ
    double angleTraj;
    double hypothenuse = 0; //Valeur de l'hypothénuse du triangle rectangle
    double x = 0;
    double y = 0;
    float a = 0; //Coefficient directeur de la droite
    float b = 0; //Constante de la droite

    //Actualsiation de la position de l'angle droit
    xAngleDroit = xD;
    yAngleDroit = yA;

    //SI l'arrivée est au dessus du point de départ
    if(yAngleDroit < yD)
    {
        DAngleDroit = (double)yD - (double)yAngleDroit;
    }
    else //SINON, l'arrivée est en dessous
    {
        DAngleDroit = (double)yAngleDroit - (double)yD;
    }

    //SI le départ est à gauche de l'arrivée
    if(xAngleDroit < xA)
    {
        AAngleDroit = (double)xA - (double)xAngleDroit;
    }
    else //SINON, le départ est à droite
    {
        AAngleDroit = (double)xAngleDroit - (double)xA;
    }

    //Calcul de l'angle D
    tanDepart = (double)AAngleDroit/(double)DAngleDroit;
    angle = atan(tanDepart);
    angle = angle*57.29;
    angleTraj = atan(tanDepart);

    //Quatre cas de figures d'orientation du triangle rectangle
    if(xAngleDroit < xA && yAngleDroit > yD)
    {
        /*
        D
        C   A
        */

        angle = 180 - angle;
    }
    else if(xAngleDroit > xA && yAngleDroit > yD)
    {
        /*
            D
        A   C
        */

        angle = 180 + angle;
    }
    else if (xAngleDroit < xA && yAngleDroit < yD)
    {
        /*
        C   A
        D
        */

        angle = angle;
    }
    else if(xAngleDroit > xA && yAngleDroit < yD)
    {
        /*
        A   C
            D
        */

        angle = 180 + (180 - angle);
    }

    angle = angle*255/360; //Produit en croix car l'angle doit être entre 0 et 255
    m_angle = angle; //Récupération de l'angle comme attribut de l'avion

    //Calcul de la progression en X et en Y à chaque UT
    hypothenuse = sqrt(DAngleDroit * DAngleDroit + AAngleDroit * AAngleDroit) / (float)m_duree_vol; //Calcul de l'hytpothénuse du triangle rectangle
    angle = angle * 360.0 / 255.0; //Calcul de l'angle
    y = cos(angleTraj) * hypothenuse; //Calcul du décalage en Y à chaque UT
    x = sqrt(hypothenuse * hypothenuse - y * y); //Calcul du décalage en X à chaque UT

    a = (yA - yD) / (xA - xD);
    b = yA - a * xA;

    //Quatre cas de figures d'orientation du triangle rectangle régalnt le signe de décalage en X et en Y
    if(xAngleDroit < xA && yAngleDroit > yD)
    {
        /*
        D
        C   A
        */

        x = x;
        y = -y;
    }
    else if(xAngleDroit > xA && yAngleDroit > yD)
    {
        /*
            D
        A   C
        */

        x = -x;
        y = -y;
    }
    else if (xAngleDroit < xA && yAngleDroit < yD)
    {
        /*
        C   A
        D
        */

        x = x;
        y = -y;
    }
    else if(xAngleDroit > xA && yAngleDroit < yD)
    {
        /*
        A   C
            D
        */

        x = -x;
        y = -y;
    }

    //Affectation des décalages en X et en Y actualisés
    m_espacementAffichageX = x;
    m_espacementAffichageY = y;

    //Affectation des caractéristiques de trajectoire
    setAArete(a);
    setBArete(b);
}


void Avion::recuperationInfosAeroportA(Aeroport &aeroportArrivee)
{
    //Récupération de l'ensemble des informations concernant les UT et la localisation de l'aéroport d'arrivée du futur trajet
    m_duree_atterrissage = aeroportArrivee.get_temps_decollage_atterissage();
    m_duree_sortie_piste = aeroportArrivee.get_temps_acces_pistes();
    m_duree_boucle_attente = aeroportArrivee.get_duree_boucle_attente_vol();
    m_coordArrivee.first = aeroportArrivee.get_position().get_coord_x();
    m_coordArrivee.second = aeroportArrivee.get_position().get_coord_y();

    //Obtention du nom de l'aéroport d'arrivée
    m_aeroportA = aeroportArrivee.get_nom();

    //Affectation de la durée de vol
    for(int i=0 ; i<int(aeroportArrivee.get_distance_aeroports().size()) ; i++) //Boucle de parcours de l'ensemble des aéroports voisins à celui de départ
    {
        //SI il s'agit de l'aéroport d'arrivée
        if(aeroportArrivee.get_distance_aeroports()[i].first == m_aeroportD)
        {
            //On calcule et on affecte la durée du vol en UT
            m_duree_vol = (((float)aeroportArrivee.get_distance_aeroports()[i].second / (float)m_vitesse) * 60.0) / (float)UT;
            break;
        }
    }
}

/////////////////////////////////////////////
bool Avion::consommer_kerosene()
{
    float consommation_UT =0;
    bool rendu = false;

    if(m_etat_reservoir == false)
    {//reservoir intact

        consommation_UT = (m_consommation * m_vitesse)/600; //calcul de la consommation à l'heure l/h puis divisé par 6 pour consommation à UT
    }
    else if(m_etat_reservoir == true)
    {//reservoir percé
        consommation_UT = (m_consommation * m_vitesse)/300;
    }
    m_quantite_kerosene = m_quantite_kerosene-consommation_UT;

    if(m_quantite_kerosene<100)
    {
        cout << "crash du vol " << m_immatriculation << endl;
        rendu = true;
    }
    return rendu;
}
///////////////////////////////////////////////////




void Avion::enregistrer_informations()
{
    const string root = "data/information_avion/";

    const string extension = ".txt";
    const string command_1 = "cd ";
    const string command_2 = "type nul>";
    const string separation = " && ";

    string command_line = command_1 + root + separation +command_2 + m_immatriculation + extension;
    string adresse = root + m_immatriculation + extension;

    system((char *)command_line.c_str());

    std::ofstream fichier;
    fichier.open((char*)adresse.c_str());

    if(!fichier)
    {}
    else
    {
        fichier << m_immatriculation << "\n";
        fichier << m_modele << "\n";
        fichier << m_type_vol << " courier" << "\n";
        fichier << "Altitude : " << m_altitude << "\n";
        fichier << "Consommation : " << m_consommation << "\n";
        fichier << "Vitesse max: " << m_vitesse<< "\n";
        fichier << "Quantite de kerosene : " << m_quantite_kerosene<< "\n";


        fichier << "\n" << "\n" << "Liste des aeroports dans lesquels l'avion est passe : "<< "\n";

        for(size_t t=0; t<m_liste_escales.size(); t++)
        {
            fichier << m_liste_escales[t] << "\n";
        }
        switch(m_action_en_cours)
        {


        case 0: //Preparation au vol

            fichier << "\n" << "Avion en preparation de vol au moment de l'enregistrement "<< "\n";
        break;


        case 1: //Roulage jusqu'à la piste

            fichier << "\n" << "Avion en roulage vers la piste au moment de l'enregistrement "<< "\n";

        break;


        case 2: //Decollage

            fichier << "\n" << "Avion en phase de décollage au moment de l'enregistrement "<< "\n";
        break;


        case 3: //Vol

            fichier << "\n" << "Avion en vol au moment de l'enregistrement "<< "\n";
        break;


        case 4: //Boucle d'attente

            fichier << "\n" << "Avion en boucle d'attente au moment de l'enregistrement "<< "\n";
        break;

        case 5: //Atterissage
            fichier << "\n" << "Avion en phase d'atterrissage au moment de l'enregistrement "<< "\n";
        break;

        case 6: //Degager la piste

            fichier << "\n" << "Avion phase de degagement de piste au moment de l'enregistrement "<< "\n";
        break;
    }
        fichier.close();
    }
}

void Avion::set_liste_escales(string escale)
{
    m_liste_escales.push_back(escale);
}
