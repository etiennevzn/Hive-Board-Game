#include <iostream>
#include "partie.hpp"
#include "joueur.hpp"
#include "plateau.hpp"

int main() {
    Plateau plateau;
    Joueur joueur1(Noir);
    Joueur joueur2(Blanc);


    Partie partie;

    std::cout << "Bienvenue dans le jeu Hive!" << std::endl;
    partie.jouer();  

    return 0;
}

