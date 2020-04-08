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
//#include "m2_helper/m2_global_variables.hpp"
//
//
//double findPathTime(std::vector<CourierSubpath> solutionPath)
//{
//    double originalTime; 
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
//    double originalTime = findPathTime(solutionPath);
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
//    
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
//                int index = deliveryOrderOpt[p][0];
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
//}
//
//bool checkOrderLegal(std::vector<std::vector<int>> order, double truckCapacity, int size, std::vector<DeliveryInfo> deliveryOpt)
//{
//    
//    double truckLoad;
//    std::vector<int> truckPackages;
//    truckPackages.resize(size);
//    double weightPick;
//    double weightDrop;
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
//        
//    }
//    
//    return dropOff_indices;
//    
//}
//
//
