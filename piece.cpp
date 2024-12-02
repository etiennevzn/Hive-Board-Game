#include "piece.hpp"
#include <vector>
#include <queue>

string toString(Couleur couleur) {
    switch (couleur) {
        case Noir: return "Noir";
        case Blanc: return "Blanc";
        default: return "Inconnu";
    }
}

Position::Position(int x, int y, int z){
    q = x+(y-(y&1))/2;
    r = y;
}   

vector<int> Position::toCube()const{
    int x = q-((r-(r&1))/2);
    int y = r;
    return {x,y,-x-y};
}

vector<Position> Position::getAdjacentCoordinates() const {
    // Directions pour les lignes paires et impaires
    std::vector<std::pair<int, int>> oddDirections = {
        {-1, 0}, {0, -1}, {1, -1},
        {1, 0}, {1, 1}, {0, 1}
    };
    std::vector<std::pair<int, int>> evenDirections = {
        {-1, 0}, {-1, -1}, {0, -1},
        {1, 0}, {0, 1}, {-1, 1}
    };

    // Choisir les directions en fonction de la parité de r
    const auto& directions = (r % 2 == 0) ? evenDirections : oddDirections;

    // Calculer les coordonnées adjacentes
    std::vector<Position> adjacent;
    for (const auto& direction : directions) {
        adjacent.emplace_back(q + direction.first, r + direction.second);
    }

    return adjacent;
}

bool Position::isAdjacent(const Position& other) const {
    vector<Position> adjacents = getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (adj == other) {
            return true;
        }
    }
    return false;
}

/*
Remarque sur cette méthode : commentée car la méthode isSlidingPossible est plus efficace. 
Je la supprime parce que elle pourra potentiellement être utile, peut-être on pourra la changer en isSurrounded pour savoir si une Position
est entourée par 6 autres positions (pour la condition de défaite par exemple)

bool Position::isAccessible(const unordered_map<Position, vector<Piece*>>& plateau)const{
    vector<Position> adjacentes = getAdjacentCoordinates();
    int piecesCount = 0;
    for(const Position& adj : adjacentes){
        auto it = plateau.find(adj);
        if(it != plateau.end() && !it->second.empty()) piecesCount++;
    }
    return piecesCount<5; //s'il y a plus de 4 pièces autour de la position, elle est innaccessible par glissement
}

*/

bool Position::isSlidingPossible(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    if (!isAdjacent(to)) {
        return false;
    }

    //on récupère les positions adjacentes à la position de départ et à la position d'arrivée (2 positions)
    vector<Position> adjacentsFrom = getAdjacentCoordinates();
    vector<Position> adjacentsTo = to.getAdjacentCoordinates();
    unordered_set<Position> setAdjTo(adjacentsTo.begin(), adjacentsTo.end()); 
    vector<Position> intersection;

    for (const Position& pos : adjacentsFrom) {
        if (setAdjTo.find(pos) != setAdjTo.end()) { 
            intersection.push_back(pos);
        }
    }

    int OccupiedCount = 0;
    for(const Position& pos : intersection){
        auto it = plateau.find(pos);
        if(it != plateau.end() && !it->second.empty()) OccupiedCount++;
    }

    return !(OccupiedCount == 2); //si les 2 positions adjacentes à la position de départ et à la position d'arrivée sont occupées, alors le glissement est impossible

}





//MÉTHODES DE LA CLASSE PIECE

bool Reine::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    if (!getPosition().isAdjacent(to)) {
        return false;
    }
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    // Check if the queen can slide to the destination
    vector<Position> adjacents = getPosition().getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (adj == to) {
            return true;
        }
    }
    return false;
}

bool Scarabee::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    //pas besoin de checker si la destination est libre car il peut monter sur d'autres pièces
    if (!getPosition().isAdjacent(to)) {
        return false;
    }
    return true;
}


bool Araignee::isValidMoveRecursive(const Position& current, const Position& target, const unordered_map<Position, vector<Piece*>>& plateau, int stepsLeft, unordered_set<Position>& visited) const {
    if (stepsLeft == 0) {
        return current == target;
    }
    visited.insert(current);
    vector<Position> adjacents = current.getAdjacentCoordinates();

    for (const Position& neighbor : adjacents) {
        if (plateau.find(neighbor) != plateau.end() && !plateau.at(neighbor).empty()) {
            vector<Position> neighborAdjacents = neighbor.getAdjacentCoordinates();
            for (const Position& candidate : neighborAdjacents) {
                if (find(adjacents.begin(), adjacents.end(), candidate) != adjacents.end() &&
                    visited.find(candidate) == visited.end()) {
                    cout<<"("<<candidate.getColonne()<<","<<candidate.getLigne()<<")"<<endl;
                    if (isValidMoveRecursive(candidate, target, plateau, stepsLeft - 1, visited)) {
                        return true;
                    }
                }
            }
        }
    }
    visited.erase(current);
    return false;
}


bool Araignee::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const {
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    unordered_set<Position> visited; // Suivi des cases déjà explorées
    return isValidMoveRecursive(getPosition(), to, plateau, 3, visited);
}




bool Sauterelle::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const {
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    /*Déterminer si 2 cases sont en "ligne droite" est très compliqué avec notre système de coordonnées actuel. Par conséquent,
    on converti d'abord les coordonnées de la pièce en coordonnées cubiques avant de regarder si les 2 cases sont en ligne droite*/
    vector<int> fromCubeCoords = getPosition().toCube();
    vector<int> toCubeCoords = to.toCube();
    if(fromCubeCoords[0]!=toCubeCoords[0] && fromCubeCoords[1]!=toCubeCoords[1] && fromCubeCoords[2]!=toCubeCoords[2]){
        return false; //sous cette condition, les 2 cases ne sont pas alignées
    }

    int distance = max(abs(fromCubeCoords[0] - toCubeCoords[0]), max(abs(fromCubeCoords[1] - toCubeCoords[1]), abs(fromCubeCoords[2] - toCubeCoords[2])));
    for (int i = 1; i < distance; ++i) {
        int interX = fromCubeCoords[0] + i * (toCubeCoords[0] - fromCubeCoords[0]) / distance;
        int interY = fromCubeCoords[1] + i * (toCubeCoords[1] - fromCubeCoords[1]) / distance;
        int interZ = fromCubeCoords[2] + i * (toCubeCoords[2] - fromCubeCoords[2]) / distance;
        Position intermediate(interX, interY, interZ); //on calcule les coordonnées de chaque position entre les deux cases

        // vérifier si la position intermédiaire est occupée
        if (plateau.find(intermediate) == plateau.end() || plateau.find(intermediate)->second.empty()) {// case vide ou absente du plateau
            return false;
        }
    }
    return true;
}

vector<Position> Fourmi::getBorderPositions(const unordered_map<Position, vector<Piece*>>& plateau)const{
    unordered_set<Position> borderPositions; //unordered set pour éviter les doublons. 
    for (const auto& entry : plateau) {
        const Position& pos = entry.first;
        if (!entry.second.empty()) { 
            vector<Position> adjacents = pos.getAdjacentCoordinates();
            for (const Position& adj : adjacents) {
                if (plateau.find(adj) == plateau.end() || plateau.at(adj).empty()) {
                    borderPositions.insert(adj);
                }
            }
        }
    }
    return vector<Position>(borderPositions.begin(), borderPositions.end());
}

bool Fourmi::canReach(const Position& current, const Position& target, const unordered_map<Position, vector<Piece*>>& plateau, unordered_set<Position>& visited, const vector<Position>& borderPositions) const {
    if (current == target) {
        return true;
    }
    visited.insert(current);
    vector<Position> adjacents = current.getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (visited.find(adj) == visited.end() && find(borderPositions.begin(), borderPositions.end(), adj) != borderPositions.end() && current.isSlidingPossible(adj, plateau)) {
            return canReach(adj, target, plateau, visited, borderPositions);

        }
    }
    return false;
}

bool Fourmi::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        cout<<"Piece sur la destination"<<endl;
        return false; 
    }
    vector<Position> borderPositions = getBorderPositions(plateau);
    unordered_set<Position> visited;
    return canReach(getPosition(), to, plateau, visited, borderPositions);
}


bool Moustique::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    return true;
}

bool Coccinelle::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    return true;
}