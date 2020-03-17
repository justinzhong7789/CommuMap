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

//Assuming function already made that says if turn_penalty is needed: is_a_turn(segment, next segment);
double compute_path_walking_time(const std::vector<StreetSegmentIndex>& path, const double walking_speed, const double turn_penalty){
    
    int size = path.size();
    double walkingTime = 0; //length/walkingSpeed
    
    if(size == 0){return 0;}
    
    for(int pathIdx = 0; pathIdx < size; pathIdx++){
        double length = find_street_segment_length(path[pathIdx]);
        double segmentWalk = length/walking_speed;
        
        if(are_directly_connected({path[pathIdx], path[pathIdx + 1]})){
             auto infoFirst = getInfoStreetSegment(path[pathIdx]);
             auto infoSecond = getInfoStreetSegment(path[pathIdx+1]);
             if(getStreetName(infoFirst.streetID) == getStreetName(infoSecond.streetID)){
                 segmentWalk = segmentWalk + turn_penalty;
             }
        }
        walkingTime = walkingTime + segmentWalk;
    }
    return 0;
    
}

// order, would take one from the start to the end intersection.
std::vector<StreetSegmentIndex> find_path_between_intersections(
		          const IntersectionIndex intersect_id_start, 
                  const IntersectionIndex intersect_id_end,
                  const double turn_penalty)
{
    return {0};
}

