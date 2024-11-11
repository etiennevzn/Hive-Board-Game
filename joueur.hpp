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
    Joueur(const Couleur& c) : couleur(c), pieces(), nb_pieces{{"R", 0}, {"F", 0}, {"S", 0}, {"H", 0}, {"C", 0}, {"A", 0}, {"M", 0}} {};
    Couleur getCouleur() const { return couleur; }
    const vector<Piece*>& getPieces() const { return pieces; }
    void jouerTour();
    bool poserPiece(char pieceType, Position pos, Plateau& plateaun,int tourActuel);
    void print_piece_left();
    vector<Position> get_liste_placements(const Plateau& plateau);
};

#endif