#include "partie.hpp"
#include <iostream>

using namespace std;

void Partie::nextTurn() {
    tourActuel++;
    joueurCourant = (joueurCourant == &joueurs[0]) ? &joueurs[1] : &joueurs[0];
    //à compléter...
}

void Partie::afficherMouvementsPossibles(Position pos) const {
    if (plateau.isPositionOccupied(pos)) {
        Piece* piece = plateau.getPlateau().at(pos).back();
        cout << "Mouvements possibles pour la piece " << piece->getType() << " a la position (" << pos.getColonne() << ", " << pos.getLigne() << "):" << endl;
        vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
    }
    else {
        cout << "Aucune pièce à cette position." << endl;
    }
}

void Partie::printPossiblePlays(Joueur* joueurCourant)const{
    cout<<"***************Actions possibles***************"<<endl;
    cout<<"******************Placements*******************"<<endl;
    joueurCourant->print_piece_left();
    cout<<"******************Déplacements*****************"<<endl;
    vector<Piece*> pieces = joueurCourant->getPieces();
    for(const auto& piece : pieces){
        vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
        if(validMoves.size() != 0){
            cout<<"Position de la piece : ("<<piece->getPosition().getColonne()<<","<<piece->getPosition().getLigne()<<") - Type de la piece : "<<piece->getType()<<endl;
            for(const auto& pos : validMoves){
                cout<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")"<<endl;
            }
        }
    }
        
}

Memento Partie::sauvegarder() {
    return Memento(tourActuel, joueurs[0], joueurs[1], joueurCourant, plateau);
}

void Partie::restaurer(const Memento& memento) {
    tourActuel = memento.tourActuel;
    joueurs[0] = memento.joueurs[0];
    joueurs[1] = memento.joueurs[1];
    joueurCourant = memento.joueurCourant;
    plateau = memento.plateau;
}

void Partie::jouer() {
    // Ajoute l'état initial à l'historique
    historique.push_back(sauvegarder());

    do{
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
    }while(!plateau.isGameOver());
}