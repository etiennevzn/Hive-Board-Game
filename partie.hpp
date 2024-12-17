#pragma once
#include "plateau.hpp"
#include "joueur.hpp"
#include <stack>

class Partie {
    struct PartieMemento {
        Plateau plateau;
        Joueur joueur1;
        Joueur joueur2;
        Joueur* joueurCourant;
        int tourActuel;

        PartieMemento(const Plateau& p, const Joueur& j1, const Joueur& j2, Joueur* jc, int ta)
            : plateau(p), joueur1(j1), joueur2(j2), joueurCourant(jc), tourActuel(ta) {}
    };

    Plateau plateau;
    Joueur joueur1;
    Joueur joueur2;
    Joueur* joueurCourant;
    int tourActuel;
    bool contreIA; // Ajout d'un attribut pour savoir si l'IA joue
    std::stack<PartieMemento> historique; // Pile pour stocker les mementos
    int maxRetoursEnArriere; // Nombre maximum de retours en arrière possibles

public:
    Partie(int maxRetours, bool ia = false); // Modifie le constructeur pour prendre un booléen
    void jouer();
    void sauvegarderEtat(); // Sauvegarde l'état actuel
    void restaurerEtat(); // Restaure l'état précédent
    void restaurerDeuxDerniersEtats(); // Restaure les deux derniers états
    bool poserPiece(); // Déclare la méthode poserPiece
    bool deplacerPiece(); // Déclare la méthode deplacerPiece
    void jouerTourIA();
    bool deplacerPieceIA();
    bool poserPieceIA();



private:
    bool isPieceSurrounded(const Position& pos) const;
    bool conditionsDeVictoire();
    bool conditionsDeMatchNul();
    void nextTurn();
};
