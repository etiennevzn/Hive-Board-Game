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
    Partie* getPartie() const {return partie;}
    void startGame();
    void loadGame();
    void printHiveArt()const;
    void startApp();
    ~GameManager() {delete partie;}
};

// Initialisation du pointeur statique
GameManager* GameManager::instance = nullptr;

#endif