#ifndef JOUEUR_H
#define JOUEUR_H

#include <QObject>
#include <vector>
#include <unordered_map>
#include "piece.hpp"
#include "plateau.h"

class Joueur {
    Couleur couleur;
    std::vector<Piece*> pieces;
    std::unordered_map<std::string, int> nb_pieces;
    bool isIa;

public:
    Joueur(const Couleur& c, bool ia = false) : couleur(c), pieces(), nb_pieces{{"R", 0}, {"F", 0}, {"S", 0}, {"H", 0}, {"C", 0}, {"A", 0}, {"M", 0}}, isIa(ia) {};
    Couleur getCouleur() const { return couleur; }
    const vector<Piece*>& getPieces() const { return pieces; }
    const unordered_map<string, int> getNbPieces() const { return nb_pieces; }

    std::vector<Position> get_liste_placements(Plateau& plateau);
    bool poserPiece(char pieceType, Position pos, Plateau& plateau, int tourActuel);

    bool getIsIa() const {return isIa;}
    bool isPieceTypeAvailable(const string& pieceType) const;
    bool playTurnIA(Plateau& plateau, int tourActuel);

    void save(std::ofstream& out) const;
    void load(std::ifstream& in, const Plateau& plateau);

    void displayListPlacement(Plateau& plateau);


};

#endif
