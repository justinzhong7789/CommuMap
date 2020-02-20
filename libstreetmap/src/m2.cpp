/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include "m2.h"
#include "m2_helper/zoom.hpp"
#include "m2_helper/global_structures.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
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

void draw_streets();
void draw_main_canvas(ezgl::renderer *g);

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
//    draw_streets();
    makeSegments_OfStreets();    
    makeStreetSizeTable();

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
    /*
    for (size_t i=0; i<streetSegments.size(); i++){
        for (size_t j=1; j<streetSegments[i].node.size(); j++){
            std::pair <float, float> start = {x_from_lon(streetSegments[i].node[j-1].lon()), y_from_lat(streetSegments[i].node[j-1].lat())};
            std::pair <float, float> end = {x_from_lon(streetSegments[i].node[j].lon()), y_from_lat(streetSegments[i].node[j].lat())};
            g->set_color(ezgl::BLACK);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
     */ 
    for (int i=0; i<bigStreetsTable.size(); i++){
        // Iterate through every street segment in the street
       for (int j=0; j<bigStreetsTable[i].segments.size(); j++){
            // Iterate through every node in the street segment
            for (int k=1; k<bigStreetsTable[i].segments[j].node.size(); k++){
                std::pair <float, float> start = {x_from_lon(bigStreetsTable[i].segments[j].node[k-1].lon()), y_from_lat((bigStreetsTable[i].segments)[j].node[k-1].lat())};
                std::pair <float, float> end = {x_from_lon(bigStreetsTable[i].segments[j].node[k].lon()), y_from_lat(bigStreetsTable[i].segments[j].node[k].lat())};
                g->set_color(ezgl::BLACK);
                g->set_line_dash(ezgl::line_dash::none);
                g->draw_line({start.first, start.second}, {end.first, end.second});
            }
        }
    }
    
    //draw the diagonal line across the 
    zoom(g);
}

/*
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
}*/
