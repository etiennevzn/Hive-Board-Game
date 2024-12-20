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
    int choice = 0;
    while(choice != 1 && choice != 2){
        cout<<"Voulez-vous creer une partie : Joueur VS Joueur (1) ou Joueur VS IA (2) ?"<<endl;
        cout<<"Choix : "<<endl;
        cin>>choice;
    }
    Joueur* j1 = nullptr;
    Joueur* j2 = nullptr;
    if(choice == 1){
        j1 = new Joueur(Noir);
        j2 = new Joueur(Blanc);
    }else if(choice == 2){
        j1 = new Joueur(Noir);
        j2 = new Joueur(Blanc, true);
    }
    int nbRetoursEnArriere = 4;
    while(nbRetoursEnArriere < 0 || nbRetoursEnArriere > 3){
        cout<<"Combien de retours en arriere souhaitez-vous ? (0 a 3) : ";
        cin>>nbRetoursEnArriere;
    }
    partie = new Partie(*j1, *j2, plateau, 0, nbRetoursEnArriere);
    partie->play();
}

void GameManager::loadGame() {
    ifstream loadFile("savegame.txt");

    // Vérifier si le fichier existe et est ouvert
    if (!loadFile.is_open()) {
        cout << "Erreur : Impossible de trouver le fichier de sauvegarde 'savegame.txt'." << endl;
        cout << "Retour au menu principal..." << endl;
        return;
    }

    // Supprimer la partie précédente, s'il y en a une
    delete partie;

    Plateau plateau;
    Joueur j1(Noir);
    Joueur j2(Blanc);
    partie = new Partie(j1, j2, plateau, 0);

    // Charger la partie depuis le fichier
    partie->loadFromFile(loadFile);
    loadFile.close();

    // Commencer la partie chargée
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