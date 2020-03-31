/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m3_global.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "m4.h"
#include "m4_global.hpp"

using namespace std;
void make_depots_to_deliveries(
		        const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int>& depots, 
                        double turn_penalty)
{
    int depot_size = depots.size();
    int deli_size = deliveries.size();
    for(int i=0;i< depot_size;i++){
        vecor<pair<path_info, path_info>> one_depot_to_all_deli;
        for(int j=0;j< deli_size;j++){
            path path_ = find_path_between_intersections(depots[i], deliveries[j].pickUp, turn_penalty);
            path_info to_PU(path_, compute_path_travel_time(path_, turn_penalty)); 
            
            path_ = find_path_between_intersections(depots[i], deliveries[j].dropOff, turn_penalty);
            path_info to_DO(path_, compute_path_travel_time(path_, turn_penalty));
            
            one_depot_to_all_deli.push_back(make_pair(to_PU, to_DO));
        }
        
        all_depots_to_all_deliveries.push_back(one_depot_to_all_deli);
    }
    
}

void make_deli_to_depots(
		        const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int>& depots, 
                        double turn_penalty)
{
    int deli_size = deliveries.size();
    int depot_size = depots.size();
    for(int i=0; i< deli_size;i++){
        vector<pair<path_info, path_info>> one_deli_to_all_depot;
        for(int j=0; j< depot_size; j++){
            path path_ = find_path_between_intersections(deliveries[i].pickUp, depots[j], turn_penalty);
            path_info PU_to_depot(path_, compute_path_travel_time(path_, turn_penalty));
            path_ = find_path_between_intersections(deliveries[i].dropOff, depots[j], turn_penalty);
            path_info DO_to_depot(path_, compute_path_travel_time(path_, turn_penalty));
            
            one_deli_to_all_depot.push_back(make_pair(PU_to_depot, DO_to_depot));
        }
        all_deli_to_all_depot.push_back(one_deli_to_all_depot);
    }
}

void make_PU_to_deli(
		        const std::vector<DeliveryInfo>& deliveries,
                        double turn_penalty)
{   
    int deli_size = deliveries.size();
    for(int i=0; i< deli_size; i++){
        vector<pair<path_info, path_info>> one_PU_to_deli;
        for(int j=0;j< deli_size; j++){
            path path_ = find_path_between_intersections(deliveries[i].pickUp, deliveries[j].pickUp, turn_penalty);
            path_info to_PU(path_, compute_path_travel_time(path_, turn_penalty));
            path_ = find_path_between_intersections(deliveries[i].pickUp, deliveries[j].dropOff, turn_penalty);
            path_info to_DO(path_, compute_path_travel_time(path_, turn_penalty));
            
            one_PU_to_deli.push_back(make_pair(to_PU, to_DO));
            
        }
        all_PU_to_other_points.push_back(one_PU_to_deli);
    }
}

void make_DO_to_deli(
		        const std::vector<DeliveryInfo>& deliveries,
                        double turn_penalty)
{   
    int deli_size = deliveries.size();
    for(int i=0; i< deli_size; i++){
        vector<pair<path_info, path_info>> one_DO_to_deli;
        for(int j=0;j< deli_size; j++){
            path path_ = find_path_between_intersections(deliveries[i].dropOff, deliveries[j].pickUp, turn_penalty);
            path_info to_PU(path_, compute_path_travel_time(path_, turn_penalty));
            path_ = find_path_between_intersections(deliveries[i].dropOff, deliveries[j].dropOff, turn_penalty);
            path_info to_DO(path_, compute_path_travel_time(path_, turn_penalty));
            
            one_DO_to_deli.push_back(make_pair(to_PU, to_DO));
            
        }
        all_DO_to_other_points.push_back(one_DO_to_deli);
    }
}

void precompute_all_paths(const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int>& depots, 
                        double turn_penalty)
{
    make_depots_to_deliveries(deliveries, depots, turn_penalty);
    make_deli_to_depots(deliveries, depots, turn_penalty);
    make_PU_to_deli(deliveries, turn_penalty);
    make_DO_to_deli(deliveries, turn_penalty);
    
}

void clear_all_precomputed_paths(){
    all_depots_to_all_deliveries.clear();
    all_deli_to_all_depot.clear();
    all_PU_to_other_points.clear();
    all_DO_to_other_points.clear();
    
}