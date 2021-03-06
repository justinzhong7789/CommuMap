//#ifndef M3_GLOBAL_HPP
//#define M3_GLOBAL_HPP
#pragma once


#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"

#include <map>
#include <list>
#include <unordered_map>
#include <math.h>
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <queue>
#include "m3.h"
#include "m2.h"
#include "MJ.hpp"
#include "m4.h"
using namespace std;
#define NO_EDGE -1
#define NO_ID -1

//this number is the biggest an int can hold in c++
#define WORST_TIME 2147483647
#define LARGEST_DISTANCE 2147483647.00

class Node {
 public:   
     // Class member variables
    IntersectionIndex id;
    IntersectionIndex parent_id;
    // Outgoing edge etc.
    // Edge outEdge;
    int outEdge;
    int reachingEdge; // ID of the edge used to reach this node
    double bestTime; // Shortest time found to this node so far
    bool visited;
    // Constructors
    Node(IntersectionIndex ID, int OutEdge){
        id = ID;
        parent_id = NO_ID; // Not applicable
        outEdge = OutEdge;
        reachingEdge = NO_EDGE; // Not applicable
        bestTime = WORST_TIME; // Not applicable
        visited = false;
    }
    Node (IntersectionIndex ID, IntersectionIndex ParentID, int OutEdge, int ReachingEdge, double BestTime, bool ifVisited){
        id = ID;
        parent_id = ParentID;
        outEdge = OutEdge;
        reachingEdge = ReachingEdge;
        bestTime = BestTime;
        visited = ifVisited;
    }
    
    // Class member functions
    void set_reachingEdge(StreetSegmentIndex reachEdge){
        reachingEdge = reachEdge;
    }
    void set_bestTime(double time){
        bestTime = time;
    }
    void set_parent_id(IntersectionIndex intersect_parent_id){
        parent_id = intersect_parent_id;
    }
    void set_visited(bool ifVisited){
        visited = ifVisited;
    }
};

/***************** Global Variables ********************/
struct WaveElem{
    Node *node; //IntesectionIndex
    double score;
    int directed;
    
    //WaveElem(toNode, outEdge_id, toNode->bestTime, constraint, score));
    WaveElem(Node* n, int constraint, double dist){
        node = n;
        directed = constraint;
        score = dist;
    }
};

struct comparatorWE{
    bool operator() (const WaveElem& we1, const WaveElem& we2){
        return ((we1.directed < we2.directed) || ((we1.directed == we2.directed) && 
                (we1.score > we2.score)));
    }
};

struct greaterWE{
    bool operator() (const WaveElem& we1, const WaveElem& we2){
        return (we1.score > we2.score);
    }
};
/***************** global variable*********************/

extern vector<Node*> nodeTable;

/************** Function Declarations ******************/ 

/*
 * pass in 2 street segment ids.
 * return true is both streets belong to the same street,
 * false if not
 */

// Pathfinding Functions
 bool there_is_turn(int from_seg_id, int to_seg_id);
 bool bfs_find_walk_path(Node* sourceNode, int destID, double turn_penalty, double walking_speed);
 Node* getNodebyID(IntersectionIndex sourceID);
 bool bfsPath(Node* sourceNode, int destID, double turn_penalty);
 double travelTime(StreetSegmentIndex segID);
 list<StreetSegmentIndex> bfsTraceback(IntersectionIndex destID);
 
 // Functions for node table
 extern void makeNodeTable();
 extern void deleteNodeTable();
 void reset_nodeTable();
 /******************************************************/
 
 
//#endif /*M3_GLOBAL_HPP*/