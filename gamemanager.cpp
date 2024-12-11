#include "gamemanager.hpp"
    
GameManager* GameManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

void GameManager::demarrerNouvellePartie() {
    delete partie;
    Plateau plateau;
    Joueur j1(Noir);
    Joueur j2(Blanc);
    partie = new Partie(j1, j2, plateau);
    partie->play();
}