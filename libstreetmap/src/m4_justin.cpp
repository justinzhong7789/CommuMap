//
///*
// * To change this license header, choose License Headers in Project Properties.
// * To change this template file, choose Tools | Templates
// * and open the template in the editor.
// */
//
//
//#include "StreetsDatabaseAPI.h"
//#include "m1.h"
//#include "m2.h"
//#include "m3.h"
//#include "m3_global.hpp"
//#include <vector>
//#include <string>
//#include <iostream>
//#include "m4.h"
//#include "m4_global.hpp"
//
//using namespace std;
//
//vector<list<StreetSegmentIndex>> tracebackTemp; // Used to store bfsTraceback paths
//
//bool one_source_multi_dest(Node* sourceNode, vector<IntersectionIndex> destinations, double turn_penalty){
//    makeNodeTable();
//
//    for (int i=0; i<destinations.size(); i++){
//        if (!bfsPath(sourceNode, destinations[i],turn_penalty)){
//            tracebackTemp.clear();
//            return false;
//        }
//        // Store the traceback paths into a global variable
//        else tracebackTemp.push_back(bfsTraceback(destinations[i]));
//        reset_nodeTable();
//    }
//    return true;
//}
//
///*bool one_source_multi_dest(Node* sourceNode, vector<IntersectionIndex> destinations, double turn_penalty){
//        // Establish min heap
//    //priority_queue <WaveElem, vector<WaveElem>, comparatorWE> pq;
//    priority_queue <WaveElem, vector<WaveElem>, greaterWE> pq;
//    pq.push(WaveElem(sourceNode, 1, 0));
//
//    vector<IntersectionIndex> dummyDest = destinations;
//    int destNum = dummyDest.size();
//    
//    while (!pq.empty()){
//            bool done = true;
//            WaveElem wave = pq.top(); // get next element
//            pq.pop(); // remove from wavefront
//            Node *currNode = wave.node;
//            //set element of dummy destination to invalid if its found
//            for(int i =0;i<destNum;i++){
//                if(dummyDest[i] != invalid){
//                    done = false;
//                    break;
//                }
//            }          
//            if(done == true){
//                return true;
//            }
//            for(int i=0;i< destNum;i++){
//                if(currNode->id == dummyDest[i]){
//                    dummyDest[i] = invalid;
//                }
//            }
//            currNode->set_visited(true);
//            for (int i=0; i<currNode->outEdge; i++){      
//                Node *toNode;
//                StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);
//                InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id); 
//                
//                bool legal = false;
//                    // Legal check
//                if (currNode->id == info_outEdge.from && currNode->parent_id != info_outEdge.to){
//                    toNode = nodeTable[info_outEdge.to];
//                    legal = true;
//                }
//                else if (currNode->id == info_outEdge.to && currNode->parent_id != info_outEdge.from && info_outEdge.oneWay != true){
//                    toNode = nodeTable[info_outEdge.from];
//                    legal = true;
//                }
//                    // Update nodes if legal
//                if (legal && !toNode->visited){
//                   double time_score;
//                   // Calculate score using travel time and turn time
//                    if (currNode->reachingEdge != NO_EDGE){
//                        time_score = currNode->bestTime + find_street_segment_travel_time(outEdge_id)+turn_penalty*there_is_turn(currNode->reachingEdge, outEdge_id);
//                    } else { time_score = find_street_segment_travel_time(outEdge_id); } // If first mode, only count travel time
//                  
//                if (time_score < toNode->bestTime){
//                      
//                    // Update table 
//                    nodeTable[toNode->id]->set_reachingEdge(outEdge_id);
//                    nodeTable[toNode->id]->set_bestTime(time_score);
//                    // Update parent_id for reaching edge
//                    nodeTable[toNode->id]->set_parent_id(currNode->id);
//                    pq.push(WaveElem(toNode, 1, time_score));
//                    }
//                }
//            }
//    }
//    return false;
//
//}*/
//
//bool make_depot_to_PU(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty){
//    int depotNum = depots.size();
//    int deliNum = deliveries.size();
//    vector<IntersectionIndex> PU_loc;
//    
//    for(int i=0; i< deliNum;i++){
//        PU_loc.push_back(deliveries[i].pickUp);
//    }
//    
//    /*for(int i=0; i< depotNum;i++){
//        bool all_path_found = false;
//        Node* source = getNodebyID(depots[i]);
//        all_path_found = one_source_multi_dest(source, PU_loc, turn_penalty);
//        vector<pathInfo> one_depot_to_all_PU;
//        if(all_path_found){
//            for(int j=0;j< PU_loc.size();j++){
//                //do traceback from all pickups
//                path to_one_PU = convertListToVec(bfsTraceback(PU_loc[j]));
//                pathInfo one_path(to_one_PU, compute_path_travel_time(to_one_PU, turn_penalty));
//                one_depot_to_all_PU.push_back(one_path);
//            }
//        
//        }
//        else{return false;}*/
//    for (int i=0; i< depotNum; i++){
//        Node* source = getNodebyID(depots[i]);
//        bool all_path_found = one_source_multi_dest(source, PU_loc, turn_penalty);
//        vector<pathInfo> one_depot_to_all_PU;
//        
//        if (all_path_found){
//            for (int j=0; j < PU_loc.size(); j++){
//                path to_one_PU = convertListToVec(tracebackTemp[j]);
//                pathInfo one_path(to_one_PU, compute_path_travel_time(to_one_PU, turn_penalty));
//                one_depot_to_all_PU.push_back(one_path);
//            }
//            tracebackTemp.clear();
//        } else {
//            tracebackTemp.clear();
//            return false;
//        }   
//        
//        depots_to_all_PU.push_back(one_depot_to_all_PU);
//        //reset_nodeTable();
//    }
//    return true;
//}
//
//vector<StreetSegmentIndex> convertListToVec(list<StreetSegmentIndex> l){
//    vector<StreetSegmentIndex> v;
//    for(int i=0;i< l.size();i++){
//        v.push_back(l.front());
//        l.pop_front();
//    }
//    return v;
//}
//
//bool make_PU_to_other_points(const std::vector<DeliveryInfo>& deliveries, double turn_penalty){
//    int deliNum = deliveries.size();
//    vector<IntersectionIndex> PU, DO, all;
//    for(int i=0;i< deliNum ;i++){
//        PU.push_back(deliveries[i].pickUp);
//        DO.push_back(deliveries[i].dropOff);
//        all.push_back(deliveries[i].pickUp);
//        all.push_back(deliveries[i].dropOff);
//    }
//    
//    vector<pair<pathInfo, pathInfo>> one_pu_to_all_points;
//    for(int i=0; i<deliNum; i++){
//        Node *source = getNodebyID(PU[i]);
//        
//        bool all_path_found = one_source_multi_dest(source, all, turn_penalty);
//        
//        // Vector traceback will have alternating pickup/dropoff locations
//        vector<list<StreetSegmentIndex>> PU_paths, DO_paths;
//        for (int j=0; j < tracebackTemp.size(); j++){
//            if (j % 2 == 0) PU_paths.push_back(tracebackTemp[j]);
//            else DO_paths.push_back(tracebackTemp[j]);
//        }
//        tracebackTemp.clear(); // No longer need this vector
//        
//        if (all_path_found){
//            for (int j=0; j < deliNum; j++){
//                path to_pu = convertListToVec(PU_paths[i]);
//                path to_do = convertListToVec(DO_paths[i]);
//                
//                pathInfo path_to_pu(to_pu, compute_path_travel_time(to_pu, turn_penalty));
//                pathInfo path_to_do(to_do, compute_path_travel_time(to_do, turn_penalty));
//                
//                one_pu_to_all_points.push_back(make_pair(path_to_pu, path_to_do));
//            }
//        } else {
//            return false;
//        }
//        
//        /*if(all_path_found){
//            for(int j=0;j< deliNum;j++){
//                path to_pu = convertListToVec(bfsTraceback(PU[i]));
//                path to_do = convertListToVec(bfsTraceback(DO[i]));
//                pathInfo path_to_pu(to_pu, compute_path_travel_time(to_pu, turn_penalty));
//                pathInfo path_to_do(to_do, compute_path_travel_time(to_do, turn_penalty));
//                one_pu_to_all_points.push_back(make_pair(path_to_pu, path_to_do));
//            }
//            
//        }
//        else {return false;}*/
//
//        all_PU_to_other_points.push_back(one_pu_to_all_points);
//        //reset_nodeTable();
//    }
//    return true;  
//}
//
//bool make_DO_to_points(const std::vector<DeliveryInfo>& deliveries,const std::vector<int>& depots, double turn_penalty){
//    int deliNum = deliveries.size();
//    int depotNum = depots.size();
//    vector<IntersectionIndex> PU, DO, all;
//    for(int i=0;i<deliNum;i++){
//        all.push_back(deliveries[i].pickUp);
//        all.push_back(deliveries[i].dropOff);
//        PU.push_back(deliveries[i].pickUp);
//        DO.push_back(deliveries[i].dropOff);
//    }
//    for(int i=0;i<depotNum;i++){
//        all.push_back(depots[i]);
//    }
//    
//    for(int i=0;i<deliNum;i++){
//        bool all_paths_found = false;
//        Node* source = getNodebyID(DO[i]);
//        all_paths_found = one_source_multi_dest(source, all, turn_penalty);
//        vector<pair<pathInfo,pathInfo>> one_DO_to_all_points;
//        vector<pathInfo> one_DO_to_all_depots;
//        
//        // Vector traceback will have alternating pickup/dropoff locations
//        // Start of vector has deliveries
//        // End of vector has depots
//        vector<list<StreetSegmentIndex>> PU_paths, DO_paths, depot_paths;
//        for (int j=0; j < deliveries.size() * 2; j++){
//            if (j % 2 == 0) PU_paths.push_back(tracebackTemp[j]);
//            else DO_paths.push_back(tracebackTemp[j]);
//        }
//        for (int j=deliveries.size() * 2; j < tracebackTemp.size(); j++){
//            depot_paths.push_back(tracebackTemp[j]);
//        }
//        tracebackTemp.clear();
//        
//        if (all_paths_found){
//            for (int j=0; (j<deliNum)&&(j<depotNum); j++){
//                if (j < deliNum){
//                    path one_do_to_one_pu = convertListToVec(PU_paths[i]);
//                    path one_do_to_one_do = convertListToVec(DO_paths[i]);
//                    
//                    pathInfo to_one_pu(one_do_to_one_pu, compute_path_travel_time(one_do_to_one_pu, turn_penalty));
//                    pathInfo to_one_do(one_do_to_one_do, compute_path_travel_time(one_do_to_one_do, turn_penalty));
//                    
//                    one_DO_to_all_points.push_back(make_pair(to_one_pu, to_one_do));
//                }
//                if (j < depotNum){
//                    path one_do_to_one_depot = convertListToVec(depot_paths[i]);
//                    pathInfo to_one_depot(one_do_to_one_depot, compute_path_travel_time(one_do_to_one_depot, turn_penalty));
//                    one_DO_to_all_depots.push_back(to_one_depot);
//                }
//            }
//        } else { return false; }
//        
//        /*if(all_paths_found){
//            for(int j =0; (j<deliNum)&& (j<depotNum);j++){
//                if(j<deliNum){
//                    path one_do_to_one_pu = convertListToVec(bfsTraceback(PU[i]));
//                    path one_do_to_one_do = convertListToVec(bfsTraceback(DO[i]));
//                    pathInfo to_one_pu(one_do_to_one_pu, compute_path_travel_time(one_do_to_one_pu, turn_penalty));
//                    pathInfo to_one_do(one_do_to_one_do, compute_path_travel_time(one_do_to_one_do, turn_penalty));
//                    one_DO_to_all_points.push_back(make_pair(to_one_pu, to_one_do));
//                }
//                if(j<depotNum){
//                    path one_do_to_one_depot = convertListToVec(bfsTraceback(depots[i]));
//                    pathInfo to_one_depot(one_do_to_one_depot, compute_path_travel_time(one_do_to_one_depot, turn_penalty));
//                    one_DO_to_all_depots.push_back(to_one_depot);
//                
//                }
//            }
//        }
//        else{
//            reset_nodeTable();
//            return false;
//        }*/
//        //reset_nodeTable();//reset node table for every loop
//        if(i<deliNum){
//            all_DO_to_other_points.push_back(one_DO_to_all_points);
//        }
//        if(i<depotNum){
//            all_DO_to_depots.push_back(one_DO_to_all_depots);
//        }
//    }
//    return true;
//}
//    
//    
//    
//bool precompute_all_paths(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty){
//    return (make_depot_to_PU(deliveries, depots, turn_penalty)
//            && make_PU_to_other_points(deliveries, turn_penalty)
//            && make_DO_to_points(deliveries,depots, turn_penalty));
//}
//void clear_all_precomputed_paths(){
//    all_PU_to_other_points.clear();
//    all_DO_to_other_points.clear();
//    depots_to_all_PU.clear();
//    all_DO_to_depots.clear();
//
//}
//std::vector<CourierSubpath> traveling_courier(
//		            const std::vector<DeliveryInfo>& deliveries,
//	       	        const std::vector<int>& depots, 
//		            const float turn_penalty, 
//		            const float truck_capacity){
//    if( precompute_all_paths(deliveries,depots ,turn_penalty)){
//        vector<CourierSubpath> shortest = greedy(deliveries, depots,
//                                                turn_penalty, truck_capacity, 0);
//    
//        for(int i =0;i<depots.size();i++){
//            vector<CourierSubpath> temp = greedy(deliveries, depots,
//                                                turn_penalty, truck_capacity, 0);
//            if(computeCourierPathTravelTime(turn_penalty,shortest) 
//            > computeCourierPathTravelTime(turn_penalty, temp)){
//                shortest = temp;
//            }
//        }
//        return shortest;
//    }
//    else {return {};}
//    
//}
//
//
//vector<CourierSubpath> greedy(const std::vector<DeliveryInfo>& deliveries,
//	       	        const std::vector<int>& depots, 
//		            const float turn_penalty, 
//		            const float truck_capacity,
//                            int startdepotIndex){
//    int deliNum = deliveries.size();
//    truck status(deliveries);
//    vector<CourierSubpath> best; //stores best courier path
//    
//    if(best.size()==0){ // add the starting path
//            
//        pathInfo closest_path = depots_to_all_PU[startdepotIndex][0];
//        for(int i=0;i<deliNum;i++){
//            if(closest_path > depots_to_all_PU[startdepotIndex][i]){
//                closest_path = depots_to_all_PU[startdepotIndex][i];
//            }
//        }
//        CourierSubpath start = initializeSubpath(depots[startdepotIndex], getEndInter(closest_path.the_path), closest_path.the_path,{});
//        best.push_back(start);
//    }
//    while(!status.all_done()){
//        CourierSubpath subpath;
//        int subpathSize = best.size();
//        vector<deliIndex> at_pu, at_do;
//        //legal drop offs we can go are the ones we are carrying
//        at_pu = find_indices_of_pu(best[subpathSize-1].end_intersection, deliveries);
//        for(int i=0;i<at_pu.size(); i++){
//            //if we are at an intersection that is a pick up, check its weight 
//            //and modify subpath
//            if(status.currWeight+deliveries[at_pu[i]].itemWeight < truck_capacity){
//                subpath.pickUp_indices.push_back(at_pu[i]);
//                status.onBoard.push_back(at_pu[i]);
//            }
//        }
//        at_do = find_indices_of_do(best[subpathSize-1].end_intersection, deliveries);
//        //check if we are at a drop off location, then we need to drop off things
//        // from the truck object
//        for(int i=0;i<at_do.size(); i++){
//            for(int j=0;j<status.onBoard.size();i++){
//                if(status.item_is_onBoard(at_do[i])){
//                    //if a corresponding item is on board when we reach its drop off location
//                    //then drop it
//                    status.one_deli_done(at_do[i]);
//                }
//            }
//        }
//        vector<deliIndex> legal_do_for_next, legal_pu_for_next;
//        legal_do_for_next = status.onBoard; //legal drop off are what we have on truck
//        for(int i=0;i<status.undone.size();i++){
//            //legal pickups are what's in undone and not on the truck
//            //and satisfies weight condition
//            if(!status.item_is_on_board(status.undone[i])){
//                if(status.currWeight + deliveries[status.undone[i]].itemWeight <truck_capacity){
//                    legal_pu_for_next.push_back(status.undone[i]);
//                }
//            }
//        }
//        if(at_pu.size()!=0){
//            //we are at a pickup location
//            vector<pair<pathInfo,pathInfo>> current_point_to_all_points = all_PU_to_other_points[at_pu[0]];
//            if(legal_pu_for_next.size()!=0){
//                pathInfo shortest_path = current_point_to_all_points[legal_pu_for_next[0]].first;
//                for(int i=0;i<legal_pu_for_next.size();i++){
//                    if(shortest_path > current_point_to_all_points[legal_pu_for_next[i]].first){
//                        shortest_path = current_point_to_all_points[legal_pu_for_next[i]].first;
//                    }
//                }
//                for(int i=0; i< legal_do_for_next.size();i++){
//                    if(shortest_path > current_point_to_all_points[legal_do_for_next[i]].second){
//                        shortest_path = current_point_to_all_points[legal_do_for_next[i]].second;
//                    }
//                }
//                subpath.subpath = shortest_path.the_path;
//                subpath.end_intersection = getEndInter(shortest_path.the_path);
//                subpath.start_intersection = getStartInter(shortest_path.the_path);
//            }
//            
//        }
//        if(at_do.size()!=0){
//            //we are at a drop off location
//            vector<pair<pathInfo, pathInfo>> current_to_all_points = all_DO_to_other_points[at_do[0]];
//            if(legal_pu_for_next.size()!=0){
//                pathInfo shortest_path = current_to_all_points[legal_pu_for_next[0]].first;
//                for(int i=0;i<legal_pu_for_next.size();i++){
//                    if(shortest_path > current_to_all_points[legal_pu_for_next[i]].first){
//                        shortest_path = current_to_all_points[legal_pu_for_next[i]].first;
//                    }
//                }
//                for(int i=0; i< legal_do_for_next.size();i++){
//                    if(shortest_path > current_to_all_points[legal_do_for_next[i]].second){
//                        shortest_path = current_to_all_points[legal_do_for_next[i]].second;
//                    }
//                }
//                subpath.subpath = shortest_path.the_path;
//                subpath.end_intersection = getEndInter(shortest_path.the_path);
//                subpath.start_intersection = getStartInter(shortest_path.the_path);
//            }        
//            
//        }
//        best.push_back(subpath);
//    }
//    int bestNum = best.size();
//    vector<int> final_drop_off = find_indices_of_do(best[bestNum-1].end_intersection, deliveries);
//    
//    if(final_drop_off.size()!=0){
//        vector<pathInfo> DO_to_all_depots = all_DO_to_depots[final_drop_off[0]];
//        pathInfo shortest_to_depot =DO_to_all_depots[0];
//        for(int i= 0;i<DO_to_all_depots.size();i++){
//            if(shortest_to_depot > DO_to_all_depots[i]){
//                shortest_to_depot = DO_to_all_depots[i];
//            }
//        }
//        CourierSubpath end_at_depot;
//        end_at_depot.end_intersection = getEndInter(shortest_to_depot.the_path);
//        end_at_depot.start_intersection = getStartInter(shortest_to_depot.the_path);
//        end_at_depot.subpath = shortest_to_depot.the_path;
//        best.push_back(end_at_depot);
//        return best;
//    }
//    return best;
//}
//
//CourierSubpath initializeSubpath(IntersectionIndex start, IntersectionIndex end, path path_, vector<unsigned> pickupIndex){
//    CourierSubpath a;
//    a.start_intersection = start;
//    a.end_intersection = end;
//    a.subpath = path_;
//    a.pickUp_indices = pickupIndex;
//    return a;
//}
//IntersectionIndex getStartInter(path path_){
//    //int size = path_.size();
//    InfoStreetSegment first = getInfoStreetSegment(path_[0]);
//    InfoStreetSegment second = getInfoStreetSegment(path_[1]);
//    if(first.from == second.to || first.from == second.from){
//        return first.to;
//    }
//    else {return first.from;}
//}
//IntersectionIndex getEndInter(path path_){
//    int size = path_.size();
//    InfoStreetSegment last = getInfoStreetSegment(path_[size-1]);
//    InfoStreetSegment second_to_last = getInfoStreetSegment(path_[size-2]);
//    if(last.from == second_to_last.from || last.from == second_to_last.to){
//        return last.to;
//    }
//    else{
//        return last.from;
//    }
//}
//
///*pass in a intersection index and deliveries. return a vector that contains 
// * the indices of deliveries that have their pickup location at this intersection*/
//vector<int> find_indices_of_pu(IntersectionIndex index, const std::vector<DeliveryInfo>& deliveries){
//    vector <int> a;
//    for(int i=0;i<deliveries.size();i++){
//        if(index == deliveries[i].pickUp){
//            a.push_back(i);
//        }
//    }
//    return a;
//}
//
//vector<int> find_indices_of_do(IntersectionIndex index, const std::vector<DeliveryInfo>& deliveries){
//    vector <int> a;
//    for(int i=0;i<deliveries.size();i++){
//        if(index == deliveries[i].dropOff){
//            a.push_back(i);
//        }
//    }
//    return a;
//}
//
//double computeCourierPathTravelTime(double turn_penalty, vector<CourierSubpath> a){
//    double travelTime= 0;
//    for(int i=0;i< a.size();i++){
//        travelTime+=compute_path_travel_time(a[i].subpath, turn_penalty);
//    }
//    return travelTime;
//}
