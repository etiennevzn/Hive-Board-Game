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

    void demarrerNouvellePartie(const Joueur& j1, const Joueur& j2, const Plateau& p) {
        delete partie;
        partie = new Partie(j1, j2, p);
    }

    Partie* getPartie() const {
        return partie;
    }
};

// Initialisation du pointeur statique
GameManager* GameManager::instance = nullptr;

#endif