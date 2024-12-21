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
public:
    Plateau()=default;
    ~Plateau() = default; //le GameManager s'occupe de la libération de la mémoire des pièces

    //Opérateur d'affectation de plateau pour restaurer un état précédent
    Plateau& operator=(const Plateau& p);

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


    void save(std::ofstream& outFile) const;  
    void load(std::ifstream& inFile);

};

#endif