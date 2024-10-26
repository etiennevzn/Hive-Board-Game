#ifndef PIECE_HPP
#define PIECE_HPP


#include <iostream>
#include <unordered_map>
#include <string>

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
    Position** get_adjacent_coordinates() const; //à voir si le le type de retour est bon
    bool isAdjacent(const Position& other) const;
};

namespace std {
    template <>
    struct hash<Position> {
        size_t operator()(const Position& pos) const {
            return hash<int>()(pos.getColonne()) ^ hash<int>()(pos.getLigne());
        }
    };
}

class Piece {

    Position pos;
    Couleur couleur;
public:
    Piece(const Position& pos, Couleur couleur)
        : pos(pos), couleur(couleur) {}
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
        // La Reine peut se déplacer d'une seule case
        if (!getPosition().isAdjacent(to)) {
            return false;
        }
        // Vérifier que la Reine glisse d'une case à une autre
        // La Reine ne peut pas sauter par-dessus d'autres pièces
        // Implémenter la logique de glissement ici
        return true; // Placeholder
    }
    string getType() const override { return "Reine"; }
    char getInitial() const override {return 'R';}
};

class Scarabee : public Piece {
public:
    Scarabee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour le Scarabée
        return true; // Placeholder
    }
    string getType() const override { return "Scarabee"; }
    char getInitial() const override {return 'S';}
};

class Araignee : public Piece {
public:
    Araignee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour l'Araignée
        return true; // Placeholder
    }
    string getType() const override { return "Araignee"; }
    char getInitial() const override {return 'A';}
};

class Sauterelle : public Piece {
public:
    Sauterelle(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour la Sauterelle
        return true; // Placeholder
    }
    string getType() const override { return "Sauterelle"; }
    char getInitial() const override {return 'H';}
};

class Fourmi : public Piece {
public:
    Fourmi(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour la Fourmi
        return true; // Placeholder
    }
    string getType() const override { return "Fourmi"; }
    char getInitial() const override {return 'F';}
};

#endif