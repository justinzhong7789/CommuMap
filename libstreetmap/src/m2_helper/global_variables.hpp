
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

/* Allows for global variable declarations over a multiple source (cpp) files */
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

struct StreetSegmentsData {
    // node takes the start, end, and curve points (if applicable) of the street segment
    StreetSegmentIndex id;
    std::vector<LatLon> node;
//    ezgl::point2d midpoint;
//    ezgl::point2d fromPos;
//    ezgl::point2d toPos;
    bool oneWay = false;
    LatLon midpoint;
    LatLon fromPos;
    LatLon toPos;
};

struct StreetData {
    StreetIndex id;
    std::vector<StreetSegmentsData> segments;
    std::string name;
    double length;
    float speed;

    //std::vector<std::vector<LatLon>> node;
};

//struct info{
//    int ID;
//    std::string name;
//};

struct StreetSize {
    std::vector<StreetData> highway;
    std::vector<StreetData> major;
    std::vector<StreetData> minor;
    std::vector<StreetData> local;
};

struct FeatureData {
    bool open;
    std::vector<ezgl::point2d> points;
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
    ezgl::rectangle current;
    double map; //level
    //ezgl::point2d map_first;
    //ezgl::point2d map_second;
    double screen; //level
    double level;  //joined level of screen and map
    double mapArea;
    int zcase;
    double small = 0.0;
};

extern Zoom zooms;

extern FeatureClass featuretypes;
extern FeatureData feature_data;
extern StreetSize streetsizes;



