#include "H_header.h"
#include "CH_avion.h"
#include "CH_ressources.h"
#include "CH_simulateur.h"

using namespace std;


int main()
{
    //Initialisation d'Allegro
    initAllegro();

    //D�claration des variables
    bool done = false; //Indique la sortie ou non du programme
    bool indicClic = false; //Permet de savoir quand un clic est maintenu afin d'�viter le d�filement des menus
    int modeSimulation = -1; //Contient le mode de simulation saisi par l'utilisateur dans le menu PRINCIPAL
    int envergureSimulation = -1; //Contient l'envergure de la saimulation saisie par l'utilisateur dans le menu ENVERGURE

    //Cr�ation de l'objet contenant l'ensemble des ressources
    Ressources motherShip;

    //Cr�ation du simulateur
    Simulateur proteus;

    //Boucle de d�roulement du programme
    do
    {
        done = menuPrincipal(motherShip, indicClic, modeSimulation, envergureSimulation);

        if(!done)
        {

            //Cr�ation des informations n�ccessaires � la simulation
            proteus.initSimulateur(modeSimulation, envergureSimulation);

            motherShip.animationAvion(motherShip.getBIT(0));

            //proteus.afficher_aeroports();

            proteus.deroulementGlobal(motherShip, indicClic, done);
        }

    }
    while(!done);


    //Suppression de l'ensemble des ressources du programme
    motherShip.~Ressources();

    cout << " ----- SUPPRESSION DES DONNEES REALISEE ----- " << endl;
    cout << " || FIN || " << endl;

    exit(EXIT_SUCCESS);
}
END_OF_MAIN();

