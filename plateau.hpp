#ifndef PLATEAU_HPP
#define PLATEAU_HPP

#include <vector>
#include <iostream>
#include "piece.hpp"
#include <unordered_set>
#include <stdexcept>
#include <limits>


class Plateau{
    unordered_map<Position, vector<Piece*>> plateau;
    //à voir si l'on peut recopier un objet de la classe Plateau pour les constructeurs de recopie 
    
public:
    Plateau()=default;
    bool isPositionOccupied(Position pos) const;
    void dfs(Position pos, const unordered_map<Position, vector<Piece*>>& tempPlateau, unordered_set<Position, hash<Position>>& visited);
    bool isHiveConnected(const unordered_map<Position, vector<Piece*>>& tempPlateau);
    bool wouldSplitHive(Position from, Position to);

    unordered_map<Position, vector<Piece*>> getPlateau()const{return plateau;}
    void addPiece(Piece* piece, Position pos);

    bool isReinePlaced(Couleur couleur) const;
    bool deplacerPiece(Position from, Position to, Couleur couleur);
    void print_board()const;
    void print_positions() const;

};

#endif