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
    unordered_map<string, int> nb_pieces;
public :
    Joueur(const Couleur& c) : couleur(c), pieces() {};
    Couleur getCouleur() const { return couleur; }
    const vector<Piece*>& getPieces() const { return pieces; }
    bool poserPiece(char pieceType, Position pos, Plateau& plateaun,int tourActuel);
    void print_piece_left();
};

#endif