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

    void dfs(Position pos, unordered_set<Position, hash<Position>>& visited);
    bool isHiveConnected();
    bool wouldSplitHive(Position from, Position to);

    Plateau()=default;
    unordered_map<Position, vector<Piece*>> getPlateau()const{return plateau;}
    void addPiece(Piece* piece, Position pos);
    bool isPositionOccupied(Position pos) const;
    bool deplacerPiece(Position from, Position to, Couleur couleur);
    bool isReinePlaced(Couleur couleur) const;
    void print_board(int size = 5)const;
    void print_positions() const;
    vector<Position> getAllAdjacentCoordinates()const;

};

#endif