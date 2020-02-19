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
//mapBounds values
double max_lat;
double min_lat;
double max_lon;
double min_lon;

void map_bounds();
void draw_main_canvas(ezgl::renderer *g);

struct intersectionData {
    LatLon position;
    std::string name;
};

std::vector <intersectionData> intersections;

void draw_map(){
    ezgl::application::settings settings;
    
    // Include headers
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";
    
    // Create EZGL application
    ezgl::application application(settings);
    ezgl::rectangle initial_world({min_lon,min_lat}, {max_lon,max_lat});
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    
    //Not sure what this is for -M
    //I made a copy of finding map bounds in a different function below -M
    
//    intersections.resize(getNumIntersections());
//    for (int id=0; id<getNumIntersections(); id++){
//        intersections[id].position = getIntersectionPosition(id);
//        intersections[id].name = getIntersectionName(id);
//    }

    application.run(nullptr, nullptr, nullptr, nullptr);
}

void draw_main_canvas(ezgl::renderer *g){
    g->draw_rectangle({0,0}, {1000,1000});
    
    for (size_t i=0; i<intersections.size(); i++){
        float x = intersections[i].position.lon();
        float y = intersections[i].position.lat();
        
        float width = 10;
        float height = width;
        
        g->fill_rectangle({x,y}, {x+width, y+height});
    }
}

void map_bounds(){
    
    max_lat = getIntersectionPosition(getNumIntersections()).lat();
    min_lat = getIntersectionPosition(0).lat();
    max_lon = getIntersectionPosition(getNumIntersections()).lon();;
    min_lon = getIntersectionPosition(0).lon();
    
    intersections.resize(getNumIntersections());
    
    for (int id=0 ; id < getNumIntersections() ; id++){
        intersections[id].position = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);
         
        if(max_lat < intersections[id].position.lat()){
            max_lat = intersections[id].position.lat();
        }
        if(max_lon < intersections[id].position.lon()){
            max_lon = intersections[id].position.lon();
        }
        
        if(min_lat > intersections[id].position.lat()){
            min_lat = intersections[id].position.lat();
        }
        if(min_lon > intersections[id].position.lon()){
            min_lon = intersections[id].position.lon();
        }
        
    }
    
}
