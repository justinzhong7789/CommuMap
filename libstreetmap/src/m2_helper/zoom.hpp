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

extern std::vector <intersectionData> intersections;

extern double max_lat;
extern double min_lat;
extern double max_lon;
extern double min_lon;

void map_bounds();

//x and y conversions
float y_from_lat(float lat);
float x_from_lon(float lon);
float lat_from_y(float lat);
float lon_from_x(float lon);

//Zoom level variables
extern rectangle full_screen; 
extern rectangle full_map;

//Main zoom function
void zoom(ezgl::renderer *g);

//Draws streets with different widths not including <unknowns>
void drawStreets(vector<StreetData> streets, ezgl::renderer *g,  int width); 
//Draws all the streets including <unknowns>
void drawAllStreets(ezgl::renderer *g, int width);

//Draws all the street names
void drawStreetNames(vector<StreetData> streets, ezgl::renderer *g, int font_size);

void makeFeaturesVector();
void drawFeatures(ezgl::renderer *g);
double x_between_2_points(LatLon first, LatLon second);
double y_between_2_points(LatLon first, LatLon second);
