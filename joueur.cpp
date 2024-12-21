#include "joueur.h"
#include <QDebug>
#include <QMessageBox>
#include <fstream>
#include <sstream>



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
                    if(!pair.second.empty()){
                        vector<Position> adjacents = pair.first.getAdjacentCoordinates();
                        for(const auto& pos : adjacents){
                            if(find(positions.begin(), positions.end(), pos) == positions.end()){
                                positions.push_back(pos);
                            }
                        }
                    }
                }
            }
            if (!positions.empty()) {
                if (poserPiece(pieceType[0], positions[0], plateau, tourActuel)) {
                    QString message = QString("IA a placé une pièce de type %1 en position (%2, %3).")
                                          .arg(QString::fromStdString(pieceType))
                                          .arg(positions[0].getColonne())
                                          .arg(positions[0].getLigne());
                    QMessageBox::information(nullptr, "IA Move", message);
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
                    QString message = QString("IA a déplacé une pièce en position (%1, %2) vers la position (%3, %4).")
                                          .arg(piece->getPosition().getColonne())
                                          .arg(piece->getPosition().getLigne())
                                          .arg(pos.getColonne())
                                          .arg(pos.getLigne());
                    QMessageBox::information(nullptr, "IA Move", message);
                    return true;
                }
            }
        }
    }
    //si on arrive la, c'est que l'IA ne peut ni placer ni déplacer de pièce
    QMessageBox::information(nullptr, "IA Move", "L'IA ne peut pas jouer.");
    return false;
}




vector<Position> Joueur::get_liste_placements(Plateau& plateau) {
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



bool Joueur::poserPiece(char pieceType, Position pos, Plateau& plateau, int tourActuel) {
    Piece* piece = nullptr;
    Position origin(0, 0);

    // Check piece availability
    if ((pieceType == 'R' && nb_pieces["R"] >= 1) ||
        (pieceType == 'A' && nb_pieces["A"] >= 2) ||
        (pieceType == 'S' && nb_pieces["S"] >= 2) ||
        (pieceType == 'F' && nb_pieces["F"] >= 3) ||
        (pieceType == 'H' && nb_pieces["H"] >= 3) ||
        (pieceType == 'C' && nb_pieces["C"] >= 1) ||
        (pieceType == 'M' && nb_pieces["M"] >= 1)) {
        QMessageBox::warning(nullptr, "Erreur", "Vous n'avez plus de pièces de ce type disponibles.");
        delete piece;
        return false;
    }

    // Create piece
    switch(pieceType) {
        case 'R': piece = new Reine(origin, couleur); break;
        case 'A': piece = new Araignee(origin, couleur); break;
        case 'S': piece = new Scarabee(origin, couleur); break;
        case 'F': piece = new Fourmi(origin, couleur); break;
        case 'H': piece = new Sauterelle(origin, couleur); break;
        case 'C': piece = new Coccinelle(origin, couleur); break;
        case 'M': piece = new Moustique(origin, couleur); break;
        default:
            QMessageBox::warning(nullptr, "Erreur", "Action invalide.");
            delete piece;
            return false;
    }

    // First turn
    if (tourActuel == 0) {
        if(plateau.getPlateau().find(origin) != plateau.getPlateau().end() && !plateau.getPlateau().at(origin).empty()) {
            QMessageBox::warning(nullptr, "Erreur", "La position d'origine est déjà occupée.");
            delete piece;
            return false;
        }
        if(pos != origin) {
            QMessageBox::warning(nullptr, "Erreur", "La première pièce doit être placée à l'origine.");
            delete piece;
            return false;
        }
        plateau.addPiece(piece, origin);
        pieces.push_back(piece);
        string typeStr(1, pieceType);
        nb_pieces[typeStr]++;
        return true;
    }

    // Second turn
    else if(tourActuel == 1) {
        bool isAdjacent = false;
        if(!plateau.isPositionOccupied(pos)) {
            for (const auto& pair : plateau.getPlateau()) {
                if (pair.first.isAdjacent(pos)) {
                    isAdjacent = true;
                    break;
                }
            }
        }
        if (!isAdjacent) {
            QMessageBox::warning(nullptr, "Erreur", "La pièce doit être placée à côté d'une autre pièce.");
            delete piece;
            return false;
        }
        plateau.addPiece(piece, pos);
        pieces.push_back(piece);
        nb_pieces[QString(pieceType).toStdString()]++;
        return true;
    }

    // Later turns
    else {
        vector<Position> possible_placements = get_liste_placements(plateau);
        if(possible_placements.empty()) {
            QMessageBox::warning(nullptr, "Erreur", "Vous ne pouvez pas placer de pièce.");
            delete piece;
            return false;
        }

        if(std::find(possible_placements.begin(), possible_placements.end(), pos) == possible_placements.end()) {
            QString message = "Position impossible pour le placement de la pièce.\nPositions valides :\n";
            for (const auto& valid_pos : possible_placements) {
                message += QString("(%1, %2)\n").arg(valid_pos.getColonne()).arg(valid_pos.getLigne());
            }
            QMessageBox::warning(nullptr, "Erreur", message);
            delete piece;
            return false;
        }

        plateau.addPiece(piece, pos);
        pieces.push_back(piece);
        nb_pieces[QString(pieceType).toStdString()]++;
        return true;
    }

    QMessageBox::warning(nullptr, "Erreur", "Une erreur s'est produite.");
    delete piece;
    return false;
}


//sauvegarde

void Joueur::save(ofstream& out) const {
    if (!out) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'écrire dans le fichier");
        return;
    }

    out << couleur << "\n";
    out << isIa << endl;
    for (const auto& pair : nb_pieces) {
        out << pair.first << " " << pair.second << "\n";
    }
    out << "END_J\n";
}

istream& operator>>(istream& in, Couleur& couleur) {
    int value;
    in >> value;
    couleur = static_cast<Couleur>(value);
    return in;
}

void Joueur::load(ifstream& in, const Plateau& plateau) {
    if (!in) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de lire le fichier");
        return;
    }

    in >> couleur;
    in >> isIa;
    pieces.clear();

    for(auto& pair : plateau.getPlateau()) {
        for(Piece* piece : pair.second) {
            if(piece->getCouleur() == couleur) {
                pieces.push_back(piece);
            }
        }
    }

    while (true) {
        std::string line;
        std::getline(in, line);
        if (line.empty()) continue;
        if (line == "END_J") break;

        std::istringstream iss(line);
        std::string type;
        int nb;
        if (iss >> type >> nb) {
            nb_pieces[type] = nb;
        } else {
            QMessageBox::critical(nullptr, "Erreur", "Format incorrect dans la section nb_pieces");
            return;
        }
    }
}
