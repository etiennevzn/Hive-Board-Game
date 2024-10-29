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
    Partie(const Joueur& j1, const Joueur& j2,const Plateau& p,int t=0)
        : joueur1(j1), joueur2(j2), tourActuel(t), joueurCourant(&joueur1),plateau(p){}

    void jouer();
    void dfs(Position pos, unordered_set<Position, hash<Position>>& visited);
    bool wouldSplitHive(Position from, Position to);
    void afficherMouvementsPossibles(Position pos);
    bool isHiveConnected();
    
    void nextTurn() {
        tourActuel++;
        joueurCourant = (joueurCourant == &joueur1) ? &joueur2 : &joueur1;
    }
};










#endif