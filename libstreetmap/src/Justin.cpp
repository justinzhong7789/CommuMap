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

    IntersectionIndex inter_for_pick_up;
    Node* sourceNode = getNodebyID(start_intersection);
    bool pathFound = false;
    if(sourceNode != nullptr){
        pathFound = bfs_find_walk_path(sourceNode, end_intersection, turn_penalty);
    
        if(pathFound){
            completeWalkPath = bfsTraceback(end_intersection);
        }
        for(int i = 0; i<completeWalkPath.size();i++){
            if(compute_path_walking_time(walkPortion, walking_speed, turn_penalty)<walking_time_limit){
                walkPortion.push_back(completeWalkPath.back());
                completeWalkPath.pop_back();
            }
            else{
                break;
            }
        }
        //discard the last element to obtain the walk path
        if(walkPortion.size()!=0){
            walkPortion.pop_back();
        }
        

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
        }
        
        
    
        vector<StreetSegmentIndex> drive_portion = find_path_between_intersections(inter_for_pick_up, end_intersection, turn_penalty);
    
        nodeTable.clear(); 
        for(int i=0;i< walkPortion.size();i++){
            cout<<" walk number"<<i<<": "<< walkPortion[i]<<endl;
        }
        
        for(int i=0;i< drive_portion.size();i++){
            cout<<"drive number"<<i<<": "<< drive_portion[i]<<endl;
        }
        return make_pair(walkPortion, drive_portion);
    }
    else{
        nodeTable.clear();
        return {{0},{0}};
    }
}


//this is completely Priscilla's code. I only changed part so that the function
//does not consider one-way streets
bool bfs_find_walk_path(Node* sourceNode, int destID, double turn_penalty){
        // vector<WaveElem> wavefront;
    // Establish min heap
    priority_queue <WaveElem, vector<WaveElem>, comparatorWE> pq;
    // debugging
    int popped = 0;
    
    LatLon source = getIntersectionPosition(sourceNode->id);
    LatLon dest = getIntersectionPosition(destID);
    // Find absolute distance from source to destination
    double original_dist = find_distance_between_two_points({source, dest});
    double constraint_dist; // initialize constraint
    if(original_dist < 1500){
        constraint_dist = 2.5 * original_dist; // short routes have more flexibility, up to 250%
    } else { constraint_dist = original_dist * 1.25; }
    // First node
    pq.push(WaveElem(sourceNode, NO_EDGE, WORST_TIME, LARGEST_DISTANCE));
    
    while (!pq.empty()){ 
        // If popped takes longer than this, chances are path cannot be found
        if (popped > 200000){
            //cout << "first test failed" << endl;
            return (false);
        }
            WaveElem wave = pq.top(); // get next element
            pq.pop(); // remove from wavefront
            Node *currNode = wave.node;
                       
            popped += 1;

            if (popped > 100000 && wave.directed == 0){
                return false;
            }
            
            if (currNode->id == destID){
                    return true;
            }
            currNode->set_visited(true);
            for (int i=0; i<currNode->outEdge; i++){      
                Node *toNode;
                StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);
                InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id);   
                
                if (currNode->id == info_outEdge.from ){
                    toNode = nodeTable[info_outEdge.to];
                }
                else if (currNode->id == info_outEdge.to ){
                    toNode = nodeTable[info_outEdge.from];
                }
                
                if (!toNode->visited){
                   double score;
                   // Calculate score using travel time and turn time
                   
                    if (currNode->reachingEdge != NO_EDGE){
                        score = currNode->bestTime + find_street_segment_travel_time(outEdge_id)+turn_penalty*there_is_turn(currNode->reachingEdge, outEdge_id);
                    } else { score = find_street_segment_travel_time(outEdge_id); } // If first mode, only count travel time
                  
                if (score < toNode->bestTime || toNode->bestTime == 0){
                    toNode->set_reachingEdge(outEdge_id);
                    
                    toNode->set_bestTime(score);
                       
                    // Update table 
                    nodeTable[toNode->id] = toNode;
                    // Update parent_id for reaching edge
                    nodeTable[toNode->id]->set_parent_id(currNode->id);
                    // Find abs distance of this node
                    LatLon current = getIntersectionPosition(toNode->id);
                    double abs_distance = find_distance_between_two_points({current, dest});
                        
                    // Testing constraints
                    int constraint = 0;
                    if (constraint_dist > abs_distance){
                        constraint = 1;
                    }                     
                    pq.push(WaveElem(toNode, outEdge_id, toNode->bestTime, constraint, score));
                    }
                }
            }
    }
    //cout << "Failed path nodes popped: " << popped << endl;
    return false;

}