#include "piece.cpp"
#include "plateau.cpp"
#include "joueur.cpp"
#include "partie.cpp"
#include <iostream>


int main(){

    cout<<"Dt du jeu\n"<<endl;

    Plateau plateau;
    Joueur j1(Noir);
    Joueur j2(Blanc);
    Partie partie(j1, j2,plateau);    
   /* Reine reine1(Position(2, 4), Noir);
    Reine reine2(Position(1, 1), Blanc);
    Scarabee scarabee1(Position(2, 6), Noir);
    Araignee araignee1(Position(1, 3), Blanc);
    Sauterelle sauterelle1(Position(4, 2), Noir);
    Fourmi fourmi1(Position(3, 5), Blanc);
    
    plateau.addPiece(&reine1, reine1.getPosition());
    plateau.addPiece(&reine2, reine2.getPosition());
    plateau.addPiece(&scarabee1, scarabee1.getPosition());
    plateau.addPiece(&araignee1, araignee1.getPosition());
    plateau.addPiece(&sauterelle1, sauterelle1.getPosition());
    plateau.addPiece(&fourmi1, fourmi1.getPosition());
   
    plateau.print_board();*/

    j1.print_piece_left();
    partie.jouer();
    cout<<"\nFin dddu jeu"<<endl;

    return 0;
}

