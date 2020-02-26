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

void makeStreetSizeTable();
void makeStreetsVector();

extern std::vector<LatLon> add_nodes(StreetSegmentIndex id);

extern std::vector <streetSegmentsData> streetSegments;

extern StreetSize streetsizes;

void sortFeatures();

void makePointsOfFeatures();

extern std::vector<std::vector<ezgl::point2d>> pointsOfFeatures;

extern double max_lat;
extern double min_lat;
extern double max_lon;
extern double min_lon;

float y_from_lat(float lat);
float x_from_lon(float lon);
float lat_from_y(float lat);
float lon_from_x(float lon);

const int HIGHWAY_LENGTH = 15000;
const int MAJOR_LENGTH = 3000;
const int MINOR_LENGTH = 750;
const int LOCAL_LENGTH = 100;


double findArea(double x1, double y1, double x2, double y2);

const FeatureType featureTypeList[10] = {Unknown, Park, Beach, Lake, River, Island, Building, Greenspace, Golfcourse, Stream};
