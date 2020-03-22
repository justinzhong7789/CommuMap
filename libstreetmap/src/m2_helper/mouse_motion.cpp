#include "mouse_motion.hpp"

void act_on_mouse_click(ezgl::application *app, GdkEventButton* event, double x, double y){
    
    if(find_w_click){
      //  cout << "Mouse clicked at (" << x << "," << y << ")\n" ;
        LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
        int id = find_closest_intersection(pos);
        std::string search_text = getIntersectionName(id);
        std::cout << "Closest Intersection: " << getIntersectionName(id) << "\n";
        find_intersection(search_text, app, id);
    }
    
    
}
