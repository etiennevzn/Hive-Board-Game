#pragma once
#include "plateau.hpp"
#include "joueur.hpp"

class Partie {
    Plateau plateau;
    Joueur joueur1;
    Joueur joueur2;
    Joueur* joueurCourant;
    int tourActuel;

public:
    Partie() : joueur1(Noir), joueur2(Blanc), joueurCourant(&joueur1), tourActuel(0) {}
    void jouer();
    void nextTurn();
    bool conditionsDeVictoire();
    bool conditionsDeMatchNul();
private:
    bool isPieceSurrounded(const Position& pos) const;  
};
