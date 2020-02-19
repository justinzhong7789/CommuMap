/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"

/* 
 * File:   m2.cpp
 * Author: cuevasm2
 * 
 * Created on February 18, 2020, 7:20 PM
 */

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
void draw_streets();
void draw_main_canvas(ezgl::renderer *g);
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

void draw_map(){
    ezgl::application::settings settings;
    
    // Include headers
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    
    // Create EZGL application
    ezgl::application application(settings);
     intersections.resize(getNumIntersections());   
   
    map_bounds();
    draw_streets();
    ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)}, {x_from_lon(max_lon),y_from_lat(max_lat)});

    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    

    application.run(nullptr, nullptr, nullptr, nullptr);
}

void draw_main_canvas(ezgl::renderer *g){

    g->draw_rectangle({x_from_lon(min_lon), y_from_lat(min_lat)}, {x_from_lon(max_lon),y_from_lat(max_lat)});
    for (size_t i=0; i<intersections.size(); i++){
        
        float x = x_from_lon(intersections[i].position.lon());
        float y = y_from_lat(intersections[i].position.lat());
        
        float width = 0.001;
        float height = width;
        
        g->fill_rectangle({x,y}, {x+width, y+height});
    }
    for (size_t i=0; i<streetSegments.size(); i++){
//    for (size_t i=0; i<1; i++){
        for (size_t j=1; j<streetSegments[i].node.size(); j++){
            std::pair <float, float> start = {x_from_lon(streetSegments[i].node[j-1].lon()), y_from_lat(streetSegments[i].node[j-1].lat())};
            std::pair <float, float> end = {x_from_lon(streetSegments[i].node[j].lon()), y_from_lat(streetSegments[i].node[j].lat())};
            g->set_color(ezgl::BLACK);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
    point2d *min = new point2d(min_lon, min_lat);
    point2d *max = new point2d(max_lon, max_lat);
    rectangle *full_map = new rectangle(*min, *max);
    rectangle current_map = g->get_visible_world();

//         g->set_color(0,30,140,255);
//        g->set_line_width(20);
//        g->draw_line({x_from_lon(min_lon),y_from_lat(min_lat)}, {x_from_lon(max_lon), y_from_lat(max_lat)});
    
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

void draw_streets(){
    streetSegments.resize(getNumStreetSegments());
    for (StreetSegmentIndex id=0; id<streetSegments.size(); id++){
        InfoStreetSegment info = getInfoStreetSegment(id);
        streetSegments[id].node.resize(info.curvePointCount + 2);
        // Find LatLon of beginning of intersection
        streetSegments[id].node[0] = getIntersectionPosition(info.from);
        // If no curve points, find LatLon of end of intersection
        if (info.curvePointCount == 0){
            streetSegments[id].node[1] = getIntersectionPosition(info.to);
            // Connect the begin and end to form a street 
        }
        // Find all LatLons of each curve point
        else {
            for (int i=1; i<=info.curvePointCount+1; i++){
                if (i>info.curvePointCount){
                    streetSegments[id].node[i] = getIntersectionPosition(info.to);
                } else {
                streetSegments[id].node[i] = getStreetSegmentCurvePoint(i-1, id);
                }
            }
        }
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
