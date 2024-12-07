#include "partie.hpp"
#include <iostream>

using namespace std;

void Partie::afficherMouvementsPossibles(Position pos, Couleur couleur) const {
    if (plateau.isPositionOccupied(pos)) {
        if(plateau.getPlateau().at(pos).back()->getCouleur() == couleur){
            Piece* piece = plateau.getPlateau().at(pos).back();
            cout << "Mouvements possibles pour la piece " << piece->getType() << " a la position (" << pos.getColonne() << ", " << pos.getLigne() << "):" << endl;
            vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
        }else {
            cout << "Ce n'est pas votre piece." << endl;
        }
    }
    else {
        cout << "Aucune pièce à cette position." << endl;
    }
}

void Partie::printPossiblePlays(Joueur* joueurCourant)const{
    cout<<"***************Actions possibles***************"<<endl;
    cout<<"****************Pieces a placer****************"<<endl;
    joueurCourant->print_piece_left();
    cout<<"***************Pieces a deplacer*****************"<<endl;
    vector<Piece*> pieces = joueurCourant->getPieces();
    if(pieces.size() == 0){
        cout<<"Aucune piece a deplacer"<<endl;
    }else{
        for(const auto& piece : pieces){
            vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
            if(validMoves.size() != 0){
                cout<<"Position de la piece : ("<<piece->getPosition().getColonne()<<","<<piece->getPosition().getLigne()<<") - Type de la piece : "<<piece->getType()<<endl;
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

void Partie::nextTurn() {
    tourActuel++;
    joueurCourant = (joueurCourant == &joueurs[0]) ? &joueurs[1] : &joueurs[0];
    playTurn();
}

void Partie::playTurn() {
    do{
        cout << "Debut du tour numero " << tourActuel << endl;
        plateau.print_board();
        cout<<endl;
        cout << "Tour du joueur " << " (" << (joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") << ")" << endl;
        printPossiblePlays(joueurCourant);
        cout<<endl;
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
                    cout<<"Premiere piece de la ruche, placee a la position (0,0)"<<endl;
                    joueurCourant->poserPiece(pieceType, Position(0, 0), plateau, tourActuel);
                    historique.push_back(sauvegarder()); 
                    nextTurn();
                    break;
                }

                vector<Position> posPossible = joueurCourant->get_liste_placements(plateau);
                if(tourActuel == 1){
                    for (const auto& pair : plateau.getPlateau()) { 
                        vector<Position> adjacents = pair.first.getAdjacentCoordinates();
                        for(const auto& pos : adjacents){
                            if(find(posPossible.begin(), posPossible.end(), pos) == posPossible.end()){
                                posPossible.push_back(pos);
                            }
                        }
                    }     
                }
                cout<<"Positions possibles pour le placement : "<<endl;
                for(const auto& pos : posPossible){
                    cout<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")"<<endl;
                }
                cout << "Entrez la position (q r): ";
                int q, r;
                cin >> q >> r;
                Position pos(q, r);

                if (joueurCourant->poserPiece(pieceType, pos, plateau, tourActuel)) {
                    historique.push_back(sauvegarder()); 
                    nextTurn();
                }
                else {
                    cout << "Impossible de poser la piece a cette position." << endl;
                }
                break;
            }
            case 2:{
                // Déplacer une pièce
                cout << "Entrez la position de depart (q r): ";
                int qFrom, rFrom;
                if(!(plateau.isPositionOccupied(Position(qFrom, rFrom)))) {
                    cout << "Aucune piece a cette position." << endl;
                    break;
                }
                if(plateau.getPlateau().at(Position(qFrom, rFrom)).back()->getCouleur() != joueurCourant->getCouleur()) {
                    cout << "Ce n'est pas votre piece." << endl;
                    break;
                }
                cin >> qFrom >> rFrom;
                Position from(qFrom, rFrom);
                cout << "Entrez la position de destination (q r): ";
                int qTo, rTo;
                cin >> qTo >> rTo;
                Position to(qTo, rTo);
                if (plateau.deplacerPiece(from, to, joueurCourant->getCouleur())) {
                    historique.push_back(sauvegarder());
                    nextTurn();
                }
                else {
                    cout << "Mouvement invalide. Réessayez." << endl;
                }
                break;
            }
            case 3: {
                // Voir les mouvements possibles pour une pièce spécifique
                cout << "Entrez la position de la piece (q r): ";
                int q, r;
                cin >> q >> r;
                Position pos(q, r);
                afficherMouvementsPossibles(pos,joueurCourant->getCouleur());
                break;
            }
            case 4: {
                // Annuler le dernier mouvement
                if (historique.size() > 1) {
                    Memento last = historique.back();
                    historique.pop_back();
                    restaurer(last);
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