#include "m1.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <vector>




/*========================Separation for MERGE error PREVENTRION ==============================*/
#pragma once

#include "m1.h"
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
using namespace ezgl; 

double max_lat;
double min_lat;
double max_lon;
double min_lon;

void map_bounds();
float y_from_lat(float lat);
float x_from_lon(float lon);

struct intersectionData {
    LatLon position;
    std::string name;
};

struct streetSegmentsData {
    // node takes the start, end, and curve points (if applicable) of the street segment
    std::vector<LatLon> node;
    std::string name;
};

std::vector <intersectionData> intersections;
std::vector <streetSegmentsData> streetSegments;

void zoom(ezgl::renderer *g);

void zoom(ezgl::renderer *g){
    
    point2d *min = new point2d(min_lon, min_lat);
    point2d *max = new point2d(max_lon, max_lat);
    rectangle *full_map = new rectangle(*min, *max);
    rectangle current_map = g->get_visible_world();

    
    double zoom_level = ((full_map->m_second.x)-(full_map->m_first.x))/((current_map.m_second.x)-(current_map.m_first.x));
    if( zoom_level > 0.9){
        cout << "what " << endl;
        
        g->set_color(0,0,140,255);
        g->set_line_width(10);
        g->draw_line({x_from_lon(min_lon),y_from_lat(min_lat) }, {x_from_lon(max_lon) , y_from_lat(max_lat) });
        
    }
    else { cout << "no"<< endl;
    }
}


float x_from_lon(float lon){
    double div1 = max_lat;
    double div2 = min_lat;
    double latAvg = (div1 + div2)/2;
    double newLon = lon*cos(latAvg * 3.1415926535 /180);
    return newLon;
}

float y_from_lat(float lat){
    return lat;
}


void map_bounds(){
    
    intersections.resize(getNumIntersections());
    max_lat = -999999;
    min_lat = 9999999;
    max_lon = -9999999;
    min_lon = 9999999;
    for (int id=0; id<getNumIntersections(); id++){
        intersections[id].position = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);
        
        max_lat = std::max(max_lat, intersections[id].position.lat());
        min_lat = std::min(min_lat, intersections[id].position.lat());
        max_lon = std::max(max_lon, intersections[id].position.lon());
        min_lon = std::min(min_lon, intersections[id].position.lon());
    }
    
}
