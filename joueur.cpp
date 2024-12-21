#include "joueur.hpp"
#include "partie.hpp"
#include <algorithm> //pour std::find
#include <sstream> //pour std::istringstream


string toString(Couleur couleur) {
    switch (couleur) {
        case Noir: return "Noir";
        case Blanc: return "Blanc";
        default: return "Inconnu";
    }
}

Couleur stringToCouleur(const std::string& str) {
    if (str == "Noir") return Noir;
    if (str == "Blanc") return Blanc;
    throw std::invalid_argument("Couleur inconnue");
}


void Joueur::print_piece_left()const{
    cout<< "La liste des pieces restantes est : "<<endl;
    for (const auto& pair : nb_pieces) {
        if(pair.first == "R"){
            cout << "Reine : " << 1-pair.second << endl;
        }else if(pair.first == "A"){
            cout << "Araignee : " << 2-pair.second << endl;
        }else if(pair.first == "S"){
            cout << "Scarabee : " << 2-pair.second << endl;
        }else if(pair.first == "F"){
            cout << "Fourmi : " << 3-pair.second << endl;
        }else if(pair.first == "H"){
            cout << "Sauterelle : " << 3-pair.second << endl;
        }else if(pair.first == "C"){
            cout << "Coccinelle : " << 1-pair.second << endl;
        }else if(pair.first == "M"){
            cout << "Moustique : " << 1-pair.second << endl;
        }          
    }
    cout << "\n";
};

vector<Position> Joueur::get_liste_placements(const Plateau& plateau)const {
    unordered_set<Position> valid_positions; // Ensemble pour éviter les doublons

    for (Piece* piece : pieces) {
        vector<Position> adj_positions = piece->getPosition().getAdjacentCoordinates(); //on prend les positions adjacentes aux pièces du joueur sur le plateau

        for (const Position& adj : adj_positions) { //pour chaque position adjacente
            if (!plateau.isPositionOccupied(adj)) { //on regarde si la position est libre
                bool adjacent_to_opponent_piece = false;
                for (const Position& other_adj : adj.getAdjacentCoordinates()) { //on regarde si la position est adjacente à une pièce adverse
                    if (plateau.isPositionOccupied(other_adj) &&
                        plateau.getPlateau().at(other_adj)[0]->getCouleur() != couleur) {
                        adjacent_to_opponent_piece = true;
                        break;
                    }
                }
                if (!adjacent_to_opponent_piece) { //si la position est libre et non adjacente a une pièce adverse, on peut poser une pièce dessus
                    valid_positions.insert(adj);
                }
            }
        }
    }
    return vector<Position>(valid_positions.begin(), valid_positions.end()); //on retourne un vecteur des positions valides
}


bool Joueur::poserPiece(char pieceType, Position pos, Plateau& plateau,int tourActuel) {
    Piece* piece = nullptr;
    Position origin(0, 0);

    // Vérifier que la pièce est encore disponible pour le Joueur 
    if ((pieceType == 'R' && nb_pieces["R"] >= 1) ||
        (pieceType == 'A' && nb_pieces["A"] >= 2) ||
        (pieceType == 'S' && nb_pieces["S"] >= 2) ||
        (pieceType == 'F' && nb_pieces["F"] >= 3) ||
        (pieceType == 'H' && nb_pieces["H"] >= 3)||
        (pieceType == 'C' && nb_pieces["C"] >= 1)||
        (pieceType == 'M' && nb_pieces["M"] >= 1)) {
        cout << "Vous n'avez plus de pieces de ce type disponibles." << endl;
        delete piece;
        return false;
    }

    switch(pieceType){ //on créé la pièce avec position temporaire l'origine, qui sera changée en fonction des cas
        case 'R':
            piece = new Reine(origin, couleur);
            break;
        case 'A':
            piece = new Araignee(origin, couleur);
            break;
        case 'S':
            piece = new Scarabee(origin, couleur);
            break;
        case 'F':
            piece = new Fourmi(origin, couleur);
            break;
        case 'H':
            piece = new Sauterelle(origin, couleur);
            break;
        case 'C':
            piece = new Coccinelle(origin, couleur);
            break;
        case 'M':
            piece = new Moustique(origin, couleur);
            break;
        default:
            cout << "Type de piece invalide." << endl;
            delete piece;
            return false;
    }

    if (tourActuel == 0) {
        //placer à 0,0
        if(plateau.getPlateau().find(origin) != plateau.getPlateau().end()){ //si l'origine est déjà occupée (c'est pas censé être le cas normalement)
            cout << "Erreur : la position d'origine est deja occupee." << endl;
            delete piece;
            return false;
        }
        if(pos != origin){ //si la position n'est pas 0,0
            cout << "La premiere piece doit etre placee a l'origine" << endl;
            delete piece;
            return false;
        }
        plateau.addPiece(piece, origin);
        pieces.push_back(piece);

        string typeStr(1, pieceType);
        nb_pieces[typeStr]++;

        return true;
    }else if(tourActuel == 1){ //Si ce n'est pas le premier tour, vérifier si la position est adjacente à une autre pièce
        bool isAdjacent = false;
        if(!plateau.isPositionOccupied(pos)){
            for (const auto& pair : plateau.getPlateau()) { //normalement à ce stade il n'y a qu'une position occupée, l'origine, mais on parcourt tout le plateau quand même
                if (pair.first.isAdjacent(pos)) {
                    isAdjacent = true;
                }
            }
        } //Résumé : si la position est libre et adjacente a une pièce (peut importe la couleur au premier tour), alors on peut placer la pièce
        if (!isAdjacent) {
            cout << "La piece doit etre placee a cote d'une autre piece." << endl;
            delete piece;
            return false;
        }
        plateau.addPiece(piece, pos);
        pieces.push_back(piece);

        string typeStr(1, pieceType);
        nb_pieces[typeStr]++;

        return true;
    }else{
        vector<Position> possible_placements = get_liste_placements(plateau);
        if(possible_placements.size() == 0){
            cout << "Vous ne pouvez pas placer de piece." << endl;
            delete piece;
            return false;
        }

        bool isValid = false;
        if(find(possible_placements.begin(), possible_placements.end(), pos) != possible_placements.end()){
            isValid = true;
        }

        if(!isValid){
            cout << "Position impossible pour le placement de la piece." << endl;
            cout << "Liste des positions valides : " << endl;
            for (const auto& pos : possible_placements) {
                cout << "("<< pos.getColonne() << ", " << pos.getLigne() << ")" << endl;
            }
            delete piece;
            return false;
        }

        plateau.addPiece(piece, pos);
        pieces.push_back(piece);

        string typeStr(1, pieceType);
        nb_pieces[typeStr]++;

        return true;
    }

    cout << "Une erreur s'est produite." << endl;
    delete piece;
    return false;
}


bool Joueur::isPieceTypeAvailable(const string& pieceType) const{
    int nb = nb_pieces.at(pieceType);
    if (pieceType == "R" && nb < 1) {
        return true;
    } else if (pieceType == "A" && nb < 2) {
        return true;
    } else if (pieceType == "S" && nb < 2) {
        return true;
    } else if (pieceType == "F" && nb < 3) {
        return true;
    } else if (pieceType == "H" && nb < 3) {
        return true;
    } else if (pieceType == "C" && nb < 1) {
        return true;
    } else if (pieceType == "M" && nb < 1) {
        return true;
    }
}

bool Joueur::playTurnIA(Plateau& plateau, int tourActuel) {
    vector<string> pieceOrder = {"R", "F", "S", "H", "C", "A", "M"};
    if(tourActuel == 0) if(poserPiece('R', Position(0, 0), plateau, tourActuel)) return true;
    for (const auto& pieceType : pieceOrder) {
        if (isPieceTypeAvailable(pieceType)) {
            vector<Position> positions = get_liste_placements(plateau);
            if(tourActuel == 1){
                for (const auto& pair : plateau.getPlateau()) { 
                    vector<Position> adjacents = pair.first.getAdjacentCoordinates();
                    for(const auto& pos : adjacents){
                        if(find(positions.begin(), positions.end(), pos) == positions.end()){
                            positions.push_back(pos);
                        }
                    }
                }     
            }
            if (!positions.empty()) {
                if (poserPiece(pieceType[0], positions[0], plateau, tourActuel)) {
                    std::cout << "IA a place une piece de type " << pieceType << " en position (" 
                            << positions[0].getColonne() << ", " 
                            << positions[0].getLigne() << ")." << endl;
                    return true;
                }
            }
        }
    }
    
    // Si l'ia ne peut pas placer de pièce, elle déplace une pièce
    for (Piece* piece : pieces) {
        vector<Position> validMoves = piece->getValidMoves(plateau.getPlateau());
        for(const auto& pos : validMoves){
            if(plateau.wouldSplitHive(piece->getPosition(), pos)){
                validMoves.erase(remove(validMoves.begin(), validMoves.end(), pos), validMoves.end());
            }
        }
        if(validMoves.size() != 0) {
            for (const Position& pos : validMoves) {
                if (plateau.deplacerPiece(piece->getPosition(), pos, getCouleur())) {
                    std::cout << "IA a déplacé une pièce en position (" 
                            << piece->getPosition().getColonne() << ", " 
                            << piece->getPosition().getLigne() << ") vers la position ("
                            << pos.getColonne() << ", " << pos.getLigne() << ")." << endl;
                    return true;
                }
            }
        }
    }
    //si on arrive la, c'est que l'IA ne peut ni placer ni déplacer de pièce
    cout<<"L'IA ne peut pas jouer."<<endl;
    return false;
}


//Sauvegarde

void Joueur::save(ofstream& out) const {
    out << couleur << endl;
    out << isIa << endl;
    for (const auto& pair : nb_pieces) {
        out << pair.first << " " << pair.second <<"\n";
    }
    out<< "END_J" <<"\n";
}

istream& operator>>(istream& in, Couleur& couleur) {
    int value;
    in >> value;
    couleur = static_cast<Couleur>(value);
    return in;
}

void Joueur::load(ifstream& in, const Plateau& plateau) {
    in >> couleur;
    in >> isIa;
    pieces.clear();
    for(const auto& pair : plateau.getPlateau()){
        for(Piece* piece : pair.second){
            if(piece->getCouleur() == couleur){
                pieces.push_back(piece);
            }
        }
    }

    while (true) {
        std::string line;
        std::getline(in, line);  // Lire la ligne entière
        if (line.empty()) continue;  // Ignorer les lignes vides
        if (line == "END_J") break;  // Vérifier si on atteint "END_J"

        std::istringstream iss(line);  // Convertir la ligne en flux
        std::string type;
        int nb;
        if (iss >> type >> nb) {
            nb_pieces[type] = nb;  // Ajouter au map
        } else {
            throw std::runtime_error("Format incorrect dans la section nb_pieces.");
        }
    }
}