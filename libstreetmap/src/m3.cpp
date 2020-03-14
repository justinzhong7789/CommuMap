/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"

#include <vector>
#include <string>
#include <iostream>
#include <m1.h>
//#define V getNumIntersections();
//=================function forward declarations=====================

const int V = getNumIntersections(); //vertices
using namespace std;

vector<vector<double>> weighted_graph_of_intersections;

void create_weighted_graph_of_intersections();


double compute_path_walking_time(const std::vector<StreetSegmentIndex>&path,
        const double walking_speed, const double turn_penalty);

/*
int min_distance(int dist[], bool sptSet[]);
void dijkstra(int graph[][], int src);
void create_weighted_graph_for_intersections();
*/
bool there_is_turn(int from_seg_id, int to_seg_id);

//===================================================================



double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path,
        const double turn_penalty){
    int vecSize = path.size();
    int turnCount = 0;
    int totalTravelTime = 0;
    if(vecSize==0){return 0;}
    else{
        for(int i=0; i+1< vecSize; i++){
            if(there_is_turn(path[i],path[i+1])){
                turnCount++;
            }
        }
        for(int j=0;j<vecSize;j++){
            totalTravelTime += find_street_segment_travel_time(path[j]);
        }
        return (totalTravelTime +(turnCount*turn_penalty));
    }
}




double compute_path_walking_time(const std::vector<StreetSegmentIndex>&path,
        const double walking_speed, const double turn_penalty){
    return 0;
}

/***************/

std::vector<StreetSegmentIndex>find_path_between_intersections(const IntersectionIndex intersect_id_start,
        const IntersectionIndex intersect_id_end, const double turn_penalty){
    
    // If size==0
    if (intersect_id_start == intersect_id_end){
        std::cout << "Error: starting intersection is same as end!" << std::endl;
        return {-1};
    }
    // Using Dijkstra's
     std::vector<StreetSegmentIndex> sptSet; // Shortest path tree set
     return {0};
}
/*
// Find vertex with minimum distance from vertices not included in sptSet
int min_distance(int dist[], bool sptSet[]){
    int min = INT_MAX, min_index;
    
    for (int v = 0; v < V; v++){
        if (sptSet[v] == false && dist[v] <= min){
            min = dist[v];
            min_index = v;
        }
        return min_index;
    }
double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path,
        const double turn_penalty){
    return 0;
}
double compute_path_walking_time(const std::vector<StreetSegmentIndex>&path,
        const double walking_speed, const double turn_penalty){
    return 0;
}

std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> find_path_with_walk_to_pick_up(
        const IntersectionIndex start_intersection,
        const IntersectionIndex end_intersection,
        const double turn_penalty,
        const double walking_speed,
        const double walking_time_limit){
    return {{NULL}, {NULL}};
}
std::vector<StreetSegmentIndex>find_path_between_intersections(const IntersectionIndex intersect_id_start,
        const IntersectionIndex intersect_id_end, const double turn_penalty){
    create_weighted_graph_of_intersections();
    weighted_graph_of_intersections.clear();
    return {NULL};
}
//    
//    // If size==0
//    if (intersect_id_start == intersect_id_end){
//        cout << "Error: starting intersection is same as end!" << endl;
//        return -1;
//    }
//    // Using Dijkstra's
//     std::vector<StreetSegmentIndex> sptSet; // Shortest path tree set
//     
//}
//
//// Find vertex with minimum distance from vertices not included in sptSet
//int min_distance(int dist[], bool sptSet[]){
//    int min = INT_MAX, min_index;
//    
//    for (int v = 0; v < V; v++){
//        if (sptSet[v] == false && dist[v] <= min){
//            min = dist[v];
//            min_index = v;
//        }
//        return min_index;
//    }
//}
//
//void dijkstra(int graph[V][V], int src){
//    int dist[V]; // output array. holds shortest distance from src to V
//    bool sptSet[V]; // true if vertix i is included in spt
//    
//    for (int i=0; i<V; i++){
//        dist[i] = INT_MAX;
//        sptSet[i] = false;
//    }
//    dist[src] = 0;
//    // Find shortst path for all vertices
//    for (int count = 0; count < V-1; count++){
//        // Choose min distance vertex
//        int u = min_distance(dist, sptSet);
//        // Mark vertex as chosen
//        sptSet[u] = true;
//        
//        for (int v=0; v<V; v++){
//            // Update dist[v] if not in sptSet
//            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[v]+graph[u][v]<dist[v]){
//                dist[v] = dist[u] + graph[u][v];
//            }
//        }
//    }
//}

// Creates weighted graph connecting all intersections
// Weight represents time to traverse between intersections
// Weight will be negative for one-way streets that cannot be traversed (?)
void create_weighted_graph_of_intersections(){
    const int total_intersections = getNumIntersections();
    
    // Initialize size of inner vectors
    vector<double> initialize_zero(total_intersections, 0);

    // Add inner vectors into outer vector
    for (int i=0; i<total_intersections; i++){
        weighted_graph_of_intersections.push_back(initialize_zero);
    }
    initialize_zero.clear();
    
    // Row
    for (int i=0; i<getNumIntersections(); i++){
        int seg_total = getIntersectionStreetSegmentCount(i);
        for (int seg_count=0; seg_count < seg_total; seg_count++){
            // ASSUME THAT CORRESPONDING INFO WILL RETURN THE ORIGINAL INTERSECTION HAS STREETSEGMENT.TO
            StreetSegmentIndex seg_id = getIntersectionStreetSegment(i, seg_count); 
            InfoStreetSegment seg_info = getInfoStreetSegment(seg_id);     
            // LOGIC CHECKING; TO BE DELETED
            if (seg_info.to == i){
                cout << "Good" << endl;
            } else {
                cout << "Bad" << endl;
            }
            // Assign weight to edge if street is NOT one-way
            if (seg_info.oneWay == false){    
                IntersectionIndex j = seg_info.to;
                weighted_graph_of_intersections[i][j] = 0;
            }
        }
    }
}
*/
// if 
bool there_is_turn(int from_seg_id, int to_seg_id){
    int from_street_id = getInfoStreetSegment(from_seg_id).streetID;
    int to_street_id = getInfoStreetSegment(to_seg_id).streetID;
    if(from_street_id == to_street_id){return false;}
    else {return true;}
}