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
//#include "m4.h"
//#include "m4_global.hpp"
//
//using namespace std;
//// Helper
//bool all_dropped_off(vector<bool> v);
////
//
//
//std::vector<std::vector<pathInfo>> pathInfoFromPickUps;
//std::vector<std::vector<pathInfo>> pathInfoFromDropOffs;
//std::vector<std::vector<pathInfo>> pathInfoFromDepots;
//
//std::chrono::steady_clock::time_point start;
//std::chrono::steady_clock::time_point testerStart;
//std::chrono::steady_clock::time_point testerEnd;
//
//std::vector<CourierSubpath> traveling_courier(
//		            const std::vector<DeliveryInfo>& deliveries,
//	       	        const std::vector<int>& depots, 
//		            const float turnPenalty, 
//		            const float truckCapacity){
//            
//    
//    
////    double bestTime = computeVectorCourierPathTime(bestPath, turnPenalty);
//    
//    start = std::chrono::steady_clock::now();
//    
//    if(truckCapacity == 0){
//        return std::vector<CourierSubpath> {};
//    }
//      
//    std::vector<CourierSubpath> bestPath = greedyHeuristic1(deliveries, depots, turnPenalty, truckCapacity);
//    
//    // check if there is a valid path, if not return an empty vector
//    if(bestPath.size() == 0){
//        return bestPath;
//    }
//    
//    
//    // find other paths and compare them
//    double bestTime = computeVectorCourierPathTime(bestPath, turnPenalty);
//    
//    
//    //USING DIJKSTRA
//    
////    std::vector<CourierSubpath> anotherPath = travelingCourierDijkstra(deliveries, depots, turnPenalty, truckCapacity);
////    
////    double anotherTime = computeVectorCourierPathTime(anotherPath, turnPenalty);
////    
////    // check if the time is valid
////    if(anotherPath.size() != 0){
////        if(anotherTime < bestTime){
////            bestPath = anotherPath;
////        }
////    }
//    
////    auto end = std::chrono::steady_clock::now();
////    
////    std::cout << std::endl<<"Elapsed time in milliseconds " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl<<std::endl;
//    
//    return bestPath;
//}
//
//
//std::vector<CourierSubpath> greedyHeuristic1(const std::vector<DeliveryInfo>& deliveries,
//	       	        const std::vector<int>& depots, 
//		            const float turnPenalty, 
//		            const float truckCapacity){
//std::vector<CourierSubpath> courierRoute;
//    CourierSubpath path;
//    int numOfDeliveries = deliveries.size();
//    int numOfDeliveriesCompleted = 0;
//    std::vector<int> deliveryStatusVector; // 0 = not picked up, 1 = picked up, 2 = dropped off
//    
//    for(int i = 0; i < numOfDeliveries; ++i){
//        deliveryStatusVector.push_back(0);
//    }
//    
//    // pair of indices in the depot and deliveries vector   
//    std::pair<int,int> startingDepotAndPickUpIndex = findDepotWithClosestPickUp(deliveries, depots);
//    
//    int startingDepotIndex = startingDepotAndPickUpIndex.first;
//    int startingPickUpIndex = startingDepotAndPickUpIndex.second;
//    
//    // path is the path between the depot and its closest pick up point
//    path = updatePathInfo(depots[startingDepotIndex], deliveries[startingPickUpIndex].pickUp, -1, turnPenalty);
//    courierRoute.push_back(path);
//    
//    // update the status vector and weight of the truck
//    deliveryStatusVector[startingPickUpIndex] = 1;
//    double truckWeight = deliveries[startingPickUpIndex].itemWeight;
//    
//    
//    std::pair<int,bool> deliveryIndexAndType;
//    int deliveryIndex = -1;
//    int pickUpIndex = startingPickUpIndex;  // index of pickup
//    bool isDropOff; // 0 = pick up, 1 = drop off 
//    IntersectionIndex sourceIntersection = deliveries[startingPickUpIndex].pickUp;
//    
//    while(numOfDeliveriesCompleted != numOfDeliveries){
//        
//        deliveryIndexAndType = findClosestPickUpOrDropOff(deliveries, sourceIntersection, deliveryStatusVector, truckWeight, truckCapacity);
//        deliveryIndex = deliveryIndexAndType.first;
//        isDropOff = deliveryIndexAndType.second; 
//        
//        if(!isDropOff){
//            // find the path between the sourceIntersection and nearest pickUp
//            // update the variables
//            path = updatePathInfo(sourceIntersection, deliveries[deliveryIndex].pickUp, pickUpIndex, turnPenalty);
//            deliveryStatusVector[deliveryIndex] = 1;
//            sourceIntersection = deliveries[deliveryIndex].pickUp;
//            pickUpIndex = deliveryIndex; //this path's end is a pick up, so next path's start is going to be a pick up so set pickUpIdx
//            truckWeight += deliveries[deliveryIndex].itemWeight;
//        }
//        else{
//            // find the path between the sourceIntersection and nearest dropOff
//            // update the variables
//            path = updatePathInfo(sourceIntersection, deliveries[deliveryIndex].dropOff, pickUpIndex, turnPenalty);
//            deliveryStatusVector[deliveryIndex] = 2;
//            sourceIntersection = deliveries[deliveryIndex].dropOff;
//            numOfDeliveriesCompleted++;
//            pickUpIndex = -1; //this path's end is a drop off, so next path's start is going to be a drop off so don't set pickUpIdx
//            truckWeight -= deliveries[deliveryIndex].itemWeight;
//        }
//        
//        // add the path to the route
//        courierRoute.push_back(path);
//    }
//    
//    // add the path from the last drop off point to its nearest depot
//    // (nearest by Euclidean distance)
//    int lastDropOffIntersection = deliveries[deliveryIndex].dropOff;
//    int closestFinishDepotIndex = findClosestDepot(depots, lastDropOffIntersection);
//    
//    path = updatePathInfo(lastDropOffIntersection, depots[closestFinishDepotIndex], -1, turnPenalty);
//    courierRoute.push_back(path);
//    
//    return courierRoute;
//    
//}
//
///**
// * finds the closest depot by Euclidean distance
// * @param depots
// * @param lastDropOffIntersection
// * @return 
// */
//int findClosestDepot(const std::vector<int> &depots, IntersectionIndex intersectoin){
//    int closestDepotIndex = 0;
//    double bestDistance = 1000000000000000;
//    LatLon sourcePosition = getIntersectionPosition(intersectoin);
//    
//    for(int depotIndex = 0; depotIndex < depots.size(); ++depotIndex){
//        LatLon destinationPostion = getIntersectionPosition(depots[depotIndex]);
//        
//        std::pair<LatLon, LatLon> points(sourcePosition, destinationPostion);
//        double distance = find_distance_between_two_points(points);
//        
//        if(distance < bestDistance){
//            bestDistance = distance;
//            closestDepotIndex = depotIndex;
//        }
//    }
//    
//    return closestDepotIndex;
//}
//
///**
// * finds the index of the closest legal pick up point or legal drop off point in the deliveries vector.
// * also returns the type of point, a pick up or drop off.
// * @param deliveriesLeft
// * @param dropOffIntersection
// * @return a pair containing index of the closest pick up or drop off point in the deliveries vector and the type of point (0 = pick up, 1 = drop off)
// */
//
//
///**
// * finds the index of the closest legal pick up point or legal drop off point
// * by Euclidean distance in the deliveries vector.
// * returns the type of point, bool isDropOff
// * 
// * @param deliveries
// * @param sourceIntersection
// * @param deliveryStatusVector
// * @param truckWeight
// * @param truckCapacity
// * @return a pair containing index of the closest pick up or drop off point in the deliveries vector and the type of point (0 = pick up, 1 = drop off) 
// */
//std::pair<int,bool> findClosestPickUpOrDropOff(const std::vector<DeliveryInfo> &deliveries, IntersectionIndex sourceIntersection, 
//        const std::vector<int> &deliveryStatusVector, double truckWeight, float truckCapacity){
//    
//    int closestDeliveryIndex = 0;
//    bool isDropOff = false; // false = pick up, true = drop off 
//    double bestDistance = 1000000000000000;
//    
//    LatLon sourcePosition = getIntersectionPosition(sourceIntersection);
//    
//    // loop through all the deliveries and check if they have been picked and/or dropped off
//    for(int deliveryIndex = 0; deliveryIndex < deliveries.size(); ++deliveryIndex){
//        
//            //has not been picked up, so check pick up point
//            if(deliveryStatusVector[deliveryIndex] == 0){
//                
//                // check if the truck can carry the extra load
//                bool isNotOverLoaded = (truckWeight + deliveries[deliveryIndex].itemWeight) < truckCapacity;
//                
//                // if so, find the distance to the pick up point
//                if(isNotOverLoaded){
//                    LatLon destinationPostion = getIntersectionPosition(deliveries[deliveryIndex].pickUp);
//
//                    std::pair<LatLon, LatLon> points(sourcePosition, destinationPostion);
//                    double distance = find_distance_between_two_points(points);
//
//                    if(distance < bestDistance){
//                        bestDistance = distance;
//                        closestDeliveryIndex = deliveryIndex;
//                        isDropOff = 0;
//                    }
//                }
//            }
//            //has been picked up, so check drop off point
//            else if(deliveryStatusVector[deliveryIndex] == 1){
//                // find the distance to the drop off point
//                LatLon destinationPostion = getIntersectionPosition(deliveries[deliveryIndex].dropOff);
//
//                std::pair<LatLon, LatLon> points(sourcePosition, destinationPostion);
//                double distance = find_distance_between_two_points(points);
//
//                if(distance < bestDistance){
//                    bestDistance = distance;
//                    closestDeliveryIndex = deliveryIndex;
//                    isDropOff = 1;
//                }
//            }
//    }
//    
//    // pair of the closest deliveryIndex and typeOfPoint (0 if the point is a pick up, 1 if it is a drop off)
//    std::pair<int,bool> indexAndType(closestDeliveryIndex,isDropOff);
//    return indexAndType;
//}
//
///**
// * Finds the depot that has the least Euclidean distance to a pick up point
// * @param deliveries
// * @param depots
// * @param turnPenalty
// * @return a pair containing the indices of the depot and pick up point
// */
//std::pair<int, int> findDepotWithClosestPickUp(const std::vector<DeliveryInfo>& deliveries,
//	       	        const std::vector<int>& depots){
//    int bestDepotIndex = 0;
//    int bestPickUpIndex = 0;
//    double bestDistance = 10000000000000;
//    int numOfDepots = depots.size();
//    int numOfDeliveries = deliveries.size();
//    
//    for(int depotIndex = 0; depotIndex < numOfDepots; ++depotIndex){
//        LatLon depotPosition = getIntersectionPosition(depots[depotIndex]);
//        for(int pickUpIndex = 0; pickUpIndex < numOfDeliveries; ++pickUpIndex){
//            LatLon pickUpPosition = getIntersectionPosition(deliveries[pickUpIndex].pickUp);
//            
//            std::pair<LatLon, LatLon> points(depotPosition, pickUpPosition);
//            double distance = find_distance_between_two_points(points);
//            
//            if(distance < bestDistance){
//                bestDistance  = distance;
//                bestDepotIndex = depotIndex;
//                bestPickUpIndex = pickUpIndex;
//            }
//        }
//    } 
//    std::pair<int, int> indices(bestDepotIndex, bestPickUpIndex);
//    return indices;
//}
//
//CourierSubpath updatePathInfo(IntersectionIndex startIntersection, IntersectionIndex endIntersection, int indexOfDelivery, int turnPenalty){
//    CourierSubpath path;
//    
//    path.start_intersection = startIntersection;
//    path.end_intersection = endIntersection;
//    if(indexOfDelivery != -1){ 
//        path.pickUp_indices.push_back(indexOfDelivery);
//    }
//    path.subpath = find_path_between_intersections(startIntersection, endIntersection, turnPenalty);    
//    
//    return path;
//}
//
//
//
//double computeVectorCourierPathTime(std::vector<CourierSubpath> path, float turnPenalty)
//{
//    double time=0;
//    for(int i = 0; i<path.size(); i++){
//        time += compute_path_travel_time(path[i].subpath, turnPenalty);
//    }
//    
//    return time;
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////std::vector<CourierSubpath> traveling_courier(
////        const vector<DeliveryInfo>& deliveries,
////        const vector<int>& depots,
////        const float turn_penalty,
////        const float truck_capacity)
////{
////    
////    if(truckCapacity == 0){
////        return std::vector<CourierSubpath> {};
////    }
////    // pesudo code:
////    // iterate through all depots
////    // calculate all travel times between each depot and each pickup
////    // take shortest
////    // use shortest to calculate to next legal index
////    // when all pickups and dropoffs are complete, return to depot
////    //vector<StreetSegmentIndex> courier_path;            // path to be returned
////    list<StreetSegmentIndex> current_path;    // path currently being calculated
////    vector<CourierSubpath> return_path;
////    
////    makeNodeTable();
////    double best_time = WORST_TIME;
////    double current_time = 0;
////    float current_weight = 0;
////    int pickup_num = 0;
////    int prev_num = 0;
////    int depot_num = 0;
////    double current_dist = 0;
////    double best_dist = LARGEST_DISTANCE;
////    
////    // Calculate shortest time between each depot and each pickup with BFS,PATH (might take too long)
////    for (int i=0; i<depots.size(); i++){
////        Node* sourceNode = getNodebyID(depots[i]);
////        for (int j=0; j<deliveries.size(); j++){
////            if (bfsPath(sourceNode, deliveries[j].pickUp, turn_penalty)){
////                current_time = getNodebyID(deliveries[j].pickUp)->bestTime;
////                if (current_time < best_time){
////                    best_time = current_time;
////                    current_path = bfsTraceback(deliveries[j].pickUp);
////                    current_weight = deliveries[j].itemWeight;
////                    pickup_num = j; // Note that this pickup location was visited
////                    depot_num = i;
////                    prev_num = j;
////                }
////            }
////            reset_nodeTable(); // reset nodetable for correct calculations in next path
////        }
////    }
////
////    // vector<StreetSegmentIndex> v{make_move_iterator(begin(current_path)), make_move_iterator(end(current_path))}; // Convert list to vector
////    // Add all elements of list into the path vector
////    int list_size = current_path.size();
////    CourierSubpath courier_path;
////    for (int i=0; i < list_size; i++){
////        (courier_path.subpath).push_back(current_path.front());
////        current_path.pop_front();
////    }
////
////    courier_path.start_intersection = depots[depot_num];
////    courier_path.end_intersection = deliveries[pickup_num].pickUp;
////    courier_path.pickUp_indices = {};        
////    
////    return_path.push_back(courier_path);
////    
////    vector<bool> pickup_checklist;
////    vector<bool> dropoff_checklist;
////    
////    for (int i=0; i < deliveries.size(); i++){
////        pickup_checklist.push_back(false);
////        dropoff_checklist.push_back(false);
////    }
////    pickup_checklist[pickup_num] = true;    // update status of first pickup
////    
////    // Calculate shortest time between fastest pickup and next legal stop
////    // Repeat this until no more deliveries are left
////    while (!all_dropped_off(dropoff_checklist)){
////        best_time = WORST_TIME;
////        best_dist = LARGEST_DISTANCE;
////        Node* subpath_start = getNodebyID(courier_path.end_intersection);     // Start of subpath is the last visited location in path
////        float new_weight = 0;
////        for (int i=0; i < deliveries.size(); i++){
////            
////            reset_nodeTable();
////            // if the delivery has not been picked up
////            if (!pickup_checklist[i] && (current_weight + deliveries[i].itemWeight < truck_capacity)){
////                /*if (bfsPath(subpath_start, deliveries[i].pickUp, turn_penalty)){
////                    current_time = getNodebyID(deliveries[i].pickUp)->bestTime;
////                    if (current_time < best_time){
////                        best_time = current_time;
////                        new_weight = (deliveries[i]).itemWeight;
////                        current_path = bfsTraceback(deliveries[i].pickUp);
////                        pickup_num = i;
////                    }
////                }*/
////                current_dist = find_distance_between_two_points(make_pair(getIntersectionPosition(courier_path.end_intersection), getIntersectionPosition(deliveries[i].pickUp)));
////                if (current_dist < best_dist){
////                    if (bfsPath(subpath_start, deliveries[i].pickUp, turn_penalty)){
////                        best_dist = current_dist;
////                        new_weight = deliveries[i].itemWeight;
////                        current_path = bfsTraceback(deliveries[i].pickUp);
////                        pickup_num = i;
////                    }
////                }
////                reset_nodeTable();
////            }
////            // if delivery has been picked up but not dropped off    
////            else if (pickup_checklist[i] && !dropoff_checklist[i]){
////                /*if (bfsPath(subpath_start, deliveries[i].dropOff, turn_penalty)){
////                    current_time = getNodebyID(deliveries[i].dropOff)->bestTime;
////                    if (current_time < best_time){
////                        best_time = current_time;
////                        new_weight = (deliveries[i]).itemWeight * -1;
////                        current_path = bfsTraceback(deliveries[i].dropOff);
////                        pickup_num = (i+1)*(-1); // dropoff is indicated by negative pickup_num
////                    }
////                }*/
////                current_dist = find_distance_between_two_points(make_pair(getIntersectionPosition(courier_path.end_intersection), getIntersectionPosition(deliveries[i].dropOff)));
////                if (current_dist < best_dist){
////                    if (bfsPath(subpath_start, deliveries[i].dropOff, turn_penalty)){
////                        best_dist = current_dist;
////                        new_weight = deliveries[i].itemWeight * (-1);
////                        current_path = bfsTraceback(deliveries[i].dropOff);
////                        pickup_num = (i+1) * (-1);
////                    }
////                reset_nodeTable();        
////                }
////            }
////        }
////        
////        current_weight += new_weight; // update truck weight
////        // add subpath to path
////        courier_path.subpath.clear(); // clears previous subpath
////        list_size = current_path.size();
////        for (int i=0; i < list_size; i++){
////            courier_path.subpath.push_back(current_path.front());
////            current_path.pop_front();
////        }
////        courier_path.start_intersection = courier_path.end_intersection;
////        
////        
////        // update checklists accordingly
////        if (pickup_num < 0) {
////            int dropoff_num = pickup_num * (-1) -1;
////            dropoff_checklist[dropoff_num] = true;
////
////            courier_path.end_intersection = deliveries[dropoff_num].dropOff;
////        } else { 
////            pickup_checklist[pickup_num] = true;
////            //courier_path.pickUp_indices = {prev_num};
////            courier_path.end_intersection = deliveries[pickup_num].pickUp;
////        }
////        if (prev_num < 0){
////            courier_path.pickUp_indices = {};
////        } else { courier_path.pickUp_indices = {prev_num}; }
////        prev_num = pickup_num;
////        return_path.push_back(courier_path);
////    }
////    
////    
////    // now find paths back to depot
////    Node* last_dropoff = getNodebyID(courier_path.end_intersection);
////    best_time = WORST_TIME;
////    for (int i=0; i<depots.size(); i++){
////        if (bfsPath(last_dropoff, depots[i], turn_penalty)){
////            current_time = getNodebyID(depots[i])->bestTime;
////            if (current_time < best_time){
////                best_time = current_time;
////                current_path = bfsTraceback(depots[i]);
////                depot_num = i;
////            }
////        }
////        reset_nodeTable();
////    }
////    // add subpath to path
////    courier_path.subpath.clear();
////    list_size = current_path.size();
////    for (int i=0; i < list_size; i++){
////        courier_path.subpath.push_back(current_path.front());
////        current_path.pop_front();
////    }
////    courier_path.pickUp_indices = {};
////    courier_path.start_intersection = courier_path.end_intersection;
////    courier_path.end_intersection = depots[depot_num];
////    
////    return_path.push_back(courier_path);
////    
////    
//////    bool check; 
//////    std::vector<std::vector<int>> order;
//////    order = makeDeliveryOrder(return_path, deliveries);
//////    
//////    int deliverySize = deliveries.size();
//////    check = checkOrderLegal(order, truck_capacity, deliverySize, deliveries);
//////    
//////    cout<<"Check: "<<check<<endl;
////    
////    return return_path;
////}
//
//bool all_dropped_off(vector<bool> v){
//    if (find(begin(v), end(v), false) == end(v)) return true;
//    else return false;
//}
//
////
////std::vector<CourierSubpath> greedyHeuristic(const std::vector<DeliveryInfo>& deliveries,
////	       	        const std::vector<int>& depots, 
////		            const float turnPenalty, 
////		            const float truckCapacity)
////{
////    
////    std::vector<CourierSubpath> result_path;
////            
////    //Find depot with closest pickup
////    LatLon firstDepot = getIntersectionPosition(depots[0]);
////    LatLon firstPickUp = getIntersectionPosition(deliveries[0].pickUp); 
////    double distance = sqrt(pow(firstPickUp.lat()-firstDepot.lat(),2) + pow(firstPickUp.lon()-firstDepot.lon(),2));
////    int depotStart;
////    int pickUpStart;
////    
////    for(int i = 0; i< depots.size(); i++){
////        LatLon depotPos = getIntersectionPosition(depots[i]);
////        for(int j = 0; j< deliveries.size(); j++){
////            LatLon pickUp = getIntersectionPosition(deliveries[j].pickUp);
////            double tempDistance = sqrt(pow(pickUp.lat()-depotPos.lat(),1)+pow(pickUp.lon()=depotPos.lon(),2));
////            if(distance > tempDistance){
////                distance = tempDistance;
////                depotStart = i;
////                pickUpStart = j;
////            }
////        }
////        
////    }
////    
////    //Found Depot Start to be depotStart
////    
////    
////    
////    
////    
////    
////}
////
////CourierSubpath addPath(int start, int end, int)
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
////bool opt_two(std::vector<CourierSubpath> solutionPath, std::vector<DeliveryInfo> deliveryOpt)
////{
////    
////    double originalTime = findPathTime(solutionPath);
////    
////    std::vector<std::vector<int>> deliveryOrderOpt;
////    
////    deliveryOrderOpt = makeDeliveryOrder(solutionPath, deliveryOpt);
////    
////    //dropOffIndices = makeDropOffIndices(solutionPath);
////    int size = deliveryOpt.size();
////    
////    for(int p = 1; p < deliveryOrderOpt.size(); p++){
////        
////        if (checkForNoDoubles(p, deliveryOrderOpt)){
////            
////            //Switch Places.
////            std::vector<int> temp = deliveryOrderOpt[p];
////            deliveryOrderOpt[p] = deliveryOrderOpt[p+2];
////            deliveryOrderOpt[p+2] = temp;
////            
////            //Check for legality
////            if(checkOrderLegal(deliveryOrderOpt, 0, size, deliveryOpt)){    
////                //int index = deliveryOrderOpt[p][0];
////                int intersect;
////                
////                //Swaping whole paths (dont think this is possible because we're changing the paths completely)
//////            std::vector<int> temp = solutionPath[p].subPath;
//////            solutionPath[p].subPath = solutionPath[p+2].subPath;
//////            solutionPath[p+2].subPath = temp;
////                
////                //Swapping the middle's to and from
////                intersect = solutionPath[p+1].end_intersection;
////                solutionPath[p+1].end_intersection = solutionPath[p+1].start_intersection;
////                solutionPath[p+1].start_intersection = intersect;
////                
////                CourierSubpath o = solutionPath[p-1];
////                CourierSubpath a = solutionPath[p];
////                CourierSubpath b = solutionPath[p+1];
////                CourierSubpath c = solutionPath[p+2];
////                CourierSubpath z = solutionPath[p+3];
////                
////                
////                b.start_intersection = c.end_intersection;
////                b.end_intersection = a.start_intersection;
////                a.start_intersection = o.end_intersection;
////                a.end_intersection = c.end_intersection;
////                c.end_intersection = z.start_intersection;
////                
////                
//////                solutionPath[p-1].end_intersection = solutionPath[p+1].start_intersection;
//////                solutionPath[p-1].end_intersection = solutionPath[p].start_intersection;
//////                solutionPath[p].end_intersection = solutionPath[p+1].start_intersection;
//////                solutionPath[p+2].start_intersection = solutionPath[p+1].end_intersection;
//////                solutionPath[p+2].end_intersection = solutionPath[p+3].start_intersection;
////                
////                std::vector<int> newPath = find_path_between_intersections(a.start_intersection, a.end_intersection, 0);
////                double newTime = compute_path_travel_time(newPath, 0);
////                
////                if(newTime > originalTime){
////                    return true;
////                }return false;
////                
////            }
////            
////        }
////        
////    }
////    return false;
////}
////
////double findPathTime(std::vector<CourierSubpath> solutionPath)
////{
////    double originalTime =0; 
////    
////    for(int i = 0; i< solutionPath.size(); i++){
////        originalTime += compute_path_travel_time(solutionPath[i].subpath, 0);
////    }
////    
////    return originalTime;
////}
////
////std::vector<std::vector<int>> makeDeliveryOrder(std::vector<CourierSubpath> solutionPath, std::vector<DeliveryInfo> deliveryOpt )
////{
////    
////    std::vector<std::vector<int>> deliveryOrderOpt;
////    
////    std::vector<std::vector<int>> dropOff_indices;
////    
////    dropOff_indices = makeDropOffIndices(solutionPath, deliveryOpt);
////    
////    int size = solutionPath.size();
////    
////    deliveryOrderOpt.resize(size);
////    
////    deliveryOrderOpt[0].push_back(solutionPath[0].start_intersection); //First Depot
////    
////    
////    for(int i = 0 ; i < solutionPath.size() ; i++){
////        
////        std::vector<unsigned> pickUp;
////        pickUp = solutionPath[i].pickUp_indices;
////        
////        for(int j = 0; j< pickUp.size() ; j++){
////                
////            if(pickUp[j] == 0){
////                pickUp[j] = 0;
////            }
////            
////            deliveryOrderOpt[i].push_back((-1)*pickUp[j]);
////        }
////        
////        std::vector<int> dropOff;
////        dropOff = dropOff_indices[i];
////        
////        for(int j = 0; j< dropOff.size() ; j++){
////            
////            if(dropOff[j] == 0){
////                dropOff[j] = 0;
////            }
////            deliveryOrderOpt[i].push_back(dropOff[j]);
////        }
////    }
////    
////    deliveryOrderOpt[size-1].push_back(solutionPath[size-1].end_intersection); //Last Depot
////    
////    return deliveryOrderOpt;
////}
////
////bool checkForNoDoubles(int index, std::vector<std::vector<int>> order)
////{
////    
////    for( int i = index; i< index+3 ; i++ ){
////        for( int j = 0 ; j< order[i].size(); j++){
////            for(int k = 0; k< order[i].size(); k++){
////                if(order[i][j] == order[i][k]){
////                    return false;
////                }
////            }
////        }
////    }
////    
////    return true;
////            
////}
////
////
////bool checkOrderLegal(std::vector<std::vector<int>> order, double truckCapacity, int size, std::vector<DeliveryInfo> deliveryOpt)
////{
////    
////    double truckLoad=0;
////    std::vector<int> truckPackages;
////    truckPackages.resize(size);
////    double weightPick = 0;
////    double weightDrop = 0;
////    
////    for(int i = 0; i< order.size(); i++){
////        
////        for(int k = 0; k< order[i].size() ; k++ ){
////            
////            if(order[i][k]<0){ //Pick up
////            
////                int index = order[i][k];
////                weightPick = deliveryOpt[index].itemWeight;
////
////                if((weightPick + truckLoad) >truckCapacity){
////                    return false;
////                }else{
////                    truckLoad += weightPick;
////                    truckPackages[index] = 1; //the one indicates that it is in the truck
////                }
////            }
////            
////            //WHAT HAPPENS FOR DROP OFFS
////            else{
////                int index = order[i][k];
////                weightDrop = deliveryOpt[index].itemWeight;
////
////                if(truckPackages[index]){ //Package is in the truck
////                    truckCapacity -= weightDrop;
////                    truckPackages[index] = 0;
////                }else{
////                    //If not found in the truck
////                    return false;
////                }
////            }
////        }
////    }
////    return true;
////}
////
////std::vector<std::vector<int>> makeDropOffIndices(std::vector<CourierSubpath> solution, std::vector<DeliveryInfo> deliveryOpt)
////{
////   
////    std::vector<std::vector<int>> dropOff_indices;
////    
////    for(int i = 0; i< solution.size(); i++){
////        
////        int startIntersection = solution[i].start_intersection;
////        
////        for(int j =0; j<deliveryOpt.size(); j++){
////            if(startIntersection == deliveryOpt[j].dropOff){
////                dropOff_indices[i].push_back(j); //Add drop off item.
////            }
////        }
////    }
////    return dropOff_indices;
////}
