#ifndef HEADER_UTILITAIRES_H_INCLUDED
#define HEADER_UTILITAIRES_H_INCLUDED

//Définition de la classe contenant un couple de coordonnées
class Coord
{
    private :
        float m_coord_x;
        float m_coord_y;

    public :
        Coord();
        Coord(float _coord_x, float _coord_y);

        ~Coord();

        float get_coord_x();
        float get_coord_y();

        void set_coord_x(float _x);
        void set_coord_y(float _y);

        void afficher_coord();

};

#endif // HEADER_UTILITAIRES_H_INCLUDED
