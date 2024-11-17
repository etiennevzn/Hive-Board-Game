#include "piece.hpp"
#include <vector>

string toString(Couleur couleur) {
    switch (couleur) {
        case Noir: return "Noir";
        case Blanc: return "Blanc";
        default: return "Inconnu";
    }
}

std::vector<Position> Position::getAdjacentCoordinates() const {
    // Directions pour les lignes paires et impaires
    std::vector<std::pair<int, int>> oddDirections = {
        {-1, 0}, {0, -1}, {1, -1},
        {1, 0}, {1, 1}, {0, 1}
    };
    std::vector<std::pair<int, int>> evenDirections = {
        {-1, 0}, {-1, -1}, {0, -1},
        {1, 0}, {0, 1}, {-1, 1}
    };

    // Choisir les directions en fonction de la parité de r
    const auto& directions = (r % 2 == 0) ? evenDirections : oddDirections;

    // Calculer les coordonnées adjacentes
    std::vector<Position> adjacent;
    for (const auto& direction : directions) {
        adjacent.emplace_back(q + direction.first, r + direction.second);
    }

    return adjacent;
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

bool Reine::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    if (!getPosition().isAdjacent(to)) {
        return false;
    }
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    // Check if the queen can slide to the destination
    vector<Position> adjacents = getPosition().getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (adj == to) {
            return true;
        }
    }
    return false;
}

bool Scarabee::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    //pas besoin de checker si la destination est libre car il peut monter sur d'autres pièces
    if (!getPosition().isAdjacent(to)) {
        return false;
    }
    return true;
}

bool Araignee::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const {
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    // Araignee must move exactly three spaces
    if (getPosition().isAdjacent(to)) {
        return false; // Direct adjacency is not allowed
    }

    // Check if the destination is three spaces away
    unordered_set<Position, hash<Position>> visited;
    return canMoveThreeSpaces(getPosition(), to, plateau, visited, 0);
    
}

bool Araignee::canMoveThreeSpaces(const Position& from, const Position& to, const unordered_map<Position, vector<Piece*>>& plateau, unordered_set<Position, hash<Position>>& visited, int depth)const{
    // Check if we've moved exactly three steps
    if (depth == 3) {
        return from == to; // If at depth 3, we must be at the destination
    }

    visited.insert(from);
    vector<Position> adjacents = from.getAdjacentCoordinates();

    for (const Position& adj : adjacents) {
        // Continue only if this position is unvisited and either empty or not the final destination
        if (visited.find(adj) == visited.end() &&
            (plateau.find(adj) == plateau.end() || plateau.at(adj).empty())) {

            // Ensure adjacency with at least one other piece
            if (isAdjacentToPiece(adj, plateau)) {
                if (canMoveThreeSpaces(adj, to, plateau, visited, depth + 1)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Araignee::isAdjacentToPiece(const Position& pos, const unordered_map<Position, vector<Piece*>>& plateau) const {
    // Checks if the position has any neighboring pieces
    for (const Position& adj : pos.getAdjacentCoordinates()) {
        if (plateau.find(adj) != plateau.end() && !plateau.at(adj).empty()) {
            return true; // Found an adjacent piece
        }
    }
    return false;
}

bool Sauterelle::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const {
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    // Check if the destination is in a straight line
    if (getPosition().getColonne() != to.getColonne() && getPosition().getLigne() != to.getLigne() &&
        abs(getPosition().getColonne() - to.getColonne()) != abs(getPosition().getLigne() - to.getLigne())) {
        return false;
    }

    // Check if there are pieces in between
    int qStep = (to.getColonne() - getPosition().getColonne()) == 0 ? 0 : (to.getColonne() - getPosition().getColonne()) / abs(to.getColonne() - getPosition().getColonne());
    int rStep = (to.getLigne() - getPosition().getLigne()) == 0 ? 0 : (to.getLigne() - getPosition().getLigne()) / abs(to.getLigne() - getPosition().getLigne());

    int q = getPosition().getColonne() + qStep;
    int r = getPosition().getLigne() + rStep;
    bool foundPiece = false;

    while (q != to.getColonne() || r != to.getLigne()) {
        Position intermediate(q, r);
        if (plateau.find(intermediate) != plateau.end() && !plateau.at(intermediate).empty()) {
            foundPiece = true;
        } else if (foundPiece) {
            return false; // There was a gap in the line of pieces
        }
        q += qStep;
        r += rStep;
    }

    return foundPiece; // The destination must be empty and there must be at least one piece in between
}

bool Fourmi::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>, hash<Position>>& plateau) const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }

    // Check if the ant can slide to the destination
    unordered_set<Position, hash<Position>> visited;
    return true; // Assuming the move is valid if the destination is not occupied
}

bool Fourmi::canSlideTo(const Position& from, const Position& to, const unordered_map<Position, vector<Piece*>, hash<Position>>& plateau, unordered_set<Position, hash<Position>>& visited) const {
    if (from == to) {
        return true;
    }

    visited.insert(from);
    vector<Position> adjacents = from.getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (visited.find(adj) == visited.end() && (plateau.find(adj) == plateau.end() || plateau.at(adj).empty())) {
            if (canSlideTo(adj, to, plateau, visited)) {
                return true;
            }
        }
    }
    return false;
}

bool Moustique::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    return true;
}

bool Coccinelle::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    return true;
}