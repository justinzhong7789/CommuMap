#pragma once

#include "m1.h"
#include "m2.h"
#include "global_structures.hpp"
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

std::vector <intersectionData> intersections;

void zoom(ezgl::renderer *g);
void drawMajorStreets(ezgl::renderer *g);
void draw_streets(ezgl::renderer *g);

void zoom(ezgl::renderer *g){
    
    point2d *min = new point2d(min_lon, min_lat);
    point2d *max = new point2d(max_lon, max_lat);
    rectangle *full_map = new rectangle(*min, *max);
    rectangle current_map = g->get_visible_world();

    
    double zoom_level = ((full_map->m_second.x)-(full_map->m_first.x))/((current_map.m_second.x)-(current_map.m_first.x));
    if( zoom_level > 0.9 && zoom_level < 2){
        cout << "PRINTING MAJOR STREETS" << endl;
       // drawMajorStreets(g);
         draw_streets(g);
    }
    else if (zoom_level > 2.3){ 
        cout << "no"<< endl;
//        draw_streets(g);
    }
    
}

void drawMajorStreets(ezgl::renderer *g){
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
}

void draw_streets(ezgl::renderer *g){
//    streetSegments.resize(getNumStreetSegments());
//    for (StreetSegmentIndex id=0; id<streetSegments.size(); id++){
//        InfoStreetSegment info = getInfoStreetSegment(id);
//        streetSegments[id].node.resize(info.curvePointCount + 2);
//        // Find LatLon of beginning of intersection
//        streetSegments[id].node[0] = getIntersectionPosition(info.from);
//        // If no curve points, find LatLon of end of intersection
//        if (info.curvePointCount == 0){
//            streetSegments[id].node[1] = getIntersectionPosition(info.to);
//            // Connect the begin and end to form a street 
//        }
//        // Find all LatLons of each curve point
//        else {
//            for (int i=1; i<=info.curvePointCount+1; i++){
//                if (i>info.curvePointCount){
//                    streetSegments[id].node[i] = getIntersectionPosition(info.to);
//                } else {
//                streetSegments[id].node[i] = getStreetSegmentCurvePoint(i-1, id);
//                }
//            }
//        }
//    }
    
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
