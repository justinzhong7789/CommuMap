
#pragma once

#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"

#include <map>
#include <list>
#include <unordered_map>
#include <math.h>
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

/*==========================Global variables from Milestone 1 file=============================*/
double x_distance_between_2_points(LatLon first, LatLon second);
double y_distance_between_2_points(LatLon first, LatLon second);

extern std::vector<std::pair<double, double>> tableOfDivisors;
extern std::multimap<std::string, StreetIndex> capitalizedStreetNamesTable;
extern std::vector<std::vector<int>> segmentsOfStreets;
extern std::vector<std::vector<int>> intersectionsOfStreets;
extern std::vector<std::vector<int>> segmentsOfIntersections;

extern std::unordered_map<OSMID, const OSMWay*> OSMWayTable;
extern std::unordered_map<OSMID, const OSMNode*> OSMNodeTable;
typedef std::multimap<int, int>::iterator StreetsIt;
typedef std::vector<int>::iterator VectorIt;

const double RADIAN_TO_DEGREE = 5.29577951326093;

/*===============StreetSegments, Street, Intersections and Feature Data Structures =============*/
struct StreetSegmentsData {
    StreetSegmentIndex id;
    std::vector<LatLon> node;
    bool oneWay = false;
    LatLon midpoint; //individual values are in x and y not latlon
    LatLon fromPos;
    LatLon toPos;
    int angle;
};

struct StreetData {
    StreetIndex id;
    std::vector<StreetSegmentsData> segments;
    std::string name;
    double length;
    float speed;
};

struct FeatureData {
    bool open;
    std::vector<ezgl::point2d> points;
};

struct intersectionData {
    LatLon position;
    std::string name;
};

/*==================Structures to sort streets and features =====================*/
struct StreetSize {
    std::vector<StreetData> highway;
    std::vector<StreetData> major;
    std::vector<StreetData> minor;
    std::vector<StreetData> local;
};

struct FeatureClass{
    
    std::vector<int> bigparks;
    std::vector<int> beaches;
    std::vector<int> lakes;
    std::vector<int> rivers;
    std::vector<int> islands;
    std::vector<int> buildings;
    std::vector<int> greenspaces;
    std::vector<int> golfcourses;
    std::vector<int> streams;
    std::vector<int> unknownFeatures;

};

struct Zoom{
    ezgl::rectangle current;    //current map rectangle
    ezgl::rectangle full;       //full map rectangle
    double map;                 //map zoom level (area ratio)
    double screen;              //screen zoom level (area ratio)
    double level;               //joined level of screen and map
    double mapArea;             //current world map area
    int zcase;                  //level case numbers
    double small = 0.0;         
};

/*=======================Global Variables with structs======================*/

extern Zoom zooms;
extern std::vector <intersectionData> intersections;
extern FeatureClass featuretypes;
extern FeatureData feature_data;
extern StreetSize streetsizes;



