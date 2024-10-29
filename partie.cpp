#include "partie.hpp"
#include <iostream>
using namespace std;


void Partie::dfs(Position pos, unordered_set<Position, hash<Position>>& visited) {
    visited.insert(pos);
    vector<Position> adjacents = pos.getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (plateau.isPositionOccupied(adj) && visited.find(adj) == visited.end()) {
            dfs(adj, visited);
        }
    }
}

void Partie::afficherMouvementsPossibles(Position pos) {
    if (plateau.isPositionOccupied(pos)) {
        Piece* piece = plateau.getPlateau().at(pos).back();
        cout << "Mouvements possibles pour la pièce " << piece->getType() << " à la position (" << pos.getColonne() << ", " << pos.getLigne() << "):" << endl;
        vector<Position> adjacents = plateau.getAllAdjacentCoordinates();
        for (const Position& adj : adjacents) {
            if (piece->isValidMove(adj, plateau.getPlateau()) && !wouldSplitHive(pos, adj)) {
                cout << "(" << adj.getColonne() << ", " << adj.getLigne() << ")" << endl;
            }
        }
    } else {
        cout << "Aucune pièce à cette position." << endl;
    }
}


bool Partie::isHiveConnected() {
    unordered_set<Position, hash<Position>> visited;
    Position start(0, 0); // Initialize with valid parameters

    // Find a starting position with a piece
    for (const auto& entry : plateau.getPlateau()) {
        if (!entry.second.empty()) {
            start = entry.first;
            break;
        }
    }

    // Perform a DFS to check connectivity
    dfs(start, visited);

    // Check if all pieces have been visited
    for (const auto& entry : plateau.getPlateau()) {
        if (!entry.second.empty() && visited.find(entry.first) == visited.end()) {
            return false;
        }
    }

    return true;
}

bool Partie::wouldSplitHive(Position from, Position to) {
    // Temporarily remove the piece from the 'from' position
    Piece* piece = plateau.getPlateau().at(from).back();
    plateau.getPlateau().at(from).pop_back();

    // Check if the hive is still connected
    bool isConnected = isHiveConnected();

    // Restore the piece to the 'from' position
    plateau.getPlateau().at(from).push_back(piece);

    return !isConnected;
}





void Partie::jouer(){

    while(true){
        cout << "Tour" <<tourActuel<<endl;
        plateau.print_board();
        plateau.print_positions();
        cout << "Tour du joueur "  << " (" << (joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") << ")" << endl;
        cout << "1. Poser une pièce" << endl;
        cout << "2. Déplacer une pièce" << endl;
        cout << "3. Voir les mouvements possibles pour une pièce" << endl;
        cout << "Choisissez une option: ";
        int choix;
        cin >> choix;
        switch (choix) {
                    case 1: {
                        // Poser une pièce
                        cout << "Choisissez une pièce à poser (R: Reine, S: Scarabee, A: Araignee, H: Sauterelle, F: Fourmi): ";
                        char pieceType;
                        cin >> pieceType;
                        //si premier tour on pose juste la pîèce au milieu
                        if(tourActuel==0){
                            
                            joueurCourant->poserPiece(pieceType, Position(0, 0),plateau,tourActuel);
                            nextTurn();
                            break;
                        }
                        cout << "Entrez la position (q r): ";
                        int q, r;
                        cin >> q >> r;
                        Position pos(q, r);
                        
                        if (joueurCourant->poserPiece(pieceType, pos,plateau,tourActuel)) {
                            nextTurn();
                        } else {
                            cout << "Impossible de poser la pièce à cette position." << endl;
                        }
                        break;
                    }
                    case 2: {
                        // Déplacer une pièce
                        cout << "Entrez la position de départ (q r): ";
                        int qFrom, rFrom;
                        cin >> qFrom >> rFrom;
                        Position from(qFrom, rFrom);
                        cout << "Entrez la position de destination (q r): ";
                        int qTo, rTo;
                        cin >> qTo >> rTo;
                        Position to(qTo, rTo);
                        if (plateau.deplacerPiece(from, to, joueurCourant->getCouleur())) {
                            nextTurn();
                        } else {
                            cout << "Mouvement invalide. Réessayez." << endl;
                        }
                        break;
                    }
                    case 3: {
                        // Voir les mouvements possibles pour une pièce
                        cout << "Entrez la position de la pièce (q r): ";
                        int q, r;
                        cin >> q >> r;
                        Position pos(q, r);
                        afficherMouvementsPossibles(pos);
                        break;
                    }
                   
                    default:
                        cout << "Choix invalide. Réessayez." << endl;
                        break;
        }
    }
            
            

}

