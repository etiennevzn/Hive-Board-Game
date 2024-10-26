#include "joueur.hpp"

void Joueur::print_piece_left(){
    cout<< "la liste des pièces restantes est : \n"<<endl;
    for(Piece * piece : pieces){
        cout<<  piece->getType()<<" "<<endl;
    }

};


bool Joueur::poserPiece(char pieceType, Position pos, Plateau& plateau) {
    Piece* piece = nullptr;
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if ((*it)->getInitial() == pieceType) {
            piece = *it;
            pieces.erase(it);
            break;
        }
    }

    if (piece == nullptr) {
        cout<<"La pièce n'a pas été trouvée dans la liste du joueur";
        return false; // 
    }

    // Vérifier si la position est valide pour poser la pièce
    if (plateau.isPositionOccupied(pos)) {
        cout<<"position occupée";
        return false; // La position est déjà occupée
    }

    plateau.addPiece(piece, pos);
    return true;
}