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
void drawFeatures(ezgl::renderer *g );
void drawBuildings(std::vector<int> features, ezgl::renderer *g );
double x_between_2_points(LatLon first, LatLon second);
double y_between_2_points(LatLon first, LatLon second);

double findArea(double x1, double y1, double x2, double y2);

const int ZOOM_ONE   = 1;
const int ZOOM_TWO   = 190;
const int ZOOM_THREE = 534;
const int ZOOM_FOUR  = 1467;
const int ZOOM_FIVE  = 2591;
const int ZOOM_SIX   = 32736;
const int ZOOM_SEVEN = 91698;
const int ZOOM_EIGHT = 730567;

const ezgl::color GRASS(192,211,192);
const ezgl::color WATER(182,196,201);
const ezgl::color HIGHWAY(131,133,134);
const ezgl::color OUTLINE(197,197,197);
const ezgl::color BUILDINGS(224,224,224);

const double area_full_screen = 1831656;
//const double area_full_screen = 360000;
const double area_full_map = 0.34959285;