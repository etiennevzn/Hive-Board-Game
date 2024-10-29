#include "piece.hpp"
#include <vector>

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

