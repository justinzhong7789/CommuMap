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
#include <list>
#include <string>
#include <iostream>
#include <m1.h>

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
        bestTime = -1; // Not applicable
    }
    Node (IntersectionIndex ID, IntersectionIndex ParentID, int OutEdge, int ReachingEdge, double BestTime){
        id = ID;
        parent_id = ParentID;
        outEdge = OutEdge;
        reachingEdge = ReachingEdge;
        bestTime = BestTime;
    }
    
    // Class member functions
    //void set_outEdges(vector<StreetSegmentIndex> edges);
    void set_reachingEdge(StreetSegmentIndex reachEdge);
    void set_bestTime(double time);
    //void set_id(IntersectionIndex intersect_id);
    void set_parent_id(IntersectionIndex intersect_parent_id);
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
    // double sort;
    
    WaveElem (Node *n, int id, float time){
        node = n;
        edgeID = id;
        travelTime = time;
    }
};
/************** Variable Declarations ******************/
 
 vector<Node*> nodeTable;
 // vector<Edge*> edgeTable;
 
/************** Function Declarations ******************/ 
 
 void makeNodeTable();
 void makeEdgeTable();
 Node* getNodebyID(IntersectionIndex sourceID);
 bool bfsPath(Node* sourceNode, int destID);
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
 
 void makeEdgeTable(){
     int edgeTable_size = getNumStreetSegments();
     // Unfinished
 }
 
 Node* getNodebyID(IntersectionIndex sourceID){
    Node *sourceNode = nodeTable[sourceID];
    return sourceNode;
 }

 bool bfsPath (Node* sourceNode, int destID){
    list<WaveElem> wavefront;
    wavefront.push_back(WaveElem(sourceNode, NO_EDGE, 0));
    
    while (!wavefront.empty()){
            WaveElem wave = wavefront.front(); // get next element
            wavefront.pop_front(); // remove from wavefront
            Node *currNode = wave.node;
            
            
            if(wave.travelTime < currNode->bestTime){
                currNode->reachingEdge = wave.edgeID;
                currNode->bestTime = wave.travelTime;
                // Check for completed path 
                if (currNode->id == destID){
                    return true;
                }
                for (int i=0; i<currNode->outEdge; i++){      
                    Node *toNode;
                    StreetSegmentIndex outEdge_id = getIntersectionStreetSegment(currNode->id, i);
                    InfoStreetSegment info_outEdge = getInfoStreetSegment(outEdge_id);
                    if (currNode->id == info_outEdge.from && info_outEdge.oneWay != true){
                        Node *toNode = nodeTable[info_outEdge.to];
                    }
                    else if (currNode->id == info_outEdge.to && info_outEdge.oneWay != true){
                        Node *toNode = nodeTable[info_outEdge.from];
                    }
                    toNode->set_reachingEdge(outEdge_id);
                    toNode->set_bestTime(toNode->bestTime + travelTime(outEdge_id));
                    // Update table 
                    nodeTable[toNode->id] = toNode;
                    // Update parent_id for reaching edge
                    nodeTable[outEdge_id]->set_parent_id(toNode->id);
                    wavefront.push_back(WaveElem(toNode, outEdge_id, currNode->bestTime+travelTime(outEdge_id)));
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
    Node *sourceNode = getNodebyID(intersect_id_start);
    bool found = bfsPath(sourceNode, intersect_id_end);
    
    if (found){
        list<StreetSegmentIndex> l = bfsTraceback(intersect_id_end);
        vector<StreetSegmentIndex> v{make_move_iterator(begin(l)), make_move_iterator(end(l))};
        return v;
    }
    else {
        return {NULL};
    }
}