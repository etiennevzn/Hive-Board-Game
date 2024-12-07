#ifndef PARTIE_HPP
#define PARTIE_HPP

#include <iostream>
#include "plateau.hpp"
#include "joueur.hpp"
#include <vector>

using namespace std;

class Memento {
    friend class Partie;
    Plateau plateau;
    int tourActuel;
    Joueur joueurs[2];
    Joueur* joueurCourant;

    Memento(int t, const Joueur& j1, const Joueur& j2, Joueur* jc, const Plateau& p)
        : tourActuel(t), joueurs{j1,j2}, joueurCourant(jc), plateau(p) {}
};

class Partie {
    Plateau plateau;
    int tourActuel;
    Joueur joueurs[2];
    Joueur* joueurCourant;
    vector<Memento> historique;
public:
    Partie(const Joueur& j1, const Joueur& j2, const Plateau& p, int t = 0)
        : joueurs{j1, j2}, tourActuel(t), joueurCourant(&joueurs[0]), plateau(p) {}
    void jouer();
    void afficherMouvementsPossibles(Position pos)const;
    void printPossiblePlays(Joueur* joueurCourant)const;
    void nextTurn();
    Memento sauvegarder();
    void restaurer(const Memento& memento);
};

#endif