#include "MJ.hpp"


//Assuming function already made that says if turn_penalty is needed: is_a_turn(segment, next segment);
double compute_path_walking_time(const std::vector<StreetSegmentIndex>& path, const double walking_speed, const double turn_penalty){
    
    int vecSize = path.size();
    int turnCount = 0;
    double totalTravelTime = 0;
    double segmentWalk;
    if(vecSize==0){return 0;}
    else{
        
        for(int i=0;i<vecSize;i++){
            
            if(i+1<vecSize){
                if(there_is_turn(path[i],path[i+1])){
                    turnCount++;
                }
            }
            
            double length = find_street_segment_length(path[i]);
            segmentWalk = length/walking_speed;
            totalTravelTime += segmentWalk;
        }
        return (totalTravelTime +((double)turnCount*turn_penalty));
    }
    
}

