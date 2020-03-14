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


bool there_is_turn(int from_seg_id, int to_seg_id){
    int from_street_id = getInfoStreetSegment(from_seg_id).streetID;
    int to_street_id = getInfoStreetSegment(to_seg_id).streetID;
    if(from_street_id == to_street_id){return false;}
    else {return true;}
}

double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path, const double turn_penalty){
    int vecSize = path.size();
    int turnCount = 0;
    int totalTravelTime = 0;
    if(vecSize==0){return 0;}
    else{
        for(int i=0; i+1< vecSize; i++){
            if(there_is_turn(path[i],path[i+1])){
                turnCount++;
            }
        }
        for(int j=0;j<vecSize;j++){
            totalTravelTime += find_street_segment_travel_time(path[j]);
        }
        return (totalTravelTime +(turnCount*turn_penalty));
    }
        
}
