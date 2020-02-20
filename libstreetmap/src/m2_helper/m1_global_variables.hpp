
#pragma once

#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <map>
#include <list>
#include <unordered_map>
#include <math.h>
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
/*==================== GLOBAL VARIABLES DECLARATIONS ====================*/
std::vector<std::pair<double, double>> tableOfDivisors; // -p
std::multimap<std::string, StreetIndex> capitalizedStreetNamesTable; // -p
std::vector<std::vector<int>> segmentsOfStreets;
std::vector<std::vector<int>> intersectionsOfStreets;
std::vector<std::vector<int>> segmentsOfIntersections;
std::vector<std::vector<std::string>> streetNamesOfIntersections;

std::unordered_map<OSMID, const OSMWay*> OSMWayTable;
std::unordered_map<OSMID, const OSMNode*> OSMNodeTable;
typedef std::multimap<int, int>::iterator StreetsIt;
typedef std::vector<int>::iterator VectorIt;

/*======================== FUNCTIONS DECLARATIONS ========================*/

void makeTableOfDivisors();
void makeCapitalizedStreetNamesTable();

void makeSegmentsOfStreets();
void makeIntersectionsOfStreets();


void makeSegmentsOfIntersections();

void makeOSMWayTable();
void makeOSMNodeTable();

double x_distance_between_2_points(LatLon first, LatLon second);
double y_distance_between_2_points(LatLon first, LatLon second);
void remove_dups_in_vecs(std::vector<int> &vectorA);

/*==================== GLOBAL FUNCTION IMPLEMENTATION ====================*/

// Removes duplicate values in a given vector
void remove_dups_in_vecs(std::vector<int> &vectorA){
    auto end = vectorA.end();
    
    // Loops through the whole vector
    for(auto it=vectorA.begin();it<end;++it){
        
        //Removes value (*it) if duplicate is found and returns the new end() pointer 
        //Pointer 'end' changes as values are removed for less loops
        end = std::remove(it+1,end, *it);
    }
    
    // Erases any empty indexes in the vector
    vectorA.erase(end, vectorA.end());
}



// Find the distance in the x component of two points using the formula provided in M1 Instructions
double x_distance_between_2_points(LatLon first, LatLon second){
    double LatAvg = (first.lat()+second.lat()) * DEGREE_TO_RADIAN / 2;
    double x1= first.lon() * DEGREE_TO_RADIAN * cos(LatAvg);
    double x2=second.lon() * DEGREE_TO_RADIAN * cos(LatAvg);
    return EARTH_RADIUS_METERS * (x2-x1);
}



// Find the distance in the y component of two points using the formula provided in M1 Instructions
double y_distance_between_2_points(LatLon first, LatLon second){  
    double y1= first.lat() * DEGREE_TO_RADIAN;   
    double y2= second.lat() * DEGREE_TO_RADIAN;
    return EARTH_RADIUS_METERS * (y2-y1); 
}



// Creates global variable tableOfDivisors used to optimize function find_street_segment_travel_time
// Info: Vector is organized by StreetSegmentIndex. First element is the street segment length, second element is its speed limit.
void makeTableOfDivisors(){
    tableOfDivisors.resize(getNumStreetSegments());
    for (StreetSegmentIndex id = 0; id<getNumStreetSegments(); id++){
        tableOfDivisors[id]=(std::make_pair<double, double>((find_street_segment_length(id)),(1/getInfoStreetSegment(id).speedLimit)));
    }
}



// Creates global variable capitalizedStreetNamesTable used to optimize function find_street_partial_name
// Info: Multimap is organized by StreetIndex string name. Key is the street name, with spaces removed and in all capital letters. Value is the corresponding StreetIndex.
void makeCapitalizedStreetNamesTable(){
    std::string capitalizedName;
    for (StreetIndex id=0; id<getNumStreets();id++){
        capitalizedName = getStreetName(id);
        capitalizedName.erase(remove(capitalizedName.begin(), capitalizedName.end(), ' '), capitalizedName.end());
        std::transform(capitalizedName.begin(), capitalizedName.end(), capitalizedName.begin(), ::toupper);
        
        capitalizedStreetNamesTable.insert(std::pair<std::string, StreetIndex>(capitalizedName, id));
    }
}



// Creates the global variable, segmentsOfStreets
// Info: First element is the street IDs and Second element is a vector of street segment IDs
void makeSegmentsOfStreets(){    

    int street_ID;
    
    // Resizing global variable to allow for inserting through indexes
    segmentsOfStreets.resize(getNumStreets());
    
    // Loops through all the street segments
    for(int j =0 ; j< getNumStreetSegments() ; ++j){
        
        street_ID = getInfoStreetSegment(j).streetID;
        
        // Inserts the segment ID into the index of the street ID 
        segmentsOfStreets[street_ID].push_back(j);
    }
    
}




// Creates the global variable, segmentsOfIntersections. This function uses segmentsOfStreets in its implementation and does not check for duplicates
// Info: First element is the intersection IDs and Second element is a vector of street IDs
void makeIntersectionsOfStreets(){

    std::vector<int> intersections_of_streets;
    
    // Resizing global variable to allow for inserting through indexes
    intersectionsOfStreets.resize(getNumStreets());
    
    // Loop through all the streets
    for(int street = 0; street <getNumStreets() ;++ street){
        
        // Clear local vector to ensure no inserting errors
        intersections_of_streets.clear();
        
        //Loop through all the streets attached to the intersection
        for(int inter =0;inter<segmentsOfStreets[street].size();inter++){
            
            // Find the intersections attached to the segment
            int interTo = getInfoStreetSegment(segmentsOfStreets[street][inter]).to;
            int interFrom = getInfoStreetSegment(segmentsOfStreets[street][inter]).from;
            
            // Inserts intersections in local vector
            intersections_of_streets.push_back(interTo);
            intersections_of_streets.push_back(interFrom);
        }
        
        // Copies local vector into vector of streets, local vector is cleared afterwards
        intersectionsOfStreets[street]= intersections_of_streets;
    }
    
}




// Creates the global variable, segmentsOfIntersections
// Info: First element is the intersection IDs and Second element is a vector of street segment IDs
void makeSegmentsOfIntersections(){  
    
    // Created a vector that will be put in into each index in the global variable
    std::vector<int> street_segments_of_intersection;
    // Resizing global variable to allow for inserting through indexes
    segmentsOfIntersections.resize(getNumIntersections());
    
    //Loop through all the intersections
    for(int intersection = 0 ; intersection < getNumIntersections() ; ++intersection ){
        
        // Clear local vector to ensure no inserting errors
        street_segments_of_intersection.clear();
        int num_segments = getIntersectionStreetSegmentCount(intersection);
        
        // Loops through all the segments attached to the intersection 
        for(int segment = 0; segment < num_segments ; ++segment){
            
            //Gets the segmentID of the segment attached to that intersection by index
            street_segments_of_intersection.push_back(getIntersectionStreetSegment(intersection, segment));
        }
        // Inserts vector of segments to the intersection index into the global variable
        segmentsOfIntersections[intersection] = street_segments_of_intersection;
    }
}




// Creates global variable OSMWayTable used to optimize function find_way_length
// Info: This unordered_map collects OSMIDs as the key and their corresponding OSMWay* as the value
void makeOSMWayTable(){
    for (int i=0; i<getNumberOfWays(); i++){
        OSMWayTable.insert(std::pair<OSMID, const OSMWay*>(getWayByIndex(i)->id(), getWayByIndex(i)));
    }
}



// Creates global variable OSMWayTable used to optimize function find_way_length
// Info: This unordered_map collects OSMIDs as the key and their corresponding OSMNode* as the value 
void makeOSMNodeTable(){
    for (int i=0; i<getNumberOfNodes(); i++){
        OSMNodeTable.insert(std::pair<OSMID, const OSMNode*>(getNodeByIndex(i)->id(), getNodeByIndex(i)));
    }
}