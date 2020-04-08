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
    //double best_time = WORST_TIME;
    //double current_time = 0;
    float current_weight = 0;
    int pickup_num = 0;
    int prev_num = 0;
    int depot_num = 0;
    double current_dist = 0;
    double best_dist = LARGEST_DISTANCE;
    
    
    // Calculate shortest time between each depot and each pickup
    for (int i=0; i<depots.size(); i++){
        LatLon depot_pos = getIntersectionPosition(depots[i]);
        
        for (int j=0; j<deliveries.size(); j++){
            current_dist = find_distance_between_two_points(make_pair(depot_pos, getIntersectionPosition(deliveries[j].pickUp)));
            //if (bfsPath(sourceNode, deliveries[j].pickUp, turn_penalty)){
                // current_time = getNodebyID(deliveries[j].pickUp)->bestTime;
               // if (current_time < best_time){
                if (current_dist < best_dist){
                    // best_time = current_time;
                    best_dist = current_dist;
                    //current_path = bfsTraceback(deliveries[j].pickUp);
                    //current_weight = deliveries[j].itemWeight;
                    pickup_num = j; // Note that this pickup location was visited
                    depot_num = i;
                    prev_num = j;
                }
            //}
            //reset_nodeTable(); // reset nodetable for correct calculations in next path
        }
    }
    Node* sourceNode = getNodebyID(depots[depot_num]);
    if (bfsPath(sourceNode, deliveries[pickup_num].pickUp, turn_penalty)){
        current_weight = deliveries[pickup_num].itemWeight;
        current_path = bfsTraceback(deliveries[pickup_num].pickUp);
        reset_nodeTable();
    }
    /*if (bfsPath(getNodebyID(depots[0]), deliveries[0].pickUp, turn_penalty)){
        current_path = bfsTraceback(deliveries[0].pickUp);
        depot_num = 0;
        pickup_num = 0;
        prev_num = 0;
    }*/
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
        //best_time = WORST_TIME;
        best_dist = LARGEST_DISTANCE;
        Node* subpath_start = getNodebyID(courier_path.end_intersection);     // Start of subpath is the last visited location in path
        LatLon last_pos = getIntersectionPosition(courier_path.end_intersection);
        //float new_weight = 0;
        for (int i=0; i < deliveries.size(); i++){
            
            //reset_nodeTable();
            // if the delivery has not been picked up
            if (!pickup_checklist[i] && ((current_weight + deliveries[i].itemWeight) < truck_capacity)){
                /*if (bfsPath(subpath_start, deliveries[i].pickUp, turn_penalty)){
                    current_time = getNodebyID(deliveries[i].pickUp)->bestTime;
                    if (current_time < best_time){
                        best_time = current_time;
                        new_weight = (deliveries[i]).itemWeight;
                        current_path = bfsTraceback(deliveries[i].pickUp);
                        pickup_num = i;
                    }
                }*/
                current_dist = find_distance_between_two_points(make_pair(last_pos, getIntersectionPosition(deliveries[i].pickUp)));
                if (current_dist < best_dist){
                    //if (bfsPath(subpath_start, deliveries[i].pickUp, turn_penalty)){
                        best_dist = current_dist;
                    //    new_weight = deliveries[i].itemWeight;
                   //    current_path = bfsTraceback(deliveries[i].pickUp);
                        pickup_num = i;
                    //}
                }
                //reset_nodeTable();
            }
            // if delivery has been picked up but not dropped off    
            else if (pickup_checklist[i] && !dropoff_checklist[i]){
                /*if (bfsPath(subpath_start, deliveries[i].dropOff, turn_penalty)){
                    current_time = getNodebyID(deliveries[i].dropOff)->bestTime;
                    if (current_time < best_time){
                        best_time = current_time;
                        new_weight = (deliveries[i]).itemWeight * -1;
                        current_path = bfsTraceback(deliveries[i].dropOff);
                        pickup_num = (i+1)*(-1); // dropoff is indicated by negative pickup_num
                    }
                }*/
                current_dist = find_distance_between_two_points(make_pair(last_pos, getIntersectionPosition(deliveries[i].dropOff)));
                if (current_dist < best_dist){
                    //if (bfsPath(subpath_start, deliveries[i].dropOff, turn_penalty)){
                        best_dist = current_dist;
                        //new_weight = deliveries[i].itemWeight * (-1);
                        //current_path = bfsTraceback(deliveries[i].dropOff);
                        pickup_num = (i+1) * (-1);
                    //}
                // reset_nodeTable();        
                }
            }
        }
        
        //current_weight += new_weight; // update truck weight
        // add subpath to path
        courier_path.subpath.clear(); // clears previous subpath
        
        courier_path.start_intersection = courier_path.end_intersection;
        
        
        // update checklists accordingly
        if (pickup_num < 0) {
            int dropoff_num = pickup_num * (-1) -1;
            dropoff_checklist[dropoff_num] = true;
            current_weight -= deliveries[dropoff_num].itemWeight;
            if (bfsPath(subpath_start, deliveries[dropoff_num].dropOff, turn_penalty)) current_path = bfsTraceback(deliveries[dropoff_num].dropOff);
            else current_path = {};

            courier_path.end_intersection = deliveries[dropoff_num].dropOff;
        } else { 
            pickup_checklist[pickup_num] = true;
            //courier_path.pickUp_indices = {prev_num};
            courier_path.end_intersection = deliveries[pickup_num].pickUp;
            current_weight += deliveries[pickup_num].itemWeight;
            if (bfsPath(subpath_start, deliveries[pickup_num].pickUp, turn_penalty)) current_path = bfsTraceback(deliveries[pickup_num].pickUp);
            else current_path = {};
        }
        reset_nodeTable();
        
        list_size = current_path.size();
        for (int i=0; i < list_size; i++){
            courier_path.subpath.push_back(current_path.front());
            current_path.pop_front();
        }
        
        if (prev_num < 0){
            courier_path.pickUp_indices = {};
        } else { courier_path.pickUp_indices = {prev_num}; }
        prev_num = pickup_num;
        return_path.push_back(courier_path);
    }
    
    
    // now find paths back to depot
    Node* last_dropoff = getNodebyID(courier_path.end_intersection);
    LatLon last_pos = getIntersectionPosition(courier_path.end_intersection);
    best_dist = LARGEST_DISTANCE;
    //best_time = WORST_TIME;
    for (int i=0; i<depots.size(); i++){

        //if (bfsPath(last_dropoff, depots[i], turn_penalty)){
            //current_time = getNodebyID(depots[i])->bestTime;
            //if (current_time < best_time){
        current_dist = find_distance_between_two_points(make_pair(last_pos, getIntersectionPosition(depots[i])));
            //    best_time = current_time;
        if (current_dist < best_dist){
                //current_path = bfsTraceback(depots[i]);
            best_dist = current_dist;    
            depot_num = i;
            
        }
        //reset_nodeTable();
    }
    if (bfsPath(last_dropoff, depots[depot_num], turn_penalty)) current_path = bfsTraceback(depots[depot_num]);
    else current_path = {};
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
    
    
//    bool check; 
//    std::vector<std::vector<int>> order;
//    order = makeDeliveryOrder(return_path, deliveries);
//    
//    int deliverySize = deliveries.size();
//    check = checkOrderLegal(order, truck_capacity, deliverySize, deliveries);
//    
//    cout<<"Check: "<<check<<endl;
    deleteNodeTable();
    return return_path;
}

bool all_dropped_off(vector<bool> v){
    if (find(begin(v), end(v), false) == end(v)) return true;
    else return false;
}


////////////////////////////////////////////////////////////////////////////////
//MJ'S FUNCTIONS
//
//bool opt_two(std::vector<CourierSubpath> solutionPath, std::vector<DeliveryInfo> deliveryOpt)
//{
//    
//    double originalTime = findPathTime(solutionPath);
//    
//    std::vector<std::vector<int>> deliveryOrderOpt;
//    
//    deliveryOrderOpt = makeDeliveryOrder(solutionPath, deliveryOpt);
//    
//    //dropOffIndices = makeDropOffIndices(solutionPath);
//    int size = deliveryOpt.size();
//    
//    for(int p = 1; p < deliveryOrderOpt.size(); p++){
//        
//        if (checkForNoDoubles(p, deliveryOrderOpt)){
//            
//            //Switch Places.
//            std::vector<int> temp = deliveryOrderOpt[p];
//            deliveryOrderOpt[p] = deliveryOrderOpt[p+2];
//            deliveryOrderOpt[p+2] = temp;
//            
//            //Check for legality
//            if(checkOrderLegal(deliveryOrderOpt, 0, size, deliveryOpt)){    
//                //int index = deliveryOrderOpt[p][0];
//                int intersect;
//                
//                //Swaping whole paths (dont think this is possible because we're changing the paths completely)
////            std::vector<int> temp = solutionPath[p].subPath;
////            solutionPath[p].subPath = solutionPath[p+2].subPath;
////            solutionPath[p+2].subPath = temp;
//                
//                //Swapping the middle's to and from
//                intersect = solutionPath[p+1].end_intersection;
//                solutionPath[p+1].end_intersection = solutionPath[p+1].start_intersection;
//                solutionPath[p+1].start_intersection = intersect;
//                
//                CourierSubpath o = solutionPath[p-1];
//                CourierSubpath a = solutionPath[p];
//                CourierSubpath b = solutionPath[p+1];
//                CourierSubpath c = solutionPath[p+2];
//                CourierSubpath z = solutionPath[p+3];
//                
//                
//                b.start_intersection = c.end_intersection;
//                b.end_intersection = a.start_intersection;
//                a.start_intersection = o.end_intersection;
//                a.end_intersection = c.end_intersection;
//                c.end_intersection = z.start_intersection;
//                
//                
////                solutionPath[p-1].end_intersection = solutionPath[p+1].start_intersection;
////                solutionPath[p-1].end_intersection = solutionPath[p].start_intersection;
////                solutionPath[p].end_intersection = solutionPath[p+1].start_intersection;
////                solutionPath[p+2].start_intersection = solutionPath[p+1].end_intersection;
////                solutionPath[p+2].end_intersection = solutionPath[p+3].start_intersection;
//                
//                std::vector<int> newPath = find_path_between_intersections(a.start_intersection, a.end_intersection, 0);
//                double newTime = compute_path_travel_time(newPath, 0);
//                
//                if(newTime > originalTime){
//                    return true;
//                }return false;
//                
//            }
//            
//        }
//        
//    }
//    return false;
//}
//
//double findPathTime(std::vector<CourierSubpath> solutionPath)
//{
//    double originalTime =0; 
//    
//    for(int i = 0; i< solutionPath.size(); i++){
//        originalTime += compute_path_travel_time(solutionPath[i].subpath, 0);
//    }
//    
//    return originalTime;
//}
//
//std::vector<std::vector<int>> makeDeliveryOrder(std::vector<CourierSubpath> solutionPath, std::vector<DeliveryInfo> deliveryOpt )
//{
//    
//    std::vector<std::vector<int>> deliveryOrderOpt;
//    
//    std::vector<std::vector<int>> dropOff_indices;
//    
//    dropOff_indices = makeDropOffIndices(solutionPath, deliveryOpt);
//    
//    int size = solutionPath.size();
//    
//    deliveryOrderOpt.resize(size);
//    
//    deliveryOrderOpt[0].push_back(solutionPath[0].start_intersection); //First Depot
//    
//    
//    for(int i = 0 ; i < solutionPath.size() ; i++){
//        
//        std::vector<unsigned> pickUp;
//        pickUp = solutionPath[i].pickUp_indices;
//        
//        for(int j = 0; j< pickUp.size() ; j++){
//                
//            if(pickUp[j] == 0){
//                pickUp[j] = 0;
//            }
//            
//            deliveryOrderOpt[i].push_back((-1)*pickUp[j]);
//        }
//        
//        std::vector<int> dropOff;
//        dropOff = dropOff_indices[i];
//        
//        for(int j = 0; j< dropOff.size() ; j++){
//            
//            if(dropOff[j] == 0){
//                dropOff[j] = 0;
//            }
//            deliveryOrderOpt[i].push_back(dropOff[j]);
//        }
//    }
//    
//    deliveryOrderOpt[size-1].push_back(solutionPath[size-1].end_intersection); //Last Depot
//    
//    return deliveryOrderOpt;
//}
//
//bool checkForNoDoubles(int index, std::vector<std::vector<int>> order)
//{
//    
//    for( int i = index; i< index+3 ; i++ ){
//        for( int j = 0 ; j< order[i].size(); j++){
//            for(int k = 0; k< order[i].size(); k++){
//                if(order[i][j] == order[i][k]){
//                    return false;
//                }
//            }
//        }
//    }
//    
//    return true;
//            
//}
//
//
//bool checkOrderLegal(std::vector<std::vector<int>> order, double truckCapacity, int size, std::vector<DeliveryInfo> deliveryOpt)
//{
//    
//    double truckLoad=0;
//    std::vector<int> truckPackages;
//    truckPackages.resize(size);
//    double weightPick = 0;
//    double weightDrop = 0;
//    
//    for(int i = 0; i< order.size(); i++){
//        
//        for(int k = 0; k< order[i].size() ; k++ ){
//            
//            if(order[i][k]<0){ //Pick up
//            
//                int index = order[i][k];
//                weightPick = deliveryOpt[index].itemWeight;
//
//                if((weightPick + truckLoad) >truckCapacity){
//                    return false;
//                }else{
//                    truckLoad += weightPick;
//                    truckPackages[index] = 1; //the one indicates that it is in the truck
//                }
//            }
//            
//            //WHAT HAPPENS FOR DROP OFFS
//            else{
//                int index = order[i][k];
//                weightDrop = deliveryOpt[index].itemWeight;
//
//                if(truckPackages[index]){ //Package is in the truck
//                    truckCapacity -= weightDrop;
//                    truckPackages[index] = 0;
//                }else{
//                    //If not found in the truck
//                    return false;
//                }
//            }
//        }
//    }
//    return true;
//}
//
//std::vector<std::vector<int>> makeDropOffIndices(std::vector<CourierSubpath> solution, std::vector<DeliveryInfo> deliveryOpt)
//{
//   
//    std::vector<std::vector<int>> dropOff_indices;
//    
//    for(int i = 0; i< solution.size(); i++){
//        
//        int startIntersection = solution[i].start_intersection;
//        
//        for(int j =0; j<deliveryOpt.size(); j++){
//            if(startIntersection == deliveryOpt[j].dropOff){
//                dropOff_indices[i].push_back(j); //Add drop off item.
//            }
//        }
//    }
//    return dropOff_indices;
//}
