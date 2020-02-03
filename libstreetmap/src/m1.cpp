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
//#include "OSMDatabaseAPI.h" //I don't know if we need to add this

/*==================== GLOBAL VARIABLES DECLARATIONS ====================*/
std::vector<LatLon> intersectionTable;
std::vector<std::string> capitalizedStreetNamesTable;
std::vector<std::vector<int>> segmentsOfStreets;
std::vector<std::vector<int>> intersectionsOfStreets;
std::vector<std::vector<int>> segmentsOfIntersections;
std::vector<std::vector<std::string>> streetNamesOfIntersections;
std::vector<std::vector<int>> adjacentIntersections;
typedef std::multimap<int, int>::iterator StreetsIt;
typedef std::vector<int>::iterator VectorIt;

/*==================== GLOBAL FUNCTIONS IMPLEMENTATIONS ====================*/

//ARE THESE EVEN NEEDED?
void makeIntersectionTable();
void makeCapitalizedStreetNamesTable();
void makeSegmentsOfStreets();
void makeSegmentsOfStreetsMap();
void makeIntersectionsOfStreets();
void makeSegmentsOfIntersections();
void makeStreetNamesOfIntersections();
void makeAdjacentIntersections();
double x_distance_between_2_points(LatLon first, LatLon second);
double y_distance_between_2_points(LatLon first, LatLon second);
std::vector<int> remove_dups_in_vecs(std::vector<int> vectorA);
bool element_exists(int element, std::vector<int> vectorA);
bool valueExistsInMultiMap(std::multimap<int,int> map, int key, int ID);
/*==================== GLOBAL FUNCTION IMPLEMENTATION ====================*/
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
// allocates vector of intersection ids
void makeIntersectionTable(){
    for (IntersectionIndex id=0; id<getNumIntersections(); id++){
        intersectionTable.push_back(getIntersectionPosition(id));
    }
}
// creates a table of capitalized street names without spaces in order of streetindex
void makeCapitalizedStreetNamesTable(){
    std::string capitalizedName;
    for (StreetIndex id=0; id<getNumStreets();id++){
        capitalizedName = getStreetName(id);
        capitalizedName.erase(remove(capitalizedName.begin(), capitalizedName.end(), ' '), capitalizedName.end());
        std::transform(capitalizedName.begin(), capitalizedName.end(), capitalizedName.begin(), ::toupper);
        
        capitalizedStreetNamesTable.push_back(capitalizedName);
    }
}

void makeSegmentsOfStreets(){    

    int street_ID;
    segmentsOfStreets.resize(getNumStreets());
    for(int j =0 ; j< getNumStreetSegments() ; ++j){
        street_ID = getInfoStreetSegment(j).streetID;
        segmentsOfStreets[street_ID].push_back(j);
    }
    
}


void makeSegmentsOfIntersections(){  
    std::vector<int> street_segments_of_intersection;
    segmentsOfIntersections.resize(getNumIntersections());
    for(int j =0 ; j< getNumIntersections() ; ++j){
        
        street_segments_of_intersection.clear();
        int num_intersections = getIntersectionStreetSegmentCount(j);
        
        //going through the segment index attached to the intersection 
        for(int i=0; i < num_intersections; ++i){
            street_segments_of_intersection.push_back(getIntersectionStreetSegment(j, i));
        }
        segmentsOfIntersections[j] = street_segments_of_intersection;
    }
}

void makeStreetNamesOfIntersections(){
    std::vector<std::string> street_names_of_intersection;
    streetNamesOfIntersections.resize(getNumIntersections());
    
    for(int j =0 ; j< getNumIntersections() ; ++j){
        street_names_of_intersection.clear();
        
        //going through the segment index attached to the intersection 
        for(int i=0; i < segmentsOfIntersections[j].size() ; ++i){
            int streetID = getInfoStreetSegment(segmentsOfIntersections[j][i]).streetID;
            street_names_of_intersection.push_back(getStreetName(streetID));
        }
        streetNamesOfIntersections[j] = street_names_of_intersection;
    }
}

//NEEDS COMMENTING
void makeAdjacentIntersections(){
    
    std::vector<int> adjacent_intersections;
    std::pair<int,int> twoIntersections;
   
    std::vector<int>::iterator checkForFind;
    
    adjacentIntersections.resize(getNumIntersections());
    
    for(int j =0 ; j< getNumIntersections() ; ++j){
        adjacent_intersections.clear();
        
        twoIntersections.first = j;
        for (int i = 0; i < getIntersectionStreetSegmentCount(j) ; ++i){ //can change to .size but won't let me for some reason

            //checks if two intersections found by both side of the segments are directly connected and therefore would be adjacent (used are_directly_connected function)
            //checks if the 'from' intersection is the intersection_id
            twoIntersections.second = getInfoStreetSegment(segmentsOfIntersections[j][i]).to;
            if(twoIntersections.first != twoIntersections.second && are_directly_connected(twoIntersections)){
                checkForFind = std::find(adjacent_intersections.begin(), adjacent_intersections.end(), twoIntersections.second);
                    if(checkForFind == adjacent_intersections.end()) //if intersection is not found

                    adjacent_intersections.push_back(twoIntersections.second);
            }
                twoIntersections.second = getInfoStreetSegment(segmentsOfIntersections[j][i]).from;
                if(twoIntersections.first != twoIntersections.second && are_directly_connected(twoIntersections)){
                    checkForFind = std::find(adjacent_intersections.begin(), adjacent_intersections.end(), twoIntersections.second);
                    if(checkForFind == adjacent_intersections.end())
                        adjacent_intersections.push_back(twoIntersections.second);
                }
        }
        adjacentIntersections[j] = adjacent_intersections;
    }
}


//Make sure it's implemented in load map after makeSegmentsOfStreets cuz its dependant on it
/* NOT SURE IF THIS WORKS CUZ NEVER TEST AND NEVER USED
 * 
 * Makes intersectionsOfStreets multimap
 * Finds the intersections (value) related to each street (key)
 * Does not check for duplicates
 * 
 * _________________________________________
 * 
 * 
 * Try using std::find(<pointer to start>, <pointer to end>, <what you're lookingfor>)
 * 
 */

void makeIntersectionsOfStreets(){
    
   std::vector<int> intersections_of_street;
   int interTo, interFrom;
   intersectionsOfStreets.resize(getNumStreets());
    
    for(int j =0 ; j< getNumStreets() ; ++j){
        intersections_of_street.clear();
        
        //going through the segment index attached to the intersection 
        for(int i=0; i < segmentsOfStreets[j].size() ; ++i){
            interTo = getInfoStreetSegment(segmentsOfStreets[j][i]).to;
            interFrom = getInfoStreetSegment(segmentsOfStreets[j][i]).from;
            
            intersections_of_street.push_back(interTo);
            intersections_of_street.push_back(interFrom);
        }
        intersectionsOfStreets[j] = remove_dups_in_vecs(intersections_of_street);
    }
}

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
}



/*============================== MILESTONE 1 ==============================*/

bool load_map(std::string map_path) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully

    // added some functions -p
    
    //Load StreetsDatabaseBIN and OSMDatabaseBIN
    //change to OR doesn't work if AND -m
    load_successful = ( loadStreetsDatabaseBIN(map_path)); 
    //loadOSMDatabaseBIN(map_path); 
    
    //make sure load map succeeds before creating structures that depend on the API
    if (load_successful){
        makeIntersectionTable();
        makeCapitalizedStreetNamesTable();
        makeSegmentsOfStreets();
        //makeSegmentsOfStreetsMap();
        makeIntersectionsOfStreets();
        makeSegmentsOfIntersections();
        makeStreetNamesOfIntersections();
        makeAdjacentIntersections();
    }
    return load_successful;
}

void close_map() {
    //Close StreetsDatabaseBIN and OSMDatabaseBIN
    closeStreetDatabase();
    //closeOSMDatabase();
    
    //Makes sure to close the structures
    intersectionTable.clear();
    capitalizedStreetNamesTable.clear();
    segmentsOfStreets.clear();

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

//MADE BY PRISCILLA -M /// status: done, needs debugging -p
//I WAS DEBUGGING. THE INPUT TO FIND_DISTANCE_BETWEEN_TWO_POINTS IS A PAIR SO I MADE IT A PAIR -M
//Returns the length of the given street segment in meters
double find_street_segment_length(int street_segment_id){
 /*
    InfoStreetSegment segInfo = getInfoStreetSegment(street_segment_id);
    double length = 0;
    if(segInfo.curvePointCount==0){
        LatLon from_position = getIntersectionPosition(segInfo.from);
        LatLon to_position = getIntersectionPosition(segInfo.to);
        std::pair<LatLon, LatLon> from_to_to = std::make_pair(from_position, to_position);
        length = find_distance_between_two_points(from_to_to);
    }
    else{
        length += find_distance_between_two_points(std::make_pair(getIntersectionPosition(segInfo.from), getStreetSegmentCurvePoint(1,street_segment_id)));
        for(int i=1; i<segInfo.curvePointCount;i++){
            length+= find_distance_between_two_points( std::make_pair(getStreetSegmentCurvePoint(i,street_segment_id), getStreetSegmentCurvePoint(i+1, street_segment_id)));
        }
        length += find_distance_between_two_points(std::make_pair(getStreetSegmentCurvePoint(segInfo.curvePointCount, street_segment_id), getIntersectionPosition(segInfo.to)));
 
    }
    return length;
    */
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
    // should be done, needs debugging -p
    return length;
     
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
//J
// convert form kmh -> m/s
// debugging: off by 3.6 so i just multiplied it lol -p
double find_street_segment_travel_time(int street_segment_id){
    double travel_time;
    travel_time= (find_street_segment_length(street_segment_id))*3.6 / (getInfoStreetSegment(street_segment_id).speedLimit);
    return travel_time;
}

//I THINK PRISCILLA SHOULD DO THIS ONE CUZ YOU NEED HER FUNCTIONS FOR THIS -M
//Returns the nearest intersection to the given position

//finished, fixed but may violate performance
int find_closest_intersection(LatLon my_position){
    // distance calculated from each intersection in the city -p
    double min_distance = 999999;
    int closest=0;
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
    return segmentsOfIntersections[intersection_id];
}

//CHECK IF U CAN USE ONE OF THE GLOBAL FUNCTIONS... MAP<STREETNAME, ID>
//I THINK THIS ALSO PASSES -M
//DEPENDENT ON find_street_segments_of_intersection WORKING -M
//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_street_names_of_intersection(int intersection_id){
    
    /*
    std::vector<int> street_segments_of_intersection = find_street_segments_of_intersection(intersection_id);
    std::vector<std::string> intersection_street_names;
    
    std::string streetName; //street name from a given streetID
    int streetID; //street ID from a given segment
    int i;
    
    for (i=0; i< street_segments_of_intersection.size(); ++i){
       
        streetID = getInfoStreetSegment(street_segments_of_intersection[i]).streetID;
        streetName = getStreetName(streetID);
        intersection_street_names.push_back(streetName);
    }
    return intersection_street_names;

     */
    return streetNamesOfIntersections[intersection_id];
}

//FINISHED -M
//Returns true if you can get from intersection_ids.first to intersection_ids.second using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(std::pair<int, int> intersection_ids){
   
    std::vector<int> intersectionSegmentsOne = find_street_segments_of_intersection(intersection_ids.first);
    
    if(intersection_ids.first == intersection_ids.second)return true;
    
    for(int i=0 ; i < intersectionSegmentsOne.size() ; ++i){
        
        int streetSegmentTo = getInfoStreetSegment(intersectionSegmentsOne[i]).to; 
        int streetSegmentFrom = getInfoStreetSegment(intersectionSegmentsOne[i]).from;
        
        if (getInfoStreetSegment(intersectionSegmentsOne[i]).oneWay){
            if(streetSegmentTo == intersection_ids.second){ //Is not accessible on oneWay streets if segment doesnt go from intersection1 to intersection2.. || streetSegmentFrom == intersection_ids.first
                return true;
            }
        }else{
            if(streetSegmentTo == intersection_ids.second || streetSegmentFrom == intersection_ids.second){
                return true;
            }
        }
    }
    return false;
}


//FINISHED-M
//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<int> find_adjacent_intersections(int intersection_id){
    return adjacentIntersections[intersection_id];
}

/* //CAN BE DELETED, CREATED FUNCTION THAT'LL TAKE LESS TIME WITH A GLOBAL MULTIMAP -M
//Returns all street segments for the given street
//j
std::vector<int> find_street_segments_of_street(int street_id){
    std::vector<int> street_segment_ids;
    //find number of street segments 
    //then check all street segments' street id
    //if same as input insert it into return_me vector
    int num_street_seg = getNumStreetSegments();
    
    for(int i=0;i<num_street_seg;i++){
        if(getInfoStreetSegment(i).streetID==street_id){
            street_segment_ids.push_back(i);
        }
    }
    return street_segment_ids;
}
*/

//tested for a few cases. it works I think -M
std::vector<int> find_street_segments_of_street(int street_id){
    return segmentsOfStreets[street_id];
}

//OPTIMIZING THIS FUNCTION -M
//Returns all intersections along the a given street
//j
/* this function might be slow 
 * might need a more efficient algorithm
 */
std::vector<int> find_intersections_of_street(int street_id){
//    std::vector<int> segments_of_street = segmentsOfStreets[street_id];
//    std::vector<int> intersections_we_want;
//    for(int i=0;i<segments_of_street.size();i++){
//        intersections_we_want.push_back(getInfoStreetSegment(segments_of_street[i]).from);
//        intersections_we_want.push_back(getInfoStreetSegment(segments_of_street[i]).to);
//    }
//    
    //intersection contains duplicate elements
    //remove dupes and return
    return intersectionsOfStreets[street_id];
}

//TRYING TO OPTOMIZE TIME -M
//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//this passes i think -p
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){
    //initialize vector to be returned
    
    std::vector<int> intersections_first = intersectionsOfStreets[street_ids.first];
    std::vector<int> intersections_second = intersectionsOfStreets[street_ids.second];
    
    std::vector<int> commonIntersection (intersections_first.size() + intersections_second.size());
    
    VectorIt it;
    std::sort(intersections_first.begin(), intersections_first.end());
    std::sort(intersections_second.begin(), intersections_second.end());
    
    it = std::set_intersection(intersections_first.begin(), intersections_first.end(),intersections_second.begin(), intersections_second.end(),
            commonIntersection.begin());
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

// should be done...idk if it works - priscilla
std::vector<int> find_street_ids_from_partial_street_name(std::string street_prefix){
    /*
    std::vector<int> matching_street_ids;
    int inputLength = street_prefix.length();
    
    if(inputLength==0){
        //do random stuff
        matching_street_ids = {0};
        return matching_street_ids;
    }
    else{
        for(int i=0; i< getNumStreets(); i++){
            bool match=false;
            std::string ith_streetName=getStreetName(i);
            int streetName_plus=0;
            for (int j=0; j< inputLength; j++){
                
                if(ith_streetName[j+streetName_plus]==' ' && street_prefix[j] !=' '){
                    streetName_plus++;
                }
                
                if(     (ith_streetName[j+streetName_plus]== street_prefix[j]) || // checks for punctuations or spaces
                   //makes comparison case insensitive
                        (ith_streetName[j+streetName_plus]=='a'&& street_prefix[j]=='A')||(ith_streetName[j+streetName_plus]=='A'&&street_prefix[j]=='a') ||
                        (ith_streetName[j+streetName_plus]=='b'&& street_prefix[j]=='B')||(ith_streetName[j+streetName_plus]=='B'&&street_prefix[j]=='b') ||
                        (ith_streetName[j+streetName_plus]=='c'&& street_prefix[j]=='C')||(ith_streetName[j+streetName_plus]=='C'&&street_prefix[j]=='c') ||
                        (ith_streetName[j+streetName_plus]=='d'&& street_prefix[j]=='D')||(ith_streetName[j+streetName_plus]=='D'&&street_prefix[j]=='d') ||
                        (ith_streetName[j+streetName_plus]=='e'&& street_prefix[j]=='E')||(ith_streetName[j+streetName_plus]=='E'&&street_prefix[j]=='e') ||
                        (ith_streetName[j+streetName_plus]=='f'&& street_prefix[j]=='F')||(ith_streetName[j+streetName_plus]=='F'&&street_prefix[j]=='f') ||
                        (ith_streetName[j+streetName_plus]=='g'&& street_prefix[j]=='G')||(ith_streetName[j+streetName_plus]=='G'&&street_prefix[j]=='g') ||
                        (ith_streetName[j+streetName_plus]=='h'&& street_prefix[j]=='H')||(ith_streetName[j+streetName_plus]=='H'&&street_prefix[j]=='h') ||
                        (ith_streetName[j+streetName_plus]=='i'&& street_prefix[j]=='I')||(ith_streetName[j+streetName_plus]=='I'&&street_prefix[j]=='i') ||
                        (ith_streetName[j+streetName_plus]=='j'&& street_prefix[j]=='J')||(ith_streetName[j+streetName_plus]=='J'&&street_prefix[j]=='j') ||
                        (ith_streetName[j+streetName_plus]=='k'&& street_prefix[j]=='K')||(ith_streetName[j+streetName_plus]=='K'&&street_prefix[j]=='j') ||
                        (ith_streetName[j+streetName_plus]=='l'&& street_prefix[j]=='L')||(ith_streetName[j+streetName_plus]=='L'&&street_prefix[j]=='l') ||
                        (ith_streetName[j+streetName_plus]=='m'&& street_prefix[j]=='M')||(ith_streetName[j+streetName_plus]=='M'&&street_prefix[j]=='m') ||
                        (ith_streetName[j+streetName_plus]=='n'&& street_prefix[j]=='N')||(ith_streetName[j+streetName_plus]=='N'&&street_prefix[j]=='n') ||
                        (ith_streetName[j+streetName_plus]=='o'&& street_prefix[j]=='O')||(ith_streetName[j+streetName_plus]=='O'&&street_prefix[j]=='o') ||
                        (ith_streetName[j+streetName_plus]=='p'&& street_prefix[j]=='P')||(ith_streetName[j+streetName_plus]=='P'&&street_prefix[j]=='p') ||
                        (ith_streetName[j+streetName_plus]=='q'&& street_prefix[j]=='Q')||(ith_streetName[j+streetName_plus]=='Q'&&street_prefix[j]=='q') ||
                        (ith_streetName[j+streetName_plus]=='r'&& street_prefix[j]=='R')||(ith_streetName[j+streetName_plus]=='R'&&street_prefix[j]=='r') ||
                        (ith_streetName[j+streetName_plus]=='s'&& street_prefix[j]=='S')||(ith_streetName[j+streetName_plus]=='S'&&street_prefix[j]=='s') ||
                        (ith_streetName[j+streetName_plus]=='t'&& street_prefix[j]=='T')||(ith_streetName[j+streetName_plus]=='T'&&street_prefix[j]=='t') ||
                        (ith_streetName[j+streetName_plus]=='u'&& street_prefix[j]=='U')||(ith_streetName[j+streetName_plus]=='U'&&street_prefix[j]=='u') ||
                        (ith_streetName[j+streetName_plus]=='v'&& street_prefix[j]=='V')||(ith_streetName[j+streetName_plus]=='V'&&street_prefix[j]=='v') ||
                        (ith_streetName[j+streetName_plus]=='w'&& street_prefix[j]=='W')||(ith_streetName[j+streetName_plus]=='W'&&street_prefix[j]=='w') ||
                        (ith_streetName[j+streetName_plus]=='x'&& street_prefix[j]=='X')||(ith_streetName[j+streetName_plus]=='X'&&street_prefix[j]=='x') ||
                        (ith_streetName[j+streetName_plus]=='y'&& street_prefix[j]=='Y')||(ith_streetName[j+streetName_plus]=='Y'&&street_prefix[j]=='y') ||
                        (ith_streetName[j+streetName_plus]=='z'&& street_prefix[j]=='Z')||(ith_streetName[j+streetName_plus]=='Z'&&street_prefix[j]=='z')
                ){}//do nothing
                else{break;}
                if(j==inputLength-1){match=true;}
            }
            if(match){matching_street_ids.push_back(i);}
        }
    }
    return matching_street_ids; 
}
 */
    
    //remove all white spaces
    street_prefix.erase(remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), ::toupper); // change all to capital
    if (street_prefix==""){ // return null if blank
        return {NULL};
    }

    std::vector<int> street_ids;
    // O(n^2) squad -p  
    std::vector<std::string>::iterator it;
    std::vector<std::string>::iterator lower = std::lower_bound(capitalizedStreetNamesTable.begin(),capitalizedStreetNamesTable.end(),street_prefix);
    std::vector<std::string>::iterator upper = std::upper_bound(capitalizedStreetNamesTable.begin(),capitalizedStreetNamesTable.end(),street_prefix);

    for (it=lower; it!=upper; it++){
    //for (it=capitalizedStreetNamesTable.begin();it!=capitalizedStreetNamesTable.end();it++){
        for (int i=0; i<(*it).length(); i++){
            if (street_prefix[i]!=(*it)[i]){
                break;
            }
            else if ((street_prefix[i]==(*it)[i])&&(i == street_prefix.length()-1)){
                street_ids.push_back(it-capitalizedStreetNamesTable.begin());
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
        x[0]=0;     
        x[featurePointCount-1]=0;
        y[0]=0;     
        y[featurePointCount-1]=0;
        
        // fill in the vector with x-y coordinates with respect to the origin in meters
        for(int i=1; i< featurePointCount-1 ;i++){
            LatLon point = getFeaturePoint(i, feature_id);
            x[i] = x_distance_between_2_points(origin, point);
            y[i] = y_distance_between_2_points(origin, point);
        }
        
        //apply shoelace formula
        for(int i=0; i <getFeaturePointCount(feature_id) ; i++ ){
            area += (x[i]*y[i+1])-(y[i]*x[i+1]);
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
    //const OSMWay* input_way_p;// = nullptr ;    
    int numberOfWays = getNumberOfWays();
    //std::cout<< numberOfWays;
    /*
    for(int i=0; i < numberOfWays; i++){
        if(getWayByIndex(i)->id() == way_id){
            input_way_p = getWayByIndex(i);
            break;
        }
    }*/
    //use OSMWay* to find all the nodes the way contains
    /*
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
    */    
    return 0;
}
