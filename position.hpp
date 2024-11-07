#pragma once
#include <vector>
#include <functional>

class Position {
    int q; // Colonne
    int r; // Ligne

public:
    Position() : q(0), r(0) {}
    Position(int col, int row) : q(col), r(row) {}

    int getColonne() const { return q; }
    int getLigne() const { return r; }

    bool operator==(const Position& other) const { return q == other.q && r == other.r; }
    bool operator!=(const Position& other) const { return !(*this == other); }

    bool isAdjacent(const Position& other) const;

    std::vector<Position> getAdjacentCoordinates() const;
};

// Spécialisation de std::hash pour la classe Position
namespace std {
    template <>
    struct hash<Position> {
        std::size_t operator()(const Position& pos) const {
            return std::hash<int>()(pos.getColonne()) ^ (std::hash<int>()(pos.getLigne()) << 1);
        }
    };
}
