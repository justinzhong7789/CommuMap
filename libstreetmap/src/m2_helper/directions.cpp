#include "directions.hpp"

struct Direction{
    string Name;
    double rad;
};

//NEEDED THESE CUZ FOR SOME REASON IT WOULDNT CALCULATE IT PROPERLY
double pi2 = M_PI*2;
double pi1_2 = M_PI*0.5;
double pi3_2 = M_PI*1.5;
double pi5_2 = M_PI*2.5;
double pi1_4 = M_PI*0.25;
double pi3_4 = M_PI*0.75;
double pi5_4 = M_PI*1.25;
double pi7_4 = M_PI*1.75;

//Declared here just because I didnt want to transfer the struc it uses **which is above    
Direction getDirection(LatLon onePos, LatLon twoPos);


void act_on_mouse_click(ezgl::application *app, GdkEventButton* /*event*/, double x, double y){
    
    if(find_w_click){
      
        LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
        int id = find_closest_intersection(pos);
        std::string search_text = getIntersectionName(id);
        //Testing purposes
        std::cout << "Closest Intersection: " << getIntersectionName(id) << " ID: "<<id<<"\n";  
        find_intersection(search_text, app, id);
    }
}

//Display the route with the given value in the search entry bars
void drive_button(GtkWidget */*widget*/, ezgl::application *application)
{
    //bool that tells the screen to draw the route
    searchingRoute = true;
   
    //gets the turn penalty from the entry
    std::string turnEntry (gtk_entry_get_text(TurnPenaltyGlobal));
    std::stringstream ss;
    ss<<turnEntry;
    ss>>turn_penalty_entry;
    
    //gets the initial and final location of the route from the entry
    string loc = gtk_entry_get_text(LocationTextGlobal);
    string dest = gtk_entry_get_text(DestinationTextGlobal);
    
    //enters the value into my global variables
    std::stringstream ss1;
    std::stringstream ss2;
    ss1<<loc;
    ss2<<dest;
    ss1>>location_ID;
    ss2>>destination_ID;
    cout<<"Loc: "<<loc<<endl;
    cout<<"Dest: "<<dest<<endl;
    cout<<"Turn Penalty: "<<turnEntry<<endl;
    
    //Puts the segments in a vector that will be drawn later
    found_route_segments = find_path_between_intersections(location_ID, destination_ID, turn_penalty_entry);
    //Writes directions
    write_Directions(location_ID, destination_ID, found_route_segments, application);
    
    //Needs to refresh the drawing to print
    application->refresh_drawing();
    cout<<"done drawing"<<endl;
}

//In progress (for find_with_path_functiion)
void walk_button(GtkWidget */*widget*/, ezgl::application *application)
{
//    searchingWalkPath = true;
//    found_walk_segments = find_path_with_walk_to_pick_up(location_ID, destination_ID, turn_penalty_entry, 
//                                                         walking_speed, walking_time_limit).first;   
//    found_route_segments = find_path_with_walk_to_pick_up(location_ID, destination_ID, turn_penalty_entry, 
//                                                         walking_speed, walking_time_limit).second;
//    write_Walk_Path_Directions(location_ID, destination_ID, found_walk_segments, found_route_segments, application);
//    application->refresh_drawing();
}

//The thing that actually draw the path
void highlight_route(std::vector<int> seg_ids, ezgl::renderer *g, ezgl:: color colour){
    std::vector<LatLon> node;
    for(int i = 0; i<seg_ids.size();i++)
    {
        node = add_nodes(seg_ids[i]);
        for (int k=1; k< node.size(); k++){
            std::pair <float, float> start = {x_from_lon(node[k-1].lon()), y_from_lat(node[k-1].lat())};
            std::pair <float, float> end = {x_from_lon(node[k].lon()), y_from_lat(node[k].lat())};

            g->set_line_dash(ezgl::line_dash::none);
            g->set_line_cap(ezgl::line_cap::round);

            //Draw Fill Colour
            g->set_color(colour);
            g->set_line_width(8);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
}

//void write_walk_directions(int location, int destination, std::vector<int>drive_seg_ids, std::vector<int> walk_seg_ids, ezgl::application *application)
//{
//    GtkLabel *display = (GtkLabel *)application->get_object("ScrollLabel");
// 
//    //Variables, (already kind of have these already though)
//    string initialPoint = getIntersectionName(location);
//    string finalPoint = getIntersectionName(destination);
//    string total, totalWalk, totalDrive;
//    string startWalk, middleWalk, endWalk;
//    string startDrive, middleDrive, endDrive;
//    int pickUpIntersection;
//    const char * print;
//    
//    
//    if(walk_seg_ids.size() == 0 && drive_seg_ids.size() == 0){
//        total = "No directions found\n"; 
//        print = total.c_str();
//        gtk_label_set_text(display, print); 
//        return;
//    }
//    if(walk_seg_ids.size() ==0)
//    {
//        totalWalk = "Driver will pick you up at your location.\n"; 
//        print = total.c_str();
//        gtk_label_set_text(display, print); 
//        return;
//    }
//
//    //will seg fault if theres no value fo seg_ids[0]
//    auto infoFirst = getInfoStreetSegment(seg_ids[0]);
//    string firstStreet = getStreetName(infoFirst.streetID);
//    
//    //Figuring out which from and to is the initial intersection ID
//    int nextInter;
//    if(infoFirst.from == location){
//        nextInter = infoFirst.to;
//    }else{
//        nextInter = infoFirst.from;
//    }
//    
//    
//    LatLon locationPos = getIntersectionPosition(location);
//    LatLon nextInterPos = getIntersectionPosition(nextInter);
//    
//    //Getting whether you have to first go north south ect.
//    Direction firstMove;
//    firstMove = getDirection(locationPos, nextInterPos);
//    
//    //Makes the first sentence for walk path
//    string unknown;
//    if(initialPoint.find(unknown)!= std::string::npos)
//    {
//        startWalk = "Starting walking path \n";
//    }
//    else {startWalk = "Start walking from " + getIntersectionName(location) +  "\n";}
//    
//    if(firstStreet == "<unknown>"){
//        startWalk = startWalk + "Walk " + firstMove.Name + "\n";
//    }else{
//        startWalk = startWalk + "Walk " + firstMove.Name + " along " + firstStreet + "\n";
//    }
//    
//    middleWalk = write_middle_directions(location, walk_seg_ids, application, &pickUpIntersection);
//   
//    string pickUpName = getIntersectionName(pickUpIntersection);
//    //Make the last sentence for walk path
//    if(pickUpName.find(unknown)!= std::string::npos){
//        endWalk = "Arrived at the pick up location\nWait for the vehicle here\n";
//    }
//    else{endWalk = "Arrived at the pick up location, " + pickUpName + "\nWait for the vehicle here\n";}
//    
//  
//    total = start + middle + end;
//    print = total.c_str();
//    gtk_label_set_text(display, print);
//}

//Writes the directions 
void write_Directions(int location, int destination, std::vector<int> seg_ids, ezgl::application *application)
{
    
    GtkLabel *display = (GtkLabel *)application->get_object("ScrollLabel");
    
    
    //Variables, (already kind of have these already though)
    string initialPoint = getIntersectionName(location);
    string finalPoint = getIntersectionName(destination);
    string start, middle,end;
    int lastIntersect;

    auto infoFirst = getInfoStreetSegment(seg_ids[0]);
    string firstStreet = getStreetName(infoFirst.streetID); 
  
    //NOT CHECKING FOR ONE-WAYS
    //Figuring out which from and to is the initial intersection ID
    int nextInter;
    if(infoFirst.from == location){
        nextInter = infoFirst.to;
    }else{
        nextInter = infoFirst.from;
    }
    
    
    LatLon locationPos = getIntersectionPosition(location);
    LatLon nextInterPos = getIntersectionPosition(nextInter);
    
    //Getting whether you have to first go north south ect.
    Direction firstMove;
    firstMove = getDirection(locationPos, nextInterPos);
    
    //Makes the first sentence
    string unknown;
    if(initialPoint.find(unknown)!= std::string::npos)
    {
        start = "Beginning route \n";
    }
    else {start = "Beginning route at " + getIntersectionName(location) +  "\n";}
    if(firstStreet == "<unknown>"){
        start = start + "Go "+firstMove.Name+"\n";
    }else{
        start = start + "Go " + firstMove.Name +" on " + firstStreet + "\n";
    }
    //Make the last sentence
    if(finalPoint.find(unknown)!= std::string::npos){
        end = "Arrived at destination";
    }
    else{end = "Arrived at " + finalPoint;}
    

    middle = write_middle_directions(location, seg_ids, application, &lastIntersect);
   
    const char * print;
    string total = start + middle + end;
    print = total.c_str();
    gtk_label_set_text(display, print);
}

std:: string write_middle_directions(int location, std::vector<int> seg_ids, ezgl::application *application, int * lastIntersection)
{
        //Initializes the start at the initial intersection position
    std::string middle;
    int origFrom = location;
    Direction move1;
    Direction move2;
    int from1, from2, to1, to2;
    int lastSegTurnIdx;
    int prvSegTurnIdx;
    double street_length;
    //Making the middle sentence
    for(int i = 0; (i+1)<seg_ids.size() ; i++){
                
        //Keep track of which intersections are connected to which part of a segment (intersection from and to)
        auto info1 = getInfoStreetSegment(seg_ids[i]);
        auto info2 = getInfoStreetSegment(seg_ids[i+1]);
        from1 = origFrom;
        if(from1 == info1.from){
            to1 = info1.to;
        }else{
            to1 = info1.from;
        }
        from2 = to1;
        if(from2 == info2.from){
            to2 = info2.to;
        }else{
            to2 = info2.from;
        }
       
        //Looking for if there was a turn  
        if(there_is_turn(seg_ids[i],seg_ids[i+1])){
           
            //Gets the lat and lon positions of each segment:
            //First Segment 1:
            LatLon to1Pos = getIntersectionPosition(to1);
            LatLon from1Pos = getIntersectionPosition(from1);
            //Second Segment 2:
            LatLon to2Pos = getIntersectionPosition(to2);
            LatLon from2Pos = getIntersectionPosition(from2);
            
            //Gets the angle of each segment
            move1 = getDirection(from1Pos,to1Pos);
            move2 = getDirection(from2Pos,to2Pos);
            
            //Gets the turn string(which way it turns)
            string radString = turn_from_direction(move1.rad,move2.rad);
            string streetName = getStreetName(info2.streetID);
            if(streetName == "<unknown>"){
                street_length = get_length_of_segments(prvSegTurnIdx+1, seg_ids, i+1);
                stringstream getLength;
                getLength << (int)street_length;
                string lengthToGo;
                getLength >> lengthToGo;
                middle = middle + radString +" in "+ lengthToGo + " meters\n";
                       
            }
            else{
                middle = middle + radString + " at " + streetName + "\n";
            }
            lastSegTurnIdx = i;
            prvSegTurnIdx = i;
        }

        origFrom = to1;
       
    }
    
    *lastIntersection = to2;
    street_length = get_length_of_segments(lastSegTurnIdx, seg_ids, seg_ids.size());
    stringstream getLength;
    getLength << (int)street_length;
    string lengthToGo;
    getLength >> lengthToGo;
    
    middle = middle + "Continue for "+lengthToGo+" meters \n";
    return middle;
}

string turn_from_direction(double curr, double next){    
    
    //Algorithm to find the angle with the first intersection in the -y axis
    double check = next - curr + pi1_2;
    
    //Make sure the angle isnt less than -pi
    if(check<0){
        check = check + pi2;
    }
    
    //Very wide range just in case cuz i dont wanna keep checking for angles anymore
    if((-pi1_2<= check && check < pi1_2)||(pi3_2<= check && check < pi5_2)){
        return "Turn right";
    }
    else if((-pi3_2 <= check && check< -pi1_2)||(pi1_2  < check && check< pi3_2)){
        return "Turn left";
    }
    else{
         return "Continue straight"; //In the off chance that check = pi/2
    }
   
}

//Gets direction for the first sentence, but more importantly it gets the angles of the segments
Direction getDirection(LatLon onePos, LatLon twoPos)
{
    
    Direction direction;
    double y = twoPos.lat()-onePos.lat();
    double x = twoPos.lon()-onePos.lon();
    
    //Making sure there's no division by 0
    if(y == 0){
        if(x > 0){
            direction.Name = "East";
            direction.rad = 0.0;
        }
        else{
            direction.Name = "West";
            direction.rad = M_PI;
        }
        return direction;
    }
    
    if(x == 0){
        if(y > 0){
            direction.Name = "North";
            direction.rad = M_PI /2 ;
        }
        else{
            direction.Name = "South";
            direction.rad = -M_PI/2;
        }
        return direction;
    }
    
    double angle = atan(y/x);
    string name;
    //Because atan is positive if both x and y are negative.
    if( x<0 && y<0){
        angle = angle - M_PI;
    }
    //trying to prevent negative angles
    if(angle<0){
        angle = angle +pi2;
    }
    
    //Included the negative values just in case
    if((-pi1_4<= angle && angle < pi1_4)||(pi7_4 <= angle && angle <= pi2)){
        name = "East";
    }
    else if (pi1_4<= angle && angle < pi3_4 ){
        name = "North";
    }
    else if ((pi3_4 <= angle && angle <pi5_4)||(-pi5_4<= angle && angle < -pi3_4 )){
        name = "West";
    }
    else if((-pi3_4<= angle && angle < -pi1_4)||(pi5_4<= angle && angle < pi7_4 )){
        name = "South";
    }
    
    direction.Name = name;
    direction.rad = angle;
    return direction;
}

//Finding the closestCurvePoint
double get_length_of_segments(int start, std::vector<int> seg_ids, int end){
    double street_length = 0;
    for(int i = start; i < end; i++ ){
        street_length += find_street_segment_length(seg_ids[i]);
    }
    return street_length;
}
 

