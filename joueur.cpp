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

    if (tourActuel == 0) {
        //placer à 0,0
    }else if(tourActuel == 1){    // Si ce n'est pas le premier tour, vérifier si la position est adjacente à une autre pièce

    }else{
        if(get_liste_placements(plateau).size() == 0){
            cout << "Vous ne pouvez pas placer de piece." << endl;
            delete piece;
            return false;
        }
        // a faire : vérifier si la position est dans la get_liste_placements, et si oui alors placer la pièce à cette position
    }


    plateau.addPiece(piece, pos);
    return true;
}