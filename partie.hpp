#ifndef PARTIE_HPP
#define PARTIE_HPP

#include <iostream>
#include "plateau.hpp"
#include "joueur.hpp"
#include <vector>
#include <stack>

using namespace std;


class Partie {
    struct PartieMemento {
        Plateau plateau;
        Joueur joueur1;
        Joueur joueur2;
        Joueur* joueurCourant;
        int tourActuel;

        PartieMemento(const Plateau& p, const Joueur& j1, const Joueur& j2, Joueur* jc, int ta)
            : plateau(p), joueur1(j1), joueur2(j2), joueurCourant(jc), tourActuel(ta) {}
        
        PartieMemento& operator=(const PartieMemento& autre) {
            if (this != &autre) { // Vérification d'auto-affectation
                plateau = autre.plateau;  // Copie du plateau
                joueur1 = autre.joueur1;   // Copie du joueur1
                joueur2 = autre.joueur2;   // Copie du joueur2
                joueurCourant = autre.joueurCourant; // Copie du pointeur vers le joueur courant
                tourActuel = autre.tourActuel;  // Copie du tour actuel
            }
            return *this;  // Retourne l'objet courant
        }
    };

    Plateau plateau;
    int tourActuel;
    Joueur joueurs[2];
    Joueur* joueurCourant;
    stack<PartieMemento> historique; // Pile pour stocker les états précédents
    int maxRetoursEnArriere; 
public:
    Partie(const Joueur& j1, const Joueur& j2, const Plateau& p, int t = 0, int maxRetours = 3);
    void afficherMouvementsPossibles(Position pos, Couleur couleur); //pas const car on utilise wouldSplitHive dedans qui n'est pas const
    void printPossiblePlays(Joueur* joueurCourant); //pareil que la méthode au dessus
    void playTurn();
    void sauvegarderEtat(); // Sauvegarde l'état actuel
    void restaurerEtat(); // Restaure l'état précédent
    void restaurerDeuxDerniersEtats(); // Restaure les deux derniers états
    void play();


    void saveToFile(ofstream& outFile) const;
    void loadFromFile(ifstream& inFile);
     
};

#endif