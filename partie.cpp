#include "partie.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>


Partie::Partie(int maxRetours, bool ia)
    : joueur1(Noir), joueur2(Blanc), joueurCourant(&joueur1),
      tourActuel(0), contreIA(ia), maxRetoursEnArriere(maxRetours) {
    std::srand(std::time(0)); // Initialiser la graine aléatoire
}


void Partie::jouer() {
    bool jeuEnCours = true;
    int retoursRestants = maxRetoursEnArriere;

    while (jeuEnCours) {
        if (!historique.empty()) {
            plateau.print_board();
        }

        std::cout << "Tour du joueur " << (joueurCourant == &joueur1 ? "1" : "2") << " ("
                  << (joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc") << ")" << std::endl;

        if (contreIA && joueurCourant == &joueur2) {
            if (contreIA && joueurCourant == &joueur2) {
                std::cout << "C'est le tour de l'IA !" << std::endl;
                jouerTourIA();
                }
        } else {
            // Tour du joueur humain
            bool actionValide = false;

            while (!actionValide) {
                int choix;
                std::cout << "Menu: \n1. Poser une pièce \n2. Déplacer une pièce \n3. Revenir en arrière ("
                          << retoursRestants << " retours restants)\nVotre choix: ";
                std::cin >> choix;

                switch (choix) {
                    case 1:
                        sauvegarderEtat();
                        actionValide = poserPiece();
                        break;
                    case 2:
                        sauvegarderEtat();
                        actionValide = deplacerPiece();
                        break;
                    case 3:
                        if (retoursRestants > 0) {
                            restaurerDeuxDerniersEtats();
                            retoursRestants--;
                        } else {
                            std::cout << "Nombre maximum de retours en arrière atteint." << std::endl;
                        }
                        break;
                    default:
                        std::cout << "Choix invalide. Réessayez." << std::endl;
                        break;
                }
            }
        }

        if (conditionsDeVictoire()) {
            jeuEnCours = false;
        } else if (conditionsDeMatchNul()) {
            std::cout << "Match nul !" << std::endl;
            jeuEnCours = false;
        } else {
            nextTurn();
        }
    }
}


bool Partie::poserPiece() {
    int pieceType, col, row;
    std::cout << "Choisissez le type de pièce à poser (1=Reine, 2=Scarabée, 3=Araignée, 4=Sauterelle, 5=Fourmi) : ";
    std::cin >> pieceType;
    std::cout << "Entrez la position pour poser la pièce (colonne et ligne) : ";
    std::cin >> col >> row;

    Position pos(col, row);
    Piece* piece = nullptr;

    switch (pieceType) {
        case 1:
            if (joueurCourant->getNbReine() == 0) {
                std::cout << "Vous n'avez plus de Reine disponible." << std::endl;
                return false;
            }
            piece = new Reine(pos, joueurCourant->getCouleur());
            break;
        case 2:
            if (joueurCourant->getNbScarabes() == 0) {
                std::cout << "Vous n'avez plus de Scarabées disponibles." << std::endl;
                return false;
            }
            piece = new Scarabee(pos, joueurCourant->getCouleur());
            break;
        case 3:
            if (joueurCourant->getNbAraignees() == 0) {
                std::cout << "Vous n'avez plus d'Araignées disponibles." << std::endl;
                return false;
            }
            piece = new Araignee(pos, joueurCourant->getCouleur());
            break;
        case 4:
            if (joueurCourant->getNbSauterelles() == 0) {
                std::cout << "Vous n'avez plus de Sauterelles disponibles." << std::endl;
                return false;
            }
            piece = new Sauterelle(pos, joueurCourant->getCouleur());
            break;
        case 5:
            if (joueurCourant->getNbFourmis() == 0) {
                std::cout << "Vous n'avez plus de Fourmis disponibles." << std::endl;
                return false;
            }
            piece = new Fourmi(pos, joueurCourant->getCouleur());
            break;
        default:
            std::cout << "Type de pièce invalide." << std::endl;
            return false;
    }

    return joueurCourant->poserPiece(piece, pos, plateau);
}

bool Partie::deplacerPiece() {
    int colFrom, rowFrom, colTo, rowTo;

    std::cout << "Entrer la position de départ (colonne et ligne) : ";
    std::cin >> colFrom >> rowFrom;

    std::cout << "Entrer la position d'arrivée (colonne et ligne) : ";
    std::cin >> colTo >> rowTo;

    Position from(colFrom, rowFrom);
    Position to(colTo, rowTo);

    if (!plateau.isPositionOccupied(from)) return false;

    Piece* piece = plateau.getPieceAtPosition(from);
    if (piece == nullptr || piece->getCouleur() != joueurCourant->getCouleur()) return false;

    if (joueurCourant->isValidMove(from, to, plateau)) {
        joueurCourant->movePiece(from, to, plateau);
        return true;
    } else {
        std::cout << "Mouvement invalide. Reessayez." << std::endl;
        return false;
    }
}

void Partie::nextTurn() {
    tourActuel++;
    joueurCourant = (joueurCourant == &joueur1) ? &joueur2 : &joueur1;
}

// Sauvegarder l'état actuel de la partie
void Partie::sauvegarderEtat() {
    historique.push(PartieMemento(plateau, joueur1, joueur2, joueurCourant, tourActuel));
}

// Restaurer l'état précédent de la partie
void Partie::restaurerEtat() {
    if (!historique.empty()) {
        PartieMemento m = historique.top();
        historique.pop();
        plateau = m.plateau;
        joueur1 = m.joueur1;
        joueur2 = m.joueur2;
        joueurCourant = m.joueurCourant;
        tourActuel = m.tourActuel;
        plateau.print_board(); // Affiche l'état du plateau après la restauration
    }
}

// Restaurer les deux derniers états de la partie
void Partie::restaurerDeuxDerniersEtats() {
    if (!historique.empty()) {
        // Restaurer le dernier état
        PartieMemento m = historique.top();
        historique.pop();
        plateau = m.plateau;
        joueur1 = m.joueur1;
        joueur2 = m.joueur2;
        joueurCourant = m.joueurCourant;
        tourActuel = m.tourActuel;

        // Restaurer l'état précédent
        if (!historique.empty()) {
            m = historique.top();
            historique.pop();
            plateau = m.plateau;
            joueur1 = m.joueur1;
            joueur2 = m.joueur2;
            joueurCourant = m.joueurCourant;
            tourActuel = m.tourActuel;
        }
        plateau.print_board(); // Affiche l'état du plateau après la restauration
    }
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
                } else if (piece->getCouleur() == joueur2.getCouleur()) {
                    posReineJoueur2 = pos;
                    foundReineJoueur2 = true;
                }
            }
        }
    }

    if (foundReineJoueur1 && isPieceSurrounded(posReineJoueur1)) {
        std::cout << "Joueur 2 a gagné !" << std::endl;
        return true;
    }

    if (foundReineJoueur2 && isPieceSurrounded(posReineJoueur2)) {
        std::cout << "Joueur 1 a gagné !" << std::endl;
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
    return false;
}

void Partie::jouerTourIA() {
    bool actionValide = false;

    while (!actionValide) {
        bool iaHasPiecesOnBoard = plateau.hasPiecesOnBoard(joueurCourant->getCouleur());

        int choix = iaHasPiecesOnBoard ? std::rand() % 2 + 1 : 1;

        if (choix == 1) { // Poser une pièce
            std::cout << "L'IA choisit de poser une pièce.\n";
            sauvegarderEtat();
            actionValide = poserPieceIA();
        } else { // Déplacer une pièce
            std::cout << "L'IA choisit de déplacer une pièce.\n";
            sauvegarderEtat();
            actionValide = deplacerPieceIA();
        }
    }
}




bool Partie::poserPieceIA() {
    int col = std::rand() % 5; // Position aléatoire
    int row = std::rand() % 5;

    Position pos(col, row);
    Piece* piece = nullptr;

    // L'IA pose une Reine si elle est disponible, sinon elle pose une Fourmi
    if (joueurCourant->getNbReine() > 0) {
        piece = new Reine(pos, joueurCourant->getCouleur());
        std::cout << "L'IA pose une Reine en position (" << col << ", " << row << ").\n";
    } else if (joueurCourant->getNbFourmis() > 0) {
        piece = new Fourmi(pos, joueurCourant->getCouleur());
        std::cout << "L'IA pose une Fourmi en position (" << col << ", " << row << ").\n";
    } else {
        return false; // Aucune pièce à poser
    }

    return joueurCourant->poserPiece(piece, pos, plateau);
}




bool Partie::deplacerPieceIA() {
    int colFrom = std::rand() % 5;
    int rowFrom = std::rand() % 5;
    int colTo = std::rand() % 5;
    int rowTo = std::rand() % 5;

    Position from(colFrom, rowFrom);
    Position to(colTo, rowTo);

    if (!plateau.isPositionOccupied(from)) {
        return false; // Aucune pièce à déplacer
    }

    Piece* piece = plateau.getPieceAtPosition(from);
    if (piece == nullptr || piece->getCouleur() != joueurCourant->getCouleur()) {
        return false;
    }

    if (piece->isValidMove(to, plateau.getPlateauMap())) {
        joueurCourant->movePiece(from, to, plateau);
        std::cout << "L'IA déplace une pièce de (" << colFrom << ", " << rowFrom << ") à ("
                  << colTo << ", " << rowTo << ").\n";
        return true;
    }

    return false;
}

