#include "plateau.hpp"
#include <iostream>
#include <iomanip>  // Pour std::setw

void Plateau::addPiece(Piece* piece, const Position& pos) {
    plateau[pos].push_back(piece);
}

void Plateau::removePiece(const Position& pos) {
    if (plateau.count(pos) && !plateau.at(pos).empty()) {
        plateau[pos].pop_back();
    }
}

bool Plateau::isPositionOccupied(const Position& pos) const {
    return plateau.count(pos) && !plateau.at(pos).empty();
}

Piece* Plateau::getPieceAtPosition(const Position& pos) const {
    if (isPositionOccupied(pos)) {
        return plateau.at(pos).back();
    }
    return nullptr;
}

const std::unordered_map<Position, std::vector<Piece*>>& Plateau::getPlateauMap() const {
    return plateau;
}

void Plateau::print_board() const {
    const int taille = 10;
    const char vide = '.';

    char grille[taille][taille];
    for (int i = 0; i < taille; ++i) {
        for (int j = 0; j < taille; ++j) {
            grille[i][j] = vide;
        }
    }

    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        const std::vector<Piece*>& pieces = entry.second;

        if (!pieces.empty()) {
            const Piece* topPiece = pieces.back();
            int x = pos.getColonne();
            int y = pos.getLigne();

            if (x >= 0 && x < taille && y >= 0 && y < taille) {
                grille[y][x] = topPiece->getInitial();
            }
        }
    }

    std::cout << "  ";
    for (int j = 0; j < taille; ++j) {
        std::cout << std::setw(2) << j;
    }
    std::cout << std::endl;

    for (int i = 0; i < taille; ++i) {
        std::cout << std::setw(2) << i;
        for (int j = 0; j < taille; ++j) {
            std::cout << std::setw(2) << grille[i][j];
        }
        std::cout << std::endl;
    }
}
