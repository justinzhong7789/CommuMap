#pragma once

#include "m1.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
using namespace ezgl; 

void makeStreetSizeTable();
void makeSegments_OfStreets();


std::vector<LatLon> add_nodes(StreetSegmentIndex id);


struct streetSegmentsData {
    // node takes the start, end, and curve points (if applicable) of the street segment
    std::vector<LatLon> node;
};

struct streetData {
    std::vector<streetSegmentsData> segments;
    std::string name;
    double length;
    //std::vector<std::vector<LatLon>> node;
};

std::vector <streetSegmentsData> streetSegments;

// Define big streets as streets with length 1+ km
std::vector <streetData> bigStreetsTable;
std::vector<std::vector<int>> segments_OfStreets;

 void makeSegments_OfStreets(){    

    int street_ID;
    // Resizing global variable to allow for inserting through indexes
    segments_OfStreets.resize(getNumStreets());
    // Loops through all the street segments
    for(int j =0 ; j< getNumStreetSegments() ; ++j){
        street_ID = getInfoStreetSegment(j).streetID;
        // Inserts the segment ID into the index of the street ID 
        segments_OfStreets[street_ID].push_back(j);
    }
}
 
// add all street names length together
void makeStreetSizeTable(){
    streetData street_data;
    streetSegmentsData street_segment_data;

    for (StreetIndex i=0; i<getNumStreets(); i++){
        double street_length = 0;
        street_data.segments.clear();
        for (int j=0; j<segments_OfStreets[i].size(); j++){
            StreetSegmentIndex id = segments_OfStreets[i][j];
            street_length += find_street_segment_length(id);
            street_segment_data.node = add_nodes(id);
            street_data.segments.push_back(street_segment_data);
        }
        street_data.name = getStreetName(i);
        street_data.length = street_length;
        
        if (street_length > 10000){           
            bigStreetsTable.push_back(street_data);
        }
    }
}

/*
void makeStreetsSizeTable(){
    streetData street_data;
    InfoStreetSegment info;
    for (StreetIndex i=0; i<getNumStreets(); i++){
        double street_length = 0;
        (street_data.node).clear();
        for (StreetSegmentIndex j=0; j<segmentsOfStreets[i].size(); j++){
            street_length += find_street_segment_length(j);
            info = getInfoStreetSegment(j);
            (street_data.node).push_back(add_nodes(j));
        }
        street_data.name = getStreetName(i);
        street_data.length = street_length;
        
        if (street_length > 1000){
            
            bigStreetsTable.push_back(street_data);
        }
    }
}*/
/*    
void makeBigStreetsTable(){
    streetSegmentsData temp;
    InfoStreetSegment info;
    for (StreetSegmentIndex id = 0; id<getNumStreetSegments(); id++){
         
        // If street length is greater than 1000m, add to the table
        //if (tableOfDivisors[id].first > 1000){
        // if (find_street_segment_length(id) > 500){
            info = getInfoStreetSegment(id);
            
            temp.node = add_nodes(id);
            temp.streetName = getStreetName(info.streetID);
            bigStreetsTable.push_back(temp);
        }
    }
} */
std::vector<LatLon> add_nodes(StreetSegmentIndex id){
    InfoStreetSegment info = getInfoStreetSegment(id);
    std::vector<LatLon> node_list;
    node_list.resize(info.curvePointCount+2);
    // Find LatLon of beginning of intersection
    node_list[0] = getIntersectionPosition(info.from);
    // If no curve points, find end of intersection
    if (info.curvePointCount == 0){
            node_list[1] = getIntersectionPosition(info.to);
    }
    else {
        for (int i=1; i<=info.curvePointCount+1; i++){
            if (i>info.curvePointCount){
                node_list[i] = getIntersectionPosition(info.to);
            } else {
                node_list[i] = getStreetSegmentCurvePoint(i-1, id);
            }
        }
    }
    return node_list;
}