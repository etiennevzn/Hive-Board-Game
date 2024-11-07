#pragma once
#include <unordered_map>
#include <vector>
#include "position.hpp"
#include "piece.hpp"

class Plateau {
    std::unordered_map<Position, std::vector<Piece*>> plateau;

public:
    Plateau() = default;
    void addPiece(Piece* piece, const Position& pos);
    void removePiece(const Position& pos);  // Déclaration ajoutée
    bool isPositionOccupied(const Position& pos) const;
    Piece* getPieceAtPosition(const Position& pos) const;
    const std::unordered_map<Position, std::vector<Piece*>>& getPlateauMap() const;
    void print_board() const;
};
