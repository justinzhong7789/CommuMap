/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include <string>
#include <iostream>
#include "m2.h"

//HEADER FILES
#include "m2_helper/mouse_motion.hpp"
#include "m2_helper/zoom.hpp"
#include "m2_helper/search.hpp"
#include "m2_helper/features.hpp"
#include "m2_helper/m2_global_variables.hpp"
#include "m2_helper/global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <sstream>
#include "point.hpp"
#include <string>
#include <map>
#include <list>
#include <unordered_map>
#include <math.h>
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <iostream>
#include <camera.hpp>
using namespace std;
using namespace ezgl;

double max_lat;
double min_lat;
double max_lon;
double min_lon;

ezgl::color GRASS;
ezgl::color SAND;
ezgl::color WATER;
ezgl::color HIGHWAY;
ezgl::color STREETS;
ezgl::color OUTLINE;
ezgl::color BUILDINGS;
ezgl::color BACKGROUND;
ezgl::color FEATURE_NAMES;
ezgl::color STREET_NAMES;
ezgl::color ONE_WAY;

string map_name;
void draw_main_canvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);//add find button
void find_button(GtkWidget */*widget*/, ezgl::application *application);
void highlight_intersections(vector<int> intersection_ids, ezgl::renderer* g);
void search_button(GtkWidget */*widget*/, ezgl::application *application);
void nightMode_button(GtkWidget *widget, ezgl::application *application);
vector<string> POItypesTable;
//const char* search_text;
void draw_main_canvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);//add find button
void find_button(GtkWidget */*widget*/, ezgl::application *application);
void highlight_intersections(vector<int> intersection_ids, ezgl::application *application);
void search_bar(GtkWidget *widget, ezgl::application *application);
string drawFindSearchBar(ezgl::application *application);
void setNight();
void setLight();
void close_M2();
void makePOITypesTable();
bool elementAlreadyExists(string st, vector<string> vec);
void drawPOI(GtkWidget */*widget*/, ezgl::application *application);
//Determining the first time drawn
int numTimesDrawn = 0;
bool night;

void draw_map() {

//    if (map_name == "beijing_china" || map_name == "cairo_egypt" || map_name == "cape-town_south-africa" ||
//            map_name == "golden-horseshoe_canada" || map_name == "hamilton_canada" || map_name == "hong-kong_china" ||
//            map_name == "iceland" || map_name == "interlaken_switzerland" || map_name == "london_england" ||
//            map_name == "moscow_russia" || map_name == "new-delhi_india" || map_name == "new-york_usa" ||
//            map_name == "rio-de-janeiro_brazil" || map_name == "saint-helena" || map_name == "singapore" ||
//            map_name == "sydney_australiia" || map_name == "tehran_iran" || map_name == "tokyo_japan" || map_name == "toronto_canada"){
//        load_map(map_name);
//    }

    setLight();   
    
    ezgl::application::settings settings;
    // Include headers
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    // Create EZGL application
    ezgl::application application(settings);
    intersections.resize(getNumIntersections());

    map_bounds();

    makeStreetsVector();
    makeStreetSizeTable();
    sortFeatures();
    makePOITypesTable();
    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},{x_from_lon(max_lon), y_from_lat(max_lat)});
    cout<<"Drawing here"<< endl;
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world, BACKGROUND);
    application.run(initial_setup, act_on_mouse_click, nullptr, act_on_key_press);
    
  //  close_M2();
    
}

void draw_main_canvas(ezgl::renderer *g) {
    
//    g->fill_rectangle(zooms.current.m_first, zooms.current.m_second);
//     g->set_color(ezgl::BLACK);
  //g->fill_rectangle({0,0}, {1000,1000});
    
//    
    if (numTimesDrawn == 0) {

        full_map.m_first.x = x_from_lon(min_lon);
        full_map.m_first.y = min_lat;
        full_map.m_second.x = x_from_lon(max_lon);
        full_map.m_second.y = max_lat;
        
        point2d botL (x_from_lon(min_lon), min_lat);
        point2d topR (x_from_lon(max_lon), max_lat);
        zooms.full.m_first = botL;
        zooms.full.m_second = topR;
        full_screen = g->world_to_screen(full_map);
        numTimesDrawn++;
    }
    
    zoom(g);
    zoomFeatures(g);
    zoomStreets(g);
    nameStreets(g);
    nameFeatures(g);
    //drawSearchBar(g);
}

void initial_setup(ezgl::application *application, bool new_window){  
//  application->create_button("find", 6, find_button);
  application->create_button("find", 6, find_button);
  application->create_button("draw POI", 7, drawPOI);
  
  GObject *SearchButton = application->get_object("SearchButton");
  g_signal_connect(SearchButton, "clicked", G_CALLBACK(search_button), application);

  GObject * NightMode = application->get_object("NightMode");
  
  g_signal_connect(NightMode, "clicked", G_CALLBACK(nightMode_button),application);
}

void nightMode_button(GtkWidget *widget, ezgl::application *application){
    
    if(!night){
        setNight();
    }
    else{
        setLight();
    }
    application->refresh_drawing();
}

void setNight(){
    GRASS.color_change(D_GRASS);
    SAND.color_change(D_SAND);
    WATER.color_change(D_WATER);
    HIGHWAY.color_change(D_HIGHWAY);
    STREETS.color_change(GREY_55);
    OUTLINE.color_change(D_OUTLINE);
    BUILDINGS.color_change(D_BUILDINGS);
    BACKGROUND.color_change(D_BACKGROUND);
    FEATURE_NAMES.color_change(D_FEATURE_NAMES);
    STREET_NAMES.color_change(WHITE);
    ONE_WAY.color_change(D_ONE_WAY);
    night = true;
}
void setLight(){
    GRASS.color_change(L_GRASS);
    SAND.color_change(L_SAND);
    WATER.color_change(L_WATER);
    HIGHWAY.color_change(L_HIGHWAY);
    STREETS.color_change(WHITE);
    OUTLINE.color_change(L_OUTLINE);
    BUILDINGS.color_change(L_BUILDINGS);
    BACKGROUND.color_change(L_BACKGROUND);
    FEATURE_NAMES.color_change(L_FEATURE_NAMES);
    STREET_NAMES.color_change(L_STREET_NAMES);
    ONE_WAY.color_change(L_ONE_WAY);
    night = false;
}

void find_button(GtkWidget */*widget*/, ezgl::application *application){
    
    cout<< "the find button is pressed."<<endl;
        // Get Object
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    std::string search_text = gtk_entry_get_text(textEntry);
    stringstream ss(search_text);
    string street1, street2;
    vector<int> street1_search_result, street2_search_result, found_intersections;
    getline(ss, street1, ',');
    ss>>street2;
    street1_search_result = find_street_ids_from_partial_street_name(street1);
    street2_search_result = find_street_ids_from_partial_street_name(street2);
    if(street1_search_result.size()==0 || street2_search_result.size()==0 ){
        if(street1_search_result.size()==0){cout<<"cannot find matching street for input 1"<<endl;}
        if(street2_search_result.size()==0){cout<<"cannot find matching street for input 2"<<endl;}
    }
    else if(street1_search_result.size()>1 || street2_search_result.size()>1){
        if(street1_search_result.size()>1){cout<<"cannot find matching street for input 1"<<endl;}
        if(street2_search_result.size()>10){cout<<"cannot find matching street for input 2"<<endl;}
    }
    else {
        found_intersections = find_intersections_of_two_streets(make_pair(street1_search_result[0], street2_search_result[0]));
        highlight_intersections(found_intersections, application);
    }
}
                              
void highlight_intersections(vector<int> intersection_ids, ezgl::application *application){
    ezgl::renderer *g = application->get_renderer();
    g->set_coordinate_system(ezgl::WORLD);
    g->set_color(YELLOW);
    for(int i=0; i< intersection_ids.size(); i++){
        LatLon intersection_position = getIntersectionPosition(intersection_ids[i]);
        g->fill_arc({x_from_lon(intersection_position.lon()), y_from_lat(intersection_position.lat())}, 0.002, 0, 360);
        cout<< "*******************Intersection "<<i+1 <<" ******************"<<endl
            <<"Intersection id :           "<<endl<< intersection_ids[i]<< endl
            <<"Intersection street names:  "<<endl;
        vector<string> street_names =  find_street_names_of_intersection(intersection_ids[i]);
        for(int j=0;j<street_names.size();j++){
            cout<< street_names[j]<<endl;
        }
        cout<<"********************************************************"<< endl;
    }
    application->flush_drawing();
}
void search_button(GtkWidget */*widget*/, ezgl::application *application){
    // Get Object
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    const char* search_text = gtk_entry_get_text(textEntry);
    std::vector<StreetIndex> street_index = find_street_ids_from_partial_street_name(search_text);
    // check if there are valid results
    if (street_index.size() == 0){
        cout << "No results found" << endl;
    }
    else {
        // Make map go to zoom fit mode
        renderer *g = application->get_renderer();
        //g->set_visible_world(ezgl::rectangle({x_from_lon(min_lon), y_from_lat(min_lat)},{x_from_lon(max_lon), y_from_lat(max_lat)}));
        //application->refresh_drawing();
        
        // Highlight street of first function
        StreetIndex street_id = street_index[0];
        vector<int> street_seg_ids = find_street_segments_of_street(street_id);
        int seg_ids_size = street_seg_ids.size();
        
        InfoStreetSegment seg_info_begin = getInfoStreetSegment(street_seg_ids[0]);
        InfoStreetSegment seg_info_stop = getInfoStreetSegment(street_seg_ids[seg_ids_size-1]);
        
        point2d beginning_of_street(x_from_lon(getIntersectionPosition(seg_info_begin.from).lon()), y_from_lat(getIntersectionPosition(seg_info_begin.from).lat()));
        point2d end_of_street(x_from_lon(getIntersectionPosition(seg_info_stop.to).lon()), y_from_lat(getIntersectionPosition(seg_info_stop.to).lat()));
        g->set_visible_world(rectangle(beginning_of_street, end_of_street));
        application->refresh_drawing();
        
        for(int i=0;i < street_seg_ids.size(); i++){
            //for(int j=i+1; j< street_seg_ids.size(); j++){
                InfoStreetSegment seg_info_start = getInfoStreetSegment(street_seg_ids[i]);
            //    InfoStreetSegment seg_info_end = getInfoStreetSegment(street_seg_ids[j]);
                g->set_color(ezgl::YELLOW);
                g->set_line_width(12);
                //if(are_directly_connected(make_pair(i,j))){    
                //    ezgl::point2d start(x_from_lon(getIntersectionPosition(seg_info_start.from).lon()), y_from_lat(getIntersectionPosition(street_seg_ids[i]).lat()));
                //    ezgl::point2d end(x_from_lon(getIntersectionPosition(street_seg_ids[j]).lon()), y_from_lat(getIntersectionPosition(street_seg_ids[j]).lat()));
                point2d from(x_from_lon(getIntersectionPosition(seg_info_start.from).lon()), y_from_lat(getIntersectionPosition(seg_info_start.from).lat()));
                point2d to(x_from_lon(getIntersectionPosition(seg_info_start.to).lon()), y_from_lat(getIntersectionPosition(seg_info_start.to).lat()));
                g->draw_line(from, to);
                //g->draw_line(start, end);
                //}
            //}
        }
    }
}

void close_M2(){
    close_map();
    streetSegments.clear();
    streetsizes.highway.clear();
    streetsizes.major.clear();
    streetsizes.minor.clear();
    streetsizes.local.clear();
    pointsOfFeatures.clear();
    featuretypes.beaches.clear();
    featuretypes.bigparks.clear();
    featuretypes.buildings.clear();
    featuretypes.golfcourses.clear();
    featuretypes.greenspaces.clear();
    featuretypes.islands.clear();
    featuretypes.lakes.clear();
    featuretypes.rivers.clear();
    featuretypes.streams.clear();
    featuretypes.unknownFeatures.clear();
    POItypesTable.clear();
}

void drawPOI(GtkWidget */*widget*/, ezgl::application *application){
    ezgl::renderer *g = application->get_renderer();
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    std::string search_text = gtk_entry_get_text(textEntry);
    
    if(elementAlreadyExists(search_text, POItypesTable)){
         g->set_coordinate_system(ezgl::WORLD);
        for(int i=0;i<getNumPointsOfInterest();i++){
            if(getPointOfInterestType(i)==search_text){
                g->set_color(YELLOW);
                LatLon POIposition = getPointOfInterestPosition(i);
                point2d position(x_from_lon(POIposition.lon()), y_from_lat(POIposition.lat())); 
                g->fill_arc(position, 0.002, 0, 360);
                g->set_color(BLACK);
                g->set_font_size(10);
                g->draw_text(position, getPointOfInterestName(i));
            }
        }
        
    }
    else{cout<<"there is no matching POI type"<<endl;}
    
    
    application->flush_drawing();
}


void makePOITypesTable(){
    int numPOI = getNumPointsOfInterest();
    for(int i=0; i< numPOI;i++){
        if(!elementAlreadyExists(getPointOfInterestType(i), POItypesTable)){
            POItypesTable.push_back(getPointOfInterestType(i));
        }
    }

}

bool elementAlreadyExists(string st, vector<string> vec){
    bool exist = false;
    for(int i=0;i<vec.size();i++){
        if(st == vec[i]){
            exist = true;
            break;
        }
    }
    return exist;
}