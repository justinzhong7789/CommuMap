#ifndef FEATURES_HPP
#define FEATURES_HPP

#include "m1.h"
#include "m2.h"
#include "global_variables.hpp"
#include "m2_global_variables.hpp"
#include "zoom.hpp"
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

double x_between_2_points(LatLon first, LatLon second);
double y_between_2_points(LatLon first, LatLon second);


void makeFeaturesVector();
void drawFeatures(std::vector<int> feature, ezgl::renderer *g) ;
void drawBuildings(std::vector<int> features, ezgl::renderer *g );
ezgl::color getFeatureColour(int i);

#endif /* FEATURES_HPP */
