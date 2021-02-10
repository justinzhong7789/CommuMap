///*
// * To change this license header, choose License Headers in Project Properties.
// * To change this template file, choose Tools | Templates
// * and open the template in the editor.
// */
//
//#include "StreetsDatabaseAPI.h"
//#include "m1.h"
//#include "m2.h"
//#include "m3.h"
//#include "m3_global.hpp"
//#include <vector>
//#include <list>
//#include <string>
//#include <iostream>
//#include <chrono>
//#include "m4.h"
//#include "m4_global.hpp"
//
//using namespace std;
//// Helperb
//
//bool nested_sort (const vector<double> &a, const vector<double> &b);
//bool all_dropped_off(vector<bool> v);
//
//vector<vector<double>> find_depot_first_pickup(const vector<DeliveryInfo>& deliveries,
//        const vector<int>& depots);
//
//pair<int, bool> find_next_stop (const vector<DeliveryInfo>& deliveries,
//        const vector<int>& depots, IntersectionIndex last_intersection,
//        vector<bool> pickup_checklist, vector<bool> dropoff_checklist,
//        float current_weight, float truck_capacity);
//
//int find_best_depot (const vector<int>& depots, IntersectionIndex last_intersection);
//
//
//double total_time_1 = 0;
//double total_time_2 = 0;
//
//std::vector<CourierSubpath> traveling_courier(
//        const vector<DeliveryInfo>& deliveries,
//        const vector<int>& depots,
//        const float turn_penalty,
//        const float truck_capacity){
//     
//    auto start_time = chrono::high_resolution_clock::now();
//    bool timeOut = false;
//    
//    list<StreetSegmentIndex> current_path;    
//    vector<CourierSubpath> return_path_1, return_path_2;
//    CourierSubpath best_courier_path, second_courier_path;
//    bool two_depots_plus = false;
//        
//    //makeNodeTable();
//    // Calculate shortest time between each depot and each pickup
//    /*for (int i=0; i<depots.size(); i++){
//        LatLon depot_pos = getIntersectionPosition(depots[i]);
//        
//        for (int j=0; j<deliveries.size(); j++){
//            current_dist = find_distance_between_two_points(make_pair(depot_pos, getIntersectionPosition(deliveries[j].pickUp)));         
//            if (current_dist < best_dist){
//                // best_time = current_time;
//                best_dist = current_dist;
//                //current_path = bfsTraceback(deliveries[j].pickUp);
//                //current_weight = deliveries[j].itemWeight;
//                pickup_num = j; // Note that this pickup location was visited
//                depot_num = i;
//                prev_num = j;
//            }
//        }
//    }
//    Node* sourceNode = getNodebyID(depots[depot_num]);
//    if (bfsPath(sourceNode, deliveries[pickup_num].pickUp, turn_penalty)){
//        current_weight = deliveries[pickup_num].itemWeight;
//        current_path = bfsTraceback(deliveries[pickup_num].pickUp);
//        reset_nodeTable();
//    }*/
//    vector<vector<double>> depot_to_first_pickup = find_depot_first_pickup(deliveries, depots);
//    // Initialize starting depot with smallest distance to pickup
//    int best_start_depot = int(depot_to_first_pickup[0][1]);    // Depot index
//    int best_start_pickup = int(depot_to_first_pickup[0][2]);   // Delivery index 
//    Node* best_source = getNodebyID(depots[best_start_depot]);   
//    
//    // Initialize starting depot with second best distance to pickup
//    int second_best_depot;
//    int second_best_pickup;
//    Node* second_source;
//    if (depot_to_first_pickup.size() > 1){
//        two_depots_plus = true;
//        second_best_depot = int(depot_to_first_pickup[1][1]);
//        second_best_pickup = int(depot_to_first_pickup[1][2]);
//        second_source = getNodebyID(depots[second_best_depot]);
//    }
//    // Calculate first subpath
//    best_courier_path.start_intersection = depots[best_start_depot];    // start
//    best_courier_path.end_intersection = deliveries[best_start_pickup].pickUp; // end
//    best_courier_path.pickUp_indices = {};                              // pickup index
//    
//    vector<unsigned> prev_pickup_1 = {best_start_pickup};
//    float current_weight_1 = deliveries[best_start_pickup].itemWeight;
//    // subpath of streetsegmentids
//    if (bfsPath(best_source, best_courier_path.end_intersection, turn_penalty)){
//        current_path = bfsTraceback(best_courier_path.end_intersection); 
//    }
//    total_time_1 = getNodebyID(best_courier_path.end_intersection)->bestTime;    // keep track of times so we can compare at the end
//   // cout << total_time_1 << endl;
//    
//    reset_nodeTable();
//    // Add all elements of list into the path vector
//    int list_size = current_path.size();
//    for (int i=0; i < list_size; i++){
//        (best_courier_path.subpath).push_back(current_path.front());
//        current_path.pop_front();
//    }
//    // Depot to first pickup finished
//    return_path_1.push_back(best_courier_path);
//    
//    float current_weight_2 = 0;
//    vector<unsigned> prev_pickup_2 ;
//    if (two_depots_plus){
//        // Calculate second subpath
//        second_courier_path.start_intersection = depots[second_best_depot];
//        second_courier_path.end_intersection = deliveries[second_best_pickup].pickUp;
//        best_courier_path.pickUp_indices = {};
//
//        prev_pickup_2 = {second_best_pickup};
//        current_weight_2 = deliveries[second_best_pickup].itemWeight;
//        // subpath
//        if (bfsPath(second_source, second_courier_path.end_intersection, turn_penalty)){
//            current_path = bfsTraceback(second_courier_path.end_intersection);
//        }
//        total_time_2 = getNodebyID(second_courier_path.end_intersection)->bestTime;
//
//        reset_nodeTable();
//        list_size = current_path.size();
//        for (int i=0; i < list_size; i++){
//            (second_courier_path.subpath).push_back(current_path.front());
//            current_path.pop_front();
//        }
//        return_path_2.push_back(second_courier_path);
//    }
//
// 
//    
//    /////////////////////////////////////////
//    vector<bool> pickup_checklist_1;
//    vector<bool> dropoff_checklist_1;
//    
//    vector<bool> pickup_checklist_2;
//    vector<bool> dropoff_checklist_2;
//    
//    for (int i=0; i < deliveries.size(); i++){
//        pickup_checklist_1.push_back(false);
//        dropoff_checklist_1.push_back(false);
//        
//        pickup_checklist_2.push_back(false);
//        dropoff_checklist_2.push_back(false);
//    }
//    pickup_checklist_1[best_start_pickup] = true;    // update status of first pickup
//    
//    if (two_depots_plus) pickup_checklist_2[second_best_pickup] = true;
//    
//    // Calculate shortest time between fastest pickup and next legal stop
//    // Repeat this until no more deliveries are left
//    while (!all_dropped_off(dropoff_checklist_1)){
//        pair<int, bool> next = find_next_stop(deliveries, depots, best_courier_path.end_intersection,
//            pickup_checklist_1, dropoff_checklist_1, current_weight_1, truck_capacity);
//            
//        best_courier_path.start_intersection = best_courier_path.end_intersection;
//        best_courier_path.pickUp_indices = prev_pickup_1;
//        best_courier_path.subpath.clear();
//        
//        Node* subpath_start = getNodebyID(best_courier_path.start_intersection);
//        
//        if (next.second){   // If next stop is a pickup           
//            if (bfsPath(subpath_start, deliveries[next.first].pickUp, turn_penalty)){
//                current_path = bfsTraceback(deliveries[next.first].pickUp);
//                pickup_checklist_1[next.first] = true;
//                current_weight_1 += deliveries[next.first].itemWeight;
//                prev_pickup_1 = {next.first};
//                best_courier_path.end_intersection = deliveries[next.first].pickUp;
//            }
//        }
//        else {          // If next stop is a dropoff
//            if (bfsPath(subpath_start, deliveries[next.first].dropOff, turn_penalty)){
//                current_path = bfsTraceback(deliveries[next.first].dropOff);
//                dropoff_checklist_1[next.first] = true;
//                current_weight_1 -= deliveries[next.first].itemWeight;
//                prev_pickup_1 = {};
//                best_courier_path.end_intersection = deliveries[next.first].dropOff;
//            }
//        }
//        total_time_1 += getNodebyID(best_courier_path.end_intersection)->bestTime;   
//      //  cout << total_time_1 << endl;
//        reset_nodeTable();
//
//        // Adding subpath          
//        list_size = current_path.size();
//        for (int i=0; i < list_size; i++){
//            best_courier_path.subpath.push_back(current_path.front());
//            current_path.pop_front();     
//        }
//        
//        return_path_1.push_back(best_courier_path);
//    }
//    
//    if (two_depots_plus){
//    // Now calculate for second_best_start path
//    while (!all_dropped_off(dropoff_checklist_2)){
//        pair<int, bool> next = find_next_stop(deliveries, depots, best_courier_path.end_intersection,
//            pickup_checklist_2, dropoff_checklist_2, current_weight_2, truck_capacity);
//            
//        second_courier_path.start_intersection = second_courier_path.end_intersection;
//        second_courier_path.pickUp_indices = prev_pickup_2;
//        second_courier_path.subpath.clear();
//        
//        Node* subpath_start = getNodebyID(second_courier_path.start_intersection);
//        
//        if (next.second){   // If next stop is a pickup           
//            if (bfsPath(subpath_start, deliveries[next.first].pickUp, turn_penalty)){
//                current_path = bfsTraceback(deliveries[next.first].pickUp);
//                pickup_checklist_2[next.first] = true;
//                current_weight_2 += deliveries[next.first].itemWeight;
//                prev_pickup_1 = {next.first};
//                second_courier_path.end_intersection = deliveries[next.first].pickUp;
//            }
//        }
//        else {          // If next stop is a dropoff
//            if (bfsPath(subpath_start, deliveries[next.first].dropOff, turn_penalty)){
//                current_path = bfsTraceback(deliveries[next.first].dropOff);
//                dropoff_checklist_2[next.first] = true;
//                current_weight_2 -= deliveries[next.first].itemWeight;
//                prev_pickup_2 = {};
//                second_courier_path.end_intersection = deliveries[next.first].dropOff;
//            }
//        }
//        total_time_2 += getNodebyID(best_courier_path.end_intersection)->bestTime;  
//        reset_nodeTable();
//
//        // Adding subpath          
//        list_size = current_path.size();
//        for (int i=0; i < list_size; i++){
//            second_courier_path.subpath.push_back(current_path.front());
//            current_path.pop_front();     
//        }
//        
//        return_path_2.push_back(second_courier_path);
//    }
//    }
//    // Now calculate distance from last dropoff point to depot
//    // for best depot path
//    Node* last_dropoff_1 = getNodebyID(best_courier_path.end_intersection);
//
//    int best_depot_id = find_best_depot(depots, best_courier_path.end_intersection);
//    if (bfsPath(last_dropoff_1, depots[best_depot_id], turn_penalty)) current_path = bfsTraceback(depots[best_depot_id]);
//    total_time_1 += getNodebyID(best_courier_path.end_intersection)->bestTime;  
//    
//    reset_nodeTable();
//    
//    // add subpath to path
//    best_courier_path.subpath.clear();
//    list_size = current_path.size();
//    for (int i=0; i < list_size; i++){
//        best_courier_path.subpath.push_back(current_path.front());
//        current_path.pop_front();
//    }
//    best_courier_path.pickUp_indices = {};
//    best_courier_path.start_intersection = best_courier_path.end_intersection;
//    best_courier_path.end_intersection = depots[best_depot_id];
//    
//    return_path_1.push_back(best_courier_path);
//    
//    //////////////////////////////////////////
//    if (two_depots_plus){
//    // this is for second best depot path
//    Node* last_dropoff_2 = getNodebyID(second_courier_path.end_intersection);
//
//    int second_depot_id = find_best_depot(depots, second_courier_path.end_intersection);
//    if (bfsPath(last_dropoff_2, depots[second_depot_id], turn_penalty)) current_path = bfsTraceback(depots[second_depot_id]);
//    total_time_2 += getNodebyID(best_courier_path.end_intersection)->bestTime;  
//    
//    reset_nodeTable();   
//    // add subpath to path
//    second_courier_path.subpath.clear();
//    list_size = current_path.size();
//    for (int i=0; i < list_size; i++){
//        second_courier_path.subpath.push_back(current_path.front());
//        current_path.pop_front();
//    }
//    second_courier_path.pickUp_indices = {};
//    second_courier_path.start_intersection = second_courier_path.end_intersection;
//    second_courier_path.end_intersection = depots[second_depot_id];
//    
//    return_path_2.push_back(second_courier_path);
//    }
//    //deleteNodeTable();
//    
//    
//    
//    
//    
////    cout << "Best depot start time is: " << total_time_1 << endl;
////    cout << "Second depot start time is: " << total_time_2 << endl;
//    
//    return return_path_1;
//         
//        //while (!timeOut) {
//        // auto current_time = chrono::high_resolution::clock()::now()
//        // auto wall_clock = chrono::duration_cast<chrono::duration<double>>(current_time - start_time);
//        //can do opt2 here}   
//}
//        
//        /*best_dist = LARGEST_DISTANCE;
//        Node* best_subpath_start = getNodebyID(courier_path.end_intersection);     // Start of subpath is the last visited location in path
//        LatLon last_pos = getIntersectionPosition(courier_path.end_intersection);
//        for (int i=0; i < deliveries.size(); i++){
//            
//            // if the delivery has not been picked up
//            if (!pickup_checklist[i] && ((current_weight + deliveries[i].itemWeight) < truck_capacity)){
//                current_dist = find_distance_between_two_points(make_pair(last_pos, getIntersectionPosition(deliveries[i].pickUp)));
//                if (current_dist < best_dist){
//                    best_dist = current_dist;
//                    pickup_num = i;
//                }
//            }
//            // if delivery has been picked up but not dropped off    
//            else if (pickup_checklist[i] && !dropoff_checklist[i]){
//                current_dist = find_distance_between_two_points(make_pair(last_pos, getIntersectionPosition(deliveries[i].dropOff)));
//                if (current_dist < best_dist){
//                    best_dist = current_dist;
//                    pickup_num = (i+1) * (-1);
//    
//                }
//            }
//        }
//        
//        // add subpath to path
//        courier_path.subpath.clear(); // clears previous subpath
//        
//        courier_path.start_intersection = courier_path.end_intersection;
//        
//        
//        // update checklists accordingly
//        if (pickup_num < 0) {
//            int dropoff_num = pickup_num * (-1) -1;
//            dropoff_checklist[dropoff_num] = true;
//            current_weight -= deliveries[dropoff_num].itemWeight;
//            if (bfsPath(subpath_start, deliveries[dropoff_num].dropOff, turn_penalty)) current_path = bfsTraceback(deliveries[dropoff_num].dropOff);
//            else current_path = {};
//
//            courier_path.end_intersection = deliveries[dropoff_num].dropOff;
//        } else { 
//            pickup_checklist[pickup_num] = true;
//            //courier_path.pickUp_indices = {prev_num};
//            courier_path.end_intersection = deliveries[pickup_num].pickUp;
//            current_weight += deliveries[pickup_num].itemWeight;
//            if (bfsPath(subpath_start, deliveries[pickup_num].pickUp, turn_penalty)) current_path = bfsTraceback(deliveries[pickup_num].pickUp);
//            else current_path = {};
//        }
//        reset_nodeTable();
//        
//        list_size = current_path.size();
//        for (int i=0; i < list_size; i++){
//            courier_path.subpath.push_back(current_path.front());
//            current_path.pop_front();
//        }
//        
//        if (prev_num < 0){
//            courier_path.pickUp_indices = {};
//        } else { courier_path.pickUp_indices = {prev_num}; }
//        
//        prev_num = pickup_num;
//        return_path_1.push_back(courier_path);
//        
//        //while (!timeOut) {
//        // auto current_time = chrono::high_resolution::clock()::now()
//        // auto wall_clock = chrono::duration_cast<chrono::duration<double>>(current_time - start_time);
//        //can do opt2 here}
//    }*/
//    
//    /*
//    // now find paths back to depot
//    Node* last_dropoff = getNodebyID(courier_path.end_intersection);
//    LatLon last_pos = getIntersectionPosition(courier_path.end_intersection);
//    best_dist = LARGEST_DISTANCE;
//    //best_time = WORST_TIME;
//    for (int i=0; i<depots.size(); i++){
//        current_dist = find_distance_between_two_points(make_pair(last_pos, getIntersectionPosition(depots[i])));
//        if (current_dist < best_dist){
//            best_dist = current_dist;    
//            depot_num = i;
//            
//        }
//    }
//    if (bfsPath(last_dropoff, depots[depot_num], turn_penalty)) current_path = bfsTraceback(depots[depot_num]);
//    else current_path = {};
//    
//    // add subpath to path
//    courier_path.subpath.clear();
//    list_size = current_path.size();
//    for (int i=0; i < list_size; i++){
//        courier_path.subpath.push_back(current_path.front());
//        current_path.pop_front();
//    }
//    courier_path.pickUp_indices = {};
//    courier_path.start_intersection = courier_path.end_intersection;
//    courier_path.end_intersection = depots[depot_num];
//    
//    return_path_1.push_back(courier_path);
//    
//    deleteNodeTable();
//    return return_path_1;
//}*/
//
//bool all_dropped_off(vector<bool> v){
//    if (find(begin(v), end(v), false) == end(v)) return true;
//    else return false;
//}
//
//// returns distances from best to worst. first in pair is depot num, second is corresponding closest pickup 
//vector<vector<double>> find_depot_first_pickup(const vector<DeliveryInfo>& deliveries,
//        const vector<int>& depots){
//    
//    vector <double> depot_pickup_ids;  // Inner vector return
//    vector <vector<double>> outer_vec;         // Outer vector return
//    
//    double best_pickup;
//    double euclid_dist, best_dist;
//    double depots_num = depots.size();
//    double deliveries_num = deliveries.size();
//    
//    // First index is the distance, second is the depot id, third is the pickup id
//    depot_pickup_ids.resize(3);
//    
//    for (double depot_id = 0; depot_id < depots_num; depot_id++){
//        best_dist = LARGEST_DISTANCE;
//        LatLon depot_pos = getIntersectionPosition(depots[depot_id]);
//        for (double pickup_id = 0; pickup_id < deliveries_num; pickup_id++){
//            LatLon pickup_pos = getIntersectionPosition(deliveries[pickup_id].pickUp);
//            euclid_dist = find_distance_between_two_points(make_pair(depot_pos, pickup_pos));
//            
//            if (euclid_dist < best_dist){
//                best_pickup = pickup_id;
//                best_dist = euclid_dist;
//            }
//            
//        }
//        // Update the best depot-to-first-pickup path to the vector
//        depot_pickup_ids[0] = best_dist;
//        depot_pickup_ids[1] = depot_id;
//        depot_pickup_ids[2] = best_pickup;
//        
//        outer_vec.push_back(depot_pickup_ids);
//    }
//    // Sort this function in ascending order (min distance is first element)
//    sort(outer_vec.begin(), outer_vec.end(), nested_sort);   
//    return outer_vec;
//}
//
//bool nested_sort (const vector<double> &a, const vector<double> &b){
//    return a[0] < b[0];
//}
//
//// Returns the next delivery indices and true if its a pickup, false if dropoff
//pair<int, bool> find_next_stop (const vector<DeliveryInfo>& deliveries,
//        const vector<int>& depots, IntersectionIndex last_intersection,
//        vector<bool> pickup_checklist, vector<bool> dropoff_checklist,
//        float current_weight, float truck_capacity){
//    
//    double current_dist;    
//    double best_dist = LARGEST_DISTANCE;
//    int deliveries_num = deliveries.size();
//    
//    int next_stop;
//    bool is_pickup;
//    
//    LatLon last_pos = getIntersectionPosition(last_intersection);
//    
//    for (int delivery_id = 0; delivery_id < deliveries.size(); delivery_id++){      
//        // if the delivery has not been picked up
//        if (!pickup_checklist[delivery_id] && ((current_weight + deliveries[delivery_id].itemWeight) < truck_capacity)){
//            LatLon next_pos = getIntersectionPosition(deliveries[delivery_id].pickUp);
//            current_dist = find_distance_between_two_points(make_pair(last_pos, next_pos));
//            if (current_dist < best_dist){
//                best_dist = current_dist;
//                next_stop = delivery_id;
//                is_pickup = true;
//            }
//        }
//        // if delivery has been picked up but not dropped off    
//        else if (pickup_checklist[delivery_id] && !dropoff_checklist[delivery_id]){
//            current_dist = find_distance_between_two_points(make_pair(last_pos, getIntersectionPosition(deliveries[delivery_id].dropOff)));
//            if (current_dist < best_dist){
//                best_dist = current_dist;
//                next_stop = delivery_id;
//                is_pickup = false;
//            }
//        }
//    }
//    return make_pair(next_stop, is_pickup);
//}
//
//// Returns closest depot index
//int find_best_depot (const vector<int>& depots, IntersectionIndex last_intersection){
//    double current_dist;
//    double best_dist = LARGEST_DISTANCE;
//    int depots_num = depots.size();
//    int best_depot;
//    
//    LatLon last_pos = getIntersectionPosition(last_intersection);
//    
//    for (int depot_id = 0; depot_id < depots_num; depot_id++){
//        LatLon depot_pos = getIntersectionPosition(depots[depot_id]);
//        current_dist = find_distance_between_two_points(make_pair(last_pos, depot_pos));
//        
//        if (current_dist < best_dist){
//            best_dist = current_dist;
//            best_depot = depot_id;
//        }
//    }
//    return best_depot;
//}