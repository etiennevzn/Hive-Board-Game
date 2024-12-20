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
    bool isIa;
public :
    Joueur(const Couleur& c, bool ia = false) : couleur(c), pieces(), nb_pieces{{"R", 0}, {"F", 0}, {"S", 0}, {"H", 0}, {"C", 0}, {"A", 0}, {"M", 0}}, isIa(ia){};
    Couleur getCouleur() const { return couleur; }
    const vector<Piece*>& getPieces() const { return pieces; }
    const unordered_map<string, int> getNbPieces() const { return nb_pieces; }
    //void jouerTour(); a voir si c'est utile
    bool poserPiece(char pieceType, Position pos, Plateau& plateau,int tourActuel);
    void print_piece_left();
    vector<Position> get_liste_placements(const Plateau& plateau);
    bool getIsIa() const {return isIa;}
    bool isPieceTypeAvailable(const string& pieceType) const;
    bool playTurnIA(Plateau& plateau, int tourActuel);

    void save(ofstream& out) const;
    void load(ifstream& in, const Plateau& plateau); 

    ~Joueur() = default;
};


#endif