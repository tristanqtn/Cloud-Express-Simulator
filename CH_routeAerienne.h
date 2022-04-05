#ifndef CH_ROUTEAERIENNE_H_INCLUDED
#define CH_ROUTEAERIENNE_H_INCLUDED


#include "H_header.h"
#include "CH_avion.h"
#include "CH_aeroport.h"


//Défintion de la classe Route Aérienne
class RouteAerienne
{
    private :

        std::vector <std::vector <int>> m_etatRoute;

        std::vector <Aeroport*> m_aeroportsRelies;
        std::vector <Avion*> m_avionsPresents;

        int m_longueur;

    public :
        RouteAerienne(Aeroport aeroport1, Aeroport aeroport2, int longueur);
        RouteAerienne();
        ~RouteAerienne();

        Aeroport* getAeroport(int indiceAeroport);
        void setLongueur(int longueur);

        void initAeroports(Aeroport &aeroport1, Aeroport &aeroport2);
        void ajoutAvion(Avion &avionAAjouter);
        void afficherInfos();
};

#endif // CH_ROUTEAERIENNE_H_INCLUDED
