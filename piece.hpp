#ifndef PIECE_HPP
#define PIECE_HPP


#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>


using namespace std;

enum Couleur {
    Noir,
    Blanc
};

class Position{
    int q; // Colonne
    int r; // Ligne
public:
    // Constructeur
    Position(int q, int r) : q(q), r(r) {}

    //getters pour les attributs
    int getColonne() const {return q;}
    int getLigne() const {return r;}

    // Opérateur d’égalité
    bool operator==(const Position& other) const {return q == other.q && r == other.r;}
    vector<Position> getAdjacentCoordinates() const;
    bool isAdjacent(const Position& other) const;
};

namespace std{
    template <>
    struct hash<Position> { //permet d'utiliser les objets position comme clé d'une unordered map
        size_t operator()(const Position& pos) const { return hash<int>()(pos.getColonne()) ^ hash<int>()(pos.getLigne());}
    };
}


class Piece {
    Position pos;
    Couleur couleur;
public:
    Piece(const Position& pos, Couleur couleur): pos(pos), couleur(couleur) {}
    virtual ~Piece() = default;
    virtual bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const = 0;
    Position getPosition() const { return pos; }
    Couleur getCouleur() const { return couleur; }
    void setPosition(const Position& newPos) { pos = newPos; }
    virtual string getType() const = 0;
    virtual char getInitial() const = 0;
};


class Reine : public Piece {
public:
    Reine(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        if (!getPosition().isAdjacent(to)) {
            return false;
        }
        if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) {
            return false; // The destination is occupied
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
    string getType() const override { return "Reine"; }
    char getInitial() const override {return 'R';}
};

class Scarabee : public Piece {
public:
    Scarabee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        if (!getPosition().isAdjacent(to)) {
            return false;
        }
        // Scarabee can move onto other pieces
        return true;
    }
    string getType() const override { return "Scarabee"; }
    char getInitial() const override {return 'S';}
};

class Araignee : public Piece {
public:
    Araignee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Araignee must move exactly three spaces
        if (getPosition().isAdjacent(to)) {
            return false; // Direct adjacency is not allowed
        }

        // Check if the destination is three spaces away
        unordered_set<Position, hash<Position>> visited;
        return canMoveThreeSpaces(getPosition(), to, plateau, visited, 0);
        
    }
    bool canMoveThreeSpaces(const Position& from, const Position& to, const unordered_map<Position, vector<Piece*>>& plateau, unordered_set<Position, hash<Position>>& visited, int depth) const {
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



    bool isAdjacentToPiece(const Position& pos, const unordered_map<Position, vector<Piece*>>& plateau) const {
        // Checks if the position has any neighboring pieces
        for (const Position& adj : pos.getAdjacentCoordinates()) {
            if (plateau.find(adj) != plateau.end() && !plateau.at(adj).empty()) {
                return true; // Found an adjacent piece
            }
        }
        return false;
    }

    string getType() const override { return "Araignee"; }
    char getInitial() const override {return 'A';}
};

class Sauterelle : public Piece {
public:
    Sauterelle(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
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
    string getType() const override { return "Sauterelle"; }
    char getInitial() const override {return 'H';}
};

class Fourmi : public Piece {
public:
    Fourmi(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>, hash<Position>>& plateau) const override {
        if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) {
            return false; // The destination is occupied
        }

        // Check if the ant can slide to the destination
        unordered_set<Position, hash<Position>> visited;
        return true; // Assuming the move is valid if the destination is not occupied
    }

    bool canSlideTo(const Position& from, const Position& to, const unordered_map<Position, vector<Piece*>, hash<Position>>& plateau, unordered_set<Position, hash<Position>>& visited) const {
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

    

    string getType() const override { return "Fourmi"; }
    char getInitial() const override {return 'F';}
};

#endif