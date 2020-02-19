/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

<<<<<<< HEAD
#include "m1.h"
=======
/* 
 * File:   m2.cpp
 * Author: cuevasm2
 * 
 * Created on February 18, 2020, 7:20 PM
 */


>>>>>>> created m2.cpp
#include "m2.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include <string>
#include <vector>

<<<<<<< HEAD
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
    ezgl::rectangle initial_world({0,0}, {1000,1000});
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    
    
    intersections.resize(getNumIntersections());
    for (int id=0; id<getNumIntersections(); id++){
        intersections[id].position = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);
    }

    application.run(nullptr, nullptr, nullptr, nullptr);
}

void draw_main_canvas(ezgl::renderer *g){
    g->draw_rectangle({0,0}, {1000,1000});
<<<<<<< HEAD
    
    for (size_t i=0; i>intersections.size(); i++){
        float x = intersections[i].position.lon();
        float y = intersections[i].position.lat();
        
        float width = 10;
        float height = width;
        
        g->fill_rectangle({x,y}, {x+width, y+height});
    }
=======
=======
void draw_map () {
  ezgl::application::settings settings; 
  settings.main_ui_resource = "libstreetmap/resources/main.ui"; 
  settings.window_identifier = "MainWindow"; 
  settings.canvas_identifier = "MainCanvas";

  ezgl::application application(settings); 


  application.run(nullptr, nullptr,nullptr, nullptr);
>>>>>>> created m2.cpp
>>>>>>> 5597e5e341bf2bdbf85ae7eb44bdc11a325aba0d
}