#ifndef PARTIE_H
#define PARTIE_H

#include <QObject>
#include <stack>
#include "joueur.h"
#include "plateau.h"

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class GameOverDialog : public QDialog {
    Q_OBJECT
public:
    explicit GameOverDialog(const QString& message, QWidget* parent = nullptr);
};


class PartieMemento {
    Plateau plateau;
    Joueur joueur1;
    Joueur joueur2;
    Joueur* joueurCourant;
    int tourActuel;

public:
    PartieMemento(const Plateau& p, const Joueur& j1, const Joueur& j2, Joueur* jc, int ta)
        : plateau(p), joueur1(j1), joueur2(j2), tourActuel(ta) {
        joueurCourant = (jc == &j1) ? &joueur1 : &joueur2;
    }

    PartieMemento& operator=(const PartieMemento& autre) {
        if (this != &autre) {
            plateau = autre.plateau;
            joueur1 = autre.joueur1;
            joueur2 = autre.joueur2;
            joueurCourant = autre.joueurCourant;
            tourActuel = autre.tourActuel;
        }
        return *this;
    }

    friend class Partie;
};

class Partie : public QObject {
    Q_OBJECT

private:
    Plateau* plateau;
    int tourActuel;
    Joueur* joueurs[2];
    Joueur* joueurCourant;
    stack<PartieMemento> historique;
    int maxRetoursEnArriere;
    vector<Position> currentValidMoves;
    char selectedPieceType;

public:
    explicit Partie(QObject *parent = nullptr)
        : QObject(parent), plateau(nullptr), tourActuel(0),
        joueurCourant(nullptr), maxRetoursEnArriere(3) {
        joueurs[0] = nullptr;
        joueurs[1] = nullptr;
    }
    Partie(Joueur* j1, Joueur* j2, Plateau* p, int t = 0, int maxRetours = 3);
    ~Partie();

    const Joueur* getJoueurCourant()const{return joueurCourant;}
    int getTourActuel()const{return tourActuel;}

    void afficherMouvementsPossibles(Position pos, Couleur couleur);

    void printPossiblePlays(Joueur* joueurCourant);

    void setSelectedPieceType(char type) { selectedPieceType = type; }
    bool isGameOver();
    void playTurn();
    void sauvegarderEtat();
    void restaurerEtat();

signals:
    void gameStateChanged();
    void noMovesAvailable(QString playerColor);
    //void validMovesChanged(const vector<Position>& moves);
    void gameInfoChanged(const QString& info);
    void currentPlayerChanged(Couleur couleur);
    void validPlacementsChanged(const std::vector<Position>& positions);
    void gameOver(QString message);

    void listHighlightedPos(const std::vector<Position>& positions);


public slots:
    void handleMove();
    void restaurerDeuxDerniersEtats();
    void handlePiecePlacement(Position pos);

    void nextTurn();

    void handleHighlightPosition();
    void handleUndo();
};

#endif // PARTIE_H
