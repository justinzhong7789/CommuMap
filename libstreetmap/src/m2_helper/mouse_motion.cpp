#include "mouse_motion.hpp"

void act_on_mouse_click(ezgl::application *app, GdkEventButton* event, double x, double y){
    
    cout << "Mouse clicked at (" << x << "," << y << ")\n" ;
    
    LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
    int id = find_closest_intersection(pos);
    
    std::cout << "Closest Intersection: " << getIntersectionName(id) << "\n";
    
    
    
}
