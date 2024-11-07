#include "piece.hpp"
#include <unordered_set>

bool Fourmi::isValidMove(const Position& to, const std::unordered_map<Position, std::vector<Piece*>, std::hash<Position>>& plateau) const {
    std::unordered_set<Position, std::hash<Position>> visited;  // Initialise `visited`
    return canSlideTo(getPosition(), to, plateau, visited);
}

bool Fourmi::canSlideTo(const Position& from, const Position& to,
    const std::unordered_map<Position, std::vector<Piece*>, std::hash<Position>>& plateau,
    std::unordered_set<Position, std::hash<Position>>& visited) const {
    if (from == to) {
        return true;  // Atteint la destination
    }

    visited.insert(from);  // Marque la position comme visitée

    for (const Position& adj : from.getAdjacentCoordinates()) {
        if (visited.find(adj) == visited.end() &&
            (plateau.find(adj) == plateau.end() || plateau.at(adj).empty())) {
            if (canSlideTo(adj, to, plateau, visited)) {
                return true;
            }
        }
    }
    return false;
}
