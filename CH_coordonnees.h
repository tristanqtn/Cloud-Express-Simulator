#ifndef HEADER_UTILITAIRES_H_INCLUDED
#define HEADER_UTILITAIRES_H_INCLUDED

//Définition de la classe contenant un couple de coordonnées
class Coord
{
    private :
        int m_coord_x;
        int m_coord_y;

    public :
        Coord();
        Coord(int _coord_x, int _coord_y);

        ~Coord();

        int get_coord_x();
        int get_coord_y();

        void set_coord_x(int _x);
        void set_coord_y(int _y);

        void afficher_coord();

};

#endif // HEADER_UTILITAIRES_H_INCLUDED
