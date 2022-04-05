#include "H_header.h"
#include "CH_ressources.h"
#include <iostream>

using namespace std;


//Constructeur par d�faut
Ressources::Ressources()
{
    cout << " ----- CHARGEMENT DES RESSOURCES ----- " << endl;

    //Initialisation du vecteur de BITMAPS
    initTabBIT(m_tabBIT);

    //Initialisation du vecteur de FONTS
    initTabFONT(m_tabFONT);

    //Initialisation du vecteur de l'animation de l'AVION
    initAnimAvion(m_animAvion);

    cout << " ----- CHARGEMENT DES RESSOURCES TERMINE ----- " << endl << endl;
}

//Destructeur
Ressources::~Ressources()
{
    cout << " ----- DESTRUCTION DES RESSOURCES ----- " << endl;

    //Destruction de l'ensemble des BITMAPS
    cout << "     ----- Destruction des BITMAPS" << endl;
    for(int i=0 ; i<NBRE_BITMAPS ; i++)
    {
        destroy_bitmap(m_tabBIT[i]);
    }
    cout << "          -> FIN" << endl;

    //Destruction de l'ensemble des FONTS
    cout << "     ----- Destruction des FONTS" << endl;
    for(int i=0 ; i<NBRE_POLICES ; i++)
    {
        destroy_font(m_tabFONT[i]);
    }
    cout << "          -> FIN" << endl;

    //Destruction de l'ensemble des BITMAPS de l'AVION
    cout << "     ----- Destruction de l'animation de l'AVION" << endl;
    for(int i=0 ; i<12 ; i++)
    {
        destroy_bitmap(m_animAvion[i]);
    }
    cout << "          -> FIN" << endl;

    cout << " ----- FIN DE LA DESTRUCTION DES RESSOURCES ----- " << endl << endl;
}


//Getter du vecteur de BITMAPS
vector<BITMAP*> Ressources::getTabBIT()
{
    return m_tabBIT;
}

//Getter du vecteur de FONTS
vector<FONT*> Ressources::getTabFONT()
{
    return m_tabFONT;
}

//Getter d'une BITMAP
BITMAP* Ressources::getBIT(int indiceBit)
{
    return m_tabBIT[indiceBit];
}

//Getter d'une FONT
FONT* Ressources::getFONT(int indiceFont)
{
    return m_tabFONT[indiceFont];
}



    //////////////
    // METHODES //
    //////////////

//M�thode de l'animation de l'AVION
void Ressources::animationAvion(BITMAP* doubleBuffer)
{
    bool derniereBitmap = false;
    int compteurImage = 0; //Permet de compter le nombre d'imgaes
    int temporisation = 30; //Permet de r�gler la vitesse de l'animation
    int imageCourante = 0; //Permet d'afficher l'image � afficher

    while(!key[KEY_ENTER])
    {
        //R�initialisation du double buffer
        clear_bitmap(doubleBuffer);

        if(derniereBitmap == false)//Si l'indicateur vaut 0, affichage de l'animation
        {
            compteurImage++;//Incr�menter imageCourante sur temporisation

            blit(m_animAvion[imageCourante],doubleBuffer,0,0,0,0,SCREEN_W, SCREEN_H); //Affichage de l'image

            if(compteurImage >= temporisation)//Quand l'indice de l'image courante arrive � temporisation, on recommence la s�quence � partir de 0
            {
                compteurImage = 0;
                imageCourante++;

                if(imageCourante >= 11)//Si on arrive � la fin de la s�quence, l'indice passe � 1
                {
                    derniereBitmap = true;
                }
            }
        }
        else //Sinon, affichage de la derni�re BITMAP en continu
        {
            blit(m_animAvion[10],doubleBuffer,0,0,0,0,SCREEN_W, SCREEN_H);
        }

        //Affichage du doubleBuffer
        blit(doubleBuffer, screen, 0,0,0,0,SCREEN_W,SCREEN_H);//Affichage du double buffer
    }
}
