#include "partie.hpp"
#include <iostream>

using namespace std;

void Partie::nextTurn() {
    tourActuel++;
    joueurCourant = (joueurCourant == &joueur1) ? &joueur2 : &joueur1;
    //à compléter...
}

void Partie::afficherMouvementsPossibles(Position pos) {
    if (plateau.isPositionOccupied(pos)) {
        Piece* piece = plateau.getPlateau().at(pos).back();
        cout << "Mouvements possibles pour la piece " << piece->getType() << " a la position (" << pos.getColonne() << ", " << pos.getLigne() << "):" << endl;
        vector<Position> adjacents = plateau.getAllAdjacentCoordinates();
        for (const Position& adj : adjacents) {
            if (piece->isValidMove(adj, plateau.getPlateau()) && !plateau.wouldSplitHive(pos, adj)) {
                cout << "(" << adj.getColonne() << ", " << adj.getLigne() << ")" << endl;
            }
        }
    }
    else {
        cout << "Aucune pièce à cette position." << endl;
    }
}

Memento Partie::sauvegarder() {
    return Memento(tourActuel, joueur1, joueur2, joueurCourant, plateau);
}

void Partie::restaurer(const Memento& memento) {
    tourActuel = memento.tourActuel;
    joueur1 = memento.joueur1;
    joueur2 = memento.joueur2;
    joueurCourant = memento.joueurCourant;
    plateau = memento.plateau;
}

void Partie::jouer() {
    // Ajoute l'état initial à l'historique
    historique.push_back(sauvegarder());

    while (true) {
        cout << "Tour " << tourActuel << endl;
        plateau.print_board();
        plateau.print_positions();
        cout << "Tour du joueur " << " (" << (joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") << ")" << endl;
        cout << "1. Poser une piece" << endl;
        cout << "2. Deplacer une piece" << endl;
        cout << "3. Voir les mouvements possibles pour une piece" << endl;
        cout << "4. Annuler le dernier mouvement" << endl;
        cout << "Choisissez une option: ";
        int choix;
        cin >> choix;
        switch (choix) {
        case 1: {
            // Poser une pièce
            cout << "Choisissez une piece a poser (R: Reine, S: Scarabee, A: Araignee, H: Sauterelle, F: Fourmi): ";
            char pieceType;
            cin >> pieceType;
            //si premier tour on pose juste la pîèce au milieu
            if (tourActuel == 0) {
                joueurCourant->poserPiece(pieceType, Position(0, 0), plateau, tourActuel);
                nextTurn();
                historique.push_back(sauvegarder());
                break;
            }
            cout << "Entrez la position (q r): ";
            int q, r;
            cin >> q >> r;
            Position pos(q, r);

            if (joueurCourant->poserPiece(pieceType, pos, plateau, tourActuel)) {
                nextTurn();
                historique.push_back(sauvegarder());
            }
            else {
                cout << "Impossible de poser la piece a cette position." << endl;
            }
            break;
        }
        case 2: {
            // Déplacer une pièce
            cout << "Entrez la position de depart (q r): ";
            int qFrom, rFrom;
            cin >> qFrom >> rFrom;
            Position from(qFrom, rFrom);
            cout << "Entrez la position de destination (q r): ";
            int qTo, rTo;
            cin >> qTo >> rTo;
            Position to(qTo, rTo);
            if (plateau.deplacerPiece(from, to, joueurCourant->getCouleur())) {
                nextTurn();
                historique.push_back(sauvegarder());
            }
            else {
                cout << "Mouvement invalide. Réessayez." << endl;
            }
            break;
        }
        case 3: {
            // Voir les mouvements possibles pour une pièce
            cout << "Entrez la position de la piece (q r): ";
            int q, r;
            cin >> q >> r;
            Position pos(q, r);
            afficherMouvementsPossibles(pos);
            break;
        }
        case 4: {
            // Annuler le dernier mouvement
            if (historique.size() > 1) {
                historique.pop_back();
                restaurer(historique.back());
            }
            else {
                cout << "Aucun mouvement à annuler." << endl;
            }
            break;
        }
        default:
            cout << "Choix invalide. Réessayez." << endl;
            break;
        }
    }
}
