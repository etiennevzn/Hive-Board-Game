#pragma once
#include <unordered_map>
#include <vector>
#include "position.hpp"
#include "piece.hpp"
#include <unordered_set>
#include <stdexcept>

class Plateau {
    std::unordered_map<Position, std::vector<Piece*>> plateau;

public:
    Plateau() = default;
    void addPiece(Piece* piece, const Position& pos);
    void removePiece(const Position& pos);
    bool isPositionOccupied(const Position& pos) const;
    Piece* getPieceAtPosition(const Position& pos) const;
    const std::unordered_map<Position, std::vector<Piece*>>& getPlateauMap() const;
    void print_board() const;
    bool hasPiecesOnBoard(Couleur couleur) const;
    bool isHiveConnected(const std::unordered_map<Position, std::vector<Piece*>>& tempPlateau);
    bool wouldSplitHive(Position from, Position to);

private:
    void dfs(const Position& pos, const std::unordered_map<Position, std::vector<Piece*>>& tempPlateau, std::unordered_set<Position>& visited) const;
};
