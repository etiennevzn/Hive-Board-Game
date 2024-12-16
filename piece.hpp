#ifndef PIECE_HPP
#define PIECE_HPP


#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <unordered_set>


using namespace std;

enum Couleur {
    Noir,
    Blanc
};

class Piece;

class Position{
    int q; // Colonne
    int r; // Ligne
public:
    // Constructeur
    Position(int q, int r) : q(q), r(r) {}
    Position(int x, int y, int z); // à faire, constructeur à partir des coordonnées cubiques
    //getters pour les attributs
    int getColonne() const {return q;}
    int getLigne() const {return r;}

    vector<int> toCube() const; //converti les coordonnées en coordonnées cubiques

    // Opérateur d’égalité
    bool operator==(const Position& other) const {return q == other.q && r == other.r;}
    bool operator!=(const Position& other) const {return q != other.q || r != other.r;}
    Position& operator=(const Position& other) {q = other.q; r = other.r; return *this;}
    vector<Position> getAdjacentCoordinates() const;
    bool isAdjacent(const Position& other) const;
    bool isAccessible(const unordered_map<Position, vector<Piece*>>& plateau)const; //vérifie si la position est accessible en glissant
    bool isSlidingPossible(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const;


    vector<Position> successeurs_valides(const unordered_map<Position, vector<Piece*>>& plateau, const vector<Position>& chemin) const;

    void sauvegarder(std::ofstream& outFile) const {
        outFile << q << " " << r << "\n"; // Sauvegarder les coordonnées
    }

    static Position charger(std::ifstream& inFile) {
        int q, r;
        inFile >> q >> r; // Lire les coordonnées
        return Position(q, r);
    }
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
    virtual vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const = 0; 
    const Position& getPosition() const { return pos; }
    const Couleur& getCouleur() const { return couleur; }
    void setPosition(const Position& newPos) { pos = newPos; }
    virtual string getType() const = 0;
    virtual char getInitial() const = 0;
    vector<Position> getBorderPositions(const unordered_map<Position, vector<Piece*>>& plateau)const;
};


class Reine : public Piece {
public:
    Reine(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isSurrounded(const unordered_map<Position, vector<Piece*>>& plateau) const;
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override;
    string getType() const override { return "Reine"; }
    char getInitial() const override {return 'R';}
    vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const; 
};

class Scarabee : public Piece {
public:
    Scarabee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override;
    string getType() const override { return "Scarabee"; }
    char getInitial() const override {return 'S';}
    vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const;
};

class Araignee : public Piece {
public:
    Araignee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMoveRecursive(const Position& current, const Position& target, const unordered_map<Position, vector<Piece*>>& plateau, int stepsLeft, unordered_set<Position>& visited) const;
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override;    
    string getType() const override { return "Araignee"; }
    char getInitial() const override {return 'A';}
    vector<Position> getValidMovesIntermediate(const Position& start, const unordered_map<Position, vector<Piece*>>& plateau) const;
    vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const; 
};

class Sauterelle : public Piece {
public:
    Sauterelle(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override;
    string getType() const override { return "Sauterelle"; }
    char getInitial() const override {return 'H';}
    vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const; 
};

class Fourmi : public Piece {
public:
    Fourmi(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool canReach(const Position& current, const Position& target, const unordered_map<Position, vector<Piece*>>& plateau, unordered_set<Position, hash<Position>>& visited, const vector<Position>& borderPositions) const ;
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override;
    string getType() const override { return "Fourmi"; }
    char getInitial() const override {return 'F';}
    vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const;
};

class Moustique : public Piece {
public:
    Moustique(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau)const;
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const override;
    string getType() const override { return "Moustique"; }
    char getInitial() const override {return 'M';}
};

class Coccinelle : public Piece {
public:
    Coccinelle(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override;
    vector<Position> getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau)const;
    string getType() const override { return "Coccinelle"; }
    char getInitial() const override {return 'C';}
};

#endif