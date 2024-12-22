#include "piece.hpp"
#include <vector>
#include <queue>
#include <functional>


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

vector<Position> Piece::getBorderPositions(const unordered_map<Position, vector<Piece*>>& plateau)const{
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

bool Reine::isSurrounded(const unordered_map<Position, vector<Piece*>>& plateau) const {
    vector<Position> adjacents = getPosition().getAdjacentCoordinates();
    for (const Position& adj : adjacents) {
        if (plateau.find(adj) == plateau.end() || plateau.at(adj).empty()) {
            return false;
        }
    }
    return true;
}

bool Reine::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    if (!getPosition().isAdjacent(to)) {
        return false;
    }
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    if(getPosition().isSlidingPossible(to, plateau)) return true; 
    return false;
}

vector<Position> Reine::getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const {
    vector<Position> adjacents = getPosition().getAdjacentCoordinates();
    vector<Position> validMoves;
    for (const Position& adj : adjacents) {
        if (plateau.find(adj) == plateau.end() || plateau.at(adj).empty()) {
            if(getPosition().isSlidingPossible(adj, plateau)) validMoves.push_back(adj);
        }
    }
    return validMoves;
}


bool Scarabee::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const{
    //pas besoin de checker si la destination est libre car il peut monter sur d'autres pièces
    if (!getPosition().isAdjacent(to)) {
        return false;
    }
    if (plateau.find(to) == plateau.end() || plateau.at(to).empty()) { 
        if(!getPosition().isSlidingPossible(to, plateau)) return false; //s'il y n'y a pas de pièce sur la destination, il faut que la position soit accessible par glissement
    }
    return true;
}

vector<Position> Scarabee::getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const {
    vector<Position> adjacents = getPosition().getAdjacentCoordinates();
    vector<Position> validMoves;
    for (const Position& adj : adjacents) {
        if (plateau.find(adj) == plateau.end() || plateau.at(adj).empty()) {
            if(getPosition().isSlidingPossible(adj, plateau)) validMoves.push_back(adj);
        }else{
            validMoves.push_back(adj);
        }
    }
    return validMoves;
}


vector<Position> Araignee::getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const {
    vector<Position> validMoves; // Stocke les mouvements valides
    unordered_set<Position> visited; // Pour éviter les boucles infinies

    // Fonction récursive interne
    function<void(const Position&, int, vector<Position>)> explore = [&](const Position& current, int depth, vector<Position> path) {
        if (depth == 3) {
            if ((plateau.find(current) == plateau.end() || plateau.at(current).empty()) &&
                !path.empty() &&
                (plateau.find(path[0]) != plateau.end() && !plateau.at(path[0]).empty())) {
                validMoves.push_back(current);
            }
            return;
        }

        visited.insert(current);
        path.push_back(current);

        vector<Position> adjacents = current.getAdjacentCoordinates();
        for (const Position& neighbor : adjacents) {
            if (visited.find(neighbor) == visited.end()) {
                bool isValid = false;
                if (depth < 3) {
                    // Les positions en profondeur <= 2 doivent être vides, adjacentes à une pièce et différentes de la position initiale
                    isValid = (plateau.find(neighbor) == plateau.end() || plateau.at(neighbor).empty());
                    if (isValid) {
                        bool adjacentToPiece = false;
                        for (const Position& adjNeighbor : neighbor.getAdjacentCoordinates()) {
                            if (plateau.find(adjNeighbor) != plateau.end() && !plateau.at(adjNeighbor).empty() && adjNeighbor != getPosition()) {
                                adjacentToPiece = true;
                                break;
                            }
                        }
                        isValid = isValid && adjacentToPiece;
                    }
                }

                if (isValid) {
                    explore(neighbor, depth + 1, path);
                }
            }
        }

        visited.erase(current);
    };

    // Lancer la recherche depuis la position actuelle
    explore(getPosition(), 0, {});
    unordered_set<Position> validMovesSet(validMoves.begin(), validMoves.end());
    vector<Position> sol;
    for (auto elem : validMovesSet) sol.push_back(elem);
    return sol;
}

bool Araignee::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false;
    }
    return true;
}



bool Sauterelle::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const {
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    vector<Position> Adjacents = getPosition().getAdjacentCoordinates();
    for(const auto& pos : Adjacents){
        if(pos == to){
            return false; //la sauterelle doit sauter au moins 1 pièce pour se déplacer, donc la destination ne peut pas être une case adjacente
        }
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

vector<Position> Sauterelle::getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const {
    vector<Position> validMoves; // Contiendra les positions valides où la sauterelle peut se déplacer
    vector<Position> borderPositions = getBorderPositions(plateau); 
    for(const Position& border : borderPositions){
        if(isValidMove(border, plateau)){
            validMoves.push_back(border);
        } 
    }
    return validMoves;
}
bool Fourmi::canReach(const Position& current, const Position& target, const unordered_map<Position, vector<Piece*>>& plateau, unordered_set<Position>& visited, const vector<Position>& borderPositions) const {
    // Utilisation d'une file pour BFS
    std::queue<Position> toVisit;
    toVisit.push(current);
    visited.insert(current);

    while (!toVisit.empty()) {
        Position current = toVisit.front();
        toVisit.pop();

        if (current == target) {
            return true;
        }

        for (const Position& adj : current.getAdjacentCoordinates()) {
            if (visited.find(adj) == visited.end() &&
                find(borderPositions.begin(), borderPositions.end(), adj) != borderPositions.end() &&
                current.isSlidingPossible(adj, plateau)) {
                toVisit.push(adj);
                visited.insert(adj);
            }
        }
    }

    return false;
}

bool Fourmi::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau) const {
    // Vérifier que la destination est vide
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) {
        return false;
    }

    // Obtenir les positions bordantes
    vector<Position> borderPositions = getBorderPositions(plateau);
    unordered_set<Position> visited;

    // Vérifier si la position est atteignable
    return canReach(getPosition(), to, plateau, visited, borderPositions);
}

vector<Position> Fourmi::getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const {
    vector<Position> borderPositions = getBorderPositions(plateau);
    vector<Position> validMoves;

    for (const Position& border : borderPositions) {
        if (isValidMove(border, plateau)) {
            validMoves.push_back(border);
        }
    }
    return validMoves;
}

vector<Position> Moustique::getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau)const{
    unordered_set<Position> validMoves; //On fait un unordered_set pour éviter que, si on peut arriver a la même position de plusieurs manières, elle soit ajoutée plusieurs fois
    vector<Position> adjacents = getPosition().getAdjacentCoordinates();
    unordered_set<string> types;
    for(const auto& pos : adjacents){
        auto it = plateau.find(pos);
        if(it != plateau.end() && !it->second.empty()){
            types.insert(it->second.back()->getType());
        }
    }
    for(const auto& type : types){
        if(type == "Reine"){
            Reine tempReine(getPosition(),getCouleur()); // on créé une instance temporaire de la reine à la même position que le Moustique
            vector<Position> validMovesReine = tempReine.getValidMoves(plateau); //on récupère ses mouvements valides
            validMoves.insert(validMovesReine.begin(), validMovesReine.end()); 
        }else if(type == "Scarabee"){
            Scarabee tempScarabee(getPosition(),getCouleur()); 
            vector<Position> validMovesScarabee = tempScarabee.getValidMoves(plateau); 
            validMoves.insert(validMovesScarabee.begin(), validMovesScarabee.end()); 
        }else if(type == "Araignee"){
            Araignee tempAraignee(getPosition(), getCouleur()); 
            vector<Position> validMovesAraignee = tempAraignee.getValidMoves(plateau); 
            validMoves.insert(validMovesAraignee.begin(), validMovesAraignee.end()); 
        }else if(type == "Sauterelle"){
            Sauterelle tempSauterelle(getPosition(), getCouleur()); 
            vector<Position> validMovesSauterelle = tempSauterelle.getValidMoves(plateau); 
            validMoves.insert(validMovesSauterelle.begin(), validMovesSauterelle.end());
        }else if(type == "Fourmi"){
            Fourmi tempFourmi(getPosition(), getCouleur()); 
            vector<Position> validMovesFourmi = tempFourmi.getValidMoves(plateau); 
            validMoves.insert(validMovesFourmi.begin(), validMovesFourmi.end());
        }else if(type == "Coccinelle"){
            Coccinelle tempCoccinelle(getPosition(), getCouleur()); 
            vector<Position> validMovesCoccinelle = tempCoccinelle.getValidMoves(plateau); 
            validMoves.insert(validMovesCoccinelle.begin(), validMovesCoccinelle.end());
        }
    }
    return vector<Position>(validMoves.begin(), validMoves.end());
}


bool Moustique::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    vector<Position> validMoves = getValidMoves(plateau);
    for(const auto& pos : validMoves){
        if(pos == to){  
            return true;
        }
    }
    return false;
}

bool Coccinelle::isValidMove(const Position& to, const unordered_map<Position, vector<Piece*>>& plateau)const{
    if (plateau.find(to) != plateau.end() && !plateau.at(to).empty()) { //pièce sur la destination
        return false; 
    }
    return true;
}

vector<Position> Coccinelle::getValidMoves(const unordered_map<Position, vector<Piece*>>& plateau) const {
    vector<Position> validMoves; // Stocke les mouvements valides
    unordered_set<Position> visited; // Pour éviter les boucles infinies

    // Fonction récursive interne
    function<void(const Position&, int, vector<Position>)> explore = [&](const Position& current, int depth, vector<Position> path) {
        if (depth == 3) {
            if ((plateau.find(current) == plateau.end() || plateau.at(current).empty()) &&
                !path.empty() &&
                (plateau.find(path[0]) != plateau.end() && !plateau.at(path[0]).empty())) {
                validMoves.push_back(current);
            }
            return;
        }

        visited.insert(current);
        path.push_back(current);

        vector<Position> adjacents = current.getAdjacentCoordinates();
        for (const Position& neighbor : adjacents) {
            if (visited.find(neighbor) == visited.end()) {
                bool isValid = false;
                if (depth < 2) {
                    // Les deux premiers mouvements doivent être sur des cases occupées
                    isValid = (plateau.find(neighbor) != plateau.end() && !plateau.at(neighbor).empty());
                } else if (depth == 2) {
                    // Le dernier mouvement doit être sur une case vide
                    isValid = (plateau.find(neighbor) == plateau.end() || plateau.at(neighbor).empty());
                }

                if (isValid) {
                    explore(neighbor, depth + 1, path);
                }
            }
        }

        visited.erase(current);
    };

    // Lancer la recherche depuis la position actuelle
    explore(getPosition(), 0, {});
    unordered_set<Position> validMovesSet(validMoves.begin(), validMoves.end());
    vector<Position> sol;
    for (auto elem : validMovesSet) sol.push_back(elem);
    return sol;
}

