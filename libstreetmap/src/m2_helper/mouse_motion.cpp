#include "mouse_motion.hpp"

struct Direction{
    string Name;
    int direction; //1 -North, 2-East, 3-South, 4-West
    double degrees;
    double rad;
};

Direction getDirection(LatLon onePos, LatLon twoPos);

void act_on_mouse_click(ezgl::application *app, GdkEventButton* /*event*/, double x, double y){
    
    if(find_w_click){
      //  cout << "Mouse clicked at (" << x << "," << y << ")\n" ;
        LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
        int id = find_closest_intersection(pos);
        std::string search_text = getIntersectionName(id);
        std::cout << "Closest Intersection: " << getIntersectionName(id) << " ID: "<<id<<"\n";
        
        find_intersection(search_text, app, id);
    }
}

void drive_button(GtkWidget */*widget*/, ezgl::application *application)
{
    searchingRoute = true;
    //clean_map(application);
    cout<<"Drive was pressed!"<<endl;
    std::string turnEntry (gtk_entry_get_text(TurnPenaltyGlobal));
    std::stringstream ss;
    ss<<turnEntry;
    ss>>turn_penalty_entry;
    found_route_segments = testingNav();
     
    write_Directions(location_ID, destination_ID, found_route_segments, application);
    application->refresh_drawing();
    cout<<"done drawing"<<endl;
}

void walk_button(GtkWidget */*widget*/, ezgl::application *application)
{
    searchingRoute = true;
    cout<<"Walk was pressed"<<endl;
    found_route_segments = find_path_between_intersections(13, 51601, 0);
   application->refresh_drawing();
}

std::vector<int> testingNav(){
    
    //CHANGE THE INPUT OF FIND_PATH HERE
    std::vector<int> test = find_path_between_intersections(location_ID, destination_ID, turn_penalty_entry);
    return test;
}

void highlight_route(std::vector<int> seg_ids, ezgl::renderer *g){
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
            g->set_color(ezgl::BLUE);
            g->set_line_width(8);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
}

void write_Directions(int location, int destination, std::vector<int> seg_ids, ezgl::application *application)
{
    
    GtkLabel *display = (GtkLabel *)application->get_object("ScrollLabel");
    
    
    //Variables, (already kind of have these already though)
    string initialPoint = getIntersectionName(location);
    string finalPoint = getIntersectionName(destination);
    string start;
    string end;
    string middle;

    auto infoFirst = getInfoStreetSegment(seg_ids[0]);
    string firstStreet = getStreetName(infoFirst.streetID); 
  
    //NOT CHECKING FOR ONE-WAYS
    int nextInter;
    if(infoFirst.from == location){
        nextInter = infoFirst.to;
    }else{
        nextInter = infoFirst.from;
    }
    
    LatLon locationPos = getIntersectionPosition(location);
    LatLon nextInterPos = getIntersectionPosition(nextInter);
    
    int curveCount = infoFirst.curvePointCount;
    
    Direction firstMove;
    if(curveCount>0){
        LatLon locationCurve = closestCurvePoint(location, seg_ids[0]);
        firstMove = getDirection(locationPos, locationCurve);
    }else{
        firstMove = getDirection(locationPos, nextInterPos);
    }
    
    
    start = "Beginning route at " + getIntersectionName(location) +  "\n";
      
    if(firstStreet == "<unknown>"){
        start = start + "Go "+firstMove.Name+"\n";
    }else{
        start = start + "Go " + firstMove.Name +" on " + firstStreet + "\n";
    }
    cout<<"First rad is: "<<firstMove.rad<<endl;

    
    end = "Arrived at " + getIntersectionName(destination);
    
    int origFrom = location;
    Direction move1;
    Direction move2;
    int from1, from2, to1, to2;
    int fromTurn1, fromTurn2, toTurn1, toTurn2, turn;
    
    for(int i = 0; (i+1)<seg_ids.size() ; i++){
                
        //Keep track of which intersections go where
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
       
        if(there_is_turn(seg_ids[i],seg_ids[i+1]))
        {
            if((info1.curvePointCount!=0)&&(info2.curvePointCount!=0)){
                LatLon closest1 = closestCurvePoint(to1, seg_ids[i]);
                LatLon closest2 = closestCurvePoint(to1, seg_ids[i+1]);
                LatLon interPos = getIntersectionPosition(to1);
                move1 = getDirection(closest1,interPos);
                move2 = getDirection(interPos,closest2);//shouldnt this be the other way around? isnt there an order?
                string streetName = getStreetName(info2.streetID);
                if (streetName == "<unknown>"){middle = middle + turn_from_direction(move1.rad, move2.rad)+"\n";}
                else{
                    middle = middle + turn_from_direction(move1.rad,move2.rad) + " at " + getStreetName(info2.streetID) + "\n";
            
                }
            }
//            else{
//                if(turn == 0){
//                    toTurn1 = to1;
//                    turn++;
//                }
//
//                if(turn == 1){
//                   fromTurn2 = toTurn1;
//                   toTurn2 = to1;
//                   turn = 0;
//                   LatLon fromPos1 = getIntersectionPosition(toTurn1);
//                   LatLon toPos1 = getIntersectionPosition(fromTurn1);
//                   LatLon fromPos2 = getIntersectionPosition(toTurn2);
//                   LatLon toPos2 = getIntersectionPosition(fromTurn2);
//                   move1 = getDirection(fromPos1,toPos1);
//                   move2 = getDirection(fromPos2,toPos2);
//                   middle = middle + turn_from_direction(move1.rad,move2.rad) + " at " + getStreetName(info2.streetID) + "\n";
//                   fromTurn1 = toTurn1;
//                }
//            }
        }

        origFrom = to1;
    }
    
    const char * print;
    string total = start + middle + end;
    print = total.c_str();
    gtk_label_set_text(display, print);
}

string turn_from_direction(double curr, double next){    
    string turn;
    double check = next - curr + M_PI/2;
     
    if((-M_PI/2 <= check && check < M_PI/2) || ( M_PI*1.5 < check && check <= 2*M_PI)){
        turn = "Turn right";
        
    }
    else if((-M_PI < check && check<= -M_PI/2)||(M_PI/2 <= check && check<= M_PI) || ( M_PI<check && check < M_PI*1.5)){
        turn = "Turn left";
    }
    else if(check == M_PI||check == M_PI*1.5){
        turn = "Continue straight";
    }
    cout<<"Curr: "<<curr<<" next: "<<next<<" check: "<<(check*180/M_PI)<<" turning: "<<turn<<endl;
    return turn;
}

Direction getDirection(LatLon onePos, LatLon twoPos)
{
    
    Direction direction;
    double y = twoPos.lat()-onePos.lat();
    double x = twoPos.lon()-onePos.lon();

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

    if( x<0 && y<0){
        angle = angle - M_PI;
    }
    
      
    if(angle< 0){
        angle = angle +2*M_PI;
    }
    
    if(-M_PI/4 <= angle && angle < M_PI/4){
        name = "East";
    }
    else if (M_PI/4 <= angle && angle < 3*M_PI/4){
        name = "North";
    }
    else if ((3*M_PI/4 <= angle && angle <M_PI)||(-M_PI <= angle && angle <= -3*M_PI/4 )){
        name = "West";
    }
    else if(-3*M_PI/4 <= angle && angle < -M_PI/4){
        name = "South";
    }
    
    
    
    direction.Name = name;
    direction.rad = angle;
    cout<<"Angle: "<<(angle*180/M_PI)<<" Direction: "<<name<<endl;
    return direction;
}

LatLon closestCurvePoint(int interID, int segID){
    LatLon interPos = getIntersectionPosition(interID);
    auto infoSeg = getInfoStreetSegment(segID);
    int curveCount = infoSeg.curvePointCount;
    double closestLat,closestLon;
//    if(segID<getNumStreetSegments()){
//        cout<<"segID is good"<<endl;
//    }
    closestLat = 0;
    closestLon =0;
    double shortestDist = 1000;
    double currentDist;
    
    for(int i = 0; i<curveCount;i++){
        LatLon curvePos = getStreetSegmentCurvePoint(i,segID);
        currentDist = pow((curvePos.lat() - interPos.lat()),2) + pow((curvePos.lon() - interPos.lon()),2);
        currentDist = sqrt(currentDist);
        
        if(shortestDist>currentDist){
            shortestDist = currentDist;
            closestLat = curvePos.lat();
            closestLon = curvePos.lon();
        }
    }
    
    LatLon closestCurve (closestLat, closestLon);
    return closestCurve;

    
}

