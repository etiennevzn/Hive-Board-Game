#include "piece.cpp"
#include "plateau.cpp"
#include "joueur.cpp"
#include "partie.cpp"
#include <iostream>
#include "gamemanager.hpp"

int main() {
    Joueur joueur1("Joueur 1", Couleur::Noir);
    Joueur joueur2("Joueur 2", Couleur::Blanc);
    Plateau plateau;

    // Démarrer une nouvelle partie via GameManager
    GameManager::getInstance()->demarrerNouvellePartie(joueur1, joueur2, plateau);

    // Obtenir l'instance de Partie et commencer le jeu
    Partie* partie = GameManager::getInstance()->getPartie();
    partie->jouer();

    return 0;
}
