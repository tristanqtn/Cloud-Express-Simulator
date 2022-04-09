#include "H_header.h"

using namespace std;


//Sous-programme g�n�rant une cha�ne al�atoire de caract�res
string chaine_aleatoire(int longueur)
{
    const string CARACTERES = "AZERTYUIOPQSDFGHJKLMWXCVBN1234567890"; //Cr�ation de la cha�ne dans laquelle piocher

    string chaine_generee = ""; //Cha�ne al�atoire sortante

    //Boucle de s�lection al�atoire d'un caract�re
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
