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
        for(int i=0;i<segmentsOfStreets[j].size();i++){
            
            // Find the intersections attached to the segment
            int interTo = getInfoStreetSegment(segmentsOfStreets[j][i]).to;
            int interFrom = getInfoStreetSegment(segmentsOfStreets[j][i]).from;
            
            // Inserts intersections in local vector
            intersections_of_streets.push_back(interTo);
            intersections_of_streets.push_back(interFrom);
        }
        
        // Copies local vector into vector of streets, local vector is cleared afterwards
        intersectionsOfStreets[j]= intersections_of_streets;
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

/*============================== MILESTONE 1 ==============================*/

bool load_map(std::string map_path) {
    bool load_Map_successful = false; //Indicates whether the map has loaded 
    bool load_OSM_successful = false;                              //successfully
    
    std::string OSMpath = map_path;
    std::string streetsBin = ".streets.bin";
    if(OSMpath.find(streetsBin) != std::string::npos){
        OSMpath.replace(OSMpath.find(streetsBin), streetsBin.length() , ".osm.bin");
        load_OSM_successful = loadOSMDatabaseBIN(OSMpath);
    }

    load_Map_successful = loadStreetsDatabaseBIN(map_path); 
    

    //make sure load map succeeds before creating structures that depend on the API
    if (load_Map_successful){

        //Functions used in street tests
        makeCapitalizedStreetNamesTable();
        makeSegmentsOfStreets();
        makeIntersectionsOfStreets(); 
        
        //Functions used in intersections tests
        makeSegmentsOfIntersections();
        
        //Functions used in distance-time tests
        makeTableOfDivisors();
        makeOSMWayTable();
        makeOSMNodeTable();
        
    }
    return load_Map_successful && load_OSM_successful;
}

void close_map() {
    
    // Close StreetsDatabaseBIN and OSMDatabaseBIN
    closeStreetDatabase();
    closeOSMDatabase();
    
    // Close data structures
    capitalizedStreetNamesTable.clear();
    segmentsOfStreets.clear();
    intersectionsOfStreets.clear();
    OSMWayTable.clear();
    OSMNodeTable.clear();
    segmentsOfIntersections.clear();
    tableOfDivisors.clear(); 
}

// Returns the distance between two coordinates in meters
double find_distance_between_two_points(std::pair<LatLon, LatLon> points){
    
    // using pythagoras theorem -more explanations? 
    double x_diff = x_distance_between_2_points(points.first, points.second);
    double y_diff = y_distance_between_2_points(points.first, points.second);
    
    return sqrt(pow(x_diff,2)+pow(y_diff,2));
}

// Returns the length of the given street segment in meters
double find_street_segment_length(int street_segment_id){
    // Pull info from API library
    InfoStreetSegment street = getInfoStreetSegment(street_segment_id);
    // Initialize total length to 0
    double length = 0;

    // If street has no curves, e.g. street is completely straight
    if (street.curvePointCount == 0){
        // calculate distance from start to end
        std::pair<LatLon,LatLon> straight_segment (getIntersectionPosition(street.from), getIntersectionPosition(street.to));    
        length += find_distance_between_two_points(straight_segment);        
    } 
    // If street has curves
    else {
        for (int i=0; i<=street.curvePointCount; i++){
            std::pair<LatLon,LatLon> curved_segment;
            // Calculate distance from beginning of street segment to first curve point     
            if (i==0){ 
                curved_segment.first = getIntersectionPosition(street.from); 
                curved_segment.second = getStreetSegmentCurvePoint(i, street_segment_id);
            }
            // Calculate distance from last curve point to end of street segment
            else if (i==street.curvePointCount){
                curved_segment.first = getStreetSegmentCurvePoint(i-1, street_segment_id);
                curved_segment.second = getIntersectionPosition(street.to);
            // Calculate distance between each curve point   
            } else { 
                curved_segment.first = getStreetSegmentCurvePoint(i-1, street_segment_id);
                curved_segment.second = getStreetSegmentCurvePoint(i, street_segment_id);       
            }
            // Add the distance to total length
            length += find_distance_between_two_points(curved_segment);
        }
    }
    return length;
}

//Returns the travel time to drive a street segment in seconds 
double find_street_segment_travel_time(int street_segment_id){
    //return find_street_segment_length(street_segment_id) * 3.6 * tableOfDivisors[street_segment_id];
    return tableOfDivisors[street_segment_id].first * 3.6 * tableOfDivisors[street_segment_id].second;
}


//Returns the nearest intersection to the given position
int find_closest_intersection(LatLon my_position){
    // Initialize IntersectionID to be returned
    IntersectionIndex closest = 0;    
    LatLon current = getIntersectionPosition(0);
    std::pair<LatLon, LatLon> current_points (my_position, current);
    // Initialize minimum distance as the distance between my_position and IntersectionIndex 0
    double min_distance = find_distance_between_two_points(current_points); // initialize "large" minimum distance as reference

    // Traverse through all IntersectionIDs and compare the distances
    for (int i=1; i<getNumIntersections();i++){
        current = getIntersectionPosition(i);
        current_points = std::make_pair(my_position, current);
        // Calculate the distance between my_position and each IntersectionIndex
        double distance = find_distance_between_two_points(current_points);
        // Replace min_distance and closest whenever a smaller distance is calculated
        if (distance < min_distance){
            min_distance = distance;
            closest = i;
        }
    }
    return closest;
}

// Returns the street segments for the given intersection 
// Uses segmentsOfIntersections to access elements
std::vector<int> find_street_segments_of_intersection(int intersection_id){
    
    // Checks to see if intersection id exists
    if(intersection_id > getNumIntersections() && intersection_id < 0) return {0};
    // Accesses the index with the corresponding intersection ID to return the vector of segments
    return segmentsOfIntersections[intersection_id];
}

// Returns the street names at the given intersection (includes duplicate street names in returned vector)
// Uses segmentsOfIntersection global variable
std::vector<std::string> find_street_names_of_intersection(int intersection_id){
    
    std::vector<std::string> street_names_of_intersection;
    
    // Loops through all the segments attached to the intersection
    for (int segment = 0; segment < segmentsOfIntersections[intersection_id].size(); ++segment) {
        
        // Gets the streetID attached to the segment
        int streetID = getInfoStreetSegment(segmentsOfIntersections[intersection_id][segment]).streetID;
        
        // Insert the name into the vector to be returned
        street_names_of_intersection.push_back(getStreetName(streetID));
    }
    return street_names_of_intersection;
}

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
    
    remove_dups_in_vecs(intersectionsOfStreets[street_id]);
    
    return intersectionsOfStreets[street_id];
}

//TRYING TO OPTOMIZE TIME -M
//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){
    // all intersections of the first street
    std::vector<int> intersections_first = find_intersections_of_street(street_ids.first);
    // all intersections of the second street
    std::vector<int> intersections_second = find_intersections_of_street(street_ids.second);
    
    //sizes the commonIntersection to make sure to fit all intersections between streets
    std::vector<int> commonIntersection (intersections_first.size() + intersections_second.size());
    
    VectorIt it;
    //Makes sure it's sorted..only way to go through the set_intersection
    std::sort(intersections_first.begin(), intersections_first.end());
    std::sort(intersections_second.begin(), intersections_second.end());
    // find intersections that are common to both streets
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
std::vector<int> find_street_ids_from_partial_street_name(std::string street_prefix){  
    // Remove all white spaces and turn all characters upper case
    street_prefix.erase(remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), ::toupper);
    
    // Return empty vector if input is blank
    if (street_prefix==""){
        return {};
    }    
    // Initialize empty return vector
    std::vector<int> street_ids = {};
    
    std::multimap<std::string, StreetIndex>::iterator it;
    // Determine first instance of street_prefix found in map
    std::multimap<std::string, StreetIndex>::iterator lower = capitalizedStreetNamesTable.lower_bound(street_prefix);
    // Determine last instance by incrementing first character in ASCII - e.g. search "MAR", upper bound will be "N"
    std::multimap<std::string, StreetIndex>::iterator upper = capitalizedStreetNamesTable.upper_bound(std::string(1, char((int(street_prefix[0])+1))));

    for (it=lower; it!=upper; it++){
        for (int i=0; i<((*it).first).length(); i++){
            // If characters are found different, stop searching
            if (street_prefix[i]!=((*it).first)[i]){
                break;
            }
            // If all characters of street_prefix match, add the StreetIndex to the return vector
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
    // improved justin's code by replacing for loops with std::map.find() -p)
    if (OSMWayTable.find(way_id)==OSMWayTable.end()){
        // id does not exist;
        return 0; //failed to find way
    }
    double length=0; // initialize length
    // find OSMWay node
    const OSMWay* input_way_p = OSMWayTable.find(way_id)->second;
    const std::vector<OSMID> wayMembers = getWayMembers(input_way_p); // osmids of nodes that form the way
    std::vector<LatLon> latlon_of_nodes; // initialize vector of LatLons of the nodes
    // get coords for each node
    for (int i=0; i<wayMembers.size();i++){
        if (OSMNodeTable.find(wayMembers[i]) != OSMNodeTable.end()){
            latlon_of_nodes.push_back(getNodeCoords((OSMNodeTable.find(wayMembers[i]))->second));
        }
    }
    // calculate length between each node and sum
    for (int i=0; i<latlon_of_nodes.size()-1;i++){
        length += find_distance_between_two_points(std::make_pair(latlon_of_nodes[i], latlon_of_nodes[i+1]));
    }
    return length;
}
