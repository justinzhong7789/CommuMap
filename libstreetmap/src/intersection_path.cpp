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
#include <list>
#include <string>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

/************** Variable Declarations ******************/
 
vector<Node*> nodeTable;

 //--------------------------------------------------------------
 
 // Creates a node table for each intersection index in the map
 void makeNodeTable(){
     // Initialize vector size
     int nodeTable_size = getNumIntersections();
     
     for (IntersectionIndex id=0; id<nodeTable_size; id++){
         int seg_connected = getIntersectionStreetSegmentCount(id);
         nodeTable.push_back(new Node(id, seg_connected));
     }
 }
 
 // Gets a node by the intersection id
 Node* getNodebyID(IntersectionIndex sourceID){
     /*this if statement prevents accessing element beyond the bound of the vector(seg fault)*/
    if(sourceID<nodeTable.size() && sourceID>=0){
        Node *sourceNode = nodeTable[sourceID];
        return sourceNode; 
    }else{return nullptr;}
 }

 // Returns true if a path can be found, false otherwise
 // Updates the node table so that if a path is found, ids and parent ids can be found easily for traceback
 bool bfsPath (Node* sourceNode, int destID, double turn_penalty){
    // Establish min heap
    //priority_queue <WaveElem, vector<WaveElem>, comparatorWE> pq;
     priority_queue <WaveElem, vector<WaveElem>, greaterWE> pq;
    
    //LatLon source = getIntersectionPosition(sourceNode->id);
    //LatLon dest = getIntersectionPosition(destID);
    // Find absolute distance from source to destination
    //double original_dist = find_distance_between_two_points({source, dest});

    /*double constraint_dist_0 = 1.25* original_dist; // initialize constraint
    double constraint_dist_1 = 0.9 * original_dist;
    double constraint_dist_2 = 0.7 * original_dist;
    double constraint_dist_3 = 0.5 * original_dist;
    //double constraint_dist_4 = 0.2 * original_dist;*/
        
    //double constraint_dist = constraint_dist_0;
    //double constraint_dist;
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
                
                bool legal = false;
                    // Legal check
                if (currNode->id == info_outEdge.from && currNode->parent_id != info_outEdge.to){
                    toNode = nodeTable[info_outEdge.to];
                    legal = true;
                }
                else if (currNode->id == info_outEdge.to && currNode->parent_id != info_outEdge.from && info_outEdge.oneWay != true){
                    toNode = nodeTable[info_outEdge.from];
                    legal = true;
                }
                    // Update nodes if legal
                if (legal && !toNode->visited){
                   double time_score;
                   // Calculate score using travel time and turn time
                    if (currNode->reachingEdge != NO_EDGE){
                        time_score = currNode->bestTime + find_street_segment_travel_time(outEdge_id)+turn_penalty*there_is_turn(currNode->reachingEdge, outEdge_id);
                    } else { time_score = find_street_segment_travel_time(outEdge_id); } // If first mode, only count travel time
                  
                if (time_score < toNode->bestTime){
                      
                    // Update table 
                    nodeTable[toNode->id]->set_reachingEdge(outEdge_id);
                    nodeTable[toNode->id]->set_bestTime(time_score);
                    // Update parent_id for reaching edge
                    nodeTable[toNode->id]->set_parent_id(currNode->id);
                    // Find abs distance of this node
                    //LatLon current = getIntersectionPosition(toNode->id);
                    //double abs_distance = find_distance_between_two_points({current, dest});
                        
                    // Testing constraints
                    //int constraint = 0;
                    /*if (original_dist > 5000){
                    // Update constraints as nodes progress closer to destination. AND condition prevents reassigning constraints to larger number once a smaller number is reached
                        if (abs_distance < 0.65*original_dist && constraint_dist > constraint_dist_1) constraint_dist = constraint_dist_1;
                        if (abs_distance < 0.45*original_dist && constraint_dist > constraint_dist_2) constraint_dist = constraint_dist_2;
                        if (abs_distance < 0.2*original_dist && constraint_dist > constraint_dist_3) constraint_dist = constraint_dist_3;
                        //if (abs_distance < 0.05*original_dist && constraint_dist > constraint_dist_4) constraint_dist = constraint_dist_4;
                    }
                    else constraint_dist = abs_distance*1.15; */
                    //constraint_dist = original_dist*1.25;
                    
                    //    if (constraint_dist > abs_distance){ constraint = 1; } 
                    //pq.push(WaveElem(toNode, constraint, time_score)); // 1/100kmh * distance
                    
                    //pq.push(WaveElem(toNode, constraint, time_score /*+ 0.75*abs_distance/(info_outEdge.speedLimit * 0.27778)*/));
                    pq.push(WaveElem(toNode, 1, time_score));
                    }
                }
            }
    }
    return false;
}

// Returns the path of street segment ids if a path exists
list<StreetSegmentIndex> bfsTraceback(StreetSegmentIndex destID){
    list<StreetSegmentIndex> path;
    Node* currNode = getNodebyID(destID);
    StreetSegmentIndex prevEdge = currNode->reachingEdge;
    
    while (prevEdge != NO_EDGE){
        path.push_front(prevEdge);
        // Find parent node
        currNode = nodeTable[currNode->parent_id];
        prevEdge = currNode->reachingEdge;
    }
    return path;
}

// Returns empty if a path cannot be found
vector<StreetSegmentIndex> find_path_between_intersections(
        const IntersectionIndex intersect_id_start,
        const IntersectionIndex intersect_id_end,
        const double turn_penalty){
    
    //makeNodeTable();
    
    Node *sourceNode = getNodebyID(intersect_id_start);
    bool found = bfsPath(sourceNode, intersect_id_end, turn_penalty);
    
    if (found){
        list<StreetSegmentIndex> l = bfsTraceback(intersect_id_end); // Call traceback
        vector<StreetSegmentIndex> v{make_move_iterator(begin(l)), make_move_iterator(end(l))}; // Convert list to vector
        // Reset
        reset_nodeTable();
        return v;
    }
    else {
        reset_nodeTable();
        return {};
    }
}

void reset_nodeTable(){
    for(int i=0; i< nodeTable.size(); i++){
        nodeTable[i]->parent_id = NO_ID;
        nodeTable[i]->reachingEdge = NO_EDGE;
        nodeTable[i]->bestTime = WORST_TIME;
        nodeTable[i]->visited = false;
    }
}

void deleteNodeTable(){
    for (auto p : nodeTable){
        delete p;
    }
    nodeTable.clear();
}