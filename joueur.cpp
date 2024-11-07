#include "joueur.hpp"
#include "partie.hpp"

void Joueur::print_piece_left(){
    cout<< "la liste des pieces restantes est : \n"<<endl;
    for(Piece * piece : pieces){
        cout<<  piece->getType()<<" "<<endl;
    }

};


bool Joueur::poserPiece(char pieceType, Position pos, Plateau& plateau,int tourActuel) {
    Piece* piece = nullptr;
    auto it = pieces.end();
    for (auto iter = pieces.begin(); iter != pieces.end(); ++iter) {
        if ((*iter)->getInitial() == pieceType) {
            piece = *iter;
            it = iter;
            break;
        }
    }

    if (piece == nullptr) {
        cout<<"La piece n'a pas ete trouvee dans la liste du joueur"<< endl;
        return false; // 
    }

    // Vérifier si la position est valide pour poser la pièce
    if (plateau.isPositionOccupied(pos)) {
        cout<<"position occupee"<< endl;
        return false; // La position est déjà occupée
    }


    // Si ce n'est pas le premier tour, vérifier si la position est adjacente à une autre pièce
    if (tourActuel != 0) {
        bool adjacent = false;
        vector<Position> adjacents = pos.getAdjacentCoordinates();
        for (const Position& adj : adjacents) {
            if (plateau.isPositionOccupied(adj)) {
                adjacent = true;
                break;
            }
        }

        if (!adjacent) {
            cout << "La piece doit être placee adjacente a une autre piece." << endl;
            return false;
        }
    }


    plateau.addPiece(piece, pos);
    pieces.erase(it);
    return true;
}