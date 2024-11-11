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
    j2.poserPiece('R', Position(0,1), plateau,1);
    plateau.print_board();
    vector<Position> vec = j1.get_liste_placements(plateau);
    for(const auto& pos : vec){
        cout << "("<< pos.getColonne() << ", " << pos.getLigne() << ")" << endl;
    }   

    /*
    Partie partie(j1, j2,plateau,0);
    partie.jouer();
    cout<<"\nFin du jeu"<<endl;
    */

    return 0;
}

