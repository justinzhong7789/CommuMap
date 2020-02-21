#pragma once

#include "m1.h"
#include "m2.h"
#include "global_variables.hpp"
#include "m2_global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <string>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;
using namespace ezgl;


struct intersectionData {
    LatLon position;
    std::string name;
};

std::vector <intersectionData> intersections;

double max_lat;
double min_lat;
double max_lon;
double min_lon;

void map_bounds();

float y_from_lat(float lat);
float x_from_lon(float lon);
float lat_from_y(float lat);
float lon_from_x(float lon);

void zoom(ezgl::renderer *g);
void drawStreets(vector<StreetData> streets, ezgl::renderer *g);
void drawAllStreets(ezgl::renderer *g);

void zoom(ezgl::renderer *g){
    
    point2d *min = new point2d(min_lon, min_lat);
    point2d *max = new point2d(max_lon, max_lat);
    rectangle *full_map = new rectangle(*min, *max);
    rectangle current_map = g->get_visible_world();

    
    double zoom_level = ((full_map->m_second.x)-(full_map->m_first.x))/((current_map.m_second.x)-(current_map.m_first.x));
    
    if (20 < zoom_level){
        cout << "all"<< endl;
        drawAllStreets(g);
    }
    if (7 < zoom_level ){
        cout << "Local" << endl;
        drawStreets(streetsizes.local, g);
    }
    if (4 < zoom_level){ 
        cout << "Minor" << endl;
       drawStreets(streetsizes.minor, g);
    }
    if ( 2 < zoom_level){
        cout << "Major" << endl;
        drawStreets(streetsizes.major, g);
    }
    if ( 0.5 < zoom_level){
        cout << "Highway" << endl;
        drawStreets(streetsizes.highway, g);
    }
}

void drawStreets(vector<StreetData> streets, ezgl::renderer *g){
    
    for (int i=0; i<streets.size(); i++){
        // Iterate through every street segment in the street
        for (int j=0; j<streets[i].segments.size(); j++){
            // Iterate through every node in the street segment
            for (int k=1; k<streets[i].segments[j].node.size(); k++){
                std::pair <float, float> start = {x_from_lon(streets[i].segments[j].node[k-1].lon()), y_from_lat((streets[i].segments)[j].node[k-1].lat())};
                std::pair <float, float> end = {x_from_lon(streets[i].segments[j].node[k].lon()), y_from_lat(streets[i].segments[j].node[k].lat())};
                g->set_color(ezgl::BLACK);
                g->set_line_dash(ezgl::line_dash::none);
                g->draw_line({start.first, start.second}, {end.first, end.second});
            }
        }
    }
}

void drawAllStreets(renderer *g){
       for (size_t i=0; i<streetSegments.size(); i++){
        for (size_t j=1; j<streetSegments[i].node.size(); j++){
            std::pair <float, float> start = {x_from_lon(streetSegments[i].node[j-1].lon()), y_from_lat(streetSegments[i].node[j-1].lat())};
            std::pair <float, float> end = {x_from_lon(streetSegments[i].node[j].lon()), y_from_lat(streetSegments[i].node[j].lat())};
            g->set_color(ezgl::BLACK);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
}

float x_from_lon(float lon){
    double latAvg = (max_lat + min_lat)/2;
    double x = lon*cos(latAvg * 3.1415926535 /180);
    return x;
}

float y_from_lat(float lat){
    return lat;
}

float lon_from_x(float x){
    double latAvg = (max_lat + min_lat)/2;
    double lon = x / cos(latAvg * 3.1415926535 /180);
    return lon;
}

float lat_from_y(float y){
    return y;
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
