#include "position.hpp"
#include <algorithm>

bool Position::isAdjacent(const Position& other) const {
    std::vector<Position> adjacents = getAdjacentCoordinates();
    return std::any_of(adjacents.begin(), adjacents.end(), [&other](const Position& adj) {
        return adj == other;
        });
}

std::vector<Position> Position::getAdjacentCoordinates() const {
    return {
        Position(q, r - 1),      // Haut
        Position(q, r + 1),      // Bas
        Position(q - 1, r),      // Gauche
        Position(q + 1, r),      // Droite
        Position(q - 1, r - 1),  // Diagonale Haut-Gauche
        Position(q + 1, r + 1),  // Diagonale Bas-Droite
        Position(q - 1, r + 1),  // Diagonale Bas-Gauche
        Position(q + 1, r - 1)   // Diagonale Haut-Droite
    };
}
