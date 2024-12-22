#include "partie.hpp"
#include <iostream>
#include <cstdlib>  // Pour std::rand() et std::srand()
#include <ctime>    // Pour std::time()

using namespace std;

Partie::Partie(const Joueur& j1, const Joueur& j2, const Plateau& p, int t, int maxRetours) : joueurs{j1, j2}, tourActuel(t),plateau(p), maxRetoursEnArriere(maxRetours) {
    if(!j1.getIsIa() && !j2.getIsIa()){
        srand(static_cast<unsigned int>(time(nullptr))); 
        joueurCourant = (std::rand() % 2 == 0) ? &joueurs[0] : &joueurs[1];
    }else if(!j1.getIsIa() && j2.getIsIa()){
        joueurCourant = &joueurs[0];
    }
    else if(j1.getIsIa() && !j2.getIsIa()){
        joueurCourant = &joueurs[1];
    }
}

void Partie::afficherMouvementsPossibles(Position pos, Couleur couleur){
    if (plateau.isPositionOccupied(pos)) {
        if(plateau.getPlateau().at(pos).back()->getCouleur() == couleur){
            Piece* piece = plateau.getPlateau().at(pos).back();
            vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
            for(const auto& pos : validMoves){
                if(plateau.wouldSplitHive(piece->getPosition(), pos)){
                    validMoves.erase(remove(validMoves.begin(), validMoves.end(), pos), validMoves.end());
                }
            }
            if(validMoves.size() == 0){
                cout << "Aucun mouvement possible pour cette piece." << endl;
            }else{
                cout << "Mouvements possibles pour la piece " << piece->getType() << " a la position (" << pos.getColonne() << ", " << pos.getLigne() << "):" << endl;
                for (const auto& move : validMoves) {
                    cout << "(" << move.getColonne() << ", " << move.getLigne() << ")" << endl;
                }
            }
            
        }else {
            cout << "Ce n'est pas votre piece." << endl;
        }
    }
    else {
        cout << "Aucune piece a cette position." << endl;
    }
}

void Partie::printPossiblePlays(Joueur* joueurCourant){
    cout<<"***************Actions possibles***************"<<endl;
    cout<<"****************Pieces a placer****************"<<endl;
    joueurCourant->print_piece_left();
    cout<<"***************Pieces a deplacer*****************"<<endl;
    vector<Piece*> pieces = joueurCourant->getPieces();
    if(pieces.size() == 0){
        cout<<"Aucune piece a deplacer"<<endl;
    }else{
        for(const auto& piece : pieces){
            vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
            for(const auto& pos : validMoves){
                if(plateau.wouldSplitHive(piece->getPosition(), pos)){
                    validMoves.erase(remove(validMoves.begin(), validMoves.end(), pos), validMoves.end());
                }
            }
            if(validMoves.size() != 0){
                cout<<"Position de la piece : ("<<piece->getPosition().getColonne()<<","<<piece->getPosition().getLigne()<<") - Type de la piece : "<<piece->getType()<<endl;
            }
        }
    }
}


//Retour en arriere

// Sauvegarder l'état actuel de la partie
void Partie::sauvegarderEtat() {
    historique.push(PartieMemento(plateau, joueurs[0], joueurs[1], joueurCourant, tourActuel));
}

// Restaurer l'état précédent de la partie
void Partie::restaurerEtat() {
    if (!historique.empty()) {
        PartieMemento m = historique.top();
        historique.pop();
        plateau = m.plateau; ///utilisation de l'opérateur d'affectation de plateau
        //Puis on recopie les joueurs et on leur attribue les pièces à partir du plateau
        joueurs[0].setCouleur(m.joueur1.getCouleur());
        joueurs[0].setIsIa(m.joueur1.getIsIa());
        for(const auto& pair : m.joueur1.getNbPieces()){
            joueurs[0].setNbPieces(pair.first, pair.second);
        }

        joueurs[1].setCouleur(m.joueur2.getCouleur());
        joueurs[1].setIsIa(m.joueur2.getIsIa());
        for(const auto& pair : m.joueur2.getNbPieces()){
            joueurs[1].setNbPieces(pair.first, pair.second);
        }

        joueurs[0].clearPieces();
        joueurs[1].clearPieces();
        for(const auto& pair : plateau.getPlateau()){
            for(Piece* piece : pair.second){
                if(piece->getCouleur() == joueurs[0].getCouleur()){
                    joueurs[0].addPiece(piece);
                }else if(piece->getCouleur() == joueurs[1].getCouleur()){
                    joueurs[1].addPiece(piece);
                }
            }
        }

        if(m.joueurCourant == &m.joueur1){
            joueurCourant = &joueurs[0];
        }else if(m.joueurCourant == &m.joueur2){
            joueurCourant = &joueurs[1];
        }
        tourActuel = m.tourActuel; 
    }
}

// Restaurer les deux derniers états de la partie
void Partie::restaurerDeuxDerniersEtats() {
    historique.pop();
    historique.pop();
    restaurerEtat();
}

bool Partie::isGameOver(){
    for(const auto& pair : plateau.getPlateau()){
        for(const Piece* piece : pair.second){
            if(piece->getType() == "Reine"){
                if(dynamic_cast<const Reine*>(piece)->isSurrounded(plateau.getPlateau())){ //defaite d'un des deux joueurs
                    return true;
                }
            }
        }
    }

    for(auto& joueur : joueurs){
        if(joueur.get_liste_placements(plateau).size() != 0){ 
            if ((joueur.getNbPieces().at("R") < 1) ||
            (joueur.getNbPieces().at("A") < 2) ||
            (joueur.getNbPieces().at("S") < 2) ||
            (joueur.getNbPieces().at("F") < 3) ||
            (joueur.getNbPieces().at("H") < 3) ||
            (joueur.getNbPieces().at("C") < 1) ||
            (joueur.getNbPieces().at("M") < 1)){
                return false; //si on des joueurs a encore au moins une pièce qu'il peut placer, la partie n'est pas finie
            }  
        }

        

    }

    for(const auto& pair : plateau.getPlateau()){
        Piece* piece = pair.second.back();
        vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
        for(const auto& pos : validMoves){
            if(plateau.wouldSplitHive(piece->getPosition(), pos)){
                validMoves.erase(remove(validMoves.begin(), validMoves.end(), pos), validMoves.end());
            }
        }
        if(validMoves.size() != 0){
            return false;
        }
    }

    return true; //si on arrive la, c'est que personne n'a gagné mais les deux joueurs ne peuvent plus ni bouger ni placer de pièces : égalité

}



void Partie::playTurn() {
    sauvegarderEtat(); //on commence par sauvegarder l'état
    bool turnOver = false;
    //Rq : on affiche tourActuel+1 car on commence c'est indexé à 0... si besoin pour debug on peut enlever ça
    cout << "************Debut du tour numero : " << tourActuel+1 <<"***************"<<endl;
    cout<<"************Etat actuel du plateau*************"<<endl;
    plateau.print_board();
    cout<<endl;
    cout << "Tour du joueur "<< joueurCourant->getNom() << " ("<<(joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") <<")"<< endl;
    if(tourActuel > 2){
        bool canPlay = false;

        for(const auto& piece : joueurCourant->getPieces()) {
            vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
            vector<Position> finalMoves;

            for(const auto& pos : validMoves) {
                if(!plateau.wouldSplitHive(piece->getPosition(), pos)) {
                    finalMoves.push_back(pos);
                }
            }

            if(!finalMoves.empty()) {
                canPlay = true;
                break;
            }
        }

        vector<Position> placementMoves = joueurCourant->get_liste_placements(plateau);
        if(!placementMoves.empty()) {
            canPlay = true;
        }
        if(!canPlay){
            cout<<"Aucun coup possible pour le joueur "<< joueurCourant->getNom() << " ("<<(joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") <<") "<<endl;
            cout<<"Vous devez passer votre tour."<<endl;
            return;
        }
    }
    if(joueurCourant->getIsIa()){
        joueurCourant->playTurnIA(plateau, tourActuel);
        turnOver = true;
    }
    if(!joueurCourant->getIsIa()) printPossiblePlays(joueurCourant);
    cout<<endl;
    while(!turnOver){
        if((tourActuel == 6 || tourActuel == 7) && !plateau.isReinePlaced(joueurCourant->getCouleur())){
            cout<<"Vous devez poser la reine avant ce tour"<<endl;
            vector<Position> posPossible = joueurCourant->get_liste_placements(plateau);
            cout<<"Positions possibles pour le placement : "<<endl;
            for(const auto& pos : posPossible){
                cout<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")"<<endl;
            }
            cout << "Entrez la position (q r): ";
            int q, r;
            cin >> q >> r;
            Position pos(q, r);

            if (joueurCourant->poserPiece('R', pos, plateau, tourActuel)) {
                turnOver = true;
                break;
            }
            else {
                cout << "Impossible de poser la Reine." << endl;
                turnOver = true;
            }
            break;
        }
        cout << "Que souhaitez vous faire ?" << endl;
        cout << "1. Poser une piece" << endl;
        cout << "2. Deplacer une piece" << endl;
        cout << "3. Voir les mouvements possibles pour une piece" << endl;
        cout << "4. Annuler votre dernier coup" << endl;
        cout << "5. Sauvegarder la partie et quitter" << endl;
        cout << "Choisissez une option: "<<endl;
        int choix;
        cin >> choix;
        switch (choix) {
            case 1: {
                // Poser une pièce
                cout << "Choisissez une piece a poser (R: Reine, S: Scarabee, A: Araignee, H: Sauterelle, F: Fourmi): ";
                char pieceType;
                cin >> pieceType;
                //si premier tour on pose juste la pîèce au milieu
                if (tourActuel == 0) {
                    cout<<"Premiere piece de la ruche, placee a la position (0,0)"<<endl;
                    if(joueurCourant->poserPiece(pieceType, Position(0, 0), plateau, tourActuel)){
                        turnOver = true;
                        break;
                    }else{
                        cout<<"Impossible de poser la piece."<<endl;
                        break;
                    }
                }

                vector<Position> posPossible = joueurCourant->get_liste_placements(plateau);
                if(tourActuel == 1){
                    for (const auto& pair : plateau.getPlateau()) { 
                        vector<Position> adjacents = pair.first.getAdjacentCoordinates();
                        for(const auto& pos : adjacents){
                            if(find(posPossible.begin(), posPossible.end(), pos) == posPossible.end()){
                                posPossible.push_back(pos);
                            }
                        }
                    }     
                }
                cout<<"Positions possibles pour le placement : "<<endl;
                for(const auto& pos : posPossible){
                    cout<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")"<<endl;
                }
                cout << "Entrez la position (q r): ";
                int q, r;
                cin >> q >> r;
                Position pos(q, r);

                if (joueurCourant->poserPiece(pieceType, pos, plateau, tourActuel)) {
                    turnOver = true;
                    break;
                }
                else {
                    cout << "Impossible de poser la piece." << endl;
                }
                break;
            }
            case 2:{
                // Déplacer une pièce
                if(!plateau.isReinePlaced(joueurCourant->getCouleur())){
                    cout<<"Vous devez d'abord placer la reine avant de pouvoir deplacer une piece."<<endl;
                    break;
                }
                cout << "Entrez la position de depart (q r): ";
                int qFrom, rFrom;
                cin >> qFrom >> rFrom;
                Position from(qFrom, rFrom);
                if(!(plateau.isPositionOccupied(Position(qFrom, rFrom)))) {
                    cout << "Aucune piece a cette position." << endl;
                    break;
                }
                if(plateau.getPlateau().at(Position(qFrom, rFrom)).back()->getCouleur() != joueurCourant->getCouleur()) {
                    cout << "Ce n'est pas votre piece." << endl;
                    break;
                }
                if(plateau.getPlateau().at(Position(qFrom, rFrom)).back()->getValidMoves(plateau.getPlateau()).size() == 0){
                    cout << "Aucun mouvement possible pour cette piece" << endl;
                    cin.clear(); 
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                vector<Position> validMoves = plateau.getPlateau().at(Position(qFrom, rFrom)).back()->getValidMoves(plateau.getPlateau());
                cout<<"Mouvements possibles pour la piece : "<<endl;
                for(const auto& pos : validMoves){
                    if(!plateau.wouldSplitHive(from,pos)){
                        cout<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")"<<endl;
                    }
                }
                cout << "Entrez la position de destination (q r): ";
                int qTo, rTo;
                cin >> qTo >> rTo;
                Position to(qTo, rTo);
                if (plateau.deplacerPiece(from, to, joueurCourant->getCouleur())) {
                    turnOver = true;
                    break;
                }
                else {
                    cout << "Mouvement invalide. Reessayez." << endl;
                }
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case 3: {
                // Voir les mouvements possibles pour une pièce spécifique
                cout << "Entrez la position de la piece (q r): ";
                int q, r;
                cin >> q >> r;
                Position pos(q, r);
                afficherMouvementsPossibles(pos,joueurCourant->getCouleur());
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case 4: {
                // Annuler le dernier mouvement
                
                if (historique.size() > 1) {
                    if(maxRetoursEnArriere == 0){
                        cout << "Vous avez atteint le nombre maximum de retours en arriere." << endl;
                        cin.clear(); 
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    cout << "Annulation du dernier mouvement..." << endl;
                    restaurerDeuxDerniersEtats();
                    --maxRetoursEnArriere;
                    cout<<"Coup annule."<<endl;
                    cout<<"Nombre de retours en arriere restants : "<<maxRetoursEnArriere<<endl;
                    cout<<"Tour actuel : "<<tourActuel+1<<endl;
                    cout<<"Etat actuel du plateau : "<<endl;
                    plateau.print_board();
                    break;
                }
                else {
                    cout << "Aucun mouvement a annuler." << endl;
                }
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            case 5: {
                choix = -1;
                while(choix != 0 && choix != 1){
                    cout << "Attention : la sauvegarde predecente sera ecrasee. Voulez-vous continuer ? (0: Non, 1: Oui)" << endl;
                    cin >> choix;
                }
                if(choix == 1){
                    cout << "Sauvegarde en cours..." << endl;
                    ofstream saveFile("savegame.txt");
                    saveToFile(saveFile);
                    saveFile.close();
                    cout << "Partie sauvegardee." << endl;
                    cout << "Merci d'avoir joue a HIVE ! " << endl;
                    exit(0);
                }else if (choix == 0){
                    cout << "Sauvegarde annulee." << endl;
                }
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            default:
                cout << "Choix invalide. Veuillez reessayer." << endl;
                // Efface l'erreur et ignore le reste de la ligne
                cin.clear(); // Réinitialise les erreurs du flux (par exemple, si un caractère non valide a été saisi)
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore les caractères jusqu'à la fin de la ligne
                break;          
        }
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Partie::play(){
    cout<<"************Debut de la partie************\n"<<endl;
    while(tourActuel<6 || !isGameOver()){
        playTurn();
        joueurCourant = (joueurCourant == &joueurs[0]) ? &joueurs[1] : &joueurs[0];
        ++tourActuel;
    }
    bool draw = true;
    for(auto& joueur : joueurs){
        if(joueurCourant->get_liste_placements(plateau).size() == 0){
            for(const auto& piece : joueur.getPieces()){
                vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
                for(const auto& pos : validMoves){
                    if(plateau.wouldSplitHive(piece->getPosition(), pos)){
                        validMoves.erase(remove(validMoves.begin(), validMoves.end(), pos), validMoves.end());
                    }
                }
                if(validMoves.size() != 0) {
                    draw = false;
                    break;
                }
            }
        }else{
            draw = false;
            break;
        }
    }
    if(draw){
        cout<<"Fin de la partie. Resultat : Egalite"<<endl;
        cout<<"Merci d'avoir joue a HIVE ! "<<endl;
        return;
    }else{
        cout<<"Fin de la partie. Resultat : ";
        for(auto& pair : plateau.getPlateau()){
            for(auto& piece : pair.second){
                if(piece->getType() == "Reine"){
                    if(dynamic_cast<Reine*>(piece)->isSurrounded(plateau.getPlateau())){
                        if(piece->getCouleur() == Noir){
                            cout<<"Le joueur Blanc a gagne."<<endl;
                        }else{
                            cout<<"Le joueur Noir a gagne."<<endl;
                        }
                        cout<<"Merci d'avoir joue a HIVE ! "<<endl;
                        return;
                    }
                }
            }
        }
    }
}




//Sauvegarde

#include <fstream>

void Partie::saveToFile(ofstream& outFile) const {
    if (!outFile) {
        cerr << "Erreur lors de l'ouverture du fichier pour la sauvegarde." << endl;
        return;
    }
    plateau.save(outFile);
    for (const auto& joueur : joueurs) {
        joueur.save(outFile);
    }
    int joueurCourantIndex = (joueurCourant == &joueurs[0]) ? 0 : 1;
    outFile << joueurCourantIndex <<"\n";
    outFile << tourActuel << "\n";
}

void Partie::loadFromFile(ifstream& inFile) {
    if (!inFile) {
        cerr << "Erreur lors de l'ouverture du fichier pour le chargement." << endl;
        return;
    }
    plateau.load(inFile);
    for (auto& joueur : joueurs) {
        joueur.load(inFile, plateau);
    }
    int joueurCourantIndex;
    inFile >> joueurCourantIndex;
    joueurCourant = &joueurs[joueurCourantIndex];
    if (!(inFile >> tourActuel)) {
        throw std::runtime_error("Erreur de lecture du tour actuel dans le fichier.");
    }
}
