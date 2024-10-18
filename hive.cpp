#include "hive.hpp"

Position** Position::get_adjacent_coordinates() const {
    //Directions pour les lignes paires et impaires
    int offset_impaires[6][2] = {{-1, 0},{0, -1},{1, -1},{1, 0},{1, 1},{0, 1}};
    int offset_paires[6][2] = {{-1, 0},{-1, -1},{0, -1},{1, 0},{0, 1},{-1, 1}};

    //Calculer les coordonnées adjacentes en fonction de la parité de y
    Position** adjacent = new Position*[6]; //tableau de pointeurs sur les positions adjacentes
    if (r % 2 == 0) {
        for(unsigned int i = 0; i<6; i++){
            adjacent[i] = new Position(q + offset_paires[i][0], r + offset_paires[i][1]);
        }
    } else {
        for(unsigned int i = 0; i<6; i++){
            adjacent[i] = new Position(q + offset_impaires[i][0], r + offset_impaires[i][1]);
        }
    }

    return adjacent;
}

bool Position::isAdjacent(const Position& other) const{
    Position** adjacent = get_adjacent_coordinates();
    for(unsigned int i = 0; i<6; i++){
        if(*adjacent[i] == other){
            return true;
        }
    }
    return false;
}