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

bool one_source_multi_dest(Node* sourceNode, vector<IntersectionIndex> destinations, double turn_penalty){
        // Establish min heap
    //priority_queue <WaveElem, vector<WaveElem>, comparatorWE> pq;
    priority_queue <WaveElem, vector<WaveElem>, greaterWE> pq;
    pq.push(WaveElem(sourceNode, 1, 0));
    
    
    
    while (!pq.empty()){
            bool done = true;
            WaveElem wave = pq.top(); // get next element
            pq.pop(); // remove from wavefront
            Node *currNode = wave.node;
            for(int i =0;i<destinations.size();i++){
                if(destinations[i] != invalid){
                    done = false;
                    break;
                }
            }          
            if(done == true){
                return true;
            }
            for(int i=0;i< destinations.size();i++){
                if(currNode->id == destinations[i]){
                    destinations[i] = invalid;
                }
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
                    pq.push(WaveElem(toNode, 1, time_score));
                    }
                }
            }
    }
    return false;

}

bool make_depot_to_PU(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty){
    int depotNum = depots.size();
    int deliNum = deliveries.size();
    vector<IntersectionIndex> PU_loc;
    
    for(int i=0; i< deliNum;i++){
        PU_loc.push_back(deliveries[i].pickUp);
    }
    
    for(int i=0; i< depotNum;i++){
        bool all_path_found = false;
        Node* source = getNodebyID(depots[i]);
        all_path_found = one_source_multi_dest(source, PU_loc, turn_penalty);
        vector<pathInfo> one_depot_to_all_PU;
        if(all_path_found){
            for(int j=0;j< PU_loc.size();j++){
                //do traceback from all pickups
                path to_one_PU = convertListToVec(bfsTraceback(PU_loc[j]));
                pathInfo one_path(to_one_PU, compute_path_travel_time(to_one_PU, turn_penalty));
                one_depot_to_all_PU.push_back(one_path);
            }
        
        }
        else{return false;}
        depots_to_all_PU.push_back(one_depot_to_all_PU);
        reset_nodeTable();
    }
    return true;
}

/*
bool make_DO_to_depots(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty){
    int deliNum = deliveries.size();
    int depotNum = depots.size();
    vector<IntersectionIndex> all_DO;
    for(int i=0;i< deliNum;i++){
        all_DO.push_back(deliveries[i].dropOff);
    }
    
    for(int i=0;i<deliNum;i++){
        vector<pathInfo> one_DO_to_all_depot;
        bool all_path_found = false;
        Node* source = getNodebyID(all_DO[i]);
        all_path_found = one_source_multi_dest(source, depots, turn_penalty);
        if(all_path_found){
            for(int j=0;j< depotNum;j++){
                path to_one_depot = convertListToVec(bfsTraceback(depots[j]));
                pathInfo one_DO_to_one_depo(to_one_depot, compute_path_travel_time(to_one_depot, turn_penalty));
                one_DO_to_all_depot.push_back(one_DO_to_one_depo);
            }
        }
        else{return false;}
        all_DO_to_depots.push_back(one_DO_to_all_depot);
    }
    return true;
}
*/
vector<StreetSegmentIndex> convertListToVec(list<StreetSegmentIndex> l){
    vector<StreetSegmentIndex> v;
    for(int i=0;i< l.size();i++){
        v.push_back(l.front());
        l.pop_front();
    }
    return v;
}

bool make_PU_to_other_points(const std::vector<DeliveryInfo>& deliveries, double turn_penalty){
    int deliNum = deliveries.size();
    vector<IntersectionIndex> PU, DO, all;
    for(int i=0;i< deliNum ;i++){
        PU.push_back(deliveries[i].pickUp);
        DO.push_back(deliveries[i].dropOff);
        all.push_back(deliveries[i].pickUp);
        all.push_back(deliveries[i].dropOff);
    }
    for(int i=0;i<deliNum ;i++){
        Node * source = getNodebyID(PU[i]);
        bool all_path_found = false;
        vector<pair<pathInfo, pathInfo>> one_pu_to_all_points;
        
        all_path_found = one_source_multi_dest(source, all, turn_penalty);
        if(all_path_found){
            for(int j=0;j< deliNum;j++){
                path to_pu = convertListToVec(bfsTraceback(PU[i]));
                path to_do = convertListToVec(bfsTraceback(DO[i]));
                pathInfo path_to_pu(to_pu, compute_path_travel_time(to_pu, turn_penalty));
                pathInfo path_to_do(to_do, compute_path_travel_time(to_do, turn_penalty));
                one_pu_to_all_points.push_back(make_pair(path_to_pu, path_to_do));
            }
            
        }
        else {return false;}
        all_PU_to_other_points.push_back(one_pu_to_all_points);
        reset_nodeTable();
    }
    
    return true;
    
}


bool make_DO_to_points(const std::vector<DeliveryInfo>& deliveries,const std::vector<int>& depots, double turn_penalty){
    int deliNum = deliveries.size();
    int depotNum = depots.size();
    vector<IntersectionIndex> PU, DO, all;
    for(int i=0;i<deliNum;i++){
        all.push_back(deliveries[i].pickUp);
        all.push_back(deliveries[i].dropOff);
        PU.push_back(deliveries[i].pickUp);
        DO.push_back(deliveries[i].dropOff);
    }
    for(int i=0;i<depotNum;i++){
        all.push_back(depots[i]);
    }
    
    for(int i=0;i<deliNum;i++){
        bool all_paths_found = false;
        Node* source = getNodebyID(DO[i]);
        all_paths_found = one_source_multi_dest(source, all, turn_penalty);
        vector<pair<pathInfo,pathInfo>> one_DO_to_all_points;
        vector<pathInfo> one_DO_to_all_depots;
        if(all_paths_found){
            for(int j =0; (j<deliNum)&& (j<depotNum);j++){
                if(j<deliNum){
                    path one_do_to_one_pu = convertListToVec(bfsTraceback(PU[i]));
                    path one_do_to_one_do = convertListToVec(bfsTraceback(DO[i]));
                    pathInfo to_one_pu(one_do_to_one_pu, compute_path_travel_time(one_do_to_one_pu, turn_penalty));
                    pathInfo to_one_do(one_do_to_one_do, compute_path_travel_time(one_do_to_one_do, turn_penalty));
                    one_DO_to_all_points.push_back(make_pair(to_one_pu, to_one_do));
                }
                if(j<depotNum){
                    path one_do_to_one_depot = convertListToVec(bfsTraceback(depots[i]));
                    pathInfo to_one_depot(one_do_to_one_depot, compute_path_travel_time(one_do_to_one_depot, turn_penalty));
                    one_DO_to_all_depots.push_back(to_one_depot);
                
                }
            }
        }
        else{
            reset_nodeTable();
            return false;
        }
        reset_nodeTable();//reset node table for every loop
        if(i<deliNum){
            all_DO_to_other_points.push_back(one_DO_to_all_points);
        }
        if(i<depotNum){
            all_DO_to_depots.push_back(one_DO_to_all_depots);
        }
    }
    return true;
}
    
    
    
bool precompute_all_paths(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty){
    return (make_depot_to_PU(deliveries, depots, turn_penalty)
            && make_PU_to_other_points(deliveries, turn_penalty)
            && make_DO_to_points(deliveries,depots, turn_penalty));
}