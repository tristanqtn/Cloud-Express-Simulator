#ifndef CH_RESSOURCES_H_INCLUDED
#define CH_RESSOURCES_H_INCLUDED

#include <allegro.h>
#include <vector>

class Ressources {
    private :
        std::vector<BITMAP*> m_tabBIT;
        std::vector<FONT*> m_tabFONT;
        std::vector<BITMAP*> m_animAvion;

    public :
        Ressources();
        ~Ressources();

        std::vector<BITMAP*> getTabBIT();
        std::vector<FONT*> getTabFONT();
        BITMAP* getBIT(int indiceBit);
        FONT* getFONT(int indiceFont);

        void animationAvion(BITMAP* doubleBuffer);
};

#endif // CH_RESSOURCES_H_INCLUDED
