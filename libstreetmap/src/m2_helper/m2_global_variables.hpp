#pragma once

#include "m1.h"
#include "m2.h"
#include "global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "global_variables.hpp"
#include <string>
#include <vector>
#include <iostream>
using namespace std;
using namespace ezgl; 

/*=======Original map max and min=======*/

extern double max_lat;
extern double min_lat;
extern double max_lon;
extern double min_lon;
extern std::vector<LatLon> add_nodes(StreetSegmentIndex id);
extern std::vector <StreetSegmentsData> streetSegments;
extern StreetSize streetsizes;

/*================Conversions================*/
float y_from_lat(float lat);
float x_from_lon(float lon);
float lat_from_y(float lat);
float lon_from_x(float lon);

ezgl:: point2d point2d_from_latlon(LatLon number);
LatLon latlon_from_point2d(ezgl::point2d point);

double findArea(double x1, double y1, double x2, double y2);

/*===================Set street lengths===================*/
const int HIGHWAY_LENGTH = 15000;
const int MAJOR_LENGTH = 3000;
const int MINOR_LENGTH = 750;
const int LOCAL_LENGTH = 100;

/*=================Helps with Feature types===================*/
const FeatureType featureTypeList[10] = {
    Unknown, 
    Park, 
    Beach, 
    Lake, 
    River, 
    Island, 
    Building, 
    Greenspace, 
    Golfcourse, 
    Stream
};

extern std::vector<std::vector<ezgl::point2d>> pointsOfFeatures;

/*===============Functions to set up global variables===============*/
void sortFeatures();
void makePointsOfFeatures();
void makeStreetSizeTable();
void makeStreetsVector();

extern const char* search_text;
