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

extern bool load_success;

/*===================== Flags for Milestone 3 =====================*/

extern bool night;
extern bool searchingPOI;
extern bool searchingIntersections;
extern bool searchingStreet;
extern bool searchingRoute;
extern bool click_OnOff;
extern bool text_OnOff;
extern bool find_w_click;
extern int num_intersections;
extern int location_ID;
extern int destination_ID;

extern string locationText;
extern string destinationText;

extern GtkEntry *LocationTextGlobal;
extern GtkEntry *DestinationTextGlobal;
extern GtkEntry *TurnPenaltyGlobal;

extern vector<int> found_route_segments;
extern double turn_penalty_entry;

/*============== HELPER FUNCTIONS FOR CALLBACK FUNCTIONS ==============*/
void on_dialog_response(GtkDialog *dialog, gint /*response_id*/, gpointer /*user_data*/);
void setNight();
void setLight();
void makePOITypesTable();
bool elementAlreadyExists(string st, vector<string> vec);
void highlight_POI(ezgl::renderer *g);
void highlight_intersections(vector<int> intersection_ids,  ezgl::renderer *g);
void highlight_street(std::vector<int> street_seg_ids, ezgl::renderer *g);
void highlight_route(std::vector<int> nav_seg_ids, ezgl::renderer *g);
bool find_intersection(std::string search_text, ezgl::application *application, int intersectID);
std::vector<int> testingNav();
void draw_Segments(std::vector<int> seg_ids,ezgl::application *application);
void write_Directions(int location, int destination, std::vector<int> seg_ids,ezgl::application *application);
string turn_from_direction(double curr, double next);

bool find_street(ezgl::application *application,std::string search_text);

/*============== CALLBACK FUNCTIONS FOR BUTTONS ==============*/
void find_button(GtkWidget */*widget*/, ezgl::application *application);
void search_button(GtkWidget */*widget*/, ezgl::application *application);
void nightMode_button(GtkWidget *widget, ezgl::application *application);
void Load_Map(GtkWidget */*widget*/, ezgl::application *application);
void drive_button(GtkWidget */*widget*/, ezgl::application *application);
void walk_button(GtkWidget */*widget*/, ezgl::application *application);
void text_switch(GtkWidget */*widget*/, ezgl::application *application);
void window_button(GtkWidget */*widget*/, ezgl::application *application);
void draw_POI(GtkWidget */*widget*/, ezgl::application *application);
void close_button(GtkWidget */*widget*/, ezgl::application *application);
void location_entry(GtkWidget */*widget*/, ezgl::application *application);
void destination_entry(GtkWidget */*widget*/, ezgl::application *application);

void search_bar(GtkWidget */*widget*/,ezgl::application *application);

void clean_map(ezgl:: application* application);
