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
    
    // Constructors
    Node(IntersectionIndex ID, int OutEdge){
        id = ID;
        parent_id = NO_ID; // Not applicable
        outEdge = OutEdge;
        reachingEdge = NO_EDGE; // Not applicable
        bestTime = 0; // Not applicable
    }
    Node (IntersectionIndex ID, IntersectionIndex ParentID, int OutEdge, int ReachingEdge, double BestTime){
        id = ID;
        parent_id = ParentID;
        outEdge = OutEdge;
        reachingEdge = ReachingEdge;
        bestTime = BestTime;
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

 
 /* 
 class Edge {
 public:
     // Member variables
     StreetSegmentIndex id;
     IntersectionIndex from;
     IntersectionIndex to;
     double travelTime; // travel time from intersection from->to
     
     // Constructors
     Edge(StreetSegmentIndex ID){
         
     }
     
     void set_id(StreetSegmentIndex segment_id);
     void set_to(IntersectionIndex intersect_to);
     void set_from(IntersectionIndex intersect_from);
     void set_travelTime(double time);
 };
 */
 
 struct WaveElem{
    Node *node; //IntesectionIndex
    int edgeID; // StreetSegmentIndex
    double travelTime;
    double distanceFromDest;
    
    WaveElem (Node *n, int id, float time, double dist){
        node = n;
        edgeID = id;
        travelTime = time;
        distanceFromDest = dist;
    }
};

struct greaterWE{
    int operator() (const WaveElem& we1, const WaveElem we2){
        return (we1.distanceFromDest > we2.distanceFromDest);
    }
};
/************** Variable Declarations ******************/
 
 vector<Node*> nodeTable;
 
/************** Function Declarations ******************/ 
 
 void makeNodeTable();
 Node* getNodebyID(IntersectionIndex sourceID);
 bool bfsPath(Node* sourceNode, int destID, double turn_penalty);
 double travelTime(StreetSegmentIndex segID);
 list<StreetSegmentIndex> bfsTraceback(IntersectionIndex destID);
 
 /******************************************************/
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
 }

 bool bfsPath (Node* sourceNode, int destID, double turn_penalty){
    // vector<WaveElem> wavefront;
    // Establish min heap
    priority_queue <WaveElem, vector<WaveElem>, greaterWE> pq;
    
    LatLon source = getIntersectionPosition(sourceNode->id);
    LatLon dest = getIntersectionPosition(destID);
    // Find absolute distance from source to destination
    double original_dist = find_distance_between_two_points({source, dest});
    // First node
    pq.push(WaveElem(sourceNode, NO_EDGE, 0, original_dist));
    
    while (!pq.empty()){
            WaveElem wave = pq.top(); // get next element
            pq.pop(); // remove from wavefront
            Node *currNode = wave.node;
            
            cout << currNode->id << endl;
            
            if (currNode->bestTime <= wave.travelTime + 0.1){            
            //if(wave.travelTime <= currNode->bestTime + 0.1){
                wave.edgeID = currNode->reachingEdge;
                wave.travelTime = currNode->bestTime;
                //currNode->reachingEdge = wave.edgeID;
                //currNode->bestTime = wave.travelTime;
                // Check for completed path 
                if (currNode->id == destID){
                    return true;
                }
                if (currNode->id == 131827){
                    cout << "stopeh" << endl;
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
                    // Update nodes if legal
                    bool update = false;
                    if (legal){
                        // If we have never been to this node before, allow update
                        if (toNode->bestTime == 0){
                            update = true;
                        }
                        // If node has been traversed but is slower than current path, allow update
                        else if (toNode->bestTime > currNode->bestTime){
                            update = true;
                        }
                        // If node has been traversed and is faster than current path, do not update
                        else { update = false; }
                    }
                    if (update){  
                        toNode->set_reachingEdge(outEdge_id);
                        toNode->set_bestTime(currNode->bestTime + travelTime(outEdge_id) + turn_penalty*there_is_turn(toNode->id, toNode->reachingEdge));
                        // Update table 
                        nodeTable[toNode->id] = toNode;
                        // Update parent_id for reaching edge
                        nodeTable[toNode->id]->set_parent_id(currNode->id);
                        // Find abs distance of this node
                        LatLon current = getIntersectionPosition(toNode->id);
                        double abs_distance = find_distance_between_two_points({current, dest});
                        pq.push(WaveElem(toNode, outEdge_id, currNode->bestTime+travelTime(outEdge_id)+turn_penalty*there_is_turn(toNode->id, toNode->reachingEdge), abs_distance));
                    }
                }

            }
    }
    return false;
}
 
 // Returns travel time of street segment
 // DOES NOT CURRENTLY ACCOUNT FOR TURN
double travelTime(StreetSegmentIndex segID){
    
    return find_street_segment_travel_time(segID);
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
        return v;
    }
    else {
        return {};
    }
}