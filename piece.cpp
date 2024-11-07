#include "piece.hpp"
#include <vector>

string toString(Couleur couleur) {
    switch (couleur) {
        case Noir: return "Noir";
        case Blanc: return "Blanc";
        default: return "Inconnu";
    }
}

vector<Position> Position::getAdjacentCoordinates() const {
    std::vector<Position> adjacents;
    
    adjacents.push_back(Position(q, r - 1));     // Up
    adjacents.push_back(Position(q, r + 1));     // Down
    adjacents.push_back(Position(q - 1, r + 1)); // Up-Left
    adjacents.push_back(Position(q + 1, r - 1)); // Down-Right
    adjacents.push_back(Position(q + 1, r + 1)); // Up-Right
    adjacents.push_back(Position(q - 1, r - 1)); // Down-Left
    return adjacents;
}

bool Position::isAdjacent(const Position& other) const {
    vector<Position> adjacents = getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (adj == other) {
            return true;
        }
    }
    return false;
}

