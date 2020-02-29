
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


extern std::vector<std::pair<double, double>> tableOfDivisors;
extern std::multimap<std::string, StreetIndex> capitalizedStreetNamesTable;
extern std::vector<std::vector<int>> segmentsOfStreets;
extern std::vector<std::vector<int>> intersectionsOfStreets;
extern std::vector<std::vector<int>> segmentsOfIntersections;

extern std::unordered_map<OSMID, const OSMWay*> OSMWayTable;
extern std::unordered_map<OSMID, const OSMNode*> OSMNodeTable;
typedef std::multimap<int, int>::iterator StreetsIt;
typedef std::vector<int>::iterator VectorIt;

struct streetSegmentsData {
    // node takes the start, end, and curve points (if applicable) of the street segment
    StreetSegmentIndex id;
    std::vector<LatLon> node;
};

struct StreetData {
    std::vector<streetSegmentsData> segments;
    std::string name;
    double length;
    float speed;
    //std::vector<std::vector<LatLon>> node;
};

struct info{
    int ID;
    std::string name;
};

struct StreetSize {
    std::vector<StreetData> highway;
    std::vector<StreetData> major;
    std::vector<StreetData> minor;
    std::vector<StreetData> local;
};

struct FeatureSize {
    std::vector<int> eight;
    std::vector<int> seven;
    std::vector<int> six;
    std::vector<int> five;
    std::vector<int> four;
    std::vector<int> three;
    std::vector<int> two;
    std::vector<int> one;
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
    
//    std::vector<info> bigparks;
//    std::vector<info> beaches;
//    std::vector<info> lakes;
//    std::vector<info> rivers;
//    std::vector<info> islands;
//    std::vector<info> buildings;
//    std::vector<info> greenspaces;
//    std::vector<info> golfcourses;
//    std::vector<info> streams;
//    std::vector<info> unknownFeatures;
    
};

struct Zoom{
    double map;
    double screen;
    double level;
    int zcase;
    double small = 0.0;
};

extern Zoom zooms;

extern FeatureClass featuretypes;
extern FeatureSize featuresizes;

extern StreetSize streetsizes;



