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
#include <string>
#include <queue>

//=================function forward declarations=====================

using namespace std;

/* class Node {
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
        bestTime = 0; // Not applicable
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
    void set_reachingEdge(StreetSegmentIndex reachEdge);
    void set_bestTime(double time);
    void set_parent_id(IntersectionIndex intersect_parent_id);
    void set_visited(bool visit);
 };
 
 void Node::set_reachingEdge(StreetSegmentIndex reachEdge){
     reachingEdge = reachEdge;
 }
 void Node::set_bestTime(double time){
     bestTime = time;
 }
 void Node::set_parent_id(IntersectionIndex intersect_parent_id){
     parent_id = intersect_parent_id;
 }
 void Node::set_visited(bool ifVisited){
     visited = ifVisited;
 }
 */
 struct WaveElem{
    Node *node; //IntesectionIndex
    int edgeID; // StreetSegmentIndex
    double travelTime;
    double score;
    int directed; // is node within 190% of Euclidean distance
    
    WaveElem (Node *n, int id, float time, int constraint, double dist){
        node = n;
        edgeID = id;
        travelTime = time;
        directed = constraint;
        score = dist;
    }
};

struct greaterWE{
    int operator() (const WaveElem& we1, const WaveElem we2){
        return (we1.score > we2.score);
    }
};

// work in progress
struct comparatorWE{
    bool operator() (const WaveElem& we1, const WaveElem we2){
        return ((we1.directed < we2.directed) || ((we1.directed == we2.directed) && 
                (we1.score > we2.score)));
    }
};
//---------------------Variable Declarations -----------------------//
 
vector<Node*> nodeTable;
 
//--------------------- Function Declarations ---------------------// 
 /*
 void makeNodeTable();
 Node* getNodebyID(IntersectionIndex sourceID);
 bool bfsPath(Node* sourceNode, int destID, double turn_penalty);
 double travelTime(StreetSegmentIndex segID);
 list<StreetSegmentIndex> bfsTraceback(IntersectionIndex destID);
 */
 //--------------------------------------------------------------
 
 // Creates a node table for each intersection index in the map
 void makeNodeTable(){
     // Initialize vector size
     int nodeTable_size = getNumIntersections();
     
     for (IntersectionIndex id=0; id<nodeTable_size; id++){
         int seg_connected = getIntersectionStreetSegmentCount(id);
         nodeTable.push_back(new Node(id, seg_connected));
     }
 }
 
 // Gets a node by the intersection id
 Node* getNodebyID(IntersectionIndex sourceID){
    Node *sourceNode = nodeTable[sourceID];
    return sourceNode;
 }

 // Returns true if a path can be found, false otherwise
 // Updates the node table so that if a path is found, ids and parent ids can be found easily for traceback
 bool bfsPath (Node* sourceNode, int destID, double turn_penalty){
    // vector<WaveElem> wavefront;
    // Establish min heap
    priority_queue <WaveElem, vector<WaveElem>, comparatorWE> pq;
    // debugging
    int popped = 0;
    
    LatLon source = getIntersectionPosition(sourceNode->id);
    LatLon dest = getIntersectionPosition(destID);
    // Find absolute distance from source to destination
    double original_dist = find_distance_between_two_points({source, dest});
    double constraint_dist; // initialize constraint
    if(original_dist < 1500){
        constraint_dist = 2.5 * original_dist; // short routes have more flexibility, up to 250%
    } else { constraint_dist = original_dist * 1.25; }
    // First node
    pq.push(WaveElem(sourceNode, NO_EDGE, 0, 1, WORST_TIME));
    
    while (!pq.empty()){
        // If popped takes longer than this, chances are path cannot be found
        if (popped > 200000){
            //cout << "first test failed" << endl;
            return (false);
        }
            WaveElem wave = pq.top(); // get next element
            pq.pop(); // remove from wavefront
            Node *currNode = wave.node;
                       
            popped += 1;
            
            if (popped > 100000 && wave.directed == 0){
            //    cout << "second test failed" << endl;
                return false;
            }
            
            if (currNode->id == destID){
            //    cout << "Nodes popped: " << popped << endl;
                    return true;
            }
            currNode->set_visited(true);
            for (int i=0; i<currNode->outEdge; i++){      
                Node *toNode;
                StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);
                InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id);   
                
                bool legal = false;
                    // Legal check
                if (currNode->id == info_outEdge.from && currNode->parent_id != info_outEdge.to){
                    toNode = nodeTable[info_outEdge.to];
                    legal = true;
                }
                else if (currNode->id == info_outEdge.to && currNode->parent_id != info_outEdge.from && info_outEdge.oneWay != true){
                    toNode = nodeTable[info_outEdge.from];
                    legal = true;
                }
                    // Update nodes if legal
                if (legal && !toNode->visited){
                   double score;
                   // Calculate score using travel time and turn time
                    if (currNode->reachingEdge != NO_EDGE){
                        score = currNode->bestTime + find_street_segment_travel_time(outEdge_id)+turn_penalty*there_is_turn(currNode->reachingEdge, outEdge_id);
                    } else { score = find_street_segment_travel_time(outEdge_id); } // If first mode, only count travel time
                  
                if (score < toNode->bestTime || toNode->bestTime == 0){
                    toNode->set_reachingEdge(outEdge_id);
                    toNode->set_bestTime(score);
                       
                    // Update table 
                    nodeTable[toNode->id] = toNode;
                    // Update parent_id for reaching edge
                    nodeTable[toNode->id]->set_parent_id(currNode->id);
                    // Find abs distance of this node
                    LatLon current = getIntersectionPosition(toNode->id);
                    double abs_distance = find_distance_between_two_points({current, dest});
                        
                    // Testing constraints
                    int constraint = 0;
                    if (constraint_dist > abs_distance){
                        constraint = 1;
                    }                     
                    pq.push(WaveElem(toNode, outEdge_id, toNode->bestTime, constraint, score));
                    }
                }
            }
    }
    cout << "Failed path nodes popped: " << popped << endl;
    return false;
}

// Returns the path of street segment ids if a path exists
list<StreetSegmentIndex> bfsTraceback(StreetSegmentIndex destID){
    list<StreetSegmentIndex> path;
    Node* currNode = getNodebyID(destID);
    StreetSegmentIndex prevEdge = currNode->reachingEdge;
    
    while (prevEdge != NO_EDGE){
        path.push_front(prevEdge);
        // Find parent node
        currNode = nodeTable[currNode->parent_id];
        prevEdge = currNode->reachingEdge;
    }
    return path;
}

// Returns empty if a path cannot be found
vector<StreetSegmentIndex> find_path_between_intersections(
        const IntersectionIndex intersect_id_start,
        const IntersectionIndex intersect_id_end,
        const double turn_penalty){
    
    makeNodeTable();    
    
    Node *sourceNode = getNodebyID(intersect_id_start);
    bool found = bfsPath(sourceNode, intersect_id_end, turn_penalty);
    
    if (found){
        list<StreetSegmentIndex> l = bfsTraceback(intersect_id_end); // Call traceback
        // Convert list to vector
        vector<StreetSegmentIndex> v{make_move_iterator(begin(l)), make_move_iterator(end(l))};
        nodeTable.clear();
        return v;
    }
    else {
        nodeTable.clear();
        return {};
    }
}