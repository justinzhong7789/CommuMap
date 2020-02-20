
#pragma once

#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
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
