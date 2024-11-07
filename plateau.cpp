#include "plateau.hpp"

void Plateau::addPiece(Piece* piece, Position pos) {
    plateau[pos].push_back(piece);
    piece->setPosition(pos);
}

bool Plateau::isPositionOccupied(Position pos) const {
    auto it = plateau.find(pos);
    return it != plateau.end() && !it->second.empty();
}

bool Plateau::deplacerPiece(Position from, Position to, Couleur couleur) {
    if (!isReinePlaced(couleur)) {
        std::cout << "La reine doit etre posee avant de deplacer une autre piece." << std::endl;
        return false;
    }

    if (isPositionOccupied(from)) {
        try {
            Piece* piece = plateau.at(from).back();
            if (piece->getCouleur() == couleur && piece->isValidMove(to, plateau)) {
                plateau[from].pop_back();
                piece->setPosition(to);
                plateau[to].push_back(piece);
                return true;
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }
    return false;
}



bool Plateau::isReinePlaced(Couleur couleur) const {
    for (const auto& entry : plateau) {
        const vector<Piece*>& pieces = entry.second;
        for (const Piece* piece : pieces) {
            if (piece->getType() == "Reine" && piece->getCouleur() == couleur) {
                return true;
            }
        }
    }
    return false;
}

void Plateau::print_board() const {
    // Determine the bounds of the board
    int minQ = numeric_limits<int>::max(), maxQ = numeric_limits<int>::min();
    int minR = numeric_limits<int>::max(), maxR = numeric_limits<int>::min();
    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        minQ = min(minQ, pos.getColonne());
        maxQ = max(maxQ, pos.getColonne());
        minR = min(minR, pos.getLigne());
        maxR = max(maxR, pos.getLigne());
    }

    // Print the board
    for (int r = minR; r <= maxR; ++r) {
        for (int q = minQ; q <= maxQ; ++q) {
            Position pos(q, r);
            try {
                if (plateau.at(pos).empty()) {
                    cout << ".";
                } else {
                    cout << plateau.at(pos).back()->getInitial();
                }
            } catch (const out_of_range& e) {
                cout << ".";
            }
            cout << " ";
        }
        cout << std::endl;
    }
}

void Plateau::print_positions() const {
    cout << "Positions des pieces sur le plateau:\n" << endl;
    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        const vector<Piece*>& pieces = entry.second;
        for (const Piece* piece : pieces) {
            cout << piece->getType() << " (" << (piece->getCouleur()== Noir ? "Noir" : "Blanc") << ") a la position (" << pos.getColonne() << ", " << pos.getLigne() << ")\n" << endl;
        }
    }
}

vector<Position> Plateau::getAllAdjacentCoordinates() const {
    unordered_set<Position, hash<Position>> allAdjacents;
    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        vector<Position> adjacents = pos.getAdjacentCoordinates();
        for (const Position& adj : adjacents) {
            auto it = plateau.find(adj);
            if (it == plateau.end() || it->second.empty()) {
                allAdjacents.insert(adj);
            }
        }
    }
    for (const Position& adj : allAdjacents){
        cout<<"("<<adj.getColonne()<< " "<<adj.getLigne()<<")"<<endl;
    }
    cout<<"---------------------------\n"<<endl;
    return vector<Position>(allAdjacents.begin(), allAdjacents.end());
}


