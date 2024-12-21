#ifndef PLATEAU_H
#define PLATEAU_H
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "piece.hpp"

struct Hexagon {
    QPolygonF polygon;
    QPointF center;
    int q, r;  // Coordonnées offset
    Position position;  // Position from piece.hpp

    Hexagon(int q = 0, int r = 0) : q(q), r(r), position(q, r) {}
    Position getPosition() const { return position; }
};

class Plateau : public QWidget {
    Q_OBJECT

public:
    explicit Plateau(QWidget *parent = nullptr);
    Plateau(const Plateau& other, QWidget *parent = nullptr); //nécessaire des les expliciter en Qt car faire hériter plateau de QWidget les supprime implicitement.
    Plateau& operator=(const Plateau& other);

    std::unordered_map<Position, std::vector<Piece*>>& getPlateau() {return plateau;}
    std::unordered_map<Position, vector<Piece*>> getPlateau() const {return plateau;}

    bool isPositionOccupied(Position pos) const;
    void dfs(Position pos, const unordered_map<Position, vector<Piece*>>& tempPlateau, unordered_set<Position, hash<Position>>& visited);
    bool isHiveConnected(const unordered_map<Position, vector<Piece*>>& tempPlateau);
    bool wouldSplitHive(Position from, Position to);
    void addPiece(Piece* piece, Position pos);
    void setTourActuel(int tour) { tourActuel = tour; }


    bool isReinePlaced(Couleur couleur) const;
    bool deplacerPiece(Position from, Position to, Couleur couleur);

    void drawAvailablePieces(QPainter& painter);
    void resizeEvent(QResizeEvent* event);

    void save(std::ofstream& outFile) const;
    void load(std::ifstream& inFile);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    QString gameInfo;

    QVector<Hexagon> hexagons;  // Pour le frontend
    std::unordered_map<Position, std::vector<Piece*>> plateau;  // Pour le backend

    const qreal HEX_SIZE = 50.0;
    Hexagon* hoveredHex = nullptr;
    const int PIECES_ZONE_HEIGHT = 200;

    vector<Position> validMoves;
    vector<Position> highlightedPlacements;

    QPolygonF createHexagon(qreal centerX, qreal centerY, qreal size);
    void createHexagonalGrid();
    Hexagon* findHexagonAt(const QPointF& pos);
    void drawAvailablePlays(QPainter& painter);

    char selectedPieceType;  // Store currently selected piece type
    std::vector<QPushButton*> pieceButtons;
    QPushButton* undoButton;
    int tourActuel = 0;
    void initializeButtons();
    Position lastClickedPos;
    Piece* selectedPiece;

    void undoMovePressed();

public slots:

    void setGameInfo(const QString& info) {
        gameInfo = info;
        update();
    }

    void setValidPlacements(const std::vector<Position>& positions) {
        validMoves = positions;
        update();
    }

    void setValidMoves(const std::vector<Position>& moves) {
        validMoves = moves;
        update();  // Force repaint
    }

    void handleListHighlightedPos(const std::vector<Position>& positions) {
        highlightedPlacements = positions;
        update();
    }


signals:
    void hexagonClicked(Position pos);
    void pieceTypeSelected(char type);
    void turnDone();
    void highlightPositions();
    void undoMove();
};

#endif
