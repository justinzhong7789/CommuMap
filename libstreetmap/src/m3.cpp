/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#pragma once

#include "StreetsDatabaseAPI.h"

#include <vector>
#include <string>
#include <iostream>

#define V getNumIntersections();

double compute_path_travel_time(const std::vector<StreetSegmentIndex>& path,
        const double turn_penalty);

std::vector<StreetSegmentIndex>find_path_between_intersections(const IntersectionIndex intersect_id_start,
        const IntersectionIndex intersect_id_end, const double turn_penalty);

double compute_path_walking_time(const std::vector<StreetSegmentIndex>&path,
        const double walking_speed, const double turn_penalty);

std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> find_path_with_walk_to_pick_up(
        const IntersectionIndex start_intersection,
        const IntersectionIndex end_intersection,
        const double turn_penalty,
        const double walking_speed,
        const double walking_time_limit);

/***************/

std::vector<StreetSegmentIndex>find_path_between_intersections(const IntersectionIndex intersect_id_start,
        const IntersectionIndex intersect_id_end, const double turn_penalty){
    
    // If size==0
    if (intersect_id_start == intersect_id_end){
        cout << "Error: starting intersection is same as end!" << endl;
        return -1;
    }
    // Using Dijkstra's
     std::vector<StreetSegmentIndex> sptSet; // Shortest path tree set
     
}

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
}

void dijkstra(int graph[V][V], int src){
    int dist[V]; // output array. holds shortest distance from src to V
    bool sptSet[V]; // true if vertix i is included in spt
    
    for (int i=0; i<V; i++){
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
    dist[src] = 0;
    // Find shortst path for all vertices
    for (int count = 0; count < V-1; count++){
        // Choose min distance vertex
        int u = min_distance(dist, sptSet);
        // Mark vertex as chosen
        sptSet[u] = true;
        
        for (int v=0; v<V; v++){
            // Update dist[v] if not in sptSet
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[v]+graph[u][v]<dist[v]){
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }
}

// Creates weighted graph connecting all intersections
// Weight represents time to traverse between intersections
// Weight will be negative for one-way streets that cannot be traversed (?)
void create_weighted_graph_for_intersections(){
    // Initialize graph
    std::vector<std::vector<IntersectionIndex>[getNumIntersections()]> weighted_graph_for_intersections;
    weighted_graph_for_intersections.resize(getNumIntersections());
    
    for (int i=0; i<getNumIntersections(); i++){
        for (int j=0; j<getNumIntersections(); j++){
            if (i==j){
                weighted_graph_for_intersections[i][j] = 0;
            }
        }
    }
}