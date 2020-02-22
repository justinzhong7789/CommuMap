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

//std::vector<LatLon> add_nodes(StreetSegmentIndex id);
//
//std::vector <streetSegmentsData> streetSegments;
//
//StreetSize streetsizes;
//
//// Define big streets as streets with length 1+ km
////std::vector <streetData> bigStreetsTable;
//
//ezgl::color GRASS(192,211,192);
//ezgl::color BACKGROUND(237,237,237);
//ezgl::color WATER(182,196,201);
//ezgl::color HIGHWAY(131,133,134);
//ezgl::color OUTLINE(197,197,197);



extern std::vector<LatLon> add_nodes(StreetSegmentIndex id);

extern std::vector <streetSegmentsData> streetSegments;

extern StreetSize streetsizes;

// Define big streets as streets with length 1+ km
////std::vector <streetData> bigStreetsTable;
//
//extern ezgl::color GRASS(192,211,192);
//extern ezgl::color BACKGROUND(237,237,237);
//extern ezgl::color WATER(182,196,201);
//extern ezgl::color HIGHWAY(131,133,134);
//extern ezgl::color OUTLINE(197,197,197);