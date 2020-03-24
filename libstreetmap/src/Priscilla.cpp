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
#define NO_EDGE -1
#define NO_ID -1
using namespace std;


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
 
 void makeNodeTable();
 Node* getNodebyID(IntersectionIndex sourceID);
 bool bfsPath(Node* sourceNode, int destID, double turn_penalty);
 double travelTime(StreetSegmentIndex segID);
 list<StreetSegmentIndex> bfsTraceback(IntersectionIndex destID);
 
 //--------------------------------------------------------------
 void makeNodeTable(){
     // Initialize vector size
     int nodeTable_size = getNumIntersections();
     
     for (IntersectionIndex id=0; id<nodeTable_size; id++){
         int seg_connected = getIntersectionStreetSegmentCount(id);
         nodeTable.push_back(new Node(id, seg_connected));
     }
 }
 
 Node* getNodebyID(IntersectionIndex sourceID){
    Node *sourceNode = nodeTable[sourceID];
    return sourceNode;
 }/*
 
 bool bfsPath (Node* sourceNode, int destID, double turn_penalty){
     list<WaveElem> wavefront;
     wavefront.push_back(WaveElem(sourceNode, -1, 0));
     while (!wavefront.empty()){
         WaveElem wave = wavefront.front();
         wavefront.pop_front();
         Node *currNode = wave.node;
         
         if (wave.travelTime < currNode->bestTime+0.01){
             currNode->reachingEdge = wave.edgeID;
             currNode->bestTime = wave.travelTime;
             if (currNode->id == destID){
                 return true;
             }
             for (int i=0; i<currNode->outEdge; i++){      
                    Node *toNode;
                    StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);
                    InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id);
                    // If oneway illegal or if visited, then not legal
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
                    if (legal){
                        toNode->set_bestTime(currNode->bestTime + find_street_segment_travel_time(outEdge_id)+turn_penalty*there_is_turn(toNode->id, outEdge_id));
                        toNode->set_reachingEdge(outEdge_id);
                        toNode->set_parent_id(currNode->id);
                        
                        nodeTable[toNode->id] = toNode;
                        wavefront.push_back(WaveElem(toNode, outEdge_id, toNode->bestTime));
                    }
             }
         }
     }
     return false;
 }*/
 

 bool bfsPath (Node* sourceNode, int destID, double turn_penalty){
    // vector<WaveElem> wavefront;
    // Establish min heap
    priority_queue <WaveElem, vector<WaveElem>, comparatorWE> pq;
    
    LatLon source = getIntersectionPosition(sourceNode->id);
    LatLon dest = getIntersectionPosition(destID);
    // Find absolute distance from source to destination
    double original_dist = find_distance_between_two_points({source, dest});
    double constraint_dist = 1.9*original_dist; // set constraint as 190% of the distance
    // First node
    pq.push(WaveElem(sourceNode, NO_EDGE, 0, 1, original_dist));
    
    while (!pq.empty()){
            WaveElem wave = pq.top(); // get next element
            pq.pop(); // remove from wavefront
            Node *currNode = wave.node;
            
            //if (currNode->bestTime <= wave.travelTime + 0.01){            
            //if(wave.travelTime <= currNode->bestTime+0.01){
               // wave.edgeID = currNode->reachingEdge;
               // wave.travelTime = currNode->bestTime;
                //currNode->reachingEdge = wave.edgeID;
                //currNode->bestTime = wave.travelTime;
                // Check for completed path 
                if (currNode->id == destID){
                    return true;
                }
                currNode->set_visited(true);
                for (int i=0; i<currNode->outEdge; i++){      
                    Node *toNode;
                    StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);

                    InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id);
                    // If oneway illegal or if visited, then not legal
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
                        double score = currNode->bestTime + find_street_segment_travel_time(outEdge_id)+turn_penalty*there_is_turn(toNode->id, outEdge_id);
                        
                        if (score < toNode->bestTime || toNode->bestTime == 0){
                        toNode->set_reachingEdge(outEdge_id);
                        toNode->set_bestTime(currNode->bestTime + find_street_segment_travel_time(outEdge_id) + turn_penalty*there_is_turn(toNode->id, toNode->reachingEdge));
                       
                        // Update table 
                        nodeTable[toNode->id] = toNode;
                        // Update parent_id for reaching edge
                        nodeTable[toNode->id]->set_parent_id(currNode->id);
                        // Find abs distance of this node
                        LatLon current = getIntersectionPosition(toNode->id);
                        double abs_distance = find_distance_between_two_points({current, dest});
                        
                        // testing constraints
                        int constraint = 0;
                        if (constraint_dist > abs_distance){
                            constraint = 1;
                        }                     
                        pq.push(WaveElem(toNode, outEdge_id, toNode->bestTime, constraint, score));
                        }
                    }
                }

          //}
    }
    return false;
}


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

vector<StreetSegmentIndex> find_path_between_intersections(
        const IntersectionIndex intersect_id_start,
        const IntersectionIndex intersect_id_end,
        const double turn_penalty){
    makeNodeTable();
    
    
    Node *sourceNode = getNodebyID(intersect_id_start);
    bool found = bfsPath(sourceNode, intersect_id_end, turn_penalty);
    
    if (found){
        list<StreetSegmentIndex> l = bfsTraceback(intersect_id_end);
        vector<StreetSegmentIndex> v{make_move_iterator(begin(l)), make_move_iterator(end(l))};
        nodeTable.clear();
        double a = compute_path_travel_time(v, 0.00000);
        cout << "travel time: " << a << endl;
        return v;
    }
    else {
        return {};
    }
}