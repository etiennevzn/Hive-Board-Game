#pragma once
#include <vector>
#include "piece.hpp"
#include "position.hpp"
#include "plateau.hpp"

class Joueur {
    Couleur couleur;
    std::vector<Piece*> pieces;

    int nbReine;
    int nbAraignees;
    int nbScarabes;
    int nbFourmis;
    int nbSauterelles;

public:
    Joueur(Couleur c);

    Couleur getCouleur() const { return couleur; }

    int getNbReine() const { return nbReine; }
    int getNbAraignees() const { return nbAraignees; }
    int getNbScarabes() const { return nbScarabes; }
    int getNbFourmis() const { return nbFourmis; }
    int getNbSauterelles() const { return nbSauterelles; }

    bool isValidMove(const Position& from, const Position& to, const Plateau& plateau) const;
    bool isValidPlacement(const Piece* piece, const Position& pos, const Plateau& plateau) const;

    void movePiece(const Position& from, const Position& to, Plateau& plateau);
    bool poserPiece(Piece* piece, const Position& pos, Plateau& plateau);
};
