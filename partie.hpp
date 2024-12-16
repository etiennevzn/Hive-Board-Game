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
    Partie(const Joueur& j1, const Joueur& j2, const Plateau& p, int t = 0);
    void afficherMouvementsPossibles(Position pos, Couleur couleur); //pas const car on utilise wouldSplitHive dedans qui n'est pas const
    void printPossiblePlays(Joueur* joueurCourant); //pareil que la méthode au dessus
    void playTurn();
    Memento sauvegarder();
    void restaurer(const Memento& memento);
    void play();


    void saveToFile(ofstream& outFile) const;
    void loadFromFile(ifstream& inFile);
     
};

#endif