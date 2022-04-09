#include "H_header.h"
#include "CH_avion&aeroport&route.h"
#include "CH_ressources.h"
#include "CH_simulateur.h"
#include <time.h>

using namespace std;


int main()
{
    //Permet les générations aléatoires
    srand(time(NULL));

    //Initialisation d'Allegro
    initAllegro();

    //Déclaration des variables
    bool done = false; //Indique la sortie ou non du programme
    bool indicClic = false; //Permet de savoir quand un clic est maintenu afin d'éviter le défilement des menus
    int modeSimulation = -1; //Contient le mode de simulation saisi par l'utilisateur dans le menu PRINCIPAL
    int envergureSimulation = -1; //Contient l'envergure de la saimulation saisie par l'utilisateur dans le menu ENVERGURE

    //Création de l'objet contenant l'ensemble des ressources
    Ressources motherShip;

    //Création du simulateur
    Simulateur proteus;

    //Boucle de déroulement du programme
    do
    {
        //Menu principal
        done = menuPrincipal(motherShip, indicClic, modeSimulation, envergureSimulation);

        //SI le programme doit tourner
        if(!done)
        {
            //Création des informations néccessaires à la simulation
            proteus.initSimulateur(modeSimulation, envergureSimulation);

            //Lancement de l'animation de l'avion
            motherShip.animationAvion(motherShip.getBIT(0));

            //Lancement du déroulement de la simulation
            proteus.deroulementGlobal(motherShip, indicClic, done);
        }

    }
    while(!done); //Tant que le progrtamme n'est pas terminé


    //Suppression de l'ensemble des ressources du programme
    motherShip.~Ressources();

    cout << " ----- SUPPRESSION DES DONNEES REALISEE ----- " << endl;
    cout << " || FIN || " << endl;

    exit(EXIT_SUCCESS);
}
END_OF_MAIN();

