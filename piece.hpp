#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "position.hpp"
#include <unordered_set>


enum Couleur {
    Noir,
    Blanc
};

class Piece {
protected:
    Position pos;
    Couleur couleur;

public:
    Piece(const Position& pos, Couleur couleur)
        : pos(pos), couleur(couleur) {}
    virtual ~Piece() = default;

    virtual bool isValidMove(const Position& to, const std::unordered_map<Position, std::vector<Piece*>>& plateau) const = 0;
    Position getPosition() const { return pos; }
    Couleur getCouleur() const { return couleur; }
    void setPosition(const Position& newPos) { pos = newPos; }

    virtual std::string getType() const = 0;
    virtual char getInitial() const = 0;
};

class Reine : public Piece {
public:
    Reine(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}

    bool isValidMove(const Position& to, const std::unordered_map<Position, std::vector<Piece*>>& plateau) const override {
        return pos.isAdjacent(to) && !plateau.count(to);
    }

    std::string getType() const override { return "Reine"; }
    char getInitial() const override { return 'R'; }
};

class Scarabee : public Piece {
public:
    Scarabee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}

    bool isValidMove(const Position& to, const std::unordered_map<Position, std::vector<Piece*>>& plateau) const override {
        return pos.isAdjacent(to); // Scarabee peut se déplacer sur d'autres pièces
    }

    std::string getType() const override { return "Scarabee"; }
    char getInitial() const override { return 'S'; }
};

class Araignee : public Piece {
public:
    Araignee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}

    bool isValidMove(const Position& to, const std::unordered_map<Position, std::vector<Piece*>>& plateau) const override {
        // L'Araignee doit bouger de trois cases, en contact continu avec la ruche
        return canMoveThreeSpaces(pos, to, plateau);
    }

private:
    bool canMoveThreeSpaces(const Position& from, const Position& to, const std::unordered_map<Position, std::vector<Piece*>>& plateau, int depth = 0) const {
        if (depth == 3) return from == to;
        for (const Position& adj : from.getAdjacentCoordinates()) {
            if (!plateau.count(adj) || depth > 3) return false;
            if (canMoveThreeSpaces(adj, to, plateau, depth + 1)) return true;
        }
        return false;
    }

public:
    std::string getType() const override { return "Araignee"; }
    char getInitial() const override { return 'A'; }
};

class Sauterelle : public Piece {
public:
    Sauterelle(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}

    bool isValidMove(const Position& to, const std::unordered_map<Position, std::vector<Piece*>>& plateau) const override {
        return isStraightLineMove(pos, to) && hasIntermediatePieces(pos, to, plateau);
    }

    std::string getType() const override { return "Sauterelle"; }
    char getInitial() const override { return 'H'; }

private:
    bool isStraightLineMove(const Position& from, const Position& to) const {
        int dq = to.getColonne() - from.getColonne();
        int dr = to.getLigne() - from.getLigne();
        return dq == 0 || dr == 0 || abs(dq) == abs(dr);
    }

    bool hasIntermediatePieces(const Position& from, const Position& to, const std::unordered_map<Position, std::vector<Piece*>>& plateau) const {
        Position current = from;
        while (current != to) {
            current = nextPositionInLine(current, to);
            if (plateau.count(current) && !plateau.at(current).empty()) return true;
        }
        return false;
    }

    Position nextPositionInLine(const Position& from, const Position& to) const {
        int qStep = (to.getColonne() - from.getColonne()) == 0 ? 0 : (to.getColonne() - from.getColonne()) / abs(to.getColonne() - from.getColonne());
        int rStep = (to.getLigne() - from.getLigne()) == 0 ? 0 : (to.getLigne() - from.getLigne()) / abs(to.getLigne() - from.getLigne());
        return Position(from.getColonne() + qStep, from.getLigne() + rStep);
    }
};


class Fourmi : public Piece {
public:
    Fourmi(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}

    bool isValidMove(const Position& to, const std::unordered_map<Position, std::vector<Piece*>, std::hash<Position>>& plateau) const override;

private:
    bool canSlideTo(const Position& from, const Position& to,
        const std::unordered_map<Position, std::vector<Piece*>, std::hash<Position>>& plateau,
        std::unordered_set<Position, std::hash<Position>>& visited) const;

    std::string getType() const override { return "Fourmi"; }
    char getInitial() const override { return 'F'; }
};


