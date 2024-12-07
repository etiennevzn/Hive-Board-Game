#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "partie.hpp"

class GameManager {
    static GameManager* instance;
    Partie* partie;

    GameManager() : partie(nullptr) {}

public:
    static GameManager* getInstance() {
        if (instance == nullptr) {
            instance = new GameManager();
        }
        return instance;
    }

    void demarrerNouvellePartie() {
        delete partie;
        Plateau plateau;
        Joueur j1(Noir);
        Joueur j2(Blanc);
        partie = new Partie(j1, j2, plateau);
        partie->playTurn();
    }

    Partie* getPartie() const {
        return partie;
    }
};

// Initialisation du pointeur statique
GameManager* GameManager::instance = nullptr;

#endif