#include <partie.h>
#include <iostream>
#include <cstdlib>  // Pour std::rand() et std::srand()
#include <ctime>
#include <QMessageBox>// Pour std::time()

GameOverDialog::GameOverDialog(const QString& message, QWidget* parent) : QDialog(parent) {
    setWindowTitle("Fin de partie");
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel(message, this);
    QPushButton* closeButton = new QPushButton("Fermer", this);

    layout->addWidget(label);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
}

Partie::Partie(Joueur* j1, Joueur* j2, Plateau* p, int t, int maxRetours) : plateau(p), tourActuel(t),joueurs{j1, j2}, maxRetoursEnArriere(maxRetours) {
    if(!j1->getIsIa() && !j2->getIsIa()){
        srand(static_cast<unsigned int>(time(nullptr)));
        joueurCourant = *((std::rand() % 2 == 0) ? &joueurs[0] : &joueurs[1]);
    }else if(!j1->getIsIa() && j2->getIsIa()){
        joueurCourant = joueurs[0];
    }
    else if(j1->getIsIa() && !j2->getIsIa()){
        joueurCourant = joueurs[1];
    }
}

void Partie::afficherMouvementsPossibles(Position pos, Couleur couleur) {
    if (plateau->isPositionOccupied(pos)) {
        if(plateau->getPlateau().at(pos).back()->getCouleur() == couleur) {
            Piece* piece = plateau->getPlateau().at(pos).back();
            vector<Position> validMoves = piece->getValidMoves(plateau->getPlateau());

            // Store moves in temp vector to avoid modifying while iterating
            vector<Position> finalMoves;
            for(const Position& move : validMoves) {
                if(!plateau->wouldSplitHive(piece->getPosition(), move)) {
                    finalMoves.push_back(move);
                }
            }

            // Update display
            plateau->setValidMoves(finalMoves);
        }
    }
}



void Partie::handleHighlightPosition(){
    std::vector<Position> validPos = joueurCourant->get_liste_placements(*plateau);
    if(tourActuel==0) validPos.push_back(Position(0,0));
    if(tourActuel == 1){
        for (const auto& pair : plateau->getPlateau()) {
            if(!pair.second.empty()){
                vector<Position> adjacents = pair.first.getAdjacentCoordinates();
                for(const auto& pos : adjacents){
                    if(find(validPos.begin(), validPos.end(), pos) == validPos.end()){
                        validPos.push_back(pos);
                    }
                }
            }
        }
    }
    emit listHighlightedPos(validPos);
}


void Partie::printPossiblePlays(Joueur* joueurCourant) {
    QString info = "<table width='100%'><tr>";
    info += "<td width='20%' style='font-family: Arial; font-size: 12pt; color: black;'>";
    info += QString("<b>Tour %1</b><br/>").arg(tourActuel+1);
    info += QString("<b>Joueur courant :  %1</b><br/>").arg((joueurCourant->getCouleur()==0)?"Noir" : "Blanc");
    info += QString("<b>Appuyez sur les boutons pour placer une pièce </b><br/>");
    info += QString("<b>Cliquez sur vos pièces sur le plateau pour les déplacer</b><br/>");
    info += "<td width='20%' style='font-family: Arial; font-size: 12pt; color: black;'>";
    info += "<b>Pieces a placer</b><br/>----------------<br/>";
    const auto& nb_pieces = joueurCourant->getNbPieces();
    for (const auto& pair : nb_pieces) {
        if(pair.first == "R") info += QString("<span style='font-size: 12pt;'>Reine: %1</span><br/>").arg(1-pair.second);
        else if(pair.first == "A") info += QString("<span style='font-size: 12pt;'>Araignee: %1</span><br/>").arg(2-pair.second);
        else if(pair.first == "S") info += QString("<span style='font-size: 12pt;'>Scarabee: %1</span><br/>").arg(2-pair.second);
        else if(pair.first == "F") info += QString("<span style='font-size: 12pt;'>Fourmi: %1</span><br/>").arg(3-pair.second);
        else if(pair.first == "H") info += QString("<span style='font-size: 12pt;'>Sauterelle: %1</span><br/>").arg(3-pair.second);
        else if(pair.first == "C") info += QString("<span style='font-size: 12pt;'>Coccinelle: %1</span><br/>").arg(1-pair.second);
        else if(pair.first == "M") info += QString("<span style='font-size: 12pt;'>Moustique: %1</span><br/>").arg(1-pair.second);
    }
    info += "</td>";
    info += "<td width='60%' style='font-family: Arial; font-size: 12pt; color: black; text-align: right;'>";
    info += QString("<b>Retours en arrière restants : %1</b><br/>").arg(maxRetoursEnArriere);
    info += "</td>";

    info += "</tr></table>";

    /*info += "<td width='33%'><b>Pieces a deplacer</b><br/>----------------<br/>";
    vector<Piece*> pieces = joueurCourant->getPieces();
    if(pieces.empty()) {
        info += "Aucune piece a deplacer<br/>";
    } else {
        for(const auto& piece : pieces) {
            vector<Position> validMoves = piece->getValidMoves(plateau->getPlateau());
            validMoves.erase(
                std::remove_if(validMoves.begin(), validMoves.end(),
                               [this, piece](const Position& pos) {
                                   return plateau->wouldSplitHive(piece->getPosition(), pos);
                               }
                               ),
                validMoves.end()
                );
            if(!validMoves.empty()) {
                info += QString("(%1,%2) - %3<br/>")
                .arg(piece->getPosition().getColonne())
                    .arg(piece->getPosition().getLigne())
                    .arg(QString::fromStdString(piece->getType()));
            }
        }
    }
    info += "</td></tr></table>";*/

    emit gameInfoChanged(info);
}

void Partie::sauvegarderEtat() {
    PartieMemento memento(*plateau, *joueurs[0], *joueurs[1], joueurCourant, tourActuel);
    historique.push(memento);
}

void Partie::restaurerEtat() {
    if (!historique.empty()) {
        PartieMemento memento = historique.top();
        historique.pop();

        *plateau = memento.plateau;
        *joueurs[0] = memento.joueur1;
        *joueurs[1] = memento.joueur2;

        if(memento.joueurCourant == &memento.joueur1){
            joueurCourant = joueurs[0];
        }else if(memento.joueurCourant == &memento.joueur2){
            joueurCourant = joueurs[1];
        }

        tourActuel = memento.tourActuel;

        emit gameStateChanged();
    }
}

void Partie::restaurerDeuxDerniersEtats() {
    historique.pop();
    historique.pop();
    restaurerEtat();
}

void Partie::handleUndo() {
    if (historique.size() > 2) {
        if (maxRetoursEnArriere == 0) {
            QMessageBox::information(nullptr, "Nombre de retour epuise", "Vous ne pouvez plus revenir en arrière !");
            return;
        }

        restaurerDeuxDerniersEtats();
        --maxRetoursEnArriere;
        printPossiblePlays(joueurCourant);
    } else {
        QMessageBox::information(nullptr, "Aucun historique", "Il n'y a aucun mouvement à annuler");
    }
}



bool Partie::isGameOver(){
    for(const auto& pair : plateau->getPlateau()){
        for(const Piece* piece : pair.second){
            if(piece->getType() == "Reine"){
                if(dynamic_cast<const Reine*>(piece)->isSurrounded(plateau->getPlateau())){ //defaite d'un des deux joueurs
                    return true;
                }
            }
        }
    }

    for(auto& joueur : joueurs){
        if(joueur->get_liste_placements(*plateau).size() != 0){
            if ((joueur->getNbPieces().at("R") < 1) ||
                (joueur->getNbPieces().at("A") < 2) ||
                (joueur->getNbPieces().at("S") < 2) ||
                (joueur->getNbPieces().at("F") < 3) ||
                (joueur->getNbPieces().at("H") < 3) ||
                (joueur->getNbPieces().at("C") < 1) ||
                (joueur->getNbPieces().at("M") < 1)){
                return false; //si on des joueurs a encore au moins une pièce qu'il peut placer, la partie n'est pas finie
            }
        }
    }

    for(const auto& pair : plateau->getPlateau()){
        Piece* piece = pair.second.back();
        vector<Position> validMoves = piece->getValidMoves(plateau->getPlateau());
        for(const auto& pos : validMoves){
            if(plateau->wouldSplitHive(piece->getPosition(), pos)){
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
    sauvegarderEtat();
    plateau->setTourActuel(tourActuel);
    // Announce current player
    emit currentPlayerChanged(joueurCourant->getCouleur());
    if(tourActuel > 2) {
        bool canPlay = false;

        // Check movable pieces
        for(const auto& piece : joueurCourant->getPieces()) {
            vector<Position> validMoves = piece->getValidMoves(plateau->getPlateau());
            vector<Position> finalMoves;

            // Store valid moves that don't split hive
            for(const auto& pos : validMoves) {
                if(!plateau->wouldSplitHive(piece->getPosition(), pos)) {
                    finalMoves.push_back(pos);
                }
            }

            if(!finalMoves.empty()) {
                canPlay = true;
                break;
            }
        }

        // Check placeable pieces
        vector<Position> placementMoves = joueurCourant->get_liste_placements(*plateau);
        if(!placementMoves.empty()) {
            canPlay = true;
        }

        if(!canPlay) {
            QString playerColor = (joueurCourant->getCouleur() == Noir ? "Noir" : "Blanc");
            emit noMovesAvailable(playerColor);
            nextTurn(); // Add this to continue game
            return;
        }
    }

    if(joueurCourant->getIsIa()){
        joueurCourant->playTurnIA(*plateau, tourActuel);
        selectedPieceType = 0;
        nextTurn();
        return;
    }

    // Show possible plays
    printPossiblePlays(joueurCourant);
}

void Partie::handlePiecePlacement(Position pos) {
    if(tourActuel == 0){
        if(joueurCourant->poserPiece(selectedPieceType, pos, *plateau, tourActuel)){
            vector<Position> placements = vector<Position>();
            emit listHighlightedPos(placements);
            selectedPieceType = 0;
            nextTurn();
        }
    }else{
        std::vector<Position> validPos = joueurCourant->get_liste_placements(*plateau);
        if(tourActuel == 1){
            for (const auto& pair : plateau->getPlateau()) {
                if(!pair.second.empty()){
                    vector<Position> adjacents = pair.first.getAdjacentCoordinates();
                    for(const auto& pos : adjacents){
                        if(find(validPos.begin(), validPos.end(), pos) == validPos.end()){
                            validPos.push_back(pos);
                        }
                    }
                }
            }
        }
        if((tourActuel == 6 || tourActuel == 7) && !plateau->isReinePlaced(joueurCourant->getCouleur()) && selectedPieceType != 'R'){
            QMessageBox::information(nullptr, "Reine non placee", "Votre 4eme tour : vous devez placer la Reine !");
            vector<Position> placements = vector<Position>();
            emit listHighlightedPos(placements);
            selectedPieceType = 0;
            return;
        }
        if (std::find(validPos.begin(), validPos.end(), pos) != validPos.end()) {
            if(joueurCourant->poserPiece(selectedPieceType, pos, *plateau, tourActuel)){
                vector<Position> placements = vector<Position>();
                emit listHighlightedPos(placements);
                selectedPieceType = 0;
                nextTurn();
            }
        }
    }
}

void Partie::nextTurn() {
    if(isGameOver()) {
        QString message;
        // Check win condition
        for(const auto& pair : plateau->getPlateau()) {
            for(const Piece* piece : pair.second) {
                if(piece->getType() == "Reine") {
                    if(dynamic_cast<const Reine*>(piece)->isSurrounded(plateau->getPlateau())) {
                        message = QString("Le joueur %1 a gagné!")
                                      .arg(piece->getCouleur() == Blanc ? "Noir" : "Blanc");
                        emit gameOver(message);
                        return;
                    }
                }
            }
        }
        // If no winner, it's a draw
        message = "Match nul!";
        emit gameOver(message);
        return;
    }
    tourActuel++;
    joueurCourant = (joueurCourant == joueurs[0]) ? joueurs[1] : joueurs[0];
    playTurn();
}

Partie::~Partie() {
    delete plateau;
    delete joueurs[0];
    delete joueurs[1];
}


void Partie::handleMove() {
    // Basic implementation to resolve linker error
    // Will be expanded later with actual move logic
    if (joueurCourant != nullptr && plateau != nullptr) {
        // Placeholder for move handling
        emit gameStateChanged();
    }
}




