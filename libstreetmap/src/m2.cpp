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
double max_lat;
double min_lat;
double max_lon;
double min_lon;

void map_bounds();
void draw_streets();
void draw_main_canvas(ezgl::renderer *g);

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
    ezgl::rectangle initial_world({min_lon,min_lat}, {max_lon,max_lat});

    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    

    application.run(nullptr, nullptr, nullptr, nullptr);
}

void draw_main_canvas(ezgl::renderer *g){
    
    g->draw_rectangle({min_lon, min_lat}, {max_lon,max_lat});
    for (size_t i=0; i<intersections.size(); i++){
        float x = intersections[i].position.lon();
        float y = intersections[i].position.lat();
        
        float width = 0.001;
        float height = width;
        
        g->fill_rectangle({x,y}, {x+width, y+height});
    }
    for (size_t i=0; i<streetSegments.size(); i++){
//    for (size_t i=0; i<1; i++){
        for (size_t j=1; j<streetSegments[i].node.size(); j++){
            std::pair <float, float> start = {streetSegments[i].node[j-1].lon(), streetSegments[i].node[j-1].lat()};
            std::pair <float, float> end = {streetSegments[i].node[j].lon(), streetSegments[i].node[j].lat()};
            g->set_color(ezgl::BLACK);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
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
