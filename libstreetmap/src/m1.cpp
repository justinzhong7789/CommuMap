/* 
 * Copyright 2020 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
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
#include <iostream>
#include <set>

/*==================== GLOBAL VARIABLES DECLARATIONS ====================*/
std::vector<std::pair<double, double>> tableOfDivisors; // -p
std::multimap<std::string, StreetIndex> capitalizedStreetNamesTable; // -p
std::vector<std::vector<int>> segmentsOfStreets;
std::vector<std::vector<int>> intersectionsOfStreets;
std::vector<std::vector<int>> segmentsOfIntersections;
std::vector<std::vector<std::string>> streetNamesOfIntersections;
//std::vector<std::vector<int>> adjacentIntersections;
typedef std::multimap<int, int>::iterator StreetsIt;
typedef std::vector<int>::iterator VectorIt;

/*==================== GLOBAL FUNCTIONS IMPLEMENTATIONS ====================*/

//ARE THESE EVEN NEEDED?
void makeTableOfDivisors();
void makeCapitalizedStreetNamesTable();

void makeSegmentsOfStreets();
void makeIntersectionsOfStreets();

//Used a lot
//void makeAdjacentIntersections();
//void makeSegmentsOfIntersections();
//void makeStreetNamesOfIntersections();


double x_distance_between_2_points(LatLon first, LatLon second);
double y_distance_between_2_points(LatLon first, LatLon second);
std::vector<int> remove_dups_in_vecs(std::vector<int> vectorA);
bool element_exists(int element, std::vector<int> vectorA);
// bool valueExistsInMultiMap(std::multimap<int,int> map, int key, int ID);
/*==================== GLOBAL FUNCTION IMPLEMENTATION ====================*/

/* I don't know if these two functions would be faster than using std::find(first interator, second iterator) 
 * And then inputing the value if NOT found.. cuz I use it alot in the functions I made -m
 */
std::vector<int> remove_dups_in_vecs(std::vector<int> vectorA){
    std::vector<int> sorted_vec;
    for(int i=0;i<vectorA.size();i++){
        //if element is not already in the vector, insert it
        if(!element_exists(vectorA[i], sorted_vec)){
            sorted_vec.push_back(vectorA[i]);
        }
    }
    return sorted_vec;
}
bool element_exists(int element, std::vector<int> vectorA){
    bool found=false;
    for (int i=0; i<vectorA.size();i++){
        if(vectorA[i]==element){
            found = true;
            break;
        }
    }
    return found;
}
/* I realized the remove_dups_in_vecs uses a two functions and both have for loops -m*/



double x_distance_between_2_points(LatLon first, LatLon second){
    double LatAvg = (first.lat()+second.lat())*DEGREE_TO_RADIAN/2;
    double x1= first.lon()*DEGREE_TO_RADIAN* cos(LatAvg);
    double x2=second.lon()*DEGREE_TO_RADIAN* cos(LatAvg);
    return EARTH_RADIUS_METERS*(x2-x1);
}

double y_distance_between_2_points(LatLon first, LatLon second){
    double y1= first.lat()*DEGREE_TO_RADIAN;   
    double y2= second.lat()*DEGREE_TO_RADIAN;
    return EARTH_RADIUS_METERS*(y2-y1); 
}

void makeTableOfDivisors(){
    tableOfDivisors.resize(getNumStreetSegments());
    for (StreetSegmentIndex id = 0; id<getNumStreetSegments(); id++){
        tableOfDivisors[id]=(std::make_pair<double, double>((find_street_segment_length(id)),(1/getInfoStreetSegment(id).speedLimit)));
    }
}
// creates a table of capitalized street names without spaces in order of streetindex
void makeCapitalizedStreetNamesTable(){
    std::string capitalizedName;
    for (StreetIndex id=0; id<getNumStreets();id++){
        capitalizedName = getStreetName(id);
        capitalizedName.erase(remove(capitalizedName.begin(), capitalizedName.end(), ' '), capitalizedName.end());
        std::transform(capitalizedName.begin(), capitalizedName.end(), capitalizedName.begin(), ::toupper);
        
        capitalizedStreetNamesTable.insert(std::pair<std::string, StreetIndex>(capitalizedName, id));
    }
}

//can't delete this one... it's implemented in a way where u cant -M
void makeSegmentsOfStreets(){    

    int street_ID;
    segmentsOfStreets.resize(getNumStreets());
    for(int j =0 ; j< getNumStreetSegments() ; ++j){
        street_ID = getInfoStreetSegment(j).streetID;
        segmentsOfStreets[street_ID].push_back(j);
    }
    
}
//
////M --Used too often. Should keep
//void makeSegmentsOfIntersections(){  
//    std::vector<int> street_segments_of_intersection;
//    segmentsOfIntersections.resize(getNumIntersections());
//    for(int j =0 ; j< getNumIntersections() ; ++j){
//        
//        street_segments_of_intersection.clear();
//        int num_intersections = getIntersectionStreetSegmentCount(j);
//        
//        //going through the segment index attached to the intersection 
//        for(int i=0; i < num_intersections; ++i){
//            street_segments_of_intersection.push_back(getIntersectionStreetSegment(j, i));
//        }
//        segmentsOfIntersections[j] = street_segments_of_intersection;
//    }
//}

//M
//KEEP IN MIND THAT WHEN your helper functions are deleted, the funtion will no longer run at 0(1)
//so if that function is called in other functions, then it'll take even loger...
////choose wisely when choosing which helper function to delete.
//void makeStreetNamesOfIntersections(){
//    std::vector<std::string> street_names_of_intersection;
//    streetNamesOfIntersections.resize(getNumIntersections());
//    
//    for(int j =0 ; j< getNumIntersections() ; ++j){
//        street_names_of_intersection.clear();
//        
//        //going through the segment index attached to the intersection 
//        for(int i=0; i < segmentsOfIntersections[j].size() ; ++i){
//            int streetID = getInfoStreetSegment(segmentsOfIntersections[j][i]).streetID;
//            street_names_of_intersection.push_back(getStreetName(streetID));
//        }
//        streetNamesOfIntersections[j] = street_names_of_intersection;
//    }
//}

//M
//void makeAdjacentIntersections(){
//    
//    std::vector<int> adjacent_intersections;
//    adjacent_intersections.clear(); //shouldn't be needed but just to be safe
//    //pair necessary for helper function input
//    std::pair<int,int> twoIntersections;
//    std::vector<int>::iterator checkForFind;//to create the ability to use the [] operator
//    adjacentIntersections.resize(getNumIntersections());
//    
//    //Loop through all the intersections so that they can be assigned
//    //variables j and i can be changed to be more descriptive
//    for(int j =0 ; j< getNumIntersections() ; ++j){
//        
//        
//        //assign the intersection that the segment should be going FROM as the first pair
//        twoIntersections.first = j;
//        
//        //loop through all the segments connected to that intersection
//        for (int i = 0; i < getIntersectionStreetSegmentCount(j) ; ++i){ //can change to .size but won't let me for some reason
//            
//            
//            //assign second pair to the TO intersection (Done first because typically, the segment should run FROM the first pair TO the second pair)
//            twoIntersections.second = getInfoStreetSegment(segmentsOfIntersections[j][i]).to;
//            
//            //checks if directly connected but not connected to itself
//            if(twoIntersections.first != twoIntersections.second && are_directly_connected(twoIntersections)){
//            
//                //checks to see if intersection is already in that vector before inserting
//                checkForFind = std::find(adjacent_intersections.begin(), adjacent_intersections.end(), twoIntersections.second);
//                if(checkForFind == adjacent_intersections.end()) //if intersection is not found
//
//                    adjacent_intersections.push_back(twoIntersections.second);
//            }
//            //Not sure why if this is necessary... I think is typically just in case for two-way streets
//            
//            //assign second pair to the FROM intersection
//            twoIntersections.second = getInfoStreetSegment(segmentsOfIntersections[j][i]).from;
//            
//            //same code as above. Could be fixed for better style
//            if(twoIntersections.first != twoIntersections.second && are_directly_connected(twoIntersections)){
//                checkForFind = std::find(adjacent_intersections.begin(), adjacent_intersections.end(), twoIntersections.second);
//                if(checkForFind == adjacent_intersections.end())
//                    adjacent_intersections.push_back(twoIntersections.second);
//            }
//        }
//        
//        //inserts the vector into the global vector of vectors with an index of the first Intersection
//        adjacentIntersections[j] = adjacent_intersections;
//        adjacent_intersections.clear();
//    } 
//}

void makeIntersectionsOfStreets(){
//    
//   std::vector<int> intersections_of_street;
//   int interTo, interFrom;
//   intersectionsOfStreets.resize(getNumStreets());
//    
//    for(int j =0 ; j< getNumStreets() ; ++j){
//        intersections_of_street.clear();
//        
//        //going through the segment index attached to the intersection 
//        for(int i=0; i < segmentsOfStreets[j].size() ; ++i){
//            interTo = getInfoStreetSegment(segmentsOfStreets[j][i]).to;
//            interFrom = getInfoStreetSegment(segmentsOfStreets[j][i]).from;
//            
//            intersections_of_street.push_back(interTo);
//            intersections_of_street.push_back(interFrom);
//        }
//        intersectionsOfStreets[j] = remove_dups_in_vecs(intersections_of_street);
//    }
//}

    intersectionsOfStreets.resize(getNumStreets());
    for(int j=0; j<getNumStreets() ;++j){
        for(int i=0;i<segmentsOfStreets[j].size();i++){
        intersectionsOfStreets[j].push_back(getInfoStreetSegment(segmentsOfStreets[j][i]).from);
        intersectionsOfStreets[j].push_back(getInfoStreetSegment(segmentsOfStreets[j][i]).to);
        }
        intersectionsOfStreets[j] = remove_dups_in_vecs(intersectionsOfStreets[j]);
    }
    
}

//Can delete, I don't think we need this anymore cuz we're not using any maps
/*
bool valueExistsInMultiMap(std::multimap<int,int> map, int key, int ID){
    
    if (map.find(key) != map.end()){
        auto  range = map.equal_range(key);
        for(StreetsIt it = range.first ; it != range.second ; ++it){
            if(it->second == ID){
                return true;
            }
        }
    }
    return false;
}*/



/*============================== MILESTONE 1 ==============================*/

bool load_map(std::string map_path) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully
    std::string OSMpath = "";
    bool pathInvalid = false;
    int mapPathLength = map_path.size();
    for(int i=0; i< mapPathLength; i++){
        //collect everything before .street.bin sequence because we want to replace it with .osm.bin
        // this condition avoids accessing map_path string beyond its largest index(otherwise segfault))
        if(i+6 < mapPathLength){
            if(map_path[i]=='.'&&map_path[i+1]=='s'&&map_path[i+2]=='t'&&map_path[i+3]=='r'&&map_path[i+4]=='e'&&map_path[i+5]=='e'&&map_path[i+6]=='t'&&map_path[i+7]=='s'){
                break;
            }
            else{OSMpath += map_path[i];} 
        }
        // this means .street suquence is not found in the given map_path
        // wrong map_math
        else if(i+6 >= mapPathLength){ pathInvalid = true;}
    }
    
    if(!pathInvalid){
        OSMpath += ".osm.bin";
        loadOSMDatabaseBIN(OSMpath); 
    }  
    
    // added some functions -p
    //Load StreetsDatabaseBIN and OSMDatabaseBIN
    //change to OR doesn't work if AND -m
    load_successful = ( loadStreetsDatabaseBIN(map_path)); 
    

    //make sure load map succeeds before creating structures that depend on the API
    if (load_successful){
        //makeIntersectionTable();
        makeCapitalizedStreetNamesTable();
        makeSegmentsOfStreets();
        makeIntersectionsOfStreets();
        //makeSegmentsOfIntersections();
       // makeStreetNamesOfIntersections();
        //makeAdjacentIntersections();
        makeTableOfDivisors();
    }
    return load_successful;
}

void close_map() {
    //Close StreetsDatabaseBIN and OSMDatabaseBIN
    closeStreetDatabase();
    closeOSMDatabase();
    
    //Makes sure to close the structures
    //intersectionTable.clear();
    capitalizedStreetNamesTable.clear();
    segmentsOfStreets.clear();
    intersectionsOfStreets.clear();
   // segmentsOfIntersections.clear();
    //streetNamesOfIntersections.clear();
   // adjacentIntersections.clear();
}
//passes -p
//Returns the distance between two coordinates in meters
double find_distance_between_two_points(std::pair<LatLon, LatLon> points){
    // see page 14 of milestone 1; (x,y)=(lon*cos(latavg), lat) -p
    // note lat() and lon() are degrees, cosine uses RADIANS -p
    double latavg = ((points.first.lat() + points.second.lat()) * DEGREE_TO_RADIAN)/2;
    // note to self: latitude is up/down, longitude is left/right -p
    double x1 = points.first.lon() * DEGREE_TO_RADIAN * cos(latavg);
    double y1 = points.first.lat() * DEGREE_TO_RADIAN;
    // note to self: radians * radius = arc length -p
    double x2 = points.second.lon() * DEGREE_TO_RADIAN * cos(latavg);
    double y2 = points.second.lat() * DEGREE_TO_RADIAN;
    
    return EARTH_RADIUS_METERS * sqrt(pow((x2-x1),2) + pow((y2-y1),2));
}

//anyway to make a helper function for this?? -M
//MADE BY PRISCILLA -M /// status: done, needs debugging -p
//I WAS DEBUGGING. THE INPUT TO FIND_DISTANCE_BETWEEN_TWO_POINTS IS A PAIR SO I MADE IT A PAIR -M
//Returns the length of the given street segment in meters
double find_street_segment_length(int street_segment_id){
    // pull info from API library
    InfoStreetSegment street = getInfoStreetSegment(street_segment_id);
    // initialize total length to 0
    double length = 0;

    // If street has no curves, e.g. street is completely straight
    if (street.curvePointCount == 0){
        // calculate distance from start to end
        std::pair<LatLon,LatLon> straight_segment (getIntersectionPosition(street.from), getIntersectionPosition(street.to));    
        length += find_distance_between_two_points(straight_segment);        
    } 
    // If street has curves, use some thicc maths to fix
    else {
        for (int i=0; i<=street.curvePointCount; i++){
            // note: if street has 1 corner, add two segments, if 2 corners, add three segments, etc -p
            // assume all curve points are corners? -p
            std::pair<LatLon,LatLon> curved_segment;
            if (i==0){ // calculate distance from beginning of street to first curve
                curved_segment.first = getIntersectionPosition(street.from); 
                curved_segment.second = getStreetSegmentCurvePoint(i, street_segment_id);
            }
            else if (i==street.curvePointCount){ // calculate distance from last curve to end of street
                curved_segment.first = getStreetSegmentCurvePoint(i-1, street_segment_id);
                curved_segment.second = getIntersectionPosition(street.to);
            } else { // calculate distance between each curve (or realistically, each corner)
                curved_segment.first = getStreetSegmentCurvePoint(i-1, street_segment_id);
                curved_segment.second = getStreetSegmentCurvePoint(i, street_segment_id);       
            }
            length += find_distance_between_two_points(curved_segment);
        }
    }
    return length;
     
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
//J
// convert form kmh -> m/s
// debugging: off by 3.6 so i just multiplied it lol -p
// need to write function without '/' operator -p
double find_street_segment_travel_time(int street_segment_id){
    //return find_street_segment_length(street_segment_id) * 3.6 * tableOfDivisors[street_segment_id];
    return tableOfDivisors[street_segment_id].first * 3.6 * tableOfDivisors[street_segment_id].second;
}

//I THINK PRISCILLA SHOULD DO THIS ONE CUZ YOU NEED HER FUNCTIONS FOR THIS -M
//Returns the nearest intersection to the given position

//finished, fixed but may violate performance
int find_closest_intersection(LatLon my_position){
    // distance calculated from each intersection in the city -p
    double min_distance = 999999;
    int closest;
    //for (std::vector<LatLon>::iterator it = intersectionTable.begin(); it != intersectionTable.end(); it++){
    for (int i=0; i<getNumIntersections();i++){
        LatLon current = getIntersectionPosition(i);
        std::pair<LatLon, LatLon> temp (my_position, current);
        //std::pair<LatLon, LatLon> temp (my_position, *it);
        double distance = find_distance_between_two_points(temp);
        if (distance < min_distance){
            min_distance = distance;
            closest = i;
            //closest = (it - intersectionTable.begin());
        }
    }
    return closest;
}

//Created new version (not sure if faster) -M 
//Returns the street segments for the given intersection 
std::vector<int> find_street_segments_of_intersection(int intersection_id){
    
    std::vector<int>street_segments_of_intersection;
    int num_intersections = getIntersectionStreetSegmentCount(intersection_id);
        
        //going through the segment index attached to the intersection 
        for(int i=0; i < num_intersections; ++i){
            street_segments_of_intersection.push_back(getIntersectionStreetSegment(intersection_id, i));
        }
    return street_segments_of_intersection;
    //return segmentsOfIntersections[intersection_id];
}

//CHECK IF U CAN USE ONE OF THE GLOBAL FUNCTIONS... MAP<STREETNAME, ID>
//I THINK THIS ALSO PASSES -M
//DEPENDENT ON find_street_segments_of_intersection WORKING -M
//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_street_names_of_intersection(int intersection_id){
    
    std::vector<std::string> street_names_of_intersection;
    for (int i = 0; i < find_street_segments_of_intersection(intersection_id).size(); ++i) {
        int streetID = getInfoStreetSegment(find_street_segments_of_intersection(intersection_id)[i]).streetID;
        street_names_of_intersection.push_back(getStreetName(streetID));
    }
    
    return street_names_of_intersection;
}

//FINISHED -M
//Returns true if you can get from intersection_ids.first to intersection_ids.second using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(std::pair<int, int> intersection_ids){
   
    //edited so that it access the segOfInter instead of going to the O(1) function
    std::vector<int> intersectionSegmentsOne = find_street_segments_of_intersection(intersection_ids.first);
    
    if(intersection_ids.first == intersection_ids.second)return true;
    
    for(int i=0 ; i < intersectionSegmentsOne.size() ; ++i){
        
        int streetSegmentTo = getInfoStreetSegment(intersectionSegmentsOne[i]).to; 
        int streetSegmentFrom = getInfoStreetSegment(intersectionSegmentsOne[i]).from;
        
        //Is not accessible on oneWay streets if segment doesnt go from intersection1 to intersection2..
        if (getInfoStreetSegment(intersectionSegmentsOne[i]).oneWay){
            if(streetSegmentTo == intersection_ids.second){
                return true;
            }
        }else{
            if(streetSegmentTo == intersection_ids.second || streetSegmentFrom == intersection_ids.second){
                return true;
            }
        }
    }
    return false;
    
/* I think I made it go through less if statements but I can't test it cuz code doesnt build -M*/
//    //checks if its connected to itself ... does not check if it's connected to itself via segment
//    if(intersection_ids.first == intersection_ids.second)return true;
//    
//    //prevents from copying vectors
//    for(int i=0 ; i < segmentsOfIntersections[intersection_ids.first].size() ; ++i){
//        
//        int streetSegmentTo = getInfoStreetSegment(segmentsOfIntersections[intersection_ids.first][i]).to; 
//        int streetSegmentFrom = getInfoStreetSegment(segmentsOfIntersections[intersection_ids.first][i]).from;
//        
//        //Can take out checking for oneWay if we already have that OR statement which checks for it anyway
//        //the statement to check for one Way come first.
//            
//        //alternatively can check if streetSegmentFrom == intersection_ids.first
//        if(streetSegmentTo == intersection_ids.second || streetSegmentFrom == intersection_ids.second){
//            return true;
//        }
//        
//    }
//    return false;
}


//FINISHED-M
//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<int> find_adjacent_intersections(int intersection_id){
    
    
    std::vector<int> adjacentIntersections;
    //std::vector<int> intersectionSegments = find_street_segments_of_intersection(intersection_id);
    std::pair<int,int> twoIntersections;
    twoIntersections.first = intersection_id;
    std::vector<int>::iterator checkForFind;
    
    
    for (int i = 0; i < getIntersectionStreetSegmentCount(intersection_id) ; ++i){
        //checks if two intersections found by both side of the segments are directly connected and therefore would be adjacent (used are_directly_connected function)
        //checks if the 'from' intersection is the intersection_id
        twoIntersections.second = getInfoStreetSegment(getIntersectionStreetSegment(intersection_id,i)).to;
        if(twoIntersections.first != twoIntersections.second && are_directly_connected(twoIntersections)){
            checkForFind = std::find(adjacentIntersections.begin(), adjacentIntersections.end(), twoIntersections.second);
                if(checkForFind == adjacentIntersections.end()) //if intersection is not found
            
                adjacentIntersections.push_back(twoIntersections.second);
        }
            twoIntersections.second = getInfoStreetSegment(getIntersectionStreetSegment(intersection_id,i)).from;
            if(twoIntersections.first != twoIntersections.second && are_directly_connected(twoIntersections)){
                checkForFind = std::find(adjacentIntersections.begin(), adjacentIntersections.end(), twoIntersections.second);
                if(checkForFind == adjacentIntersections.end())
                    adjacentIntersections.push_back(twoIntersections.second);
            }
    }
    return adjacentIntersections;
    //return adjacentIntersections[intersection_id];
}

//tested for a few cases. it works I think -M
std::vector<int> find_street_segments_of_street(int street_id){
    return segmentsOfStreets[street_id];
}

//OPTIMIZING THIS FUNCTION -M
//Returns all intersections along the a given street
//j
std::vector<int> find_intersections_of_street(int street_id){
    
//   std::vector<int> segments_of_street = find_street_segments_of_street(street_id);
//    std::vector<int> intersections_we_want;
//    for(int i=0;i<segments_of_street.size();i++){
//        intersections_we_want.push_back(getInfoStreetSegment(segments_of_street[i]).from);
//        intersections_we_want.push_back(getInfoStreetSegment(segments_of_street[i]).to);
//    }
//    
//    //intersection contains duplicate elements
//    //remove dupes and return
//    return remove_dups_in_vecs(intersections_we_want);
    
    return intersectionsOfStreets[street_id];
}

//TRYING TO OPTOMIZE TIME -M
//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){
    //initialize vector to be returned
    
    //would it make a difference if we didn't copy it? speed wise? -M
    std::vector<int> intersections_first = find_intersections_of_street(street_ids.first);
    std::vector<int> intersections_second = find_intersections_of_street(street_ids.second);
    
    //sizes the commonIntersection to make sure to fit all intersections between streets
    std::vector<int> commonIntersection (intersections_first.size() + intersections_second.size());
    
    VectorIt it;
    //Makes sure it's sorted..only way to go through the set_intersection
    std::sort(intersections_first.begin(), intersections_first.end());
    std::sort(intersections_second.begin(), intersections_second.end());
    
    it = std::set_intersection(intersections_first.begin(), intersections_first.end(),intersections_second.begin(), intersections_second.end(),
            commonIntersection.begin());
    
    //resize to delete un initialized variables
    commonIntersection.resize(it-commonIntersection.begin());
    
    return commonIntersection;
}

//Returns all street ids corresponding to street names that start with the given prefix
//The function should be case-insensitive to the street prefix. You should ignore spaces.
//For example, both "bloor " and "BloOrst" are prefixes to "Bloor Street East".
//If no street names match the given prefix, this routine returns an empty (length 0) 
//vector.
//You can choose what to return if the street prefix passed in is an empty (length 0) 
//string, but your program must not crash if street_prefix is a length 0 string.

// done, passes performance - priscilla
std::vector<int> find_street_ids_from_partial_street_name(std::string street_prefix){
    //I removed Justin's code -M...will have to checkout dfc3 to see again
    
    //remove all white spaces
    street_prefix.erase(remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), ::toupper); // change all to capital
    if (street_prefix==""){ // return null if blank
        return {NULL};
    }

    std::vector<int> street_ids;
    
    std::multimap<std::string, StreetIndex>::iterator it;
    // determine first instance of street_prefix found in map
    std::multimap<std::string, StreetIndex>::iterator lower = capitalizedStreetNamesTable.lower_bound(street_prefix);
    // determine last instance by incrementing first character in ASCII - e.g. search "MAR", upper bound will be "N"
    std::multimap<std::string, StreetIndex>::iterator upper = capitalizedStreetNamesTable.upper_bound(std::string(1, char((int(street_prefix[0])+1))));

    for (it=lower; it!=upper; it++){
    //for (it=capitalizedStreetNamesTable.begin();it!=capitalizedStreetNamesTable.end();it++){
        for (int i=0; i<((*it).first).length(); i++){
            if (street_prefix[i]!=((*it).first)[i]){
                break;
            }
            else if ((street_prefix[i]==((*it).first)[i])&&(i == street_prefix.length()-1)){
                street_ids.push_back((*it).second);
            }
        } 
    }

    return street_ids;

}
   
//Returns the area of the given closed feature in square meters
//Assume a non self-intersecting polygon (i.e. no holes)
//Return 0 if this feature is not a closed polygon.
double find_feature_area(int feature_id){
    
    if((getFeaturePoint(0, feature_id).lat()==getFeaturePoint(getFeaturePointCount(feature_id)-1, feature_id).lat()) &&
      ((getFeaturePoint(0, feature_id).lon()==getFeaturePoint(getFeaturePointCount(feature_id)-1, feature_id).lon()))){ // == is not defined in LatLon class
        double area=0;
        int featurePointCount=getFeaturePointCount(feature_id);
        std::vector<double> x;
        std::vector<double> y;
        x.resize(featurePointCount);
        y.resize(featurePointCount);
        LatLon origin = getFeaturePoint(0, feature_id);
        
        //initialize the first and last elements of x and y vector to 0 
        //this makes the polygon start at the origin and end at the origin
        x[0]=0;         x[featurePointCount-1]=0;
        y[0]=0;         y[featurePointCount-1]=0;
        
        // fill in the vector with x-y coordinates with respect to the origin in meters
        for(int i=1; i< featurePointCount-1 ;i++){
            LatLon point = getFeaturePoint(i, feature_id);
            x[i] = x_distance_between_2_points(origin, point);
            y[i] = y_distance_between_2_points(origin, point);
        }
        
        //apply shoelace formula
        for(int i=0; i <getFeaturePointCount(feature_id) ; i++ ){
            if(i+1< getFeaturePointCount(feature_id)){
                area += (x[i]*y[i+1])-(y[i]*x[i+1]);
            }
        }
        return abs(area/2);
        
    }else{return 0;}
    
    return 0;
}    

    
//Returns the length of the OSMWay that has the given OSMID, in meters.
//To implement this function you will have to  access the OSMDatabaseAPI.h 
//functions.
double find_way_length(OSMID way_id){
    
    //J's edits   
    //find OMSWay* 
    const OSMWay* input_way_p = nullptr ;    
    int numberOfWays = getNumberOfWays();
    for(int i=0; i < numberOfWays; i++){
        if(getWayByIndex(i)->id() == way_id){
            input_way_p = getWayByIndex(i);
            break;
        }
    }
    //use OSMWay* to find all the nodes the way contains
    if(input_way_p== nullptr){return 0;}//failed to find the way
    else{
        double length=0;
        const std::vector<OSMID> wayMembers = getWayMembers(input_way_p);
        //find latlons of nodes using the OSMIDs from wayMembers
        std::vector<LatLon> latlon_of_nodes;
        for(int i=0;i<wayMembers.size();i++){
            for(int j=0; j<getNumberOfNodes();j++){
                if(getNodeByIndex(j)->id()==wayMembers[i]){
                    latlon_of_nodes.push_back(getNodeCoords(getNodeByIndex(j)));
                    break;
                }
            }
        }
        //use latlons from above to calculate cumulative distance of way
        
        for(int i=0; i < latlon_of_nodes.size()-1 ;i++){
            length += find_distance_between_two_points(std::make_pair(latlon_of_nodes[i], latlon_of_nodes[i+1]));
        }
        
        return length;
    }
    return 0;
}
