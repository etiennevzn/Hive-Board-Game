#ifndef PLATEAU_HPP
#define PLATEAU_HPP

#include <vector>
#include <iostream>
#include "piece.hpp"

class Plateau{
    unordered_map<Position, vector<Piece*>> plateau;
    //à voir si l'on peut recopier un objet de la classe Plateau
    Plateau(const Plateau& p);
    Plateau& operator=(const Plateau& other); 
public:
    Plateau()=default;
    void addPiece(Piece* piece, Position pos) {
        plateau[pos].push_back(piece);
    }

    void print_board() {
        // Determine the bounds of the board
        int minQ = INT_MAX, maxQ = INT_MIN, minR = INT_MAX, maxR = INT_MIN;
        for (const auto& entry : plateau) {
            const Position& pos = entry.first;
            minQ = min(minQ, pos.getColonne());
            maxQ = max(maxQ, pos.getColonne());
            minR = min(minR, pos.getLigne());
            maxR = max(maxR, pos.getLigne());
        }

        // Print the board
        for (int r = minR; r <= maxR; ++r) {
            for (int q = minQ; q <= maxQ; ++q) {
                Position pos(q, r);
                if (plateau.find(pos) != plateau.end() && !plateau[pos].empty()) {
                    cout << plateau[pos].back()->getInitial()<<(plateau[pos].back()->getCouleur() == Noir ?"N":"B");
                } else {
                    cout << ".";
                }
                cout << " ";
            }
            cout << endl;
        }
    }

    

};



#endif