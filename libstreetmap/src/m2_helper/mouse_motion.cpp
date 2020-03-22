#include "mouse_motion.hpp"

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
    found_route_segments = testingNav();
     found_route_segments = testingNav();
    for(int i=0;i<found_route_segments.size();i++){
        cout<<"route seg: "<<found_route_segments[i]<<endl;
    }
    application->refresh_drawing();
    cout<<"done drawing"<<endl;
}

void walk_button(GtkWidget */*widget*/, ezgl::application *application)
{
    cout<<"Walk was pressed"<<endl;
     clean_map(application);
}

std::vector<int> testingNav(){
    
    
    std::vector<int> test = {62, 1914, 1913,844, 845, 155, 1451, 1338, 156, 1861, 1862, 1863, 1864, 1865};
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
            g->set_line_width(12);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
}
//void write_Directions(int location, int destination, std::vector<int> seg_ids)
//{
//    
//}