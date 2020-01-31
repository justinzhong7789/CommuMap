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
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>

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
/* void makeIntersection_StreetTable(){
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
}*/
/*
class segmentsInfo {
   
    OSMID wayOSMID;   // OSM ID of the source way
                      // NOTE: Multiple segments may match a single OSM way ID

    IntersectionIndex from, to;  // intersection ID this segment runs from/to
    bool oneWay;            // if true, then can only travel in from->to direction

    int curvePointCount;    // number of curve points between the ends
    float speedLimit;            // in km/h

    StreetIndex	streetID;        // index of street this segment belongs to

     
};       

std::string segmentName(int segment_id){
    return getStreetName(getInfoStreetSegment(segment_id).streetID);
}
 */ 

bool load_map(std::string map_path) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully

    // added some functions -p
    //Load your map related data structures here
    load_successful = loadStreetsDatabaseBIN(map_path); //Make sure this is updated to reflect whether
                            //loading the map succeeded or failed
    makeIntersectionTable();
    makeStreetNamesTable();
    // makeIntersection_StreetTable();
    return load_successful;
}

void close_map() {
    //Clean-up your map related data structures here
    closeStreetDatabase();
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

//I THINK THIS PASSES -M
//Returns the street segments for the given intersection 
std::vector<int> find_street_segments_of_intersection(int intersection_id){
    std::vector<int> street_segments_of_intersection;
    //Getting the number of segments attached to the intersection
    int num_segments = getIntersectionStreetSegmentCount(intersection_id);
    int i;
    //going through the segment index attached to the intersection 
    for(i=0; i < num_segments; ++i){
            street_segments_of_intersection.push_back(getIntersectionStreetSegment(intersection_id, i));
    }
    return street_segments_of_intersection;
}

//I THINK THIS ALSO PASSES -M
//DEPENDENT ON find_street_segments_of_intersection WORKING -M
//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_street_names_of_intersection(int intersection_id){
    
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
}

//FINISHED -M
//Returns true if you can get from intersection_ids.first to intersection_ids.second using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(std::pair<int, int> intersection_ids){
    
    std::vector<int> intersectionSegmentsOne = find_street_segments_of_intersection(intersection_ids.first);
    
    if(intersection_ids.first == intersection_ids.second){return true;}
    int streetSegmentTo = 0;
    int streetSegmentFrom = 0;
    if(intersection_ids.first == intersection_ids.second)return true;
    
    for(int i=0 ; i < intersectionSegmentsOne.size() ; ++i){
        
        streetSegmentTo = getInfoStreetSegment(intersectionSegmentsOne[i]).to; 
        streetSegmentFrom = getInfoStreetSegment(intersectionSegmentsOne[i]).from; 
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
//this passes i think -p
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
 
    /*
    //remove all white spaces
    street_prefix.erase(remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(), ::toupper); // change all to capital
    if (street_prefix==""){ // return null if blank
        return {NULL};
    }

    std::vector<int> street_ids;
    std::string street_name;
    // O(n^2) squad -p  
    std::unordered_map<std::string, StreetIndex>::iterator it;
    it=StreetNamesTable.begin();

    for (it=StreetNamesTable.begin();it!=StreetNamesTable.end();it++){
        street_name = it->first;
        street_name.erase(remove(street_name.begin(), street_name.end(), ' '), street_name.end());
        std::transform(street_name.begin(), street_name.end(), street_name.begin(), ::toupper); // all to capital
 
        for (int i=0; i<street_prefix.length(); i++){
            if (street_prefix[i]!=street_name[i]){
                break;
            }

            else if ((street_prefix[i]==street_name[i])&&(i==street_prefix.length()-1)){
                street_ids.push_back(it->second);
            }
        } 
    }
    return street_ids
    
}
   */
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
    
