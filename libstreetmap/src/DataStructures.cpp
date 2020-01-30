/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "DataStructures.h"

std::vector<LatLon> intersectionPoints;
std::unordered_map<std::string, StreetIndex> StreetNames;

void makeIntersectionTable(){
    for (IntersectionIndex id=0; id<getNumIntersections();id++){
        intersectionPoints.push_back(getIntersectionPosition(id));
    }
}

void makeStreetNamesTable(){
    for (int id=0; id<getNumStreets();id++){
        StreetNames.insert({getStreetName(id), id});
    }
}