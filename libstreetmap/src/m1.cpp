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
#include <map>
#include <unordered_map>
#include <math.h>
#include <vector>
#include <algorithm>


std::vector<LatLon> intersectionTable;
std::unordered_map< std::string, StreetIndex> StreetNamesTable;
std::unordered_map< IntersectionIndex, std::vector<int> > intersection_StreetTable;

void makeIntersectionTable();
void makeStreetNamesTable();
void makeIntersection_StreetTable();

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
        
        // add vector and intersection id to the map
        intersection_StreetTable.insert({id, streets_attached});
        // clear vector for next for loop
        streets_attached.clear();
    
        }
    }
}

bool load_map(std::string map_path) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully

    // added some functions -p
    //Load your map related data structures here
    load_successful = loadStreetsDatabaseBIN(map_path); //Make sure this is updated to reflect whether
                            //loading the map succeeded or failed

    return load_successful;
}

void close_map() {
    //Clean-up your map related data structures here
    closeStreetDatabase();
}
//MADE BY PRISCILLA -M
//Returns the distance between two coordinates in meters
double find_distance_between_two_points(std::pair<LatLon, LatLon> points){
    // see page 14 of milestone 1; (x,y)=(lon*cos(latavg), lat) -p
    // note lat() and lon() are degrees, cosine uses RADIANS -p
    double latavg = ((points.first.lat() + points.second.lat()) * DEGREE_TO_RADIAN)/2;
    // note to self: latitude is up/down, longitude is left/right -p
    double x1 = EARTH_RADIUS_METERS * points.first.lat() * DEGREE_TO_RADIAN * cos(latavg);
    double y1 = EARTH_RADIUS_METERS * points.first.lat() * DEGREE_TO_RADIAN;
    // note to self: radians * radius = arc length -p
    double x2 = EARTH_RADIUS_METERS * points.second.lat() * DEGREE_TO_RADIAN * cos(latavg);
    double y2 = EARTH_RADIUS_METERS * points.second.lat() * DEGREE_TO_RADIAN;
    
    // c^2 = a^2 + b^2 -p
    //DEBUGGING CHANGED THE ^ TO POW
    double result = sqrt(pow((x2-x1),2) + pow((y2-y1),2));
    return result;
}

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
        length = find_distance_between_two_points(straight_segment);        
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
                curved_segment.first = getStreetSegmentCurvePoint(i, street_segment_id);
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

//WORKING ON THIS ONE -J
//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
//J
double find_street_segment_travel_time(int street_segment_id){
    double travel_time;
    travel_time=find_street_segment_length(street_segment_id)/ (getInfoStreetSegment(street_segment_id).speedLimit);
    return travel_time;
}

//I THINK PRISCILLA SHOULD DO THIS ONE CUZ YOU NEED HER FUNCTIONS FOR THIS -M
//Returns the nearest intersection to the given position

//finished, may need MAJOR debugging
// changed from index to iterator...not sure if correct
int find_closest_intersection(LatLon my_position){
    // distance calculated from each intersection in the city -p
    double min_distance = 99999;
    IntersectionIndex closest;
    for (std::vector<LatLon>::iterator it = intersectionTable.begin(); it != intersectionTable.end(); it++){
        std::pair<LatLon, LatLon> temp (my_position, *it);
        double distance = find_distance_between_two_points(temp);
        if (distance<min_distance){
            min_distance = distance;
            closest = (it-intersectionTable.begin());
        }
    }
    return closest;
}

//DONE, NEED TESTING -M
//Returns the street segments for the given intersection 
std::vector<int> find_street_segments_of_intersection(int intersection_id){
   
    
    std::vector<int> street_segments_of_intersection;
    
    //Getting the number of segments attached to the intersection
    int num_segments = getIntersectionStreetSegmentCount(intersection_id);
    int i;
    
    //going through the segment index attached to the intersection 
    for(i=0; i < num_segments; ++i){
        //Double checks to make sure the streetSegment is actually apart of that intersection
        //COULD GO FASTER IF THIS IS CHECKED?
        if(getInfoStreetSegment(getIntersectionStreetSegment(intersection_id, i)).oneWay){
             street_segments_of_intersection.push_back(getIntersectionStreetSegment(intersection_id, i));
        }
        //Using an OR to make sure that a segment isn't counted twice?? DOUBLE CHECK THIS LOGIC
        else if(getInfoStreetSegment(getIntersectionStreetSegment(intersection_id, i)).from == intersection_id || getInfoStreetSegment(getIntersectionStreetSegment(intersection_id, i)).to == intersection_id){
            street_segments_of_intersection.push_back(getIntersectionStreetSegment(intersection_id, i));
        }
    }
    return street_segments_of_intersection;
}

//DONE, NEED TESTING -M
//DEPENDENT ON find_street_segments_of_intersection WORKING -M
//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_street_names_of_intersection(int intersection_id){
    std::vector<int> street_segments_of_intersection = find_street_segments_of_intersection(intersection_id);
    std::vector<std::string> intersection_street_names;
    int i;
    for (i=0; i< street_segments_of_intersection.size(); ++i){
        //Starting from outmost brackets: putting street name into vector to return
        //getting the street name from the street ID
        //found the streetID through the InfoSteetSegment struct
        //Used function to get the InfoStreetSegment
        intersection_street_names.push_back(getStreetName(getInfoStreetSegment(street_segments_of_intersection[i]).streetID));
        //includes duplicate names so no need to check for that
    }
    return intersection_street_names;
}

//Tested received 2 out of 4 failed -M
//Returns true if you can get from intersection_ids.first to intersection_ids.second using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(std::pair<int, int> intersection_ids){

    //std::vector<int> intersectionSegmentsOne = find_street_segments_of_intersection(intersection_ids.first);
    //std::vector<int> intersectionSegmentsTwo = find_street_segments_of_intersection(intersection_ids.second);
    
    std::vector<int> intersectionSegmentsOne = find_street_segments_of_intersection(intersection_ids.first);
    
    if(intersection_ids.first == intersection_ids.second)return true;
    
    for(int i=0 ; i < intersectionSegmentsOne.size() ; ++i){
        
        if (getInfoStreetSegment(intersectionSegmentsOne[i]).oneWay){
            if(getInfoStreetSegment(intersectionSegmentsOne[i]).to == intersection_ids.second){
                return true;
            }
            return false;
        }else if(getInfoStreetSegment(intersectionSegmentsOne[i]).to == intersection_ids.second){
            return true;
        }
    }
    return false;
}


//SUPER ROUGH MIGHT TAKE WAYYY TOO LONG -M
//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<int> find_adjacent_intersections(int intersection_id){

    
    std::vector<int> adjacentIntersections;
    std::vector<int> intersectionStreetSegments = find_street_segments_of_intersection(intersection_id);
    std::pair<int,int> twoIntersections;
    twoIntersections.first = intersection_id;
    
    for (int i = 0; i < intersectionStreetSegments.size() ; ++i){
            //checks if two intersections found by both side of the segments are directly connected and therefore would be adjacent (used are_directly_connected function)
            //checks if the 'from' intersection is the intersection_id
            twoIntersections.second = getInfoStreetSegment(intersectionStreetSegments[i]).from;
            if(are_directly_connected(twoIntersections)){
                adjacentIntersections.push_back(getInfoStreetSegment(intersectionStreetSegments[i]).from);
                twoIntersections.second = getInfoStreetSegment(intersectionStreetSegments[i]).to;
            }else if(are_directly_connected(twoIntersections)){
                adjacentIntersections.push_back(getInfoStreetSegment(intersectionStreetSegments[i]).to);
            }
    }
    return adjacentIntersections;
}

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

//Returns all intersections along the a given street
//j
/* this function might be slow 
 * might need a more efficient algorithm
 */
std::vector<int> find_intersections_of_street(int street_id){
    std::vector<int> intersection_ids;
    //find all street segments on the given street using the previous function
    std::vector<int> street_segment_ids=find_street_segments_of_street(street_id);
    int num_of_intersections = getNumIntersections();
    
    //if intersection id matches the from and to in any street segments, put it into vector
    for(int inter_index=0;inter_index<num_of_intersections;inter_index++){
        bool match=false;
        for(int i=0;i<street_segment_ids.size();i++){
            if(inter_index==getInfoStreetSegment(street_segment_ids[i]).from||inter_index==getInfoStreetSegment(street_segment_ids[i]).to){
                match=true;
                break;
            }
        }
        if(match){
            intersection_ids.push_back(inter_index);
        }
    }
    return intersection_ids;
}
//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//working on this -p
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){
    //initialize vector to be returned
    std::vector<int> intersections_first = find_intersections_of_street(street_ids.first);
    std::vector<int> intersections_second = find_intersections_of_street(street_ids.second);
    
    std::vector<int> output(99999);
    std::vector<int>::iterator it;
    
    std::sort(intersections_first.begin(), intersections_first.end());
    std::sort(intersections_second.begin(), intersections_second.end());
    
    it = std::set_intersection(intersections_first.begin(), intersections_first.end(),
            intersections_second.begin(), intersections_second.end(), output.begin());
    output.resize(it-output.begin());
    return output;
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
    //remove all white spaces
    street_prefix.erase(remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    street_prefix = toupper(street_prefix); // change all to capital
    if (street_prefix==""){ // return null if blank
        return {NULL};
    }
    std::vector<int> street_ids = {NULL};
    std::string street_name;
    // O(n^2) squad -p
    std::map<string, StreetIndex>::iterator it;
    for (it=StreetNamesTable.begin();it!=StreetNamesTable.end();it++){
        street_name = it->first;
        street_name.erase(remove(street_name.begin(), street_name.end(), ' '), street_name.end());
        street_name = toupper(street_name); // all to capital
 
        for (int i=0; i<street_prefix.length(); i++){
            if (street_prefix[i]!=street_name[i]){
                break;
            }
            else if ((i-1) = street_prefix.length()){
                street_ids.push_back(it->second);
            }
        }
    }
    return street_ids;
}
//Returns the area of the given closed feature in square meters
//Assume a non self-intersecting polygon (i.e. no holes)
//Return 0 if this feature is not a closed polygon.
double find_feature_area(int feature_id){
    return 0;
}    

//Returns the length of the OSMWay that has the given OSMID, in meters.
//To implement this function you will have to  access the OSMDatabaseAPI.h 
//functions.
double find_way_length(OSMID way_id){
    return 0;
}
    
