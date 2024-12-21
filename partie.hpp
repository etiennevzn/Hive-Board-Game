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

        PartieMemento(const Plateau& p, const Joueur& j1, const Joueur& j2, Joueur* jc, int ta) : joueur1(j1), joueur2(j2), tourActuel(ta) {
            plateau = p;  
            if (jc == &j1) {
                joueurCourant = &joueur1;
            } else if (jc == &j2) {
                joueurCourant = &joueur2;
            }
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
    const Plateau& getPlateau() const{return plateau;}
    void afficherMouvementsPossibles(Position pos, Couleur couleur); //pas const car on utilise wouldSplitHive dedans qui n'est pas const
    void printPossiblePlays(Joueur* joueurCourant); //pareil que la méthode au dessus
    bool isGameOver();//pareil que la méthode au dessus
    void playTurn();
    void sauvegarderEtat(); // Sauvegarde l'état actuel
    void restaurerEtat(); // Restaure l'état précédent
    void restaurerDeuxDerniersEtats(); // Restaure les deux derniers états
    void play();
    ~Partie() = default; //le GameManager s'occuper de la libération de la mémoire des joueurs

    void saveToFile(ofstream& outFile) const;
    void loadFromFile(ifstream& inFile);
     
};

#endif