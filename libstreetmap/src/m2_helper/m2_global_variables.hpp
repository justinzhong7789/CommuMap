#pragma once

#include "m1.h"
#include "m2.h"
#include "m4.h"
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
extern bool searchingWalkPath;
extern bool click_OnOff;
extern bool text_OnOff;
extern bool find_w_click;
extern bool usingIDs;
extern int num_intersections;
extern int location_ID;
extern int destination_ID;
extern int pickUp_ID;



extern string locationText;
extern string destinationText;

extern GtkEntry *LocationTextGlobal;
extern GtkEntry *DestinationTextGlobal;
extern GtkEntry *LocationIDGlobal;
extern GtkEntry *DestinationIDGlobal;
extern GtkEntry *TurnPenaltyGlobal;
extern GtkEntry *WalkingSpeedGlobal;
extern GtkEntry *WalkingTimeLimitGlobal;
extern GtkEntry *LocationComboEntry;
extern GtkEntry *DestinationComboEntry;

extern vector<int> found_route_segments;
extern vector<int> found_walk_segments;
extern vector<int> found_intersections;
extern double turn_penalty_entry;
extern double walking_speed_entry;
extern double walking_time_limit_entry;

/*============== HELPER FUNCTIONS FOR CALLBACK FUNCTIONS ==============*/
void on_dialog_response(GtkDialog *dialog, gint /*response_id*/, gpointer /*user_data*/);
void setNight();
void setLight();
void makePOITypesTable();
bool elementAlreadyExists(string st, vector<string> vec);
void highlight_POI(ezgl::renderer *g);
void highlight_intersections(vector<int> intersection_ids,  ezgl::renderer *g);
void highlight_street(std::vector<int> street_seg_ids, ezgl::renderer *g);
void highlight_route(std::vector<int> nav_seg_ids, ezgl::renderer *g, ezgl::color colour);
bool find_intersection(std::string search_text, ezgl::application *application, int intersectID);
void draw_Segments(std::vector<int> seg_ids,ezgl::application *application);
std::string write_drive_directions(int location, int destination, std::vector<int> seg_ids,ezgl::application *application);
string turn_from_direction(double curr, double next);
LatLon closestCurvePoint(int interID, int segID);
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
void location_combo(GtkComboBox *widget, ezgl::application * application);
void search_bar(GtkWidget */*widget*/,ezgl::application *application);
void combo_entry(GtkComboBox *widget, ezgl::application * application);


bool validIntersection(std::string street1, std::string street2, ezgl::application *application);
void useIDs_button(GtkWidget *widget, ezgl::application *application);

void clean_map(ezgl:: application* application);
double get_length_of_segments(int start, std::vector<int> seg_ids, int end);
std:: string write_middle_directions(int location, std::vector<int> seg_ids, ezgl::application *application,int * lastIntersection);
void show_destination_autofill(GtkWidget *widget, ezgl::application *application);
void destination_combo(GtkComboBox *widget, ezgl::application * application);
void write_walk_path_directions(int location, int destination, std::vector<int>walk_seg_ids, std::vector<int> drive_seg_ids, ezgl::application *application);
void show_location_autofill(GtkWidget *widget, ezgl::application *application);
void write_drive_path_directions(int location, int destination, std::vector<int> seg_ids, ezgl::application *application);
void drawStartEndPoints(ezgl::renderer *g);

void instructions(GtkWidget */*widget*/,ezgl::application *application);



//Global drawing deliveries
extern bool making_deliveries_show;
extern bool set_depots_show;
extern bool set_deliveries_show;
extern bool set_pickups_show;
extern bool set_dropoffs_show;
extern bool set_weight_show;
extern bool searchingDeliveryPath;
extern int numDepotsShow;
extern int numDeliveriesShow;
extern int weightShow;
extern int pickUpShow;
extern int dropOffShow;
extern int indexShow;
extern std::vector<int> depotsShow;
extern std::vector<DeliveryInfo> deliveriesShow;
//extern std::vector<int> pickupsIntersectionShow; //store all intersections to display on the
//extern std::vector<int> dropoffsIntersectionShow; //store all drop off intersections
extern std::vector<CourierSubpath> subpathShow;
extern string allDepotsShow;
extern string allPickUpsShow;
extern string allDropOffsShow;
extern string allWeightShow;
extern string allIndexShow;
extern string indexString;
extern float turnPenaltyShow;
extern float truckCapacityShow;
//GtkEntries for deliveries
extern GtkEntry *TruckCapacityGlobal;
extern GtkEntry *DeliveryTextGlobal;

//Call back functions for the delivery window
void delivery_button(GtkWidget */*widget*/, ezgl::application *application);
void delivery_entry(GtkWidget */*widget*/, ezgl::application *application);
void num_depots_entry(GtkWidget */*widget*/, ezgl::application *application);
void num_delivery_entry(GtkWidget */*widget*/, ezgl::application *application);
void depots_done(GtkComboBox /**widget*/, ezgl::application * /*application*/);
void pickups_done(GtkWidget */*widget*/,ezgl::application */*application*/);
void make_deliveries(GtkWidget */*widget*/,ezgl::application */*application*/);
void close_button_delivery(GtkWidget */*widget*/, ezgl::application *application);
//void weight_done(GtkWidget */*widget*/,ezgl::application *application);

void highlight_deliveries(ezgl::renderer * g);
void highlightDropOff(ezgl::renderer *g, int intersectionID, char num);
void highlightPickUp (ezgl::renderer *g, int intersectionID, char num);
void highlightDepot (ezgl::renderer *g, int intersectionID, char num);