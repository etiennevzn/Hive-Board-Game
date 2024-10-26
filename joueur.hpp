#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include <iostream>
#include <string>
#include "piece.hpp"

using namespace std;
class Joueur{
    Couleur couleur;
    vector<Piece*> pieces;
    
    
public :
    Joueur(Couleur couleur)
        :  couleur(couleur) {
        
        pieces.push_back(new Reine(Position(0, 0), couleur));
        pieces.push_back(new Scarabee(Position(0, 0), couleur));
        pieces.push_back(new Araignee(Position(0, 0), couleur));
        pieces.push_back(new Sauterelle(Position(0, 0), couleur));
        pieces.push_back(new Fourmi(Position(0, 0), couleur));
        
    }
    ~Joueur() {
        
        for (Piece* piece : pieces) {
            delete piece;
        }
    }
    Couleur getCouleur() const { return couleur; }
    const vector<Piece*>& getPieces() const { return pieces; }
    void print_piece_left();
};

#endif