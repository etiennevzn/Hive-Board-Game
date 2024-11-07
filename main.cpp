#include "piece.cpp"
#include "plateau.cpp"
#include "joueur.cpp"
#include "partie.cpp"
#include <iostream>


int main(){

   cout<<"Debut du jeu\n"<<endl;

    Plateau plateau;
    Joueur j1(Noir);
    Joueur j2(Blanc);
        
    Reine reine1(Position(6, 4), Noir);
    Reine reine2(Position(5, 6), Blanc);
    Sauterelle sauterelle1(Position(5, 5), Noir);
    Araignee araignee1(Position(5, 7), Noir);
    

    
    plateau.addPiece(&reine1, reine1.getPosition());
    plateau.addPiece(&reine2, reine2.getPosition());
    plateau.addPiece(&sauterelle1, sauterelle1.getPosition());
    plateau.addPiece(&araignee1, araignee1.getPosition());
   
    plateau.print_board();

    //j1.print_piece_left();
    Partie partie(j1, j2,plateau,2);
    partie.jouer();
    cout<<"\nFin du jeu"<<endl;

    return 0;
}

