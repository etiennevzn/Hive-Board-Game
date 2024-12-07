#include "piece.cpp"
#include "plateau.cpp"
#include "joueur.cpp"
#include "partie.cpp"
#include <iostream>


int main(){
    /*
    // Démarrer une nouvelle partie via GameManager
    GameManager::getInstance()->demarrerNouvellePartie(joueur1, joueur2, plateau);

    // Obtenir l'instance de Partie et commencer le jeu
    Partie* partie = GameManager::getInstance()->getPartie();
    partie->jouer();
    */

   cout<<"Debut du jeu\n"<<endl;

    Plateau plateau;
    Joueur j1(Noir);
    Joueur j2(Blanc);


    j1.poserPiece('R', Position(0,0), plateau,0);
    j2.poserPiece('R', Position(1,0), plateau,1);


    j2.poserPiece('F', Position(1,-1), plateau,2);
    j2.poserPiece('M', Position(2,-2), plateau,2);
    j2.poserPiece('F', Position(1,-3), plateau,2);
    j2.poserPiece('H', Position(1,-4), plateau,2);
    j1.poserPiece('H', Position(-1,0), plateau,3);
    j1.poserPiece('H', Position(-2,-1), plateau,4);
    j1.poserPiece('F', Position(-1,-2), plateau,5);  
    j1.poserPiece('A', Position(-1,-3), plateau,4);

    //j2.poserPiece('H', Position(3,-2), plateau,4);

    //j1.poserPiece('F', Position(-1,-1), plateau,5);  
    plateau.print_board();
    cout<<endl;
    //cout<<Position(-1,-1).isSlidingPossible(Position(0,-1), plateau.getPlateau())<<endl;
    for(const auto& piece : j1.getPieces()){
        if(piece->getType() == "Araignee"){
            vector<Position> validMoves = dynamic_cast<Araignee*>(piece)->getValidMoves(piece->getPosition(), plateau.getPlateau());
            cout<<"Position de la piece : ("<<piece->getPosition().getColonne()<<","<<piece->getPosition().getLigne()<<")"<<endl;
            for(const auto& pos : validMoves){
                //cout<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")"<<endl;
                if(!plateau.wouldSplitHive(piece->getPosition(), pos)){
                    cout<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")"<<endl;
                }
            }
        }
    }

    //plateau.deplacerPiece(Position(2,-2), Position(0,-4), Blanc);
    //plateau.print_board();
    /*
    Partie partie(j1, j2,plateau,0);
    partie.jouer();
    cout<<"\nFin du jeu"<<endl;
    */
    return 0;
}

