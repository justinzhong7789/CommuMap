/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/*
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
            
            
            if(wave.travelTime <= currNode->bestTime + 0.1){
                if (currNode->reachingEdge!=123492){
                currNode->reachingEdge = wave.edgeID;
                currNode->bestTime = wave.travelTime;
                }
                // Check for completed path 
                if (currNode->id == destID){
                    return true;
                }
                if (currNode->id == 103387){
                    cout <<"hi" << endl;
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
                    // Update nodes if legal and if node is not slower
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
                        toNode->set_bestTime(currNode->bestTime + find_street_segment_travel_time(outEdge_id) + turn_penalty*there_is_turn(toNode->id, toNode->reachingEdge));
                        // Update table 
                        nodeTable[toNode->id] = toNode;
                        // Update parent_id for reaching edge
                        nodeTable[toNode->id]->set_parent_id(currNode->id);
                        // Find abs distance of this node
                        LatLon current = getIntersectionPosition(toNode->id);
                        double abs_distance = find_distance_between_two_points({current, dest});
                        pq.push(WaveElem(toNode, outEdge_id, currNode->bestTime + find_street_segment_travel_time(outEdge_id) + turn_penalty*there_is_turn(toNode->id, toNode->reachingEdge), abs_distance));
                    }
                }
            }
    }
    return false;
}*/