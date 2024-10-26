#ifndef PARTIE_HPP
#define PARTIE_HPP

#include <iostream>
#include "plateau.hpp"
#include "joueur.hpp"
using namespace std;
class Partie{

    Plateau plateau;
    int tourActuel;
    Joueur joueur1;
    Joueur joueur2;
    Joueur* joueurCourant;
public:
    Partie(const Joueur& j1, const Joueur& j2,const Plateau& p)
        : joueur1(j1), joueur2(j2), tourActuel(0), joueurCourant(&joueur1),plateau(p){}

    void jouer();
    void nextTurn() {
        tourActuel++;
        joueurCourant = (joueurCourant == &joueur1) ? &joueur2 : &joueur1;
    }
};










#endif