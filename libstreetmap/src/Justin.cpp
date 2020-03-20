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

