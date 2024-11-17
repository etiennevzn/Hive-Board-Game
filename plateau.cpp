#include "plateau.hpp"
#include <iomanip>

bool Plateau::isPositionOccupied(Position pos) const {
    auto it = plateau.find(pos);
    return it != plateau.end() && !it->second.empty();
}


void Plateau::dfs(Position pos, const unordered_map<Position, vector<Piece*>>& tempPlateau, unordered_set<Position>& visited) {
    visited.insert(pos);
    vector<Position> adjacents = pos.getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        auto it = tempPlateau.find(adj);
        if (it != tempPlateau.end() && !it->second.empty() && visited.find(adj) == visited.end()) {
            dfs(adj, tempPlateau, visited);
        }
    }
}


bool Plateau::isHiveConnected(const unordered_map<Position, vector<Piece*>>& tempPlateau){
    unordered_set<Position> visited;
    Position start(0,0); 
    bool foundStart = false;
    for (const auto& entry : tempPlateau) {
        if (!entry.second.empty()) {
            start = entry.first; // Trouve la première position occupée
            foundStart = true;
            break;
        }
    }

    if (!foundStart) return true;  // Si aucune position occupée n'existe, la ruche est connectée par défaut

    dfs(start, tempPlateau, visited); //on fait un recherche en profondeur avec tous les voisins

    for (const auto& entry : tempPlateau) {
        if (!entry.second.empty() && visited.find(entry.first) == visited.end()) { //si une position est non vide dans le plateau mais qu'elle n'est pas dans visited, cela signifie que la ruche n'est pas en 1 morceau
            return false;
        }
    }

    return true;
}


bool Plateau::wouldSplitHive(Position from, Position to) {

    if (plateau[from].empty()) { // Vérifie qu'il y a une pièce à la position d'origine 
        throw std::runtime_error("Aucune piece a deplacer depuis cette position.");
    } //a voir si on fait ça ou simplement un return false (car dans si il n'y a pas de pièce à déplacer ça va pas split)

    unordered_map<Position, vector<Piece*>> tempPlateau = plateau; //on fait une copie du plateau pour pouvoir faire des changements temporaires

    Piece* piece = tempPlateau[from].back(); // Récupère la dernière pièce (pièce la plus en haut de l'empilement en cas d'empilement)

    //simulation du déplacement
    tempPlateau[from].pop_back();
    tempPlateau[to].push_back(piece);
    return !isHiveConnected(tempPlateau); //si le plateau n'est pas conecté alors on return true (ça casserait la ruche), et vice-versa
}


void Plateau::addPiece(Piece* piece, Position pos) {
    plateau[pos].push_back(piece);
    piece->setPosition(pos);
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
        } catch (const out_of_range& e) {
            cerr << "Error: " << e.what() << std::endl;
            return false;
        }
    }
    return false;
}


void Plateau::print_board(int size) const {

    int min_q = 0, max_q = 0, min_r = 0, max_r = 0;
    for (const auto& entry : plateau) { //on détermine les limites du plateau (les pièces les plus excentrées posées dessus)
        if (!entry.second.empty()) { 
            min_q = min(min_q, entry.first.getColonne());
            max_q = max(max_q, entry.first.getColonne());
            min_r = min(min_r, entry.first.getLigne());
            max_r = max(max_r, entry.first.getLigne());
        }
    }
    
    for (int r = min_r; r <= max_r; ++r) { //on parcourt toutes les lignes et colonnes du plateau
        int offset = (r % 2 == 0) ? 0 : 3; //avec notre système de coordonnées, les lignes impaires sont décalées par rapport à la ligne 0 mais pas les lignes paires
        cout << string(offset, ' ');

        for (int q = min_q; q<=max_q; ++q) {
            Position pos(q, r);
            if (isPositionOccupied(pos)) {
                Piece* piece = plateau.at(pos).back(); // on affiche la pièce la plus en haut de l'empilement (dernière pièce ajouté au vecteur)
                piece->getCouleur() == Blanc ? cout << "[" << piece->getInitial() << ".B" << "] " : cout << "[" << piece->getInitial() << ".N" << "] "; // Affiche la pièce avec son type et couleur
            } else {
                cout << "[   ] "; 
            }
        }
        
        cout << endl; 
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


