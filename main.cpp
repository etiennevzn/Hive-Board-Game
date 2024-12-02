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


    j1.poserPiece('R', Position(0,0), plateau,0);
    j2.poserPiece('R', Position(1,0), plateau,1);


    j2.poserPiece('A', Position(1,-1), plateau,2);
    j2.poserPiece('F', Position(2,-2), plateau,2);
    j2.poserPiece('F', Position(1,-3), plateau,2);
    j2.poserPiece('A', Position(1,-4), plateau,2);
    j1.poserPiece('H', Position(-1,0), plateau,3);
    j1.poserPiece('H', Position(-2,-1), plateau,4);
    j1.poserPiece('F', Position(-1,-2), plateau,5);  
    j1.poserPiece('A', Position(-1,-3), plateau,4);

    //j2.poserPiece('H', Position(3,-2), plateau,4);

    //j1.poserPiece('F', Position(-1,-1), plateau,5);  
    plateau.print_board();
    //cout<<Position(-1,-1).isSlidingPossible(Position(0,-1), plateau.getPlateau())<<endl;
    for(const auto& piece : j2.getPieces()){
        if(piece->getType() == "Araignee"){
            cout<<piece->isValidMove(Position(3,-2), plateau.getPlateau());
        }
    }
    /*
    Partie partie(j1, j2,plateau,0);
    partie.jouer();
    cout<<"\nFin du jeu"<<endl;
    */
    return 0;
}

