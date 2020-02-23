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
