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
void drawStreets(vector<StreetData> streets, ezgl::renderer *g,  int width); 
void drawAllStreets(ezgl::renderer *g, int width);
void drawStreetNames(vector<StreetData> streets, ezgl::renderer *g, int font_size);

void zoom(ezgl::renderer *g){
  
    point2d *min = new point2d(min_lon, min_lat);
    point2d *max = new point2d(max_lon, max_lat);
    rectangle *full_map = new rectangle(*min, *max);
    rectangle current_map = g->get_visible_world();
      
    
    double zoom_level = ((full_map->m_second.x)-(full_map->m_first.x))/((current_map.m_second.x)-(current_map.m_first.x));
    
    //check when zoom is close to 0
    int width = zoom_level*pow(2,1/zoom_level);
    
    if (20 < zoom_level){
        cout << "all"<< endl;
        drawAllStreets(g, width);
    }
    if (7 < zoom_level ){
        cout << "Local" << endl;
        drawStreets(streetsizes.local, g, width );//1
    }
    if (4 < zoom_level){ 
        cout << "Minor" << endl;
       drawStreets(streetsizes.minor, g, width);//2
    }
    if ( 2 < zoom_level){
        cout << "Major" << endl;
        drawStreets(streetsizes.major, g, width);//3
    }
    if ( 0.5 < zoom_level){
        cout << "Highway" << endl;
        //CURRENTLY NO SPECIFIC DRAWING FOR HIGHWAY

        drawStreets(streetsizes.highway, g, 5);//5
        if (6 < zoom_level){
            drawStreetNames(streetsizes.highway, g, 6);
        }
        if (10 < zoom_level){
            drawStreetNames(streetsizes.major, g, 6);
        }
        if (15 < zoom_level){
            drawStreetNames(streetsizes.minor, g, 6);
        }
        if (25 < zoom_level){
            drawStreetNames(streetsizes.local, g, 6);
        }

        drawStreets(streetsizes.highway, g, width);//5
    }
    delete min;
    delete max;
    delete full_map;
}

void drawStreets(vector<StreetData> streets, ezgl::renderer *g, int width ){
    
    for (int i=0; i<streets.size(); i++){
        // Iterate through every street segment in the street
        for (int j=0; j<streets[i].segments.size(); j++){
            // Iterate through every node in the street segment
            for (int k=1; k<streets[i].segments[j].node.size(); k++){
                std::pair <float, float> start = {x_from_lon(streets[i].segments[j].node[k-1].lon()), y_from_lat((streets[i].segments)[j].node[k-1].lat())};
                std::pair <float, float> end = {x_from_lon(streets[i].segments[j].node[k].lon()), y_from_lat(streets[i].segments[j].node[k].lat())};
                
                g->set_line_dash(ezgl::line_dash::none);
                
                //Outline colour
                g->set_color(OUTLINE);
                g->set_line_width(width+2);
                g->draw_line({start.first, start.second}, {end.first, end.second});
                
                //Fill colour
                g->set_color(WHITE);
                g->set_line_width(width);
                g->draw_line({start.first, start.second}, {end.first, end.second});
                
            }
        }
    }
}

void drawAllStreets(renderer *g, int width){
       for (size_t i=0; i<streetSegments.size(); i++){
        for (size_t j=1; j<streetSegments[i].node.size(); j++){
            std::pair <float, float> start = {x_from_lon(streetSegments[i].node[j-1].lon()), y_from_lat(streetSegments[i].node[j-1].lat())};
            std::pair <float, float> end = {x_from_lon(streetSegments[i].node[j].lon()), y_from_lat(streetSegments[i].node[j].lat())};
            
            g->set_color(OUTLINE);
            g->set_line_width(width+2);
            g->draw_line({start.first, start.second}, {end.first, end.second});
            
            g->set_line_width(width);
            g->set_color(ezgl::WHITE);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
}

void drawStreetNames(vector<StreetData> streets, renderer *g, int font_size){
    double angle = 0;
    InfoStreetSegment info;
    //const float textsquare_width = 100;
    //ezgl::rectangle textsquare = {{100, 400}, textsquare_width, textsquare_width};
    
    for (size_t i=0; i<streets.size(); i++){
        std::string street_name = streets[i].name;
        float last_position = x_from_lon(getIntersectionPosition(getInfoStreetSegment(streets[i].segments[0].id).from).lon())-1;
        
        for (size_t j=0; j<streets[i].segments.size(); j++){
            
            info = getInfoStreetSegment(streets[i].segments[j].id);
            std::pair <float, float> start = {x_from_lon((getIntersectionPosition(info.from).lon())), y_from_lat(getIntersectionPosition(info.from).lat())};
            std::pair <float, float> end = {x_from_lon((getIntersectionPosition(info.to).lon())), y_from_lat(getIntersectionPosition(info.to).lat())};

            std::pair <float, float> center = {(end.first+start.first)/2, (end.second+start.second)/2};
            if (center.first > last_position + 1){
                last_position = center.first;
            angle = atan((end.second - start.second)/(end.first - start.first))*180/M_PI;
            
            g->set_font_size(font_size);
            g->set_color(ezgl::BLACK);
            g->set_text_rotation(angle);
            //g->draw_text({center.first, center.second}, street_name);
            //g->draw_text({start.first, start.second}, street_name, 100, 100);
            g->draw_text({center.first, center.second}, street_name);
            }
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
