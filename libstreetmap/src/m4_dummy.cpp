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
#include "m4.h"
#include "m4_global.hpp"

using namespace std;
// Helper
bool all_dropped_off(vector<bool> v);
//

std::vector<CourierSubpath> traveling_courier(
        const vector<DeliveryInfo>& deliveries,
        const vector<int>& depots,
        const float turn_penalty,
        const float truck_capacity){
    // pesudo code:
    // iterate through all depots
    // calculate all travel times between each depot and each pickup
    // take shortest
    // use shortest to calculate to next legal index
    // when all pickups and dropoffs are complete, return to depot
    //vector<StreetSegmentIndex> courier_path;            // path to be returned
    list<StreetSegmentIndex> current_path;    // path currently being calculated
    vector<CourierSubpath> return_path;
    
    makeNodeTable();
    double best_time = WORST_TIME;
    double current_time;
    float current_weight;
    int pickup_num;
    int prev_num;
    int depot_num;
    
    // Calculate shortest time between each depot and each pickup
    /*for (int i=0; i<depots.size(); i++){
        Node* sourceNode = getNodebyID(depots[i]);
        for (int j=0; j<deliveries.size(); j++){
            if (bfsPath(sourceNode, deliveries[j].pickUp, turn_penalty)){
                current_time = getNodebyID(deliveries[j].pickUp)->bestTime;
                if (current_time < best_time){
                    best_time = current_time;
                    current_path = bfsTraceback(deliveries[j].pickUp);
                    current_weight = deliveries[j].itemWeight;
                    pickup_num = j; // Note that this pickup location was visited
                    depot_num = i;
                }
            }
            reset_nodeTable(); // reset nodetable for correct calculations in next path
        }
    }*/
    if (bfsPath(getNodebyID(depots[0]), deliveries[0].pickUp, turn_penalty)){
        current_path = bfsTraceback(deliveries[0].pickUp);
        depot_num = 0;
        pickup_num = 0;
        prev_num = 0;
    }
    // vector<StreetSegmentIndex> v{make_move_iterator(begin(current_path)), make_move_iterator(end(current_path))}; // Convert list to vector
    // Add all elements of list into the path vector
    int list_size = current_path.size();
    CourierSubpath courier_path;
    for (int i=0; i < list_size; i++){
        (courier_path.subpath).push_back(current_path.front());
        current_path.pop_front();
    }

    courier_path.start_intersection = depots[depot_num];
    courier_path.end_intersection = deliveries[pickup_num].pickUp;
    courier_path.pickUp_indices = {};        
    
    return_path.push_back(courier_path);
    
    vector<bool> pickup_checklist;
    vector<bool> dropoff_checklist;
    
    for (int i=0; i < deliveries.size(); i++){
        pickup_checklist.push_back(false);
        dropoff_checklist.push_back(false);
    }
    pickup_checklist[pickup_num] = true;    // update status of first pickup
    
    // Calculate shortest time between fastest pickup and next legal stop
    // Repeat this until no more deliveries are left
    while (!all_dropped_off(dropoff_checklist)){
        best_time = WORST_TIME;
        Node* subpath_start = getNodebyID(courier_path.end_intersection);     // Start of subpath is the last visited location in path
        float new_weight;
        for (int i=0; i < deliveries.size(); i++){
            
            reset_nodeTable();
            // if the delivery has not been picked up
            if (!pickup_checklist[i] && (current_weight + deliveries[i].itemWeight < truck_capacity)){
                if (bfsPath(subpath_start, deliveries[i].pickUp, turn_penalty)){
                    current_time = getNodebyID(deliveries[i].pickUp)->bestTime;
                    if (current_time < best_time){
                        best_time = current_time;
                        new_weight = (deliveries[i]).itemWeight;
                        current_path = bfsTraceback(deliveries[i].pickUp);
                        pickup_num = i;
                    }
                }
                reset_nodeTable();
            }
            // if delivery has been picked up but not dropped off    
            else if (pickup_checklist[i] && !dropoff_checklist[i]){
                if (bfsPath(subpath_start, deliveries[i].dropOff, turn_penalty)){
                    current_time = getNodebyID(deliveries[i].dropOff)->bestTime;
                    if (current_time < best_time){
                        best_time = current_time;
                        new_weight = (deliveries[i]).itemWeight * -1;
                        current_path = bfsTraceback(deliveries[i].dropOff);
                        pickup_num = (i+1)*(-1); // dropoff is indicated by negative pickup_num
                    }
                }
                reset_nodeTable();               
            }
        }
        
        current_weight += new_weight; // update truck weight
        // add subpath to path
        courier_path.subpath.clear(); // clears previous subpath
        list_size = current_path.size();
        for (int i=0; i < list_size; i++){
            courier_path.subpath.push_back(current_path.front());
            current_path.pop_front();
        }
        courier_path.start_intersection = courier_path.end_intersection;
        
        
        // update checklists accordingly
        if (pickup_num < 0) {
            int dropoff_num = pickup_num * (-1) -1;
            dropoff_checklist[dropoff_num] = true;

            courier_path.end_intersection = deliveries[dropoff_num].dropOff;
        } else { 
            pickup_checklist[pickup_num] = true;
            //courier_path.pickUp_indices = {prev_num};
            courier_path.end_intersection = deliveries[pickup_num].pickUp;
        }
        if (prev_num < 0){
            courier_path.pickUp_indices = {};
        } else { courier_path.pickUp_indices = {prev_num}; }
        prev_num = pickup_num;
        return_path.push_back(courier_path);
    }
    
    
    // now find paths back to depot
    Node* last_dropoff = getNodebyID(courier_path.end_intersection);
    best_time = WORST_TIME;
    for (int i=0; i<depots.size(); i++){
        if (bfsPath(last_dropoff, depots[i], turn_penalty)){
            current_time = getNodebyID(depots[i])->bestTime;
            if (current_time < best_time){
                best_time = current_time;
                current_path = bfsTraceback(depots[i]);
                depot_num = i;
            }
        }
        reset_nodeTable();
    }
    // add subpath to path
    courier_path.subpath.clear();
    list_size = current_path.size();
    for (int i=0; i < list_size; i++){
        courier_path.subpath.push_back(current_path.front());
        current_path.pop_front();
    }
    courier_path.pickUp_indices = {};
    courier_path.start_intersection = courier_path.end_intersection;
    courier_path.end_intersection = depots[depot_num];
    
    return_path.push_back(courier_path);
    
    return return_path;
}

bool all_dropped_off(vector<bool> v){
    if (find(begin(v), end(v), false) == end(v)) return true;
    else return false;
}