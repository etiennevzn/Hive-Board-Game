#include "joueur.hpp"
#include "partie.hpp"


void Joueur::print_piece_left(){
    cout<< "la liste des pieces restantes est : \n"<<endl;
    for (const auto& pair : nb_pieces) {
        std::cout << "Pièce " << pair.first << " : " << pair.second << " unités" << std::endl;
    }

};

vector<Position> Joueur::get_liste_placements(const Plateau& plateau){ //donne toutes les cases possibles ou un joueur peut placer une piece
    vector<Position> liste_placements;
    for (const auto& piece : pieces) {
        vector<Position> adjacents = piece->getPosition().getAdjacentCoordinates();
        for (const auto& pos : adjacents) {
            if (!plateau.isPositionOccupied(pos)) {
                vector<Position> adjacents_pos_libre = pos.getAdjacentCoordinates();
                for (const auto& pos_finale : adjacents_pos_libre) {
                    bool isFriendly = true;
                    if(plateau.getPlateau().find(pos_finale) != plateau.getPlateau().end()){
                        for (const auto& piece2 : plateau.getPlateau().at(pos_finale)) {
                            if (piece2->getCouleur() != couleur) {
                                isFriendly = false;
                            }
                        }
                    }
                    if (isFriendly) {
                        liste_placements.push_back(pos);
                    }
                }               
            }
        }
    }
    return liste_placements;
}

bool Joueur::poserPiece(char pieceType, Position pos, Plateau& plateau,int tourActuel) {
    Piece* piece = nullptr;

    // Vérifier que la pièce est encore disponible pour le Joueur 
    if ((piece->getType() == "Reine" && nb_pieces["R"] >= 1) ||
        (piece->getType() == "Araignee" && nb_pieces["A"] >= 2) ||
        (piece->getType() == "Scarabee" && nb_pieces["S"] >= 2) ||
        (piece->getType() == "Fourmi" && nb_pieces["F"] >= 3) ||
        (piece->getType() == "Sauterelle" && nb_pieces["H"] >= 3)||
        (piece->getType() == "Coccinelle" && nb_pieces["C"] >= 1)||
        (piece->getType() == "Moustique" && nb_pieces["M"] >= 1)) {
        cout << "Vous n'avez plus de pieces de ce type disponibles." << endl;
        delete piece;
        return false;
    }

    // Vérifier si la position est valide pour poser la pièce
    if (plateau.isPositionOccupied(pos)) {
        cout<<"position occupee"<< endl;
        return false; // La position est déjà occupée
    }



    if (tourActuel == 0) {
        //placer à 0,0
    }else{    // Si ce n'est pas le premier tour, vérifier si la position est adjacente à une autre pièce
        bool adjacent = false;
        vector<Position> adjacents = pos.getAdjacentCoordinates();
        for (const Position& adj : adjacents) {
            if (plateau.isPositionOccupied(adj)) {
                adjacent = true;
                break;
            }
        }
        if (!adjacent) {
            cout << "La piece doit etre placee adjacente a une autre piece." << endl;
            return false;
        }
    }


    plateau.addPiece(piece, pos);
    return true;
}