#include "H_header.h"

using namespace std;


//Sous-programme générant une chaîne aléatoire de caractères
string chaine_aleatoire(int longueur)
{
    const string CARACTERES = "AZERTYUIOPQSDFGHJKLMWXCVBN1234567890"; //Création de la chaîne dans laquelle piocher

    string chaine_generee = ""; //Chaîne aléatoire sortante

    //Boucle de sélection aléatoire d'un caractère
    for(int i=0; i<longueur; i++)
    {
        int aleatoire = rand()%36;
        chaine_generee += CARACTERES[aleatoire];
    }

    return chaine_generee;
}


//Sous-programme de conversion d'une string en un char* (pour pouvoir afficher via textprintf)
const char* convertisseurStringChar(string motAConvertir)
{
    //Conversion de la string
    const char *chaineAAfficher = motAConvertir.c_str();

    //Envoi du char*
    return chaineAAfficher;
}
