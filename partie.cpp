#include "partie.hpp"
#include <iostream>
using namespace std;

void Partie::jouer(){

    while(true){
        plateau.print_board();
        cout << "Tour du joueur "  << " (" << (joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") << ")" << endl;
        cout << "1. Poser une pièce" << endl;
        cout << "2. Déplacer une pièce" << endl;
        cout << "3. Voir les mouvements possibles pour une pièce" << endl;
        cout << "Choisissez une option: ";
        int choix;
        cin >> choix;
        switch (choix) {
                    case 1: {
                        // Poser une pièce
                        cout << "Choisissez une pièce à poser (R: Reine, S: Scarabee, A: Araignee, H: Sauterelle, F: Fourmi): ";
                        char pieceType;
                        cin >> pieceType;
                        cout << "Entrez la position (q r): ";
                        int q, r;
                        cin >> q >> r;
                        Position pos(q, r);
                        
                        if (joueurCourant->poserPiece(pieceType, pos,plateau)) {
                            nextTurn();
                        } else {
                            cout << "Impossible de poser la pièce à cette position." << endl;
                        }
                        break;
                    }
                    /*case 2: {
                        // Déplacer une pièce
                        cout << "Entrez la position de départ (q r): ";
                        int qFrom, rFrom;
                        cin >> qFrom >> rFrom;
                        Position from(qFrom, rFrom);
                        cout << "Entrez la position de destination (q r): ";
                        int qTo, rTo;
                        cin >> qTo >> rTo;
                        Position to(qTo, rTo);
                        if (deplacerPiece(from, to)) {
                            nextTurn();
                        } else {
                            cout << "Mouvement invalide. Réessayez." << endl;
                        }
                        break;
                    }
                    case 3: {
                        // Voir les mouvements possibles pour une pièce
                        cout << "Entrez la position de la pièce (q r): ";
                        int q, r;
                        cin >> q >> r;
                        Position pos(q, r);
                        afficherMouvementsPossibles(pos);
                        break;
                    }
                   
                    default:
                        cout << "Choix invalide. Réessayez." << endl;
                        break;*/
        }
    }
            
            

}

