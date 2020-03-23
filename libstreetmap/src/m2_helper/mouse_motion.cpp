#include "mouse_motion.hpp"

struct Direction{
    string Name;
    int direction; //1 -North, 2-East, 3-South, 4-West
    double degrees;
    double rad;
};

Direction getDirection(int intersectionOne, int intesectionTwo);

void act_on_mouse_click(ezgl::application *app, GdkEventButton* event, double x, double y){
    
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
     
    //write_Directions(13, 51601, found_route_segments, application);
    application->refresh_drawing();
    cout<<"done drawing"<<endl;
}

void walk_button(GtkWidget */*widget*/, ezgl::application *application)
{
    cout<<"Walk was pressed"<<endl;
     clean_map(application);
}

std::vector<int> testingNav(){
    
    //CHANGE THE INPUT OF FIND_PATH HERE
    std::vector<int> test = find_path_between_intersections(location_ID, destination_ID, turn_penalty_entry);
    return test;
}

void highlight_route(std::vector<int> seg_ids, ezgl::renderer *g){
    cout<<"here"<<endl;
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
    
    GtkLabel *display = (GtkLabel *)application->get_object("Label");
    
    
    //Variables, (already kind of have these already though)
    string initialPoint = getIntersectionName(location);
    string finalPoint = getIntersectionName(destination);
    string total;
    string start;
    string end;
    string middle = " ";

    auto infoFirst = getInfoStreetSegment(seg_ids[0]);
    string firstStreet = getStreetName(infoFirst.streetID); 
  
    //NOT CHECKING FOR ONE-WAYS
    int nextInter;
    if(infoFirst.from == location){
        nextInter = infoFirst.to;
    }else{
        nextInter = infoFirst.from;
    }
    
    Direction firstMove = getDirection(location, nextInter);
    
      
    if(firstStreet == "<unknown>"){
        start = "Go "+firstMove.Name+"\n";
    }else{
    start = "Go " + firstMove.Name +"on " + firstStreet + "\n";
    }
    
    int origFrom = location;
    Direction move1;
    Direction move2;
    int from1, from2, to1, to2;
    
    for(int i = 0; i<seg_ids.size() ; i++){
        
        if(i+1<seg_ids.size()){
            
            
                
            auto info1 = getInfoStreetSegment(seg_ids[i]);
            auto info2 = getInfoStreetSegment(seg_ids[i+1]);


            from1 = origFrom;
            if(from1 == info1.from){
                to1 = info1.to;
            }else{
                to1 = info1.from;
            }

            if(to1 == info2.from){
                to2 = info2.to;
            }else{
                to2 = info2.from;
            }
            to1 = from2;
            move1 = getDirection(from1,to1);
            move2 = getDirection(from2,to2);
            
            if(there_is_turn(seg_ids[i],seg_ids[i+1])){
                middle = middle + "Turn " + turn_from_direction(move1.Name,move2.Name) + " at " + getStreetName(info2.streetID) + "\n";
                
            }
            
            origFrom = to1;
            
        }
      
    }
    const char * print;
    total = start + middle + end;
    print = total.c_str();
    gtk_label_set_text(display, print);
    
    
}

string turn_from_direction(string current, string next){
    if(current == "North" && next == "West"){
        return "left";
    }
    if(current == "North" && next == "East"){
        return "right";
    }
    if(current == "South" && next == "West"){
        return "right";
    }
    if(current == "South" && next == "East"){
        return "left";
    }
    if(current == "West" && next == "North"){
        return "right";
    }
    if(current == "West" && next == "South"){
        return "left";
    }
    if(current == "East" && next == "North"){
        return "left";
    }
    if(current == "West" && next == "South"){
        return "right";
    }
    return "straight";
}

Direction getDirection(int intersectionOne, int intersectionTwo){
    
    
    int id1 = intersectionOne;
    int id2 = intersectionTwo;
    Direction direction;
    LatLon onePos = getIntersectionPosition(id1);
    LatLon twoPos = getIntersectionPosition(id2);
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
    return direction;
}