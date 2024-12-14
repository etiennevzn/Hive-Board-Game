#include <iostream>
#include "partie.hpp"
#include "joueur.hpp"
#include "plateau.hpp"


int main() {
    Partie partie(3, true); // Initialiser une partie avec 3 retours en arrière possibles et avec IA
    partie.jouer();

    return 0;
}
