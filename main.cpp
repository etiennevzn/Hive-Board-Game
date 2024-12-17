#include <iostream>
#include "partie.hpp"
#include "joueur.hpp"
#include "plateau.hpp"


/*-----------------------
Ajouts IA :

-partie :
bool contreIA;
Partie(int maxRetours, bool ia = false);
void jouerTourIA();
bool deplacerPieceIA();

-partie.cpp :
ajustement constructeur


-plateau :
bool Plateau::hasPiecesOnBoard(Couleur couleur) const

--------------------------*/


int main() {
    Partie partie(3, true); // Active l'IA (true)
    partie.jouer();

    return 0;
}


