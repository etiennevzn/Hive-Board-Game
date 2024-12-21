#include "plateau.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextDocument>
#include <QPushButton>
#include <fstream>
#include <sstream>

Plateau::Plateau(QWidget *parent) : QWidget(parent), lastClickedPos(-10000, -10000) {
    setMinimumSize(800, 600);
    initializeButtons();
    setMouseTracking(true);
    createHexagonalGrid();
}

Plateau::Plateau(const Plateau& other, QWidget *parent) : QWidget(parent), hexagons(other.hexagons), plateau(other.plateau), HEX_SIZE(other.HEX_SIZE),PIECES_ZONE_HEIGHT(other.PIECES_ZONE_HEIGHT), lastClickedPos(-10000, -10000) {
    hoveredHex = nullptr;  // Reset hover state
}

// Assignment operator
Plateau& Plateau::operator=(const Plateau& other) {
    if (this != &other) {
        plateau = other.plateau;
        hexagons = other.hexagons;
        hoveredHex = nullptr;  // Reset hover state
    }
    return *this;
}

QPolygonF Plateau::createHexagon(qreal centerX, qreal centerY, qreal size) {
    QPolygonF hexagon;
    for (int i = 0; i < 6; ++i) {
        qreal angleDeg = 60 * i - 30;
        qreal angleRad = qDegreesToRadians(angleDeg);

        qreal x = centerX + size * qCos(angleRad);
        qreal y = centerY + size * qSin(angleRad);

        hexagon << QPointF(x, y);
    }
    return hexagon;
}

void Plateau::createHexagonalGrid() {
    // Exact measurements based on hexagon geometry
    const qreal a = HEX_SIZE;  // Side length
    const qreal h = a * sqrt(3) / 2;  // Height from center to middle of any side
    //const qreal r = a;  // Radius (center to vertex)

    // Exact spacing between hexagons
    const qreal width_spacing = 2 * h;  // Distance between column centers
    const qreal height_spacing = a * 1.5;  // Distance between row centers

    int NUM_ROWS = (height() - PIECES_ZONE_HEIGHT) / height_spacing + 2;
    int NUM_COLS = width() / width_spacing + 2;

    qreal startX = width() / 2.0;
    qreal startY = (height() - PIECES_ZONE_HEIGHT) / 2.0;

    hexagons.clear();
    plateau.clear();

    for (int row = -NUM_ROWS/2; row <= NUM_ROWS/2; ++row) {
        for (int col = -NUM_COLS/2; col <= NUM_COLS/2; ++col) {
            qreal x = startX + col * width_spacing;
            if (row % 2 != 0) {
                x += h;  // Offset odd rows by exactly half a hex width
            }
            qreal y = startY + row * height_spacing;

            Hexagon hex(col, row);
            hex.polygon = createHexagon(x, y, a);
            hex.center = QPointF(x, y);

            hexagons.append(hex);
            plateau[hex.position] = std::vector<Piece*>();
        }
    }
}


void Plateau::mouseMoveEvent(QMouseEvent *event) {
    Hexagon* currentHoveredHex = findHexagonAt(event->pos());

    //qDebug() << "Mouse move. Hex found: "
    //         << (currentHoveredHex ? "Yes" : "No");

    if (currentHoveredHex != hoveredHex) {
        hoveredHex = currentHoveredHex;
        update();
    }
}

/*
void Plateau::mousePressEvent(QMouseEvent *event) {
    Hexagon* clickedHex = findHexagonAt(event->pos());

    if (clickedHex) {
        Position pos = clickedHex->getPosition();
        QString message = QString("Coordonnées (q, r): (%1, %2)\n"
                                  "Position methods available")
                              .arg(clickedHex->q)
                              .arg(clickedHex->r);
        QMessageBox::information(this, "Hexagone", message);
    }
}
*/

void Plateau::mousePressEvent(QMouseEvent* event) {
    for (const auto& hex : hexagons) {
        if (hex.polygon.containsPoint(event->pos(), Qt::OddEvenFill)) {
            if (!validMoves.empty() &&
                std::find(validMoves.begin(), validMoves.end(), hex.getPosition()) != validMoves.end()) {
                if (deplacerPiece(lastClickedPos, hex.getPosition(), plateau[lastClickedPos].back()->getCouleur())) {
                    validMoves.clear();
                    lastClickedPos = Position();
                    emit turnDone();
                }
            }
            else if (hex.getPosition() == lastClickedPos) {
                validMoves.clear();
                lastClickedPos = Position();
            }
            else {
                vector<Position> placements = vector<Position>();
                highlightedPlacements = placements;
                validMoves.clear();
                lastClickedPos = hex.getPosition();
                emit hexagonClicked(hex.getPosition());
            }
            update();
            break;
        }
    }
}


void Plateau::leaveEvent(QEvent *) {
    if (hoveredHex) {
        hoveredHex = nullptr;
        update(); // Force un repaint
    }
}

Hexagon* Plateau::findHexagonAt(const QPointF& pos) {
    for (auto& hex : hexagons) {
        if (hex.polygon.containsPoint(pos, Qt::WindingFill)) {
            //qDebug() << "Found hex at coordinates (q, r): "
            //         << hex.q << ", " << hex.r;
            return &hex;
        }
    }
    return nullptr;
}


void Plateau::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    /*
    for(const auto& pos : highlightedPlacements){
        qDebug()<<"("<<pos.getColonne()<<","<<pos.getLigne()<<")";
    }*/
    for (const auto& hex : hexagons) {
        Position pos = hex.getPosition();
        bool shouldDraw = false;

        if (plateau.find(pos) != plateau.end() && !plateau[pos].empty()) {
            shouldDraw = true;
        }
        else if (std::find(validMoves.begin(), validMoves.end(), pos) != validMoves.end()) {
            shouldDraw = true;
        }
        else if (std::find(highlightedPlacements.begin(), highlightedPlacements.end(), pos) != highlightedPlacements.end()) {
            shouldDraw = true;
        }

        if (shouldDraw) {
            painter.setPen(QPen(Qt::black, 1));

            if (std::find(validMoves.begin(), validMoves.end(), pos) != validMoves.end()) {
                painter.setBrush(QBrush(QColor(255, 0, 0)));
            }else if (std::find(highlightedPlacements.begin(), highlightedPlacements.end(), pos) != highlightedPlacements.end()) {
                painter.setBrush(QBrush(QColor(0, 0, 255, 100)));  // Blue for placements
            }
            else if (&hex == hoveredHex) {
                painter.setBrush(QBrush(QColor(255, 255, 200)));
            } else {
                painter.setBrush(QBrush(Qt::white));
            }

            painter.drawPolygon(hex.polygon);

            auto it = plateau.find(pos);
            if (it != plateau.end() && !it->second.empty()) {
                Piece* piece = it->second.back();
                QString text = QString("%1.%2")
                                   .arg(piece->getInitial())
                                   .arg(piece->getCouleur() == Couleur::Blanc ? "B" : "N");
                QFontMetrics fm(painter.font());
                QRect textRect = fm.boundingRect(text);
                QPointF textPos = hex.center - QPointF(textRect.width()/2.0, -textRect.height()/4.0);
                painter.drawText(textPos, text);
            }
        }
    }

    drawAvailablePlays(painter);
}

void Plateau::drawAvailablePlays(QPainter& painter) {
    painter.fillRect(0, height() - PIECES_ZONE_HEIGHT, width(), PIECES_ZONE_HEIGHT, Qt::lightGray);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(0, height() - PIECES_ZONE_HEIGHT, width(), height() - PIECES_ZONE_HEIGHT);

    QTextDocument doc;
    doc.setHtml(gameInfo);
    doc.setTextWidth(width() - 20);

    painter.translate(10, height() - PIECES_ZONE_HEIGHT + 10);
    doc.drawContents(&painter);
    painter.translate(-10, -(height() - PIECES_ZONE_HEIGHT + 10));
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

    // Force le rafraîchissement de l'affichage
    update();
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
        QMessageBox::warning(this, "Erreur", "La reine doit être posée avant de déplacer une autre pièce.");
        return false;
    }
    if(wouldSplitHive(from,to)){
        QMessageBox::warning(this, "Erreur", "Le mouvement casserait la ruche !");
        return false;
    }
    if(from.getColonne() == to.getColonne() && from.getLigne() == to.getLigne()){
        QMessageBox::warning(this, "Erreur", "La destination doit être différente de l'origine !");
        return false;
    }
    if (isPositionOccupied(from)) {
        Piece* piece = plateau.at(from).back();
        if (piece->getCouleur() == couleur && piece->isValidMove(to, plateau)) {
            plateau[from].pop_back();
            addPiece(piece, to);
            update(); // Force UI refresh
            return true;
        }else if (piece->getCouleur() != couleur){
            QMessageBox::warning(this, "Erreur", "Ce n'est pas votre pièce");
            return false;
        }else if(!piece->isValidMove(to, plateau)){
            QMessageBox::warning(this, "Erreur", "Le mouvement n'est pas valide !");
            return false;
        }
    }else{
        QMessageBox::warning(this, "Erreur", "La position d'origine du déplacement est vide");
    }
    return false;
}




void Plateau::drawAvailablePieces(QPainter& painter) {
    // Draw background zone
    painter.fillRect(0, height() - PIECES_ZONE_HEIGHT, width(), PIECES_ZONE_HEIGHT, Qt::lightGray);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(0, height() - PIECES_ZONE_HEIGHT, width(), height() - PIECES_ZONE_HEIGHT);

    // Create buttons first time
    if (pieceButtons.empty()) {
        initializeButtons();
    }

    // Draw game info text
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    QRect textRect(10, height() - PIECES_ZONE_HEIGHT/2, width() - 20, PIECES_ZONE_HEIGHT/2 - 10);
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignTop, gameInfo);
}








void Plateau::save(std::ofstream& outFile) const {
    if (!outFile) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'écrire dans le fichier");
        return;
    }

    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        const vector<Piece*>& pieces = entry.second;
        for (const Piece* piece : pieces) {
            outFile << pos.getColonne() << " " << pos.getLigne() << " "
                    << piece->getType() << " "
                    << (piece->getCouleur() == Noir ? "Noir" : "Blanc") << "\n";
        }
    }

    outFile << "END_PIECES" << "\n";
}

void Plateau::load(ifstream& inFile) {
    try {
        if (!inFile) {
            throw runtime_error("Impossible de lire le fichier");
        }
        plateau.clear();

        while(true) {
            string line;
            getline(inFile, line);

            if (line.empty()) continue;
            if (line == "END_PIECES") break;

            istringstream iss(line);
            int q, r;
            string type, couleurStr;

            iss >> q >> r >> type >> couleurStr;

            if (type.empty() || couleurStr.empty()) {
                throw runtime_error("Format de fichier incorrect");
            }

            Couleur couleur = (couleurStr == "Noir") ? Noir : Blanc;
            Position pos(q, r);
            Piece* piece = nullptr;

            if (type == "Reine") piece = new Reine(pos, couleur);
            else if (type == "Scarabee") piece = new Scarabee(pos, couleur);
            else if (type == "Araignee") piece = new Araignee(pos, couleur);
            else if (type == "Sauterelle") piece = new Sauterelle(pos, couleur);
            else if (type == "Fourmi") piece = new Fourmi(pos, couleur);
            else if (type == "Moustique") piece = new Moustique(pos, couleur);
            else if (type == "Coccinelle") piece = new Coccinelle(pos, couleur);
            else throw std::invalid_argument("Type de pièce inconnu");

            plateau[pos].push_back(piece);
        }
        update(); // Refresh display
    }
    catch(const std::exception& e) {
        QMessageBox::critical(this, "Erreur", e.what());
    }
}

void Plateau::undoMovePressed(){
    emit undoMove();
    update();
}


void Plateau::initializeButtons() {
    const std::vector<std::pair<char, QString>> pieces = {
        {'R', "Reine"}, {'A', "Araignée"}, {'S', "Scarabée"},
        {'F', "Fourmi"}, {'H', "Sauterelle"}, {'C', "Coccinelle"},
        {'M', "Moustique"}
    };

    int buttonWidth = 100;
    int buttonHeight = 20;

    // Calculate center x position
    int centerX = width() / 2 - buttonWidth / 2;
    int y = height() - PIECES_ZONE_HEIGHT + 10;

    for (const auto& piece : pieces) {
        QPushButton* button = new QPushButton(piece.second, this);
        button->setFixedSize(buttonWidth, buttonHeight);
        button->move(centerX, y);
        button->show();
        connect(button, &QPushButton::clicked, this,
                [this, type=piece.first]() {
                    emit pieceTypeSelected(type);
                });
        connect(button, &QPushButton::clicked, this,
                [this]() {
                    validMoves = vector<Position>();
                    emit highlightPositions();
                });
        pieceButtons.push_back(button);
        y += buttonHeight + 5;
    }

    undoButton = new QPushButton("Annuler votre coup précédent", this);
    undoButton->setFixedSize(buttonWidth+100, buttonHeight);
    int rightX = width() - buttonWidth - 150;  // Position to the right
    int rightY = height() - PIECES_ZONE_HEIGHT + 35 + buttonHeight;  // Position below the text
    undoButton->move(rightX, rightY);
    undoButton->show();
    connect(undoButton, &QPushButton::clicked, this, &Plateau::undoMovePressed);
}

void Plateau::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    int buttonWidth = 100;
    int buttonHeight = 20;
    int rightX = width() - buttonWidth - 150;  // Position to the right
    int rightY = height() - PIECES_ZONE_HEIGHT + 35 + buttonHeight;  // Position below the text

    if (undoButton) {
        undoButton->move(rightX, rightY);
    }

    int centerX = width() / 2 - buttonWidth / 2;
    int y = height() - PIECES_ZONE_HEIGHT + 10;

    for (auto button : pieceButtons) {
        button->move(centerX, y);
        y += buttonHeight + 5;
    }

}

