#include "joueur.hpp"

void Joueur::print_piece_left(){
    cout<< "la liste des pièces restantes est : \n"<<endl;
    for(Piece * piece : pieces){
        cout<<  piece->getType()<<" "<<endl;
    }

};