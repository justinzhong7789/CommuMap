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
#include <math.h>
#include <vector>

bool load_map(std::string /*map_path*/) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully

    //
    //Load your map related data structures here
    //

    

    load_successful = true; //Make sure this is updated to reflect whether
                            //loading the map succeeded or failed

    return load_successful;
}

void close_map() {
    //Clean-up your map related data structures here
    
}

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
    double result = sqrt(pow((x2-x1),2) + pow((y2-y1),2));
    return result;
}

//Returns the length of the given street segment in meters
double find_street_segment_length(int street_segment_id){
    // pull info from streetsdatabase library
    InfoStreetSegment temp = getInfoStreetSegment(street_segment_id);
    // initialize total length to 0
    double length = 0;
    if(temp.curvePointCount==0){
    
        //put start and end LatLon into a pair
        std::pair<LatLon, LatLon> start_n_end(getIntersectionPosition(temp.from), getIntersectionPosition(temp.to));
        // calculate distance from start to end
        length = find_distance_between_two_points(start_n_end);
        // THIS FUNCTION IS NOT COMPLETED, NEED TO ACCOUNT FOR CURVATURE
        return length;
    }
    else{
        //add distances between end points and curve points
        return length;
    }
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
//J
double find_street_segment_travel_time(int street_segment_id){
    double travel_time;
    travel_time=find_street_segment_length(street_segment_id)/ (getInfoStreetSegment(street_segment_id).speedLimit);
    return travel_time;
}

//Returns the nearest intersection to the given position
int find_closest_intersection(LatLon my_position){
    

}

//Returns the street segments for the given intersection 
std::vector<int> find_street_segments_of_intersection(int intersection_id){
   
    

}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_street_names_of_intersection(int intersection_id){

}

//Returns true if you can get from intersection_ids.first to intersection_ids.second using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(std::pair<int, int> intersection_ids){

}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<int> find_adjacent_intersections(int intersection_id){

    
}

//Returns all street segments for the given street
std::vector<int> find_street_segments_of_street(int street_id){

}

//Returns all intersections along the a given street
std::vector<int> find_intersections_of_street(int street_id){

}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
std::vector<int> find_intersections_of_two_streets(std::pair<int, int> street_ids){

}

//Returns all street ids corresponding to street names that start with the given prefix
//The function should be case-insensitive to the street prefix. You should ignore spaces.
//For example, both "bloor " and "BloOrst" are prefixes to "Bloor Street East".
//If no street names match the given prefix, this routine returns an empty (length 0) 
//vector.
//You can choose what to return if the street prefix passed in is an empty (length 0) 
//string, but your program must not crash if street_prefix is a length 0 string.
std::vector<int> find_street_ids_from_partial_street_name(std::string street_prefix){

}
//Returns the area of the given closed feature in square meters
//Assume a non self-intersecting polygon (i.e. no holes)
//Return 0 if this feature is not a closed polygon.
double find_feature_area(int feature_id){

}    

//Returns the length of the OSMWay that has the given OSMID, in meters.
//To implement this function you will have to  access the OSMDatabaseAPI.h 
//functions.
double find_way_length(OSMID way_id){

}