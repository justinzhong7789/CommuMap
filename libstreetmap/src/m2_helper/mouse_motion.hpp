
#pragma once

#include "m1.h"
#include "m2.h"
#include "zoom.hpp"
#include "global_variables.hpp"
#include "m2_global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <vector>
#include <iostream>

void act_on_mouse_click(ezgl::application *app, GdkEventButton* event, double x, double y);


void act_on_mouse_click(ezgl::application *app, GdkEventButton* event, double x, double y){
    
    cout << "Mouse clicked at (" << x << "," << y << ")\n" ;
    
    LatLon pos = LatLon(lat_from_y(y), lon_from_x(x));
    int id = find_closest_intersection(pos);
    
    std::cout << "Closest Intersection: " << getIntersectionName(id) << "\n";
    
}