#include "gamemanager.hpp"
    
GameManager* GameManager::getInstance() {
    if (instance == nullptr) {
        instance = new GameManager();
    }
    return instance;
}

void GameManager::startGame() {
    delete partie;
    Plateau plateau;
    Joueur j1(Noir);
    Joueur j2(Blanc);
    partie = new Partie(j1, j2, plateau);
    partie->play();
}

void GameManager::loadGame(){
    delete partie;
    ifstream loadFile("savegame.txt");
    Plateau plateau;
    Joueur j1(Noir);
    Joueur j2(Blanc);
    partie = new Partie(j1, j2, plateau, 0);
    partie->loadFromFile(loadFile);
    loadFile.close();
    partie->play();
}

void GameManager::printHiveArt() const{
    cout << " .----------------.  .----------------.  .----------------.  .----------------. " << endl;
    cout << "| .--------------. || .--------------. || .--------------. || .--------------. |" << endl;
    cout << "| |  ____  ____  | || |     _____    | || | ____   ____  | || |  _________   | |" << endl;
    cout << "| | |_   ||   _| | || |    |_   _|   | || ||_  _| |_  _| | || | |_   ___  |  | |" << endl;
    cout << "| |   | |__| |   | || |      | |     | || |  \\ \\   / /   | || |   | |_  \\_|  | |" << endl;
    cout << "| |   |  __  |   | || |      | |     | || |   \\ \\ / /    | || |   |  _|  _   | |" << endl;
    cout << "| |  _| |  | |_  | || |     _| |_    | || |    \\ ' /     | || |  _| |___/ |  | |" << endl;
    cout << "| | |____||____| | || |    |_____|   | || |     \\_/      | || | |_________|  | |" << endl;
    cout << "| |              | || |              | || |              | || |              | |" << endl;
    cout << "| '--------------' || '--------------' || '--------------' || '--------------' |" << endl;
    cout << " '----------------'  '----------------'  '----------------'  '----------------' " << endl;
}

void GameManager::startApp(){
    int running = 1;
    while(running == 1){
        printHiveArt();
        cout<<"1. Commencer une nouvelle partie"<<endl;
        cout<<"2. Charger une partie existante"<<endl;
        cout<<"3. Quitter"<<endl;
        cout<<"Choisissez une option : ";
        int choix;
        cin>>choix;
        cout<<endl;
        switch(choix){
            case 1:
                startGame();
                break;
            case 2:
                loadGame();
                break;
            case 3:
                running = 0;
                break;
            default:
                cout<<"Choix invalide. Veuillez reessayer."<<endl;
                // Efface l'erreur et ignore le reste de la ligne
                std::cin.clear(); // Réinitialise les erreurs du flux (par exemple, si un caractère non valide a été saisi)
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore les caractères jusqu'à la fin de la ligne
                break;
        }
    }
}