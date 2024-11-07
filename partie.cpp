#include "partie.hpp"
#include <iostream>

void Partie::jouer() {
    bool jeuEnCours = true;

    while (jeuEnCours) {
        plateau.print_board();

        std::cout << "Tour du joueur " << (joueurCourant == &joueur1 ? "1" : "2") << " ("
            << (joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") << ")" << std::endl;

        bool actionValide = false;
        while (!actionValide) {
            char choix;
            std::cout << "Voulez-vous (P)oser une nouvelle piece ou (D)eplacer une piece existante ? ";
            std::cin >> choix;

            if (choix == 'P' || choix == 'p') {
                int pieceType, col, row;
                std::cout << "Choisissez le type de piece a poser (1=Reine, 2=Scarabee, 3=Araignee, 4=Sauterelle, 5=Fourmi) : ";
                std::cin >> pieceType;
                std::cout << "Entrez la position pour poser la piece (colonne et ligne) : ";
                std::cin >> col >> row;

                Position pos(col, row);
                Piece* piece = nullptr;

                switch (pieceType) {
                case 1:
                    if (joueurCourant->getNbReine() == 0) {
                        std::cout << "Vous n'avez plus de Reine disponible." << std::endl;
                        continue;
                    }
                    piece = new Reine(pos, joueurCourant->getCouleur());
                    break;
                case 2:
                    if (joueurCourant->getNbScarabes() == 0) {
                        std::cout << "Vous n'avez plus de Scarabees disponibles." << std::endl;
                        continue;
                    }
                    piece = new Scarabee(pos, joueurCourant->getCouleur());
                    break;
                case 3:
                    if (joueurCourant->getNbAraignees() == 0) {
                        std::cout << "Vous n'avez plus d'Araignees disponibles." << std::endl;
                        continue;
                    }
                    piece = new Araignee(pos, joueurCourant->getCouleur());
                    break;
                case 4:
                    if (joueurCourant->getNbSauterelles() == 0) {
                        std::cout << "Vous n'avez plus de Sauterelles disponibles." << std::endl;
                        continue;
                    }
                    piece = new Sauterelle(pos, joueurCourant->getCouleur());
                    break;
                case 5:
                    if (joueurCourant->getNbFourmis() == 0) {
                        std::cout << "Vous n'avez plus de Fourmis disponibles." << std::endl;
                        continue;
                    }
                    piece = new Fourmi(pos, joueurCourant->getCouleur());
                    break;
                default:
                    std::cout << "Type de piece invalide." << std::endl;
                    continue;
                }

                if (joueurCourant->poserPiece(piece, pos, plateau)) {
                    actionValide = true; // Action valide seulement si poserPiece retourne true
                }
            }
            else if (choix == 'D' || choix == 'd') {
                int colFrom, rowFrom, colTo, rowTo;

                std::cout << "Entrer la position de depart (colonne et ligne) : ";
                std::cin >> colFrom >> rowFrom;

                std::cout << "Entrer la position d'arrivee (colonne et ligne) : ";
                std::cin >> colTo >> rowTo;

                Position from(colFrom, rowFrom);
                Position to(colTo, rowTo);

                if (!plateau.isPositionOccupied(from)) {
                    std::cout << "Aucune piece a la position de depart. Reessayez." << std::endl;
                    continue;
                }

                Piece* piece = plateau.getPieceAtPosition(from);
                if (piece == nullptr || piece->getCouleur() != joueurCourant->getCouleur()) {
                    std::cout << "Vous ne pouvez deplacer que vos propres pieces. Reessayez." << std::endl;
                    continue;
                }

                if (joueurCourant->isValidMove(from, to, plateau)) {
                    joueurCourant->movePiece(from, to, plateau);
                    actionValide = true;
                }
                else {
                    std::cout << "Mouvement invalide. Reessayez." << std::endl;
                }
            }
            else {
                std::cout << "Choix invalide. Reessayez." << std::endl;
            }
        }

        if (conditionsDeVictoire()) {
            jeuEnCours = false;
        }
        else if (conditionsDeMatchNul()) {
            std::cout << "Match nul !" << std::endl;
            jeuEnCours = false;
        }
        else {
            nextTurn();
        }
    }
}

void Partie::nextTurn() {
    tourActuel++;
    joueurCourant = (joueurCourant == &joueur1) ? &joueur2 : &joueur1;
}

bool Partie::conditionsDeVictoire() {
    Position posReineJoueur1;
    Position posReineJoueur2;
    bool foundReineJoueur1 = false;
    bool foundReineJoueur2 = false;

    for (const auto& entry : plateau.getPlateauMap()) {
        const Position& pos = entry.first;
        const std::vector<Piece*>& pieces = entry.second;

        for (const Piece* piece : pieces) {
            if (piece->getType() == "Reine") {
                if (piece->getCouleur() == joueur1.getCouleur()) {
                    posReineJoueur1 = pos;
                    foundReineJoueur1 = true;
                }
                else if (piece->getCouleur() == joueur2.getCouleur()) {
                    posReineJoueur2 = pos;
                    foundReineJoueur2 = true;
                }
            }
        }
    }

    if (foundReineJoueur1 && isPieceSurrounded(posReineJoueur1)) {
        std::cout << "Joueur 2 a gagne !" << std::endl;
        return true;
    }

    if (foundReineJoueur2 && isPieceSurrounded(posReineJoueur2)) {
        std::cout << "Joueur 1 a gagne !" << std::endl;
        return true;
    }

    return false;
}

bool Partie::isPieceSurrounded(const Position& pos) const {
    std::vector<Position> adjacents = pos.getAdjacentCoordinates();

    for (const Position& adj : adjacents) {
        if (!plateau.isPositionOccupied(adj)) {
            return false;
        }
    }
    return true;
}

bool Partie::conditionsDeMatchNul() {
    // Implementer la logique pour verifier un match nul
    return false;
}
