
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

#define invalid -1
using namespace std;
typedef vector<StreetSegmentIndex> path;
/*
 * 
 * first element of pair 
 */

class pathInfo{
public: 
    pathInfo(path path_, double time):the_path(path_), path_travel_time(time){};
    path the_path;
    double path_travel_time;
    
    
    
friend bool operator>(const pathInfo& a, const pathInfo& b){
    return a.path_travel_time>b.path_travel_time;
}
friend bool operator<(const pathInfo& a, const pathInfo& b){
    return a.path_travel_time<b.path_travel_time;
}
};


vector<vector<pair<pathInfo, pathInfo>>> all_PU_to_other_points;
vector<vector<pair<pathInfo, pathInfo>>> all_DO_to_other_points;
vector<vector<pathInfo>> depots_to_all_PU;
vector<vector<pathInfo>> all_DO_to_depots;


bool one_source_multi_dest(Node* sourceNode, vector<IntersectionIndex> destinations, double turn_penalty);
bool make_depot_to_PU(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty);
vector<StreetSegmentIndex> convertListToVec(list<StreetSegmentIndex> l);
bool make_PU_to_other_points(const std::vector<DeliveryInfo>& deliveries, double turn_penalty);
bool make_DO_to_points(const std::vector<DeliveryInfo>& deliveries,const std::vector<int>& depots, double turn_penalty);
bool precompute_all_paths(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty);