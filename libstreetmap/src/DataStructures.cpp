/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "DataStructures.h"

std::vector<LatLon> intersectionTable;
std::unordered_map<std::string, StreetIndex> StreetNamesTable;
std::unordered_map<std::vector<int>, IntersectionIndex> Intersection_StreetTable;

// allocates vector of intersection ids
void makeIntersectionTable(){
    for (IntersectionIndex id=0; id<getNumIntersections(); id++){
        intersectionTable.push_back(getIntersectionPosition(id));
    }
}
// allocates map of all street names + street index
void makeStreetNamesTable(){
    for (StreetIndex id=0; id<getNumStreets();id++){
        StreetNamesTable.insert({getStreetName(id), id});
    }
}
// my implementation of find streets in intersection -p
void makeIntersection_StreetTable(){
    std::vector<int> streets_attached;
    for (IntersectionIndex id=0; id<getNumIntersections(); id++){
        for(int i=0; i < getIntersectionStreetSegmentCount(id); ++i){
            // add each street connecting to the intersection into streets_attached vector
            StreetSegmentIndex temp = getIntersectionStreetSegment(id, i);
            streets_attached.push_back((getInfoStreetSegment(temp)).streetID);
        }
        // add vector and intersection id to the map
        Intersection_StreetTable.insert({streets_attached, id});
        // clear vector for next for loop
        streets_attached.clear();
    }
}

