#pragma once


#include "m1.h"
#include "m2.h"
#include "zoom.hpp"
#include "global_variables.hpp"
#include "m2_global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <vector>
#include <iostream>
#include "m4.h"
#include "m3_global.hpp"

//m4 Heuristic ++
#include <boost/functional/hash.hpp>
#include <bits/stdc++.h>
#include <chrono>
#include <thread>

//m4 Global Variables
#define TIME_LIMIT 45
#define PICK_UP_TYPE 0
#define DROP_OFF_TYPE 1
#define DEPOT_TYPE 2

#define NOT_PICKED_UP 0
#define PICKED_UP 1
#define DROPPED_OFF 2
//End

#define invalid -1
#define no_pickup -1
using namespace std;
//MJ commented this out
typedef vector<StreetSegmentIndex> path;
typedef int deliIndex;
typedef int PickUpIndex;
/*
 * 
 * first element of pair 
 */
class pathInfo{
public: 
    pathInfo(std::vector<int> path_, double time):the_path(path_), path_travel_time(time){};
    std::vector<int> the_path;
    double path_travel_time;
    
    

    friend bool operator>(const pathInfo& a, const pathInfo& b){
        return a.path_travel_time>b.path_travel_time;
    }
    friend bool operator<(const pathInfo& a, const pathInfo& b){
        return a.path_travel_time<b.path_travel_time;
    }
};


class truck{
public:
    /*contains current weight of the truck*/
    float currWeight;
    /*undone contains the indices of deliveries that are undone*/
    vector<deliIndex> undone;
    /*contains the indices of delivery items on the truck*/
    vector<deliIndex> onBoard;
    truck(const vector<DeliveryInfo> a){
        for(int i=0;i<a.size();i++){
            undone.push_back(i);
        }
        currWeight= 0;
        onBoard = {};
    }
    bool all_done(){
        return undone.size()==0;
    }
    bool item_is_on_board(int index){
        for(int i=0;i<onBoard.size();i++){
            if(index == onBoard[i]){
                return true;
            }
        }
        return false;
    }
    /*pass in the delivery index that is done, and corresponding index is taken 
     out of the undone member variable*/
    void one_deli_done(int Index){
        vector<int> a;
        for(int i=0;i< undone.size();i++){
            if(undone[i] !=Index){
                a.push_back(undone[i]);
            }
        }
        undone = a;
        
        vector<int> b;
        for(int j=0;j<onBoard.size();j++){
            if(onBoard[j] != Index){
                b.push_back(onBoard[j]);
            }
        }
        onBoard = b;
    }
    
    bool item_is_onBoard(int index){
        for(int i=0;i<onBoard.size();i++){
            if(onBoard[i] == index){
                return true;
            }
        }
        return false;
    }
};

vector<vector<pair<pathInfo, pathInfo>>> all_PU_to_other_points;
vector<vector<pair<pathInfo, pathInfo>>> all_DO_to_other_points;

/*each element is one_depot to all pickups
 eg. depots_to_all_PU[0][1] is path from depots[0] to delivery 1 's pick up*/
vector<vector<pathInfo>> depots_to_all_PU;
vector<vector<pathInfo>> all_DO_to_depots;


bool one_source_multi_dest(Node* sourceNode, vector<IntersectionIndex> destinations, double turn_penalty);
bool make_depot_to_PU(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty);
vector<StreetSegmentIndex> convertListToVec(list<StreetSegmentIndex> l);
bool make_PU_to_other_points(const std::vector<DeliveryInfo>& deliveries, double turn_penalty);
bool make_DO_to_points(const std::vector<DeliveryInfo>& deliveries,const std::vector<int>& depots, double turn_penalty);
bool precompute_all_paths(const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, double turn_penalty);
CourierSubpath initializeSubpath(IntersectionIndex start, IntersectionIndex end, std::vector<int> path_, vector<unsigned> pickupIndex);
double computeCourierPathTravelTime(double turn_penalty, vector<CourierSubpath> a);

vector<int> find_indices_of_do(IntersectionIndex index, const std::vector<DeliveryInfo>& deliveries);
vector<int> find_indices_of_pu(IntersectionIndex index, const std::vector<DeliveryInfo>& deliveries);

IntersectionIndex getStartInter(std::vector<int> path_);
IntersectionIndex getEndInter(std::vector<int> path_);
void clear_all_precomputed_paths();
vector<CourierSubpath> greedy(const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int>& depots, 
		            const float turn_penalty, 
		            const float truck_capacity,
                            int startdepotIndex);

//M4 GLOBALS

extern std::vector<std::vector<pathInfo>> pathInfoFromPickUps;
extern std::vector<std::vector<pathInfo>> pathInfoFromDropOffs;
extern std::vector<std::vector<pathInfo>> pathInfoFromDepots;

extern std::chrono::steady_clock::time_point start;

extern std::chrono::steady_clock::time_point testerStart;
extern std::chrono::steady_clock::time_point testerEnd;

//M4 HEURISTIC1

std::vector<CourierSubpath> greedyHeuristic1(const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int>& depots, 
		            const float turnPenalty, 
		            const float truckCapacity);

CourierSubpath updatePathInfo(IntersectionIndex startIntersection,
        IntersectionIndex endIntersection,
        int indexOfDelivery,
        int turnPenalty);

std::pair<int, int> findDepotWithClosestPickUp(const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int> &depots);

std::pair<int,bool> findClosestPickUpOrDropOff(const std::vector<DeliveryInfo> &deliveries, IntersectionIndex sourceIntersection, 
        const std::vector<int> &deliveryStatusVector, double truckWeight, float truckCapacity);

int findClosestDepot(const std::vector<int> &depots, IntersectionIndex intersection);
double computeVectorCourierPathTime(std::vector<CourierSubpath> path, float turnPenalty);

//MJ FUNCTIONS
std::pair<IntersectionIndex, PickUpIndex> find_next_delivery_or_pickup(const std::vector<DeliveryInfo> & deliveries, std::vector<bool> & completeDeliveries,
                                                                    std::vector<bool> & truckContainer, IntersectionIndex start, const float & truckLoad, const float truckCapacity );
CourierSubpath makeSubPath(int start, int end, int pickUpIndex, float turnPenalty);
std::vector<CourierSubpath> greedyHeuristic(const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int>& depots, 
		            const float turnPenalty, 
		            const float truckCapacity);


//Opt 2 functions
std::vector<std::vector<int>> makeDropOffIndices(const std::vector<CourierSubpath> & currentPath, const std::vector<DeliveryInfo> & deliveries);
bool containsPickUpAndDelivery(const int trial, const std::vector<CourierSubpath> & currentPath, const std::vector<std::vector<int>> & dropOffIndices);
bool checkOrderLegal(const std::vector<CourierSubpath> & newPath,std::vector<std::vector<int>> & dropOffIndices, const float & truckCapacity, const std::vector<DeliveryInfo> & deliveries);
std::vector<CourierSubpath> opt_two(int & trial, const std::vector<CourierSubpath> & currentPath, const std::vector<DeliveryInfo> & deliveries, 
                                               const std::vector<std::vector<int>> & dropOffIndices,const float & turnPenalty, const float & truckCapacity);


