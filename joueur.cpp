#include "joueur.hpp"
#include "partie.hpp"
#include <algorithm> //pour std::find


void Joueur::print_piece_left(){
    cout<< "la liste des pieces restantes est : \n"<<endl;
    for (const auto& pair : nb_pieces) {
        std::cout << "Pièce " << pair.first << " : " << pair.second << " unités" << std::endl;
    }

};

vector<Position> Joueur::get_liste_placements(const Plateau& plateau) {
    unordered_set<Position> valid_positions; // Ensemble pour éviter les doublons

    for (Piece* piece : pieces) {
        vector<Position> adj_positions = piece->getPosition().getAdjacentCoordinates(); //on prend les positions adjacentes aux pièces du joueur sur le plateau

        for (const Position& adj : adj_positions) { //pour chaque position adjacente
            if (!plateau.isPositionOccupied(adj)) { //on regarde si la position est libre
                bool adjacent_to_opponent_piece = false;
                for (const Position& other_adj : adj.getAdjacentCoordinates()) { //on regarde si la position est adjacente à une pièce adverse
                    if (plateau.isPositionOccupied(other_adj) &&
                        plateau.getPlateau().at(other_adj)[0]->getCouleur() != couleur) {
                        adjacent_to_opponent_piece = true;
                        break;
                    }
                }
                if (!adjacent_to_opponent_piece) { //si la position est libre et non adjacente a une pièce adverse, on peut poser une pièce dessus
                    valid_positions.insert(adj);
                }
            }
        }
    }
    return vector<Position>(valid_positions.begin(), valid_positions.end()); //on retourne un vecteur des positions valides
}


bool Joueur::poserPiece(char pieceType, Position pos, Plateau& plateau,int tourActuel) {
    Piece* piece = nullptr;
    Position origin(0, 0);

    // Vérifier que la pièce est encore disponible pour le Joueur 
    if ((pieceType == 'R' && nb_pieces["R"] >= 1) ||
        (pieceType == 'A' && nb_pieces["A"] >= 2) ||
        (pieceType == 'S' && nb_pieces["S"] >= 2) ||
        (pieceType == 'F' && nb_pieces["F"] >= 3) ||
        (pieceType == 'H' && nb_pieces["H"] >= 3)||
        (pieceType == 'C' && nb_pieces["C"] >= 1)||
        (pieceType == 'M' && nb_pieces["M"] >= 1)) {
        cout << "Vous n'avez plus de pieces de ce type disponibles." << endl;
        delete piece;
        return false;
    }

    switch(pieceType){ //on créé la pièce avec position temporaire l'origine, qui sera changée en fonction des cas
        case 'R':
            piece = new Reine(origin, couleur);
            break;
        case 'A':
            piece = new Araignee(origin, couleur);
            break;
        case 'S':
            piece = new Scarabee(origin, couleur);
            break;
        case 'F':
            piece = new Fourmi(origin, couleur);
            break;
        case 'H':
            piece = new Sauterelle(origin, couleur);
            break;
        case 'C':
            piece = new Coccinelle(origin, couleur);
            break;
        case 'M':
            piece = new Moustique(origin, couleur);
            break;
    }

    if (tourActuel == 0) {
        //placer à 0,0
        if(plateau.getPlateau().find(origin) != plateau.getPlateau().end()){ //si l'origine est déjà occupée (c'est pas censé être le cas normalement)
            cout << "Erreur : la position d'origine est deja occupee." << endl;
            delete piece;
            return false;
        }
        if(pos != origin){ //si la position n'est pas 0,0
            cout << "La premiere piece doit etre placee a l'origine" << endl;
            delete piece;
            return false;
        }
        plateau.addPiece(piece, origin);
        pieces.push_back(piece);

        string typeStr(1, pieceType);
        nb_pieces[typeStr]++;

        return true;
    }else if(tourActuel == 1){ //Si ce n'est pas le premier tour, vérifier si la position est adjacente à une autre pièce
        bool isAdjacent = false;
        if(!plateau.isPositionOccupied(pos)){
            for (const auto& pair : plateau.getPlateau()) { //normalement à ce stade il n'y a qu'une position occupée, l'origine, mais on parcourt tout le plateau quand même
                if (pair.first.isAdjacent(pos)) {
                    isAdjacent = true;
                }
            }
        } //Résumé : si la position est libre et adjacente a un pièce (peut importe la couleur au premier tour), alors on peut placer la pièce
        if (!isAdjacent) {
            cout << "La piece doit etre placee a cote d'une autre piece." << endl;
            delete piece;
            return false;
        }
        plateau.addPiece(piece, pos);
        pieces.push_back(piece);

        string typeStr(1, pieceType);
        nb_pieces[typeStr]++;

        return true;
    }else{
        vector<Position> possible_placements = get_liste_placements(plateau);
        if(possible_placements.size() == 0){
            cout << "Vous ne pouvez pas placer de piece." << endl;
            delete piece;
            return false;
        }

        bool isValid = false;
        if(find(possible_placements.begin(), possible_placements.end(), pos) != possible_placements.end()){
            isValid = true;
        }

        if(!isValid){
            cout << "Position impossible pour le placement de la piece." << endl;
            cout << "Liste des positions valides : " << endl;
            for (const auto& pos : possible_placements) {
                cout << "("<< pos.getColonne() << ", " << pos.getLigne() << ")" << endl;
            }
            delete piece;
            return false;
        }

        plateau.addPiece(piece, pos);
        pieces.push_back(piece);

        string typeStr(1, pieceType);
        nb_pieces[typeStr]++;

        return true;
    }

    cout << "Une erreur s'est produite." << endl;
    delete piece;
    return false;
}