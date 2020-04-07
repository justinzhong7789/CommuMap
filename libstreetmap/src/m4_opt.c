#include "m2_helper/m4_opt.hpp"

std::vector<DeliveryInfo> deliveryOPT;
std::vector<CourierSubpath> solutionPath;
std::vector<CourierSubpath> tempPath;
std::vector<int> depotsOPT;
std::vector<std::vector<int>> deliveryOrderOpt;
std::vector<std::vector<int>> dropOff_indices;
std::vector<std::vector<int>> pickUp_indices;
double originalTime;

//std::vector<CourierSubpath> traveling_courier(const std::vector<DeliveryInfo>& deliveries,const std::vector<int>& depots,const float turn_penalty,const float truck_capacity){
//    opt_two();
//    return 0;
//}


void findPathTime(std::vector<CourierSubPath> solutionPath)
{
    for(int i = 0; i< solutionPath.size(); i++){
        originalTime += compute_path_travel_time(solutionPath[i].subpath);
    }
}

void makeDeliveryOrder(std::vector<CourierSubPath> solutionPath, std::vector<std::vector<int>> deliveryOrderOpt )
{
    
    findPathTime(solutionPath);
    
    int size = solutionPath.size();
    
    deliveryOrderOpt.resize(size);
    
    deliveryOrderOpt[0].push_back(solutionPath[0].start_intersection); //First Depot
    
    for(int i = 0 ; i < solutionPath.size() ; i++){
        
        std::vector<unsigned> pickUp;
        pickUp = solutionPath[i].pickUp_indices;
        
        for(int j = 0; j< pickUp.size() ; j++){
                
            if(pickUp[j] == 0){
                pickUp[j] = 0;
            }
            
            deliveryOrderOpt[i].push_back((-1)*pickUp[j]);
        }
        
        std::vector<int> dropOff;
        dropOff = dropOff_indices[i];
        
        for(int j = 0; j< dropOff.size() ; j++){
            
            if(dropOff[j] == 0){
                dropOff = 0;
            }
            deliveryOrderOpt[i].push_back(dropOff[j]);
        }
    }
    
    deliveryOrderOpt[size-1].push_back(solutionPath[size-1].end_intersection); //Last Depot
    
    
}

bool checkForNoDoubles(int index, std::vector<std::vector<int>> order)
{
    
    for( int i = index; i< index+3 ; i++ ){
        for( int j = 0 ; j< order[i].size(); j++){
            for(int k = 0; k< order[i].size*(); k++){
                if(order[i][j] == order[i][k]){
                    return false;
                }
            }
        }
    }
//    int a = abs(order[index].number);
//    int b = abs(order[index+1].number);
//    int c = abs(order[index+2].number);
//    
//    int A = order[index].index;
//    int B = order[index].index;
//    int C = order[index].index;
//    
//    
//    
//    
//    if(a == b || a == c || b == c){
//        return false;
//    }
    return true;
            
}

bool opt_two()
{
    
    for(int p = 1; p < deliveryOrderOpt.size(); i++){
        
        if (checkForNoDoubles(p, deliveryOrderOpt)){
            
            //Switch Places.
            std::vector<int> temp = deliveryOrderOpt[p];
            deliveryOrderOpt[p] = deliveryOrderOpt[p+2];
            deliveryOrderOpt[p+2] = temp;
            
            //Check for legality
            if(checkOrderLegality(deliveryOrderOpt, 0)){    
                int index = deliveryOrderOpt[p][0];
                int intersect;
                
                //Swaping whole paths (dont think this is possible because we're changing the paths completely)
//            std::vector<int> temp = solutionPath[p].subPath;
//            solutionPath[p].subPath = solutionPath[p+2].subPath;
//            solutionPath[p+2].subPath = temp;
                
                //Swapping the middle's to and from
                intersect = solutionPath[p+1].end_intersection;
                solutionPath[p+1].end_intersection = solutionPath[p+1].start_intersection;
                solutionPath[p+1].start_intersection = intersect;
                
                CourierSubpath o = solutionPath[p-1];
                CourierSubpath a = solutionPath[p];
                CourierSubpath b = solutionPath[p+1];
                CourierSubpath c = solutionPath[p+2];
                CourierSubpath z = solutionPath[p+3];
                
                
                b.start_intersection = c.end_intersection;
                b.end_intersection = a.start_intersection;
                a.start_intersection = o.end_intersection;
                a.end_intersection = c.end_intersection;
                c.end_intersection = z.start_intersection;
                
                
//                solutionPath[p-1].end_intersection = solutionPath[p+1].start_intersection;
//                solutionPath[p-1].end_intersection = solutionPath[p].start_intersection;
//                solutionPath[p].end_intersection = solutionPath[p+1].start_intersection;
//                solutionPath[p+2].start_intersection = solutionPath[p+1].end_intersection;
//                solutionPath[p+2].end_intersection = solutionPath[p+3].start_intersection;
                
                std::vector<int> newPath = find_path_between_intersection(a.start_intersection, a.end_intersection, 0);
                double newTime = compute_path_travel_time(newPath, 0);
                if(newTime > originalTime){
                    return true;
                }return false;
                
            }
            
        }
        
    }
}

bool checkOrderLegal(std::vector<std::vector<int>> order, double truckCapacity)
{
    
    double truckLoad;
    int size = deliveryOpt.size();
    std::vector<int> truckPackages[size];
    double weightPick;
    double weightDrop;
    
    for(int i = 0; i< order.size(); i++){
        
        for(int k = 0; k< order[i].size() ; k++ ){
            
            if(order[i][k]<0){ //Pick up
            
                int index = order[i][k];
                weightPick = deliveryOpt[index].weight;

                if((weightPick + truckLoad) >truckCapcity){
                    return false;
                }else{
                    truckLoad += weightPick;
                    truckPackages[index] = 1; //the one indicates that it is in the truck
                }
            }
            
            //WHAT HAPPENS FOR DROP OFFS
            else{
                int index = order[i][k];
                weightDrop = deliverOpt[index].weight;

                if(truckPackages[index]){ //Package is in the truck
                    truckCapacity -= weightDrop;
                    truckPackages[index] = 0;
                }else{
                    //If not found in the truck
                    return false;
                }
            }
            
            
        }
        
        //Does the pick up fit the truck Capacity?
        
    }
    
    return true;
}

void makeDropOffIndices(std::vector<CourierSubpath> solution)
{
   
    std::vector<std::vector<int>> dropOff_indices;
    
    for(int i = 0; i< solution.size(); i++){
        
        int startIntersection = solution[i].start_intersection;
        
        for(int j =0; j<deliveryOpt.size(); j++){
            if(startIntersection == deliverOpt[j].dropOff){
                dropOff_indices[i].push_back(j); //Add drop off item.
            }
        }
        
    }
}


//bool opt_three(){
//    //Get top subpath solution and put it in std::vector<CourierSubpath> solutionPath;
//    
//    std::vector<CourierSubpath> solutionPath = greedy(deliveryOPT, depotsOPT, 0,20);
//    bool legal;
//    double originalTime, newTime;
//    
//    //find longest route -- Might be able to find the longest solutionPath and pass the index into a global variable that can be called on while finding the route
//    //O(N*M) = O(N)
//    double longestLength = 0;
//    int longestIndex;
//    for(int i = 0 ; i< solutionPath.size(); i++){
//        //HAS A FOR LOOP INSIDE:  double get_length_of_segments(int start, std::vector<int> seg_ids, int end);
//        double length = get_length_of_segments(solutionPath[i].start_intersection, solutionPath[i].subpath, solutionPath[i].end_intersection);
//        if(length > longestLength){
//            longestLength = length; 
//            longestIndex = i;
//        }
//        
//        originalTime += compute_path_travel_time(solutionPath[i].subpath, 0);
//        
//    }
//    
//    //Change its to and from
//    CourierSubpath longestPath = solutionPath[longestIndex].subpath;
//    
//    int temp;
//    temp = longestPath.start_intersection;
//    longestPath.start_intersection = longestPath.end_intersection;
//    longestPath.end_intersection = temp;
//    
//    //Check legality
//    legal = checkLegalSolutionPath(solutionPath);
//    
//    
//    //Change to become legal
//    while(!legal){
//        std::vector<int> newPath;
//        //get previous path and make end_intersection be the start_intersection and find best path and calculate time
//        CourierSubpath previousPath = solutionPath[longestIndex - 1];
//        previousPath.end_intersection = longestPath.start_intersection;
//        newPath = find_path_between_intersections(previousPath.start_intersection, previousPath.start_intersection, 0);
//        previousPath.subpath = newPath;
//        legal = checkLegalSolutionPath(solutionPath);
//    }
//    
//    for(int i = 0; i< solutionPath.size(); i++){
//        newTime += compute_path_travel_time(solutionPath[i].subpath, 0);
//        
//    }
//    
//    if(newTime<originalTime){
//        return true;
//    }else{
//        return false;
//    }
//    
//}
//
//
////Make global variable that will place the drop off indices for each CourierSubpath by index
////std::vector<std::vector<int>> dropOff_indices;
////dropOff_indices[2][3] means that it's the "dropOff_indices" vector for the CourierSubpath solutionPath[2] and the [3] is the third element in the dropOff_indices vector (fourth item that will be delivered at that location)
//bool checkLegalSolutionPath(std::vector<std::vector<int>> dropOff_indices, std::vector<CourierSubpath> subPath){
//    
//    double truckCapacity;
//    int size = deliverOpt.size();
//    std::vector truckLoad[size];
//    
//    
//    for(int i = 0; i< subPath.size(); i++){
//        std::vector<int> pickUp = subPath[i].pickup_indices;
//        std::vector<int> dropOff = dropOff_indices[i];
//        
//        //CHECKS FOR PICK PACKAGING
//        double weight;
//        for(int j = 0 ; j< pickUp.size(); j++){
//            
//            int index = pickUp[j];
//            weight += deliverOpt[index].weight;
//        
//            if(weight>truckCapcity){
//                return false;
//            }else{
//                truckCapcity += weight;
//                truckLoad[index] = 1; //the one indicates that it is in the truck
//            }
//        }
//        
//        //CHECKS FOR DROP OFF
//        weight = 0;
//        for(int j =0; j<dropOff. size(); j++){
//            int index = dropOff[j];
//            weight = deliverOpt[index].weight;
//            
//            if(truckLoad[index]){
//                truckCapacity -= weight;
//            }else{
//                //If not found in the truck
//                return false;
//            }
//        }
//        //Does the pick up fit the truck Capacity?
//        
//    }
//}




//bool opt_two(){
//    
//    for(int p = 1; p < deliveryOrderOpt.size(); i++){
//        
//        if (checkForNoDoubles(p, deliveryOrderOpt)){
//            
//            //Switch Places.
//            int first = deliveryOrderOpt[p].number;
//            deliveryOrderOpt[p].number = deliveryOrderOpt[p+2].number;
//            deliveryOrderOpt[p+2].number = first;
//            
//            //Check for legality
//            if(checkOrderLegality()){
//                if(p!=1){
//                    int t = deliveryOrderOpt[p-1].index;
//                    int c;
//                    if(deliveryOrderOpt[p+2].)
//                    solutionPath[t].end_intersection =  
//                }
//            }
//            
//        }
//        else{
//            //NEXT 
//        }
//        
//        //Check to see that no two #s are within the scope
//        
//    }
//}
//
//bool checkForNoDoubles(int index, std::vector<Packages> order){
//    //Cant start at zero. start at 1
//    //Starts at 1
//    //Get value [1], and value [3]
//    
//    int a = abs(order[index].number);
//    int b = abs(order[index+1].number);
//    int c = abs(order[index+2].number);
//    
//    int A = order[index].index;
//    int B = order[index].index;
//    int C = order[index].index;
//    
//    if(a == b || a == c || b == c){
//        return false;
//    }
//    return true;
//    
//}
//
//bool checkOrderLegality(){
//    
//    double truckCurrentCapacity;
//    double truckCapacity;
//    int size = deliveryOpt.size();
//    std::vector<int> truckLoad[size];
//    int previousIndex = 0;
//    double weightPick; 
//    double weightDrop;
//    for(int i = 0; i< deliveryOrderOpt.size(); i++){
//        
//        if(deliveryOrderOpt[i]<0){ //then it;s a pick up
//           
//            int index = deliveryOrderOpt[i].number;
//            weightPick = deliveryOpt[index].weight; 
//            
//            if( weight+truckCurrentCapacity >truckCapacity){
//                return false;
//            }else{
//                truckCurrentCapacity += weightPick;
//                truckLoad[index] = 1; //the one indicates that it is in the truck
//            }
//        }
//        
//        //then it's a drop off
//        else{
//            int index = deliveryOrderOpt[j].number;
//            weightDrop = deliveryOpt[index].weight;
//            
//            if(truckLoad[index]){
//                truckCapacity -= weightDrop;
//                truckLoad[index] = 0;
//            }else{
//                //If not found in the truck
//                return false;
//            }
//            
//        }
//        
//    }
//    return true;
//}
//
//
//
//
//void makeDeliveryOrder(std::vector<int>){
//    
//    
//    deliveryOrderOpt.push_back(solutionPath[0].start_intersection); //First Depot
//    
//    for(int i =0 ; i < solutionPath.size(); i++){
//        std::vector<unsigned> pickUp;
//        pickUp = solutionPath[i].pickUp_indices;
//        
//        for(int j =0; j< pickUp.size() ; j++){
//                
//            if(pickUp[j] == 0){
//                pickUp[j] = 0;
//            }
//            Package pack;
//            pack.number = -pickUp[j];
//            pack.index = i;
//            deliveryOrderOpt.push_back(pack);
//        }
//        
//        std::vector<int> dropOff;
//        dropOff = dropOff_indices[i];
//        
//        for(int j = 0; j< dropOff.size() ; j++){
//            
//            if(dropOff[j] == 0){
//                dropOff = 0;
//            }
//            Package pack;
//            pack.number = dropOff[j];
//            pack.index = i;
//            deliveryOrderOpt.push_back(pack);
//        }
//    }
//    
//    deliveryOrderOpt.push_back(solutionPath[solutionPath.size()-1].end_intersection); //Last Depot
//    
//    
//}
//
//bool opt_three(){
//    //Get top subpath solution and put it in std::vector<CourierSubpath> solutionPath;
//    
//    std::vector<CourierSubpath> solutionPath = greedy(deliveryOPT, depotsOPT, 0,20);
//    bool legal;
//    double originalTime, newTime;
//    
//    //find longest route -- Might be able to find the longest solutionPath and pass the index into a global variable that can be called on while finding the route
//    //O(N*M) = O(N)
//    double longestLength = 0;
//    int longestIndex;
//    for(int i = 0 ; i< solutionPath.size(); i++){
//        //HAS A FOR LOOP INSIDE:  double get_length_of_segments(int start, std::vector<int> seg_ids, int end);
//        double length = get_length_of_segments(solutionPath[i].start_intersection, solutionPath[i].subpath, solutionPath[i].end_intersection);
//        if(length > longestLength){
//            longestLength = length; 
//            longestIndex = i;
//        }
//        
//        originalTime += compute_path_travel_time(solutionPath[i].subpath, 0);
//        
//    }
//    
//    //Change its to and from
//    CourierSubpath longestPath = solutionPath[longestIndex].subpath;
//    
//    int temp;
//    temp = longestPath.start_intersection;
//    longestPath.start_intersection = longestPath.end_intersection;
//    longestPath.end_intersection = temp;
//    
//    //Check legality
//    legal = checkLegalSolutionPath(solutionPath);
//    
//    
//    //Change to become legal
//    while(!legal){
//        std::vector<int> newPath;
//        //get previous path and make end_intersection be the start_intersection and find best path and calculate time
//        CourierSubpath previousPath = solutionPath[longestIndex - 1];
//        previousPath.end_intersection = longestPath.start_intersection;
//        newPath = find_path_between_intersections(previousPath.start_intersection, previousPath.start_intersection, 0);
//        previousPath.subpath = newPath;
//        legal = checkLegalSolutionPath(solutionPath);
//    }
//    
//    for(int i = 0; i< solutionPath.size(); i++){
//        newTime += compute_path_travel_time(solutionPath[i].subpath, 0);
//        
//    }
//    
//    if(newTime<originalTime){
//        return true;
//    }else{
//        return false;
//    }
//    
//}
//
//
////Make global variable that will place the drop off indices for each CourierSubpath by index
////std::vector<std::vector<int>> dropOff_indices;
////dropOff_indices[2][3] means that it's the "dropOff_indices" vector for the CourierSubpath solutionPath[2] and the [3] is the third element in the dropOff_indices vector (fourth item that will be delivered at that location)
//bool checkLegalSolutionPath(std::vector<std::vector<int>> dropOff_indices, std::vector<CourierSubpath> subPath){
//    
//    double truckCapacity;
//    int size = deliverOpt.size();
//    std::vector truckLoad[size];
//    
//    
//    for(int i = 0; i< subPath.size(); i++){
//        std::vector<int> pickUp = subPath[i].pickup_indices;
//        std::vector<int> dropOff = dropOff_indices[i];
//        
//        //CHECKS FOR PICK PACKAGING
//        double weight;
//        for(int j = 0 ; j< pickUp.size(); j++){
//            
//            int index = pickUp[j];
//            weight += deliverOpt[index].weight;
//        
//            if(weight>truckCapcity){
//                return false;
//            }else{
//                truckCapcity += weight;
//                truckLoad[index] = 1; //the one indicates that it is in the truck
//            }
//        }
//        
//        //CHECKS FOR DROP OFF
//        weight = 0;
//        for(int j =0; j<dropOff. size(); j++){
//            int index = dropOff[j];
//            weight = deliverOpt[index].weight;
//            
//            if(truckLoad[index]){
//                truckCapacity -= weight;
//            }else{
//                //If not found in the truck
//                return false;
//            }
//        }
//        //Does the pick up fit the truck Capacity?
//        
//    }
//}
//
//void makeDropOffIndices(std::vector<CourierSubpath> solution){
//   
//    std::vector<std::vector<int>> dropOff_indices;
//    
//    for(int i = 0; i< solution.size(); i++){
//        
//        int startIntersection = solution[i].start_intersection;
//        
//        for(int j =0; j<deliveryOpt.size(); j++){
//            if(startIntersection == deliverOpt[j].dropOff){
//                dropOff_indices[i].push_back(j); //Add drop off item.
//            }
//        }
//        
//    }
//}
//
//
//
