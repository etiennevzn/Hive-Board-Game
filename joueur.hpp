#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include <iostream>
#include <string>
#include "piece.hpp"
#include "plateau.hpp"

using namespace std;
class Joueur{
    Couleur couleur;
    vector<Piece*> pieces;
    
    
public :
    Joueur(Couleur couleur)
        :  couleur(couleur) {
        
        pieces.push_back(new Reine(Position(0, 0), couleur));
        pieces.push_back(new Scarabee(Position(1, 0), couleur));
        pieces.push_back(new Araignee(Position(2, 0), couleur));
        pieces.push_back(new Sauterelle(Position(3, 0), couleur));
        pieces.push_back(new Fourmi(Position(4, 0), couleur));
        
    }
    
    Couleur getCouleur() const { return couleur; }
    const vector<Piece*>& getPieces() const { return pieces; }
    bool poserPiece(char pieceType, Position pos, Plateau& plateau);
    void print_piece_left();
};

#endif