#ifndef HIVE_HPP
#define HIVE_HPP
#include <vector>
#include <iostream>
#include <ostream>
#include <unordered_map>
#include <string>
#include <utility> // pour pair 

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
};

class Scarabee : public Piece {
public:
    Scarabee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour le Scarabée
        return true; // Placeholder
    }
};

class Araignee : public Piece {
public:
    Araignee(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour l'Araignée
        return true; // Placeholder
    }
};

class Sauterelle : public Piece {
public:
    Sauterelle(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour la Sauterelle
        return true; // Placeholder
    }
};

class Fourmi : public Piece {
public:
    Fourmi(const Position& pos, Couleur couleur) : Piece(pos, couleur) {}
    bool isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const override {
        // Implémenter la logique de mouvement pour la Fourmi
        return true; // Placeholder
    }
};



class Plateau{
    unordered_map<Position, vector<Piece*>> plateau;
    //à voir si l'on peut recopier un objet de la classe Plateau
    Plateau(const Plateau& p);
    Plateau& operator=(const Plateau& other); 
public:
    void addPiece(Piece* piece, Position pos) {
        plateau[pos].push_back(piece);
    }
};


class Joueur{
    string nom;
    string prenom;
    //const Partie& partie; ??
    enum Couleur couleur;
public :

};
class Partie{
/* 
Probablement design pattern singleton. 
*/
    const Plateau& plateau;
    int tourActuel;
    Joueur joueur1;
    Joueur joueur2;
public:
    
};










#endif