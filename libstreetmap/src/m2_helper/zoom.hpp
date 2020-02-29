#pragma once

#include "m1.h"
#include "m2.h"
#include "global_variables.hpp"
#include "m2_global_variables.hpp"
#include "features.hpp"

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

//Zoom level variables
extern rectangle full_screen; 
extern rectangle full_map;

//Constants

//Main zoom function
void zoom(ezgl::renderer *g);
void nameStreets(ezgl::renderer *g);
void drawOneWay(ezgl::renderer *g);

//Draws streets with different widths not including <unknowns>
void drawStreets(vector<StreetData> streets, ezgl::renderer *g,  int width, ezgl::color colour); 
//Draws all the streets including <unknowns>
void drawAllStreets(ezgl::renderer *g, int width);

//Draws all the street names
void drawStreetNames(vector<StreetData> streets, ezgl::renderer *g, int font_size);
void drawStreetNamesTest(ezgl::renderer *g, int streetID, int segID);

void drawFeatures(ezgl::renderer *g );

void highlightStreet(ezgl::renderer* g, int street_id);
const int ZOOM_ONE   = 1;
const int ZOOM_TWO   = 190;
const int ZOOM_THREE = 534;
const int ZOOM_FOUR  = 1467;
const int ZOOM_FIVE  = 2591;
const int ZOOM_SIX   = 32736;
const int ZOOM_SEVEN = 91698;
const int ZOOM_EIGHT = 730567;
const int ZOOM_ZERO  = 2000000;

const double ZOOM_SCREEN_INIT = 1.5;

const ezgl::color GRASS(192,211,192);
const ezgl::color SAND(254,227,225);
const ezgl::color WATER(182,196,201);
const ezgl::color HIGHWAY(131,133,134);
const ezgl::color OUTLINE(197,197,197);
const ezgl::color BUILDINGS(224,224,224);
const ezgl::color BACKGROUND(237,237,237);
const ezgl::color FEATURE_NAMES(120,126,131);
const ezgl::color STREET_NAMES(88,94,97);
const double RADIAN_TO_DEGREE = 1.0 / DEGREE_TO_RADIAN;

const double area_full_screen = 1831656;
//const double area_full_screen = 360000;
const double area_full_map = 0.34959285;

const int zoomArraySize = 8;
//const int zoomLevel[zoomArraySize] = {
////    1, 
////    190, 
////    534, 
////    1467,
////    2691, 
////    32736,
////    91698,
//    730567,
//    91698,
//    32736,
//    2691,
//    1467,
//    534,
//    190,
//    1
//};  

const int zoomLevel[zoomArraySize] = {
   2000000, //level 0
    730567, //level a - 1
//    91698,  //level a - 1 
    38000,  //level l - 2 //draw 
  //  1600,   //level l -2 //draw only the names of the major
    4500,   //level minor -3
//    534,    //level major -3
    300,    //level major -4
    1 //level highway  -5
};


//Main zoom function
void map_bounds();
void zoomStreets(ezgl::renderer *g);
