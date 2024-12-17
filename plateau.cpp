#include "plateau.hpp"
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <unordered_set>

// Ajoutez la méthode DFS
void Plateau::dfs(const Position& pos, const std::unordered_map<Position, std::vector<Piece*>>& tempPlateau, std::unordered_set<Position>& visited) const {
    visited.insert(pos);
    std::vector<Position> adjacents = pos.getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (tempPlateau.count(adj) && !tempPlateau.at(adj).empty() && visited.find(adj) == visited.end()) {
            dfs(adj, tempPlateau, visited);
        }
    }
}

// Vérifiez si la ruche est connectée
bool Plateau::isHiveConnected(const std::unordered_map<Position, std::vector<Piece*>>& tempPlateau) {
    std::unordered_set<Position> visited;
    Position start(0, 0);
    bool foundStart = false;
    for (const auto& entry : tempPlateau) {
        if (!entry.second.empty()) {
            start = entry.first; // Trouve la première position occupée
            foundStart = true;
            break;
        }
    }

    if (!foundStart) return true;  // Si aucune position occupée n'existe, la ruche est connectée par défaut

    dfs(start, tempPlateau, visited); //on fait une recherche en profondeur avec tous les voisins

    for (const auto& entry : tempPlateau) {
        if (!entry.second.empty() && visited.find(entry.first) == visited.end()) { //si une position est non vide dans le plateau mais qu'elle n'est pas dans visited, cela signifie que la ruche n'est pas en 1 morceau
            return false;
        }
    }

    return true;
}

// Vérifiez si un déplacement briserait la ruche
bool Plateau::wouldSplitHive(Position from, Position to) {
    if (plateau[from].empty()) { // Vérifie qu'il y a une pièce à la position d'origine
        throw std::runtime_error("Aucune piece a deplacer depuis cette position.");
    }

    std::unordered_map<Position, std::vector<Piece*>> tempPlateau = plateau; //on fait une copie du plateau pour pouvoir faire des changements temporaires

    Piece* piece = tempPlateau[from].back(); // Récupère la dernière pièce (pièce la plus en haut de l'empilement en cas d'empilement)

    //simulation du déplacement
    tempPlateau[from].pop_back();
    tempPlateau[to].push_back(piece);
    return !isHiveConnected(tempPlateau); //si le plateau n'est pas connecté alors on return true (ça casserait la ruche), et vice-versa
}


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

bool Plateau::hasPiecesOnBoard(Couleur couleur) const {
    for (const auto& entry : plateau) {
        const std::vector<Piece*>& pieces = entry.second;
        for (const Piece* piece : pieces) {
            if (piece->getCouleur() == couleur) {
                return true; // Une pièce du joueur est trouvée
            }
        }
    }
    return false; // Aucune pièce trouvée pour ce joueur
}




