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
        return (totalTravelTime +((double)turnCount*turn_penalty));
    }
        
}

//strategy is find the walking path from given start and end.
//then select the portion that gives travel time closest to the given walk time limit
//finally find drive path continueing from there
/*std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> 
         find_path_with_walk_to_pick_up(
                          const IntersectionIndex start_intersection, 
                          const IntersectionIndex end_intersection,
                          const double turn_penalty,
                          const double walking_speed, 
                          const double walking_time_limit){
    list<StreetSegmentIndex> completeWalkPath;
    vector<StreetSegmentIndex> walkPortion;
    Node* sourceNode = getNodebyID(start_intersection);
    bool pathFound = bfs_find_walk_path(sourceNode, end_intersection, turn_penalty);
    if(pathFound){
        completeWalkPath = bfsTraceback(end_intersection);
        //reverse the list to get correct ordered path
        
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
    walkPortion.pop_back();
    InfoStreetSegment lastSeg = getInfoStreetSegment(walkPortion[walkPortion.size()-1]);
    InfoStreetSegment secondTolastSeg = getInfoStreetSegment(walkPortion[walkPortion.size()-2]);
    IntersectionIndex inter_for_pick_up;
    if(lastSeg.from == secondTolastSeg.from){
        inter_for_pick_up = lastSeg.to;
    }
    else if(lastSeg.from == secondTolastSeg.to){
        inter_for_pick_up = lastSeg.to;
    }
    else if(lastSeg.to == secondTolastSeg.from){
        inter_for_pick_up = lastSeg.from;
    }
    else if(lastSeg.to == secondTolastSeg.to){
        inter_for_pick_up = lastSeg.from;
    }
    
    vector<StreetSegmentIndex> drive_portion = find_path_between_intersections(inter_for_pick_up, end_intersection, turn_penalty);
    
    
    return {walkPortion, drive_portion};
}


//this is completely Priscilla's code. I only changed part so that the function
//does not consider one-way streets
bool bfs_find_walk_path(Node* sourceNode, int destID, double turn_penalty){
    //uses breadth search algorithm
    // vector<WaveElem> wavefront;
    // Establish min heap
    priority_queue <WaveElem, vector<WaveElem>, greaterWE> pq;
    
    LatLon source = getIntersectionPosition(sourceNode->id);
    LatLon dest = getIntersectionPosition(destID);
    // Find absolute distance from source to destination
    double original_dist = find_distance_between_two_points({source, dest});
    // First node
    pq.push(WaveElem(sourceNode, NO_EDGE, 0, original_dist));
    
    while (!pq.empty()){
            WaveElem wave = pq.top(); // get next element
            pq.pop(); // remove from wavefront
            Node *currNode = wave.node;
                        
            if(wave.travelTime <= currNode->bestTime + 0.1){
                currNode->reachingEdge = wave.edgeID;
                currNode->bestTime = wave.travelTime;
                // Check for completed path 
                if (currNode->id == destID){
                    return true;
                }
                for (int i=0; i<currNode->outEdge; i++){      
                    Node *toNode;
                    StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);
                    InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id);
                    bool legal = false;
                    // Legal check
                    if(currNode->id == info_outEdge.from && currNode->parent_id != info_outEdge.to){
                        legal= true;
                        toNode = nodeTable[info_outEdge.to];
                    }
                    else if(currNode->id == info_outEdge.to && currNode->parent_id !=info_outEdge.from){
                        legal = true;
                        toNode = nodeTable[info_outEdge.from];
                    }
                    // Update nodes if legal
                    if (legal){
                        toNode->set_reachingEdge(outEdge_id);
                        toNode->set_bestTime(currNode->bestTime + travelTime(outEdge_id) + turn_penalty*there_is_turn(toNode->id, toNode->reachingEdge));
                        // Update table 
                        nodeTable[toNode->id] = toNode;
                        // Update parent_id for reaching edge
                        nodeTable[toNode->id]->set_parent_id(currNode->id);
                        // Find abs distance of this node
                        LatLon current = getIntersectionPosition(toNode->id);
                        double abs_distance = find_distance_between_two_points({current, dest});
                        pq.push(WaveElem(toNode, outEdge_id, currNode->bestTime+travelTime(outEdge_id)+turn_penalty*there_is_turn(toNode->id, toNode->reachingEdge), abs_distance));
                    }
                }

            }
    }
    return false;
}
*/

/*
 reset the whole nodeTable
 */
void reset_nodeTable(){
    for(int i=0;i<nodeTable.size();i++){
        nodeTable[i]->parent_id = NO_ID;
        nodeTable[i]->reachingEdge = NO_EDGE;
        nodeTable[i]->bestTime = WORST_TIME;
    }

}
/*
 if pass in the nodes accessed and changed as a vector<int>
 * it only changes those nodes. Faster
 */
void reset_nodeTable(vector<int> nodesAccessed){
    for(int i=0; i< nodesAccessed.size(); i++){
        nodeTable[nodesAccessed[i]]->parent_id = NO_ID;
        nodeTable[nodesAccessed[i]]->reachingEdge = NO_EDGE;
        nodeTable[nodesAccessed[i]]->bestTime = WORST_TIME;
    }

}

/*delete the memory allocated on the heap by new when the nodeTable was made*/
void delete_nodeTable(){
    for(int i=0; i< nodeTable.size();i++){
        delete nodeTable[i];
    }
}
