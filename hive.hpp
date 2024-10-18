#ifndef HIVE_HPP
#define HIVE_HPP

#include <iostream>
#include <ostream>
#include <unordered_map>
#include <string>
#include <utility> // pour pair 

using namespace std;

class Partie{
/* 
Probablement design pattern singleton. 
*/
    const Plateau& plateau;
    int tourActuel;
public:

};

enum Couleur {
    Noir,
    Blanc
};

class Joueur{
    string nom;
    string prenom;
    const Partie& partie;
    enum Couleur couleur;
public :

};

class Plateau{
    unordered_map<Position, vector<Piece*>> plateau;
    //à voir si l'on peut recopier un objet de la classe Plateau
    Plateau(const Plateau& p);
    Plateau& operator=(const Plateau& other); 
public:

};

class Piece{

public:

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
    Position** Position::get_adjacent_coordinates() const; //à voir si le le type de retour est bon
    bool isAdjacent(const Position& other) const;
};

#endif