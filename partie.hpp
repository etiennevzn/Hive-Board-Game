#ifndef PARTIE_HPP
#define PARTIE_HPP

#include <iostream>
#include "plateau.hpp"
#include "joueur.hpp"
#include <vector>

using namespace std;

class Memento {
    friend class Partie;
    int tourActuel;
    Joueur joueur1;
    Joueur joueur2;
    Joueur* joueurCourant;
    Plateau plateau;

    Memento(int t, const Joueur& j1, const Joueur& j2, Joueur* jc, const Plateau& p)
        : tourActuel(t), joueur1(j1), joueur2(j2), joueurCourant(jc), plateau(p) {}
};

class Partie {
    Plateau plateau;
    int tourActuel;
    Joueur joueur1;
    Joueur joueur2;
    Joueur* joueurCourant;
    vector<Memento> historique;

public:
    Partie(const Joueur& j1, const Joueur& j2, const Plateau& p, int t = 0)
        : joueur1(j1), joueur2(j2), tourActuel(t), joueurCourant(&joueur1), plateau(p) {}

    void jouer();
    void afficherMouvementsPossibles(Position pos);
    void nextTurn();
    Memento sauvegarder();
    void restaurer(const Memento& memento);
};

#endif