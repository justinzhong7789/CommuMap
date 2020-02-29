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

//Zoom level variables
extern rectangle full_screen; 
extern rectangle full_map;

//Main zoom function
void zoom(ezgl::renderer *g);

//Draws streets with different widths not including <unknowns>
void drawStreets(vector<StreetData> streets, ezgl::renderer *g,  int width, ezgl::color colour); 
//Draws all the streets including <unknowns>
void drawAllStreets(ezgl::renderer *g, int width);
//Draws all the street names
void drawStreetNames(vector<StreetData> streets, ezgl::renderer *g, int font_size);
void nameStreets(ezgl::renderer *g);

//Draws one-way streets
void drawOneWay(ezgl::renderer *g);
//Highlights Streets
void highlightStreet(ezgl::renderer* g, int street_id);

//Constants for zoom
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

//Array of zoom levels
const int zoomArraySize = 8;
const int zoomLevel[zoomArraySize] = {
   2000000, //level 0
    730567, //level 1
    38000,  //level 2 
    4500,   //level 3
    300,    //level 4
    1       //level 5
};

//Previously calculated areas of original map and screen
const double area_full_screen = 1831656;
const double area_full_map = 0.34959285;

//Constants for colour
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


//Main zoom function
void map_bounds();
void zoomStreets(ezgl::renderer *g);
