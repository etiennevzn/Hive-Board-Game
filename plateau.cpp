#include "plateau.hpp"
#include <iomanip>
#include <sstream>

Plateau& Plateau::operator=(const Plateau& p) {
    plateau.clear();
    if(this != &p){
        //Pour chaque position du plateau, on créé des pointeurs vers des pièces identiques, mais pas les mêmes pointeurs. 
        for(auto& pair : p.plateau){
            Position posTraitee = pair.first;
            if(pair.second.size() > 0){
                for(auto& piece : pair.second){
                    Piece* newPiece = nullptr;
                    char pieceType = piece->getInitial();
                    Couleur pieceCouleur = piece->getCouleur();
                    switch(pieceType){ //on créé la pièce avec position temporaire l'origine, qui sera changée en fonction des cas
                        case 'R':
                            newPiece = new Reine(posTraitee, pieceCouleur);
                            break;
                        case 'A':
                            newPiece = new Araignee(posTraitee, pieceCouleur);
                            break;
                        case 'S':
                            newPiece = new Scarabee(posTraitee, pieceCouleur);
                            break;
                        case 'F':
                            newPiece = new Fourmi(posTraitee, pieceCouleur);
                            break;
                        case 'H':
                            newPiece = new Sauterelle(posTraitee, pieceCouleur);
                            break;
                        case 'C':
                            newPiece = new Coccinelle(posTraitee, pieceCouleur);
                            break;
                        case 'M':
                            newPiece = new Moustique(posTraitee, pieceCouleur);
                            break;
                        default:
                            cout << "Type de piece invalide." << endl;
                            delete piece;
                    }
                    plateau[posTraitee].push_back(newPiece);
                }
            }else{
                //plateau[posTraitee] = vector<Piece*>();
            }
        }

    }
    return *this;
}


bool Plateau::isPositionOccupied(Position pos) const {
    auto it = plateau.find(pos);
    return it != plateau.end() && !it->second.empty();
}



void Plateau::dfs(Position pos, const unordered_map<Position, vector<Piece*>>& tempPlateau, unordered_set<Position>& visited) {
    visited.insert(pos);
    vector<Position> adjacents = pos.getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        auto it = tempPlateau.find(adj);
        if (it != tempPlateau.end() && !it->second.empty() && visited.find(adj) == visited.end()) {
            dfs(adj, tempPlateau, visited);
        }
    }
}


bool Plateau::isHiveConnected(const unordered_map<Position, vector<Piece*>>& tempPlateau){
    unordered_set<Position> visited;
    Position start(0,0); 
    bool foundStart = false;
    for (const auto& entry : tempPlateau) {
        if (!entry.second.empty()) {
            start = entry.first; // Trouve la première position occupée
            foundStart = true;
            break;
        }
    }

    if (!foundStart) return true;  // Si aucune position occupée n'existe, la ruche est connectée par défaut

    dfs(start, tempPlateau, visited); //on fait un recherche en profondeur avec tous les voisins

    for (const auto& entry : tempPlateau) {
        if (!entry.second.empty() && visited.find(entry.first) == visited.end()) { //si une position est non vide dans le plateau mais qu'elle n'est pas dans visited, cela signifie que la ruche n'est pas en 1 morceau
            return false;
        }
    }

    return true;
}


bool Plateau::wouldSplitHive(Position from, Position to){

    if (plateau[from].empty()) { // Vérifie qu'il y a une pièce à la position d'origine 
        return false;
        //throw std::runtime_error("Aucune piece a deplacer depuis cette position.");
    } //a voir si on fait ça ou simplement un return false (car dans si il n'y a pas de pièce à déplacer ça va pas split)

    unordered_map<Position, vector<Piece*>> tempPlateau = plateau; //on fait une copie du plateau pour pouvoir faire des changements temporaires

    Piece* piece = tempPlateau[from].back(); // Récupère la dernière pièce (pièce la plus en haut de l'empilement en cas d'empilement)

    //simulation du déplacement
    tempPlateau[from].pop_back();
    tempPlateau[to].push_back(piece);
    return !isHiveConnected(tempPlateau); //si le plateau n'est pas conecté alors on return true (ça casserait la ruche), et vice-versa
}

void Plateau::addPiece(Piece* piece, Position pos) {
    plateau[pos].push_back(piece);
    piece->setPosition(pos);
}

bool Plateau::isReinePlaced(Couleur couleur) const {
    for (const auto& pair : plateau) {
        const vector<Piece*>& pieces = pair.second;
        for (const Piece* piece : pieces) {
            if (piece->getType() == "Reine" && piece->getCouleur() == couleur) {
                return true;
            }
        }
    }
    return false;
}

bool Plateau::deplacerPiece(Position from, Position to, Couleur couleur) {
    if (!isReinePlaced(couleur)) {
        cout << "La reine doit etre posee avant de deplacer une autre piece." <<endl;
        return false;
    }
    if(wouldSplitHive(from,to)){
        cout << "Le mouvement casserait la ruche !" <<endl; 
        return false;   
    }
    if(from.getColonne() == to.getColonne() && from.getLigne() == to.getLigne()){
        cout<<"La destination doit-être différente de l'origine !"<<endl;
        return false; 
    }
    if (isPositionOccupied(from)) {  
        Piece* piece = plateau.at(from).back();
        if (piece->getCouleur() == couleur && piece->isValidMove(to, plateau)) {
            plateau[from].pop_back();
            addPiece(piece, to);
            return true;
        }else if (piece->getCouleur() != couleur){
            cout<<"Ce n'est pas votre piece"<<endl;
            return false;
        }else if(!piece->isValidMove(to, plateau)){
            cout<<"Le mouvement n'est pas valide !"<<endl;
            return false;
        }
    }else{
        cout<<"Erreur : La position d'origine du déplacement est vide"<<endl;
    }
    return false;
}


void Plateau::print_board() const {

    int min_q = 0, max_q = 0, min_r = 0, max_r = 0;
    for (const auto& entry : plateau) { //on détermine les limites du plateau (les pièces les plus excentrées posées dessus)
        if (!entry.second.empty()) { 
            min_q = min(min_q, entry.first.getColonne());
            max_q = max(max_q, entry.first.getColonne());
            min_r = min(min_r, entry.first.getLigne());
            max_r = max(max_r, entry.first.getLigne());
        }
    }
    
    for (int r = min_r; r <= max_r; ++r) { //on parcourt toutes les lignes et colonnes du plateau
        int offset = (r % 2 == 0) ? 0 : 3; //avec notre système de coordonnées, les lignes impaires sont décalées par rapport à la ligne 0 mais pas les lignes paires
        cout << string(offset, ' ');

        for (int q = min_q; q<=max_q; ++q) {
            Position pos(q, r);
            if (isPositionOccupied(pos)) {
                Piece* piece = plateau.at(pos).back(); // on affiche la pièce la plus en haut de l'empilement (dernière pièce ajouté au vecteur)
                piece->getCouleur() == Blanc ? cout << "[" << piece->getInitial() << ".B" << "] " : cout << "[" << piece->getInitial() << ".N" << "] "; // Affiche la pièce avec son type et couleur
            } else {
                cout << "[   ] "; 
            }
        }
        
        cout << endl; 
    }
}

void Plateau::print_positions() const {
    cout << "Positions des pieces sur le plateau:\n" << endl;
    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        const vector<Piece*>& pieces = entry.second;
        for (const Piece* piece : pieces) {
            cout << piece->getType() << " (" << (piece->getCouleur()== Noir ? "Noir" : "Blanc") << ") a la position (" << pos.getColonne() << ", " << pos.getLigne() << ")\n" << endl;
        }
    }
}




//Sauvegarde


void Plateau::save(std::ofstream& outFile) const {
    if (!outFile) {
        throw std::runtime_error("Cannot write to the provided ofstream");
    }

    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        const vector<Piece*>& pieces = entry.second;
        for (const Piece* piece : pieces) {
            outFile << pos.getColonne() << " " << pos.getLigne() << " "<< piece->getType() << " " << (piece->getCouleur() == Noir ? "Noir" : "Blanc") << "\n";
        }
    }
    
    outFile << "END_PIECES" <<"\n";
}

void Plateau::load(ifstream& inFile) {
    if (!inFile) {
        throw runtime_error("Cannot read from the provided ifstream");
    }
    plateau.clear();
    int q, r;
    string type, couleurStr;
    while(true) {
        string line;
        getline(inFile, line);  // Lire la ligne entière

        if (line.empty()) continue;  // Ignorer les lignes vides
        if (line == "END_PIECES") break;
        istringstream iss(line);

        int q, r;
        string type, couleurStr;

        iss >> q >> r >> type >> couleurStr;

        if (type.empty() || couleurStr.empty()) {
            throw runtime_error("Format incorrect dans le fichier.");
        }
        Couleur couleur = (couleurStr == "Noir") ? Noir : Blanc;
        Position pos(q, r);
        Piece* piece = nullptr;

        if (type == "Reine") {
            piece = new Reine(pos, couleur);
        } else if (type == "Scarabee") {
            piece = new Scarabee(pos, couleur);
        } else if (type == "Araignee") {
            piece = new Araignee(pos, couleur);
        } else if (type == "Sauterelle") {
            piece = new Sauterelle(pos, couleur);
        } else if (type == "Fourmi") {
            piece = new Fourmi(pos, couleur);
        } else if (type == "Moustique") {
            piece = new Moustique(pos, couleur);
        } else if (type == "Coccinelle") {
            piece = new Coccinelle(pos, couleur);
        } else {
            throw std::invalid_argument("Type de pièce inconnu");
        }
        plateau[pos].push_back(piece);
    }
}
