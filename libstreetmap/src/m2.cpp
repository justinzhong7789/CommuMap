/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include "m2.h"
#include "m2_helper/helper.hpp"
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


void makeStreetSizeTable();
void makeSegments_OfStreets();

std::vector<LatLon> add_nodes(StreetSegmentIndex id);


struct streetSegmentsData {
    // node takes the start, end, and curve points (if applicable) of the street segment
    std::vector<LatLon> node;
};

struct streetData {
    std::vector<streetSegmentsData> segments;
    std::string name;
    double length;
    //std::vector<std::vector<LatLon>> node;
};

std::vector <streetSegmentsData> streetSegments;

// Define big streets as streets with length 1+ km
std::vector <streetData> bigStreetsTable;
std::vector<std::vector<int>> segments_OfStreets;

 void makeSegments_OfStreets(){    

    int street_ID;
    // Resizing global variable to allow for inserting through indexes
    segments_OfStreets.resize(getNumStreets());
    // Loops through all the street segments
    for(int j =0 ; j< getNumStreetSegments() ; ++j){
        street_ID = getInfoStreetSegment(j).streetID;
        // Inserts the segment ID into the index of the street ID 
        segments_OfStreets[street_ID].push_back(j);
    }
}
 
// add all street names length together
void makeStreetSizeTable(){
    streetData street_data;
    streetSegmentsData street_segment_data;

    for (StreetIndex i=0; i<getNumStreets(); i++){
        double street_length = 0;
        street_data.segments.clear();
        for (int j=0; j<segments_OfStreets[i].size(); j++){
            StreetSegmentIndex id = segments_OfStreets[i][j];
            street_length += find_street_segment_length(id);
            street_segment_data.node = add_nodes(id);
            street_data.segments.push_back(street_segment_data);
        }
        street_data.name = getStreetName(i);
        street_data.length = street_length;
        
        if (street_length > 10000){           
            bigStreetsTable.push_back(street_data);
        }
    }
}

/*
void makeStreetsSizeTable(){
    streetData street_data;
    InfoStreetSegment info;
    for (StreetIndex i=0; i<getNumStreets(); i++){
        double street_length = 0;
        (street_data.node).clear();
        for (StreetSegmentIndex j=0; j<segmentsOfStreets[i].size(); j++){
            street_length += find_street_segment_length(j);
            info = getInfoStreetSegment(j);
            (street_data.node).push_back(add_nodes(j));
        }
        street_data.name = getStreetName(i);
        street_data.length = street_length;
        
        if (street_length > 1000){
            
            bigStreetsTable.push_back(street_data);
        }
    }
}*/
/*    
void makeBigStreetsTable(){
    streetSegmentsData temp;
    InfoStreetSegment info;
    for (StreetSegmentIndex id = 0; id<getNumStreetSegments(); id++){
         
        // If street length is greater than 1000m, add to the table
        //if (tableOfDivisors[id].first > 1000){
        // if (find_street_segment_length(id) > 500){
            info = getInfoStreetSegment(id);
            
            temp.node = add_nodes(id);
            temp.streetName = getStreetName(info.streetID);
            bigStreetsTable.push_back(temp);
        }
    }
} */
std::vector<LatLon> add_nodes(StreetSegmentIndex id){
    InfoStreetSegment info = getInfoStreetSegment(id);
    std::vector<LatLon> node_list;
    node_list.resize(info.curvePointCount+2);
    // Find LatLon of beginning of intersection
    node_list[0] = getIntersectionPosition(info.from);
    // If no curve points, find end of intersection
    if (info.curvePointCount == 0){
            node_list[1] = getIntersectionPosition(info.to);
    }
    else {
        for (int i=1; i<=info.curvePointCount+1; i++){
            if (i>info.curvePointCount){
                node_list[i] = getIntersectionPosition(info.to);
            } else {
                node_list[i] = getStreetSegmentCurvePoint(i-1, id);
            }
        }
    }
    return node_list;
}

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
