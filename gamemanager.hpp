#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "partie.hpp"

class GameManager {
    static GameManager* instance;
    Partie* partie;
    GameManager() : partie(nullptr) {}
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
public:
    static GameManager* getInstance();
    void demarrerNouvellePartie();
    Partie* getPartie() const {return partie;}
};

// Initialisation du pointeur statique
GameManager* GameManager::instance = nullptr;

#endif