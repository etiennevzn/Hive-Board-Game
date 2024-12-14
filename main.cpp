#include <iostream>
#include "partie.hpp"
#include "joueur.hpp"
#include "plateau.hpp"


/*-----------------------
Pour memento :

partie.hpp
struct PartieMemento
historique
maxRetoursEnArriere

partie.cpp
constructeur de Partie
methodes sauvergarderEtat(), restaurerEtat(), restaurerDeuxDerniersEtats()
--------------------------*/

int main() {
    Partie partie(3, true); // Initialiser une partie avec 3 retours en arrière possibles et avec IA
    partie.jouer();

    return 0;
}
