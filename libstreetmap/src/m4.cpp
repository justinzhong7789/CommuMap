#include <iostream>
#include "m4.h"
#include "m4_global.hpp"


































//    
//std::vector<CourierSubpath> greedyHeuristic(const std::vector<DeliveryInfo>& deliveries,
//	       	        const std::vector<int>& depots, 
//		            const float turnPenalty, 
//		            const float truckCapacity)
//{
//    int deliverySize = deliveries.size();
//    std::vector<CourierSubpath> result_path;
//    CourierSubpath subpath;
//    
//    std::vector<bool> completeDeliveries;
//    completeDeliveries.resize(deliverySize);
//    
//    std::vector<bool> truckContainer;
//    truckContainer.resize(deliverySize);
//    
//    float truckLoad=0;
//    float packageWeight = 0;
//    
//    //Find depot with closest pickup
//    LatLon firstDepot = getIntersectionPosition(depots[0]);
//    LatLon firstPickUp = getIntersectionPosition(deliveries[0].pickUp); 
//    double distance = find_distance_between_two_points({firstDepot, firstPickUp});
//    int depotStart;
//    int pickUpStart;
//    
//    for(int i = 0; i< depots.size(); i++){
//        LatLon depotPos = getIntersectionPosition(depots[i]);
//        for(int j = 0; j< deliveries.size(); j++){
//            LatLon pickUp = getIntersectionPosition(deliveries[j].pickUp);
//            double tempDistance = find_distance_between_two_points({depotPos, pickUp});
//            if(distance > tempDistance){
//                distance = tempDistance;
//                depotStart = i;
//                pickUpStart = j;
//            }
//        }
//    }
//    
//    
//    
//    //Why aren't we checking if there's other things to pickup here?
//    subPath = makeSubPath(depots[depotStart], deliveries[pickUpStart].pickUp);
//    result_path.push_back(subPath);
//
//
//    packageWeight = deliveries[pickUpStart].itemWeight;
//    truckLoad += packageWeight;
//    
//    ////Tested up to here
//    
//    truckContainer[pickUpStart] = true;
//    
//    
//    
//    int start = deliveries[pickUpStart].pickUp;
//    int deliveryDone = 0;
//    std::pair<IntersectionIndex, PickUpIndex>  = nextStop;
//    int end;
//    int packageIndex;
//    int pickUpIndices;
//    
//    while(deliveryDone != deliverySize){
//     //Find next delivery/pickup location (return start and end)
//    
//            nextStop = find_next_delivery_or_pickup(deliveries, completeDeliveries,truckContainer, start, truckLoad, truckCapacity );
//            
//           
//            if(abs(nextStop.second) == 400){ //Know that max amount will be 250 deliveries
//                packageIndex = 0;
//            }else packageIndex = abs(nextStop.second);
//            
//            end = nextStop.first;
//            DeliveryInfo package = deliveries[packageIndex];
//            
//            if(nextStop.second > 0){//PickUP if pickUpIndex > 0
//                truckLoad += package.itemWeight;
//                truckContainer[packageIndex] = true; //Truck has package
//                
//            }else{ //Drop off
//                truckLoad -= package.itemWeight; //Update Truck Load
//                truckContainer[packageIndex] = false; //Truck no longer has package
//                completeDeliveries[packageIndex] = true; //Deliver is Complete
//                deliveryDone ++;
//                packageIndex = -1;
//            }
//            
//            subPath = makeSubpath(start, end, packageIndex, turnPenalty);
//            result_path.push_back(subPath);
//            start = end;
//    }
//    
//    if(abs(nextStop.second) == 400){ //Know that max amount will be 250 deliveries
//                packageIndex = 0;
//    }else packageIndex = abs(nextStop.second);
//    
//    int lastDropOffIntersection = deliveries[packageIndex].dropOff;
//    int closestFinishDepotIndex = findClosestDepot(depots, lastDropOffIntersection);
//    
//    subPath = makeSubPath(lastDropOffIntersection, depots[closestFinishDepotIndex], -1, turnPenalty);
//    result_path.push_back(path);
//    
//}
//
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
//CourierSubpath makeSubPath(int start, int end, int pickUpIndex, float turnPenalty)
//{
//    
//    CourierSubpath path;
//    
//    path.start_intersection = start;
//    path.end_intersection = end;
//    
//    path.subpath = find_path_between_intersections(start, end, turnPenalty);
//    
//    if(pickUpIndex > 0){
//        path.pickUp_indices.push_back(pickUpIndex);
//    }
//    
//    return path;
//}
//
//std::pair<IntersectionIndex, PickUpIndex> find_next_delivery_or_pickup(const std::vector<DeliveryInfo> & deliveries, std::vector<bool> & completeDeliveries,
//                                                                    std::vector<bool> & truckContainer, IntersectionIndex start, const float & truckLoad, const float truckCapacity )
//{
//    //find which deliveries need to be done == which packages we already have; (get closest pick up and closest drop off...)
//    //if drop off can't be done because truckContainer doesnt have it, return pickup
//    //make pickup <0, and make return >0 ... If == 0, make it 0.5
//    
//    
//    double distance = 10000000000000;
//    
//    int pickUpIndex;
//    double pickUpDistance;
//    int dropOffIndex;
//    double dropOffDistance;
//    bool pickMeUp;
//    int finalIntersection;
//     double tempDistance;
//    int finalIndex;
//    
//    LatLon startPos = getIntersectionPosition(start);
//    
//    for(int j = 0 ; j< truckContainer.size(); j++){
//        
//        if(truckContainer[j] != true && completeDeliveries[j] != true){ //Therefore, does not have package and delivery is incomplete
//            
//            LatLon pickUpPos = getIntersectionPosition(deliveries[j].pickUp);
//            tempDistance = find_distance_between_two_points({startPos, pickUpPos});
//            if(distance > tempDistance && (truckLoad + deliveries[j].itemweight) < truckCapacity){
//                
//                if(j == 0){
//                    pickUpIndex = 400;
//                }else pickUpIndex = -j;
//                
//                distance = tempDistance;
//                pickUpDistance = tempDistance;
//            }
//        }else{ //Already have package compare with drop off intersection
//            
//            LatLon dropOffPos = getIntersectionPosition(deliveries[j].dropOff)
//            tempDistance = find_distance_between_two_points({startPos, dropOffPos});
//            if(distance> tempDistance){
//                
//                if(j == 0){
//                    dropOffIndex = -400;
//                }else dropOffIndex = -j;
//                
//                distance = tempDistance;
//                dropOffDistance = tempDistance;
//            }
//        }
//    }
//    
//    if(dropOffDistance > pickUpDistance){
//        finalIndex = pickUpIndex;
//        finalIntersection = deliveries[pickUpIndex].pickUp;
//        
//    }else{
//        finalIndex = dropOffIndex;
//        finalIntersection = deliveries[dropOffIndex].dropOff;
//    }
//    
//    return {finalIntersection, finalIndex};
//
//    
//}