
#pragma once


#include "m1.h"
#include "m2.h"
#include "zoom.hpp"
#include "global_variables.hpp"
#include "m2_global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <vector>
#include <iostream>

#include "m3_global.hpp"

using namespace std;
typedef vector<StreetSegmentIndex> path;
/*
 * 
 * first element of pair 
 */

class path_info{
public: 
    path the_path;
    double path_travel_time;
    
    path_info(path path_, double time):path(path_), path_travel_time(time){};
    
friend bool operator>(const& path_info a, const& path_info b){
    return a.path_travel_time>b.path_travel_time;
};
friend bool operator<(const& path_info a, const& path_info b);
    return a.path_travel_time<b.path_travel_time;
}

/*
 * this vector has size equal to the size of depots given
 * every element is a vector of a pair of path_info which contains the path and its travel_time
 * the first of the pair is the path to a pick up, and the second is the path to a drop off
 * eg1. (all_depots_to_all_deliveries[0])[1].first()
 * gives the path from depot[0] to deliveries[1]'s pickUp location
 * 
 * eg2. (all_deli_to_all_depot[0])[1].first()
 * gives the path from deliveries[0]'s pickUp to depot[1]
 * 
 * eg3. (all_PU_to_other_points[0])[1].first()
 * gives the path from deliveries[0]'s pickUp location to deliveries[1]'s pickUp 
 *   
 */
vector<vector<pair<path_info, path_info>>> all_depots_to_all_deliveries;
vector<vector<pair<path_info, path_info>>> all_deli_to_all_depot;
vector<vector<pair<path_info, path_info>>> all_PU_to_other_points;
vector<vector<pair<path_info, path_info>>> all_DO_to_other_points;

