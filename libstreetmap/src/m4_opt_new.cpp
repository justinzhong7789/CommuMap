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
//		            const float truckCapacity)
//{
//     auto mystart = std::chrono::steady_clock::now();        
//    std::vector<CourierSubpath> anotherPath;
//    double otherTime;
//    double bestTime;
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
//    bestTime = computeVectorCourierPathTime(bestPath, turnPenalty);
//    
//    //In your travelling_courier function:
//    
////    
////    double bestOptTime = bestTime;
////    std::vector<CourierSubpath> opt2Path;
////    int time;
////    std::vector<std::vector<int>> dropOffIndices = makeDropOffIndices(bestPath, deliveries);
////    
////    //int trial = (rand()%(bestPath.size()-4))+1;
////   // for(int trial = 1; trial < bestPath.size()-2; trial++){
////     int trial = 1;   
////        anotherPath = opt_two(trial, bestPath, deliveries, dropOffIndices, turnPenalty, truckCapacity);
////        
////        otherTime = computeVectorCourierPathTime(anotherPath, turnPenalty);
//////        if(bestOptTime > otherTime){
//////            bestOptTime = otherTime;
//////            opt2Path = anotherPath;
//////        }
////        
//////        auto myend = std::chrono::steady_clock::now();
//////        time = chrono::duration_cast<chrono::seconds>(myend-mystart).count();
//////        if(time > 40){
//////            break;
//////        }
//// //   }
////    
////        bestPath = anotherPath;
//    
//    
//    return bestPath;
//}
//
//
//
//std::vector<CourierSubpath> opt_two(int & trial, const std::vector<CourierSubpath> & currentPath, const std::vector<DeliveryInfo> & deliveries, 
//                                               const std::vector<std::vector<int>> & dropOffIndices, const float & turnPenalty, const float & truckCapacity){
//    
//    std::vector<CourierSubpath> result = currentPath;
//    CourierSubpath subPath;
//    std::vector<int> deliveryOrder;
//    std::vector<std::vector<int>> dropOffInd = dropOffIndices;
//    
//    int deliverySize = deliveries.size();
//    
//    if(!containsPickUpAndDelivery(trial, currentPath, dropOffIndices)){
//        
//        std::vector<unsigned> temp1 = result[trial+1].pickUp_indices;
//        result[trial+1].pickUp_indices = result[trial+3].pickUp_indices;
//        result[trial+3].pickUp_indices = temp1;
//        
//        //Dont think this is necessary
//        std::vector<int> temp = dropOffInd[trial+1]; //Its the start not the end
//        dropOffInd[trial+1] = dropOffInd[trial+3];
//        dropOffInd[trial+3] = temp;
//        
//        bool legal = checkOrderLegal(result, dropOffInd, truckCapacity, deliveries);
//        if(legal){
//            
//           
//            CourierSubpath* a = &result[trial];
//            CourierSubpath* b = &result[trial+1];
//            CourierSubpath* c = &result[trial+2];
//            CourierSubpath* d = &result[trial+3];
//            
//            int tempInter;
//            
//            int interTwo = (*b).start_intersection;
//            int interFour= (*d).start_intersection;
//            
//            (*a).end_intersection = interFour;
//            (*a).subpath = find_path_between_intersections((*a).start_intersection, (*a).end_intersection, turnPenalty);
//            
//            (*b).start_intersection = interFour;
//            (*b).subpath = find_path_between_intersections((*b).start_intersection, (*b).end_intersection,turnPenalty);
//            
//            
//            (*c).end_intersection = interTwo;
//            (*c).subpath = find_path_between_intersections((*c).start_intersection, (*c).end_intersection,turnPenalty);
//            
//            (*d).start_intersection = interTwo;
//            (*d).subpath = find_path_between_intersections((*d).start_intersection, (*d).end_intersection,  turnPenalty);
//        }
//    }
//    else{
//        trial++;
//    }
//    
//    return result;
//}
//
//
//
//
//
//bool checkOrderLegal(const std::vector<CourierSubpath> & newPath, std::vector<std::vector<int>> & dropOffIndices, const float & truckCapacity, const std::vector<DeliveryInfo> & deliveries)
//{
//    int size = deliveries.size();
//    float truckLoad=0;
//    std::vector<bool> truckContainer;
//    truckContainer.resize(size);
//    double weightPick = 0;
//    double weightDrop = 0;
//    
//    //HAVENT CHECKED FALSE PICK UPS
//    
//    for(int i = 0; i< newPath.size(); i++){
//        
//        std::vector<unsigned> pickUpInd = newPath[i].pickUp_indices;
//        
//        for(int p= 0; p< pickUpInd.size() ; p++ ){
//            
//            int index = pickUpInd[p];
//
//            weightPick = deliveries[index].itemWeight;
//
//            //SHOULD RETURN FALSE IF ALREADY DELIVERED
//            if((weightPick + truckLoad) >truckCapacity){
//                return false;
//            }else{
//                truckLoad += weightPick;
//                truckContainer[index] = true; //the one indicates that it is in the truck
//            }
//            
//        }   
//            
//        std::vector<int> dropOffInd = dropOffIndices[i];
//        for(int d = 0; d<dropOffInd.size(); d++){    //WHAT HAPPENS FOR DROP OFFS
//            
//            int index = dropOffInd[d];
//            weightDrop = deliveries[index].itemWeight;
//
//            if(truckContainer[index]){ 
//                truckLoad -= weightDrop;
//                truckContainer[index] = false;
//            }else{
//                return false;
//            }
//        
//        }
//    }
//    return true;
//}
//
//std::vector<std::vector<int>> makeDropOffIndices(const std::vector<CourierSubpath> & currentPath, const std::vector<DeliveryInfo> & deliveries)
//{
//     std::vector<std::vector<int>> dropOff_indices;
//     dropOff_indices.resize(currentPath.size());
//
//    for(int i= 0; i< currentPath.size(); i++){
//        
//        int startIntersection = currentPath[i].start_intersection;
//        
//        for(int j =0; j<deliveries.size(); j++){
//            if(startIntersection == deliveries[j].dropOff){
//                dropOff_indices[i].push_back(j); //Add drop off item.
//            }
//        }
//    }
//     
//    return dropOff_indices;
//}
//
//bool containsPickUpAndDelivery(const int trial, const std::vector<CourierSubpath> & currentPath, const std::vector<std::vector<int>> & dropOffIndices)
//{
//    std::vector<int> total;
//    
//    for(int i = trial; i< trial + 3; i++){
//        std::vector<unsigned> pickUpIndices = currentPath[i].pickUp_indices;
//        for(int pu = 0; pu< pickUpIndices.size(); pu++ ){
//            total.push_back(pickUpIndices[pu]);
//        }
//        for(int df = 0; df< dropOffIndices[i].size(); df++){
//            total.push_back(dropOffIndices[i][df]);
//        }
//    }
//    
//    std::sort(total.begin(), total.end());
//    int num = total[0];
//    for(int i = 1; i< total.size(); i++){
//        if(num == total[i]){
//            return true;
//        }else{
//            num = total[i];
//        }
//    }
//    return false;   
//}
//
//
//std::vector<CourierSubpath> greedyHeuristic1(const std::vector<DeliveryInfo>& deliveries,
//	       	        const std::vector<int>& depots, 
//		            const float turnPenalty, 
//		            const float truckCapacity)
//{
//    std::vector<CourierSubpath> courierRoute;
//    CourierSubpath path;
//    int numOfDeliveries = deliveries.size();
//    int numOfDeliveriesCompleted = 0;
//    std::vector<int> deliveryStatusVector; // 0 = not picked up, 1 = picked up, 2 = dropped off
//    
//    
////    std::vector<std::vector<int>> dropOffIndices;//MJ
////    std::vector<std::vector<int>> pickUpIndices;//MJ
////    
////    int indices = 1;//MJ
////    
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
//    
////    pickUpIndices[0].push_back(startingPickUpIndex);//mj
//    
//    // update the status vector and weight of the truck
//    deliveryStatusVector[startingPickUpIndex] = 1;
//    double truckWeight = deliveries[startingPickUpIndex].itemWeight;
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
////            pickUpIndices[indices].push_back(deliveryIndex);//mj
//            
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
////            dropOffIndices[indices].push_back(deliveryIndex);//mj
//        }
//        
////        indices++;//mj
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
//
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
//            //std::pair<LatLon, LatLon> points(depotPosition, pickUpPosition);
//            double distance = find_distance_between_two_points({depotPosition, pickUpPosition});
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
////#include <iostream>
////#include "m4.h"
////#include "m4_global.hpp"
////
////using namespace std;
////
////std::vector<CourierSubpath> traveling_courier(
////		            const std::vector<DeliveryInfo>& deliveries,
////	       	        const std::vector<int>& depots, 
////		            const float turnPenalty, 
////		            const float truckCapacity)
////{
////
////    
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
////            double tempDistance = find_distance_between_two_point({depotPos, pickUp});
////            if(distance > tempDistance){
////                distance = tempDistance;
////                depotStart = i;
////                pickUpStart = j;
////            }
////        }
////        
////    }
////    
////    result_path = makeSubPath(depots[depotStart], deliveries[pickUpStart].pickUp);
////    
////    
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
////CourierSubpath makeSubPath(int start, int end, int pickUpIndex, float turnPenalty){
////    
////    CourierSubpath path;
////    
////    path.start_intersection = start;
////    path.end_intersection = end;
////    
////    path.subpath = find_path_between_intersections(start, end, turnPenalty);
////    
////    if(pickUpIndex > 0){
////        path.pickUp_indices.push_back(pickUpIndex);
////    }
////    
////    return path;
////}
//
