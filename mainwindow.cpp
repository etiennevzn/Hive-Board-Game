#include "mainwindow.h"
#include "plateau.h"
#include "joueur.h"
#include "partie.h"
#include <QInputDialog>
#include <QMessageBox>

void MainWindow::handleGameOver(QString message) {
    GameOverDialog* dialog = new GameOverDialog(message, this);
    dialog->exec();
    close();
}

void MainWindow::setGameMode(int mode) {
    gameMode = mode;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), hexBoard(nullptr), partie(nullptr), joueurBlanc(nullptr), joueurNoir(nullptr), gameMode(0) {
    setWindowTitle("Hive Game");
    setMinimumSize(800, 600);
}

void MainWindow::initializeGame() {
    delete partie;

    // Create game components
    hexBoard = new Plateau(this);
    Joueur* j1 = nullptr;
    Joueur* j2 = nullptr;
    if(gameMode == 1){
        j1 = new Joueur(Noir);
        j2 = new Joueur(Blanc);
    }else if(gameMode == 2){
        j1 = new Joueur(Noir);
        j2 = new Joueur(Blanc, true);
    }

    int nbRetoursEnArriere = -1;
    bool ok = false;

    while(nbRetoursEnArriere < 0 || nbRetoursEnArriere > 3) {
        nbRetoursEnArriere = QInputDialog::getInt(this, "Retours en arrière",
                                                  "Combien de retours en arriere souhaitez-vous ? (0 à 3) :",
                                                  0, 0, 3, 1, &ok);
        if (!ok) {
            QMessageBox::critical(this, "Erreur", "Vous devez entrer un nombre entre 0 et 3.");
        }
    }

    partie = new Partie(j1, j2, hexBoard, 0, nbRetoursEnArriere);

    // Setup connections
    connect(hexBoard, &Plateau::pieceTypeSelected,
            partie, &Partie::setSelectedPieceType);

    connect(hexBoard, &Plateau::hexagonClicked,
            partie, &Partie::handlePiecePlacement);

    connect(partie, &Partie::gameInfoChanged,
            hexBoard, &Plateau::setGameInfo);
    connect(hexBoard, &Plateau::hexagonClicked,
            [this](Position pos) {
                partie->afficherMouvementsPossibles(pos, partie->getJoueurCourant()->getCouleur());
            });
    connect(hexBoard, &Plateau::turnDone,
            partie, &Partie::nextTurn);
    connect(partie, SIGNAL(gameOver(QString)),
            this, SLOT(handleGameOver(QString)));
    connect(partie, &Partie::noMovesAvailable,
            [this](QString playerColor) {
                QMessageBox::warning(this, "Tour passé",
                                     "Aucun coup possible pour le joueur " + playerColor + ".\n"
                                                                                           "Le tour est passé.");
            });

    connect(hexBoard, &Plateau::highlightPositions,
            partie, &Partie::handleHighlightPosition);

    connect(partie, &Partie::listHighlightedPos, hexBoard, &Plateau::handleListHighlightedPos);

    connect(hexBoard, &Plateau::undoMove, partie, &Partie::handleUndo);

    // Start game
    setCentralWidget(hexBoard);
    partie->playTurn();
}

MainWindow::~MainWindow() {
    delete joueurBlanc;
    delete joueurNoir;
}
