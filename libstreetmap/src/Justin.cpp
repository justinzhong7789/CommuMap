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
#include <list>
using namespace std;
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
        //cout<<totalTravelTime +((double)turnCount*turn_penalty)<<endl;
        return (totalTravelTime +((double)turnCount*turn_penalty));
    }
        
}

//strategy is find the walking path from given start and end.
//then select the portion that gives travel time closest to the given walk time limit
//finally find drive path continueing from there
std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> 
         find_path_with_walk_to_pick_up(
                          const IntersectionIndex start_intersection, 
                          const IntersectionIndex end_intersection,
                          const double turn_penalty,
                          const double walking_speed, 
                          const double walking_time_limit){
    if(nodeTable.size()==0){
        makeNodeTable();
    }
    list<StreetSegmentIndex> completeWalkPath;
    vector<StreetSegmentIndex> walkPortion;
    vector<StreetSegmentIndex> drive_portion;
    IntersectionIndex inter_for_pick_up = NO_EDGE;
    Node* sourceNode = getNodebyID(start_intersection);
    bool pathFound = false;
    if(sourceNode != nullptr){
        pathFound = bfs_find_walk_path(sourceNode, end_intersection, turn_penalty);
    
        if(pathFound){
            completeWalkPath = bfsTraceback(end_intersection);
        }
        for(int i = 0; i<completeWalkPath.size();i++){
            if(compute_path_walking_time(walkPortion, walking_speed, turn_penalty)<walking_time_limit){
                walkPortion.push_back(completeWalkPath.front());
                completeWalkPath.pop_front();
            }
            else{
                break;
            }
        }
        //discard the last element to obtain the walk path
        if(walkPortion.size()!=0){
            walkPortion.pop_back();
        }
     //   cout<< "walk portion size "<<walkPortion.size()<<endl;
        if(walkPortion.size()==0){
            inter_for_pick_up = start_intersection;
        }
        else if(walkPortion.size() == 1){
            if(getInfoStreetSegment (walkPortion[0]).from == start_intersection){
                inter_for_pick_up = getInfoStreetSegment (walkPortion[0]).to;
            }
            else {
                inter_for_pick_up = getInfoStreetSegment(walkPortion[0]).from;
            }
        }
        else {//when walk portion has 2 or more segments
            InfoStreetSegment lastSeg = getInfoStreetSegment(walkPortion[walkPortion.size()-1]);
            InfoStreetSegment secondTolastSeg = getInfoStreetSegment(walkPortion[walkPortion.size()-2]);            
            if(lastSeg.from == secondTolastSeg.from ||lastSeg.from == secondTolastSeg.to){
                inter_for_pick_up = lastSeg.to;
            }
            else if(lastSeg.to == secondTolastSeg.from || lastSeg.to == secondTolastSeg.to){
                inter_for_pick_up = lastSeg.from;
            }
            else{return {{},{}};}//path is discontinuous
        }
        reset_nodeTable();
        drive_portion = find_path_between_intersections(inter_for_pick_up, end_intersection, turn_penalty);
    
        
        
        //cout<< drive_portion.size()<<endl;
        reset_nodeTable();
        return make_pair(walkPortion, drive_portion);
    }
    else{
        reset_nodeTable();
        return {{},{}};
    }
    return {{},{}};
}


//this is completely Priscilla's code. I only changed part so that the function
//does not consider one-way streets
bool bfs_find_walk_path(Node* sourceNode, int destID, double turn_penalty){
        // vector<WaveElem> wavefront;
    // Establish min heap
    // Establish min heap
    priority_queue <WaveElem, vector<WaveElem>, comparatorWE> pq;
    
    // int popped = 0;
    
    LatLon source = getIntersectionPosition(sourceNode->id);
    LatLon dest = getIntersectionPosition(destID);
    // Find absolute distance from source to destination
    double original_dist = find_distance_between_two_points({source, dest});

    double constraint_dist_0 = 1.25* original_dist; // initialize constraint
    double constraint_dist_1 = 0.9 * original_dist;
    double constraint_dist_2 = 0.7 * original_dist;
    double constraint_dist_3 = 0.5 * original_dist;
    //double constraint_dist_4 = 0.2 * original_dist;
        
    double constraint_dist = constraint_dist_0;
    
    /*if(original_dist < 1500){
        constraint_dist = 2.5 * original_dist; // short routes have more flexibility, up to 250%
    } else { constraint_dist = original_dist * 1.25; }*/
            
    // First node
    pq.push(WaveElem(sourceNode, 1, 0));
    
    while (!pq.empty()){
            WaveElem wave = pq.top(); // get next element
            pq.pop(); // remove from wavefront
            Node *currNode = wave.node;
                      
            
            if (currNode->id == destID){
                //cout << currNode->bestTime << endl;
                return true;
            }
            currNode->set_visited(true);
            for (int i=0; i<currNode->outEdge; i++){      
                Node *toNode;
                StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);
                InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id); 
                
                    // Legal check
                if (currNode->id == info_outEdge.from){
                    toNode = nodeTable[info_outEdge.to];
                }
                else if (currNode->id == info_outEdge.to){
                    toNode = nodeTable[info_outEdge.from];
                }
                    // Update nodes if legal
                if (!toNode->visited){
                   double time_score;
                   // Calculate score using travel time and turn time
                    if (currNode->reachingEdge != NO_EDGE){
                        time_score = currNode->bestTime + find_street_segment_travel_time(outEdge_id)+turn_penalty*there_is_turn(currNode->reachingEdge, outEdge_id);
                    } else { time_score = find_street_segment_travel_time(outEdge_id); } // If first mode, only count travel time
                  
                if (time_score < toNode->bestTime){
                    toNode->set_reachingEdge(outEdge_id);
                    toNode->set_bestTime(time_score);
                       
                    // Update table 
                    nodeTable[toNode->id] = toNode;
                    // Update parent_id for reaching edge
                    nodeTable[toNode->id]->set_parent_id(currNode->id);
                    // Find abs distance of this node
                    LatLon current = getIntersectionPosition(toNode->id);
                    double abs_distance = find_distance_between_two_points({current, dest});
                        
                    // Testing constraints
                    int constraint = 0;
                    if (original_dist > 5000){
                    // Update constraints as nodes progress closer to destination. AND condition prevents reassigning constraints to larger number once a smaller number is reached
                        if (abs_distance < 0.65*original_dist && constraint_dist > constraint_dist_1) constraint_dist = constraint_dist_1;
                        if (abs_distance < 0.45*original_dist && constraint_dist > constraint_dist_2) constraint_dist = constraint_dist_2;
                        if (abs_distance < 0.2*original_dist && constraint_dist > constraint_dist_3) constraint_dist = constraint_dist_3;
                        //if (abs_distance < 0.05*original_dist && constraint_dist > constraint_dist_4) constraint_dist = constraint_dist_4;
                    }
                    else constraint_dist = abs_distance*1.15; 
                    
                        if (constraint_dist > abs_distance){ constraint = 1; } 
                    //pq.push(WaveElem(toNode, constraint, time_score)); // 1/100kmh * distance
                    
                    // lets try another heuristic lol
                    pq.push(WaveElem(toNode, constraint, time_score /*+ 0.75*abs_distance/(info_outEdge.speedLimit * 0.27778)*/));
                    }
                }
            }
    }
    return false;

}