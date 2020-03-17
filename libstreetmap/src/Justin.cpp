/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"

#include <vector>
#include <string>
#include <iostream>
#include "m3_global.hpp"

bool there_is_turn(int from_seg_id, int to_seg_id){
    int from_street_id = getInfoStreetSegment(from_seg_id).streetID;
    int to_street_id = getInfoStreetSegment(to_seg_id).streetID;
    if(from_street_id == to_street_id){return false;}
    else {return true;}
}

double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path, const double turn_penalty){
    int vecSize = path.size();
    int turnCount = 0;
    double totalTravelTime = 0;
    if(vecSize==0){return 0;}
    else{
        
        for(int i=0;i<vecSize;i++){
            
            if(i+1<vecSize){
                if(there_is_turn(path[i],path[i+1])){
                    turnCount++;
                }
            }
            totalTravelTime += find_street_segment_travel_time(path[i]);
        }
        return (totalTravelTime +((double)turnCount*turn_penalty));
    }
        
}

std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> 
         find_path_with_walk_to_pick_up(
                          const IntersectionIndex start_intersection, 
                          const IntersectionIndex end_intersection,
                          const double turn_penalty,
                          const double walking_speed, 
                          const double walking_time_limit){
                          
    
    
}
std::vector<StreetSegmentIndex>find_path_between_intersections(const IntersectionIndex intersect_id_start, const IntersectionIndex intersect_id_end, const double turn_penalty){
    
     return {0};
}