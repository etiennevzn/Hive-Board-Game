#include "joueur.hpp"
#include "plateau.hpp"
#include "piece.hpp"
#include <iostream>

Joueur::Joueur(Couleur c) : couleur(c), nbReine(1), nbAraignees(2), nbScarabes(2), nbFourmis(3), nbSauterelles(3) {}

bool Joueur::isValidMove(const Position& from, const Position& to, const Plateau& plateau) const {
    if (!plateau.isPositionOccupied(from)) {
        return false;
    }

    Piece* piece = plateau.getPieceAtPosition(from);
    if (piece == nullptr || piece->getCouleur() != couleur) {
        return false;
    }

    if (!piece->isValidMove(to, plateau.getPlateauMap())) {
        return false;
    }

    return true;
}

bool Joueur::isValidPlacement(const Piece* piece, const Position& pos, const Plateau& plateau) const {
    if (plateau.isPositionOccupied(pos)) {
        return false;
    }

    bool adjacentToOwnPiece = false;
    bool adjacentToAnyPiece = false;
    for (const Position& adj : pos.getAdjacentCoordinates()) {
        if (plateau.isPositionOccupied(adj)) {
            Piece* adjacentPiece = plateau.getPieceAtPosition(adj);
            if (adjacentPiece != nullptr) {
                adjacentToAnyPiece = true;
                if (adjacentPiece->getCouleur() == couleur) {
                    adjacentToOwnPiece = true;
                }
            }
        }
    }

    if (!adjacentToAnyPiece && pieces.empty()) {
        return true;
    }

    if (!adjacentToAnyPiece) {
        return false;
    }

    if (pieces.empty() && adjacentToOwnPiece) {
        return false;
    }

    if (piece->getType() == "Reine" && pieces.size() >= 3) {
        return false;
    }

    return true;
}

void Joueur::movePiece(const Position& from, const Position& to, Plateau& plateau) {
    Piece* piece = plateau.getPieceAtPosition(from);
    if (piece && piece->getCouleur() == couleur) {
        plateau.addPiece(piece, to);
        plateau.removePiece(from);
    }
}

bool Joueur::poserPiece(Piece* piece, const Position& pos, Plateau& plateau) {
    if ((piece->getType() == "Reine" && nbReine == 0) ||
        (piece->getType() == "Araignee" && nbAraignees == 0) ||
        (piece->getType() == "Scarabee" && nbScarabes == 0) ||
        (piece->getType() == "Fourmi" && nbFourmis == 0) ||
        (piece->getType() == "Sauterelle" && nbSauterelles == 0)) {
        std::cout << "Vous n'avez plus de pieces de ce type disponibles." << std::endl;
        delete piece;
        return false;
    }

    if (isValidPlacement(piece, pos, plateau)) {
        pieces.push_back(piece);
        plateau.addPiece(piece, pos);

        if (piece->getType() == "Reine") nbReine--;
        else if (piece->getType() == "Araignee") nbAraignees--;
        else if (piece->getType() == "Scarabee") nbScarabes--;
        else if (piece->getType() == "Fourmi") nbFourmis--;
        else if (piece->getType() == "Sauterelle") nbSauterelles--;

        return true;
    }
    else {
        std::cout << "Impossible de poser la piece a cette position. Reessayez." << std::endl;
        delete piece;
        return false;
    }
}
