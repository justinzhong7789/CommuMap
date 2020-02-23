/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include <string>
#include <iostream>
#include "m2.h"

//HEADER FILES
#include "m2_helper/mouse_motion.hpp"
#include "m2_helper/zoom.hpp"
#include "m2_helper/m2_global_variables.hpp"
#include "m2_helper/global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "point.hpp"
#include <string>
#include <map>
#include <list>
#include <unordered_map>
#include <math.h>
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <iostream>

using namespace std;
using namespace ezgl;

void draw_main_canvas(ezgl::renderer *g);


//Determining the first time drawn
int numTimesDrawn = 0;
ezgl::color BACKGROUND(237,237,237);

void draw_map() {
    ezgl::application::settings settings;

    // Include headers
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    // Create EZGL application
    ezgl::application application(settings);
    intersections.resize(getNumIntersections());

    map_bounds();
    cout << "minLon: " << min_lon << " maxLon: " << max_lon << endl;
    cout << "minLat: " << min_lat << " maxLat: " << max_lat << endl;

    makeStreetsVector();
    makeStreetSizeTable();
  //  sortFeatures();
    
    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},{x_from_lon(max_lon), y_from_lat(max_lat)});

    application.add_canvas("MainCanvas", draw_main_canvas, initial_world, BACKGROUND);


    application.run(nullptr, act_on_mouse_click, nullptr, nullptr);
}

void draw_main_canvas(ezgl::renderer *g) {

    if (numTimesDrawn == 0) {

        full_map.m_first.x = x_from_lon(min_lon);
        full_map.m_first.y = min_lat;
        
        full_map.m_second.x = x_from_lon(max_lon);
        full_map.m_second.y = max_lat;
        
        full_screen = g->world_to_screen(full_map);
    //    area_full_screen = findArea(full_map.m_first.x, full_map.m_first.y, full_map.m_second.x, full_map.m_second.y);
        numTimesDrawn++;
    }
    
    drawFeatures(g);
    zoom(g);

}

