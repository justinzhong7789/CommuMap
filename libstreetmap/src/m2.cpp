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

const char* search_text;
void draw_main_canvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool /*new_window*/);//add find button
void find_button(GtkWidget */*widget*/, ezgl::application *application);
void highlight_intersections(vector<int> intersection_ids, ezgl::renderer* g);
void search_bar(GtkWidget *widget, ezgl::application *application);
//Determining the first time drawn
int numTimesDrawn = 0;
//ezgl::color BACKGROUND(237,237,237);

void draw_map() {
    std::string map_name;
    if (map_name == "beijing_china" || map_name == "cairo_egypt" || map_name == "cape-town_south-africa" ||
            map_name == "golden-horseshoe_canada" || map_name == "hamilton_canada" || map_name == "hong-kong_china" ||
            map_name == "iceland" || map_name == "interlaken_switzerland" || map_name == "london_england" ||
            map_name == "moscow_russia" || map_name == "new-delhi_india" || map_name == "new-york_usa" ||
            map_name == "rio-de-janeiro_brazil" || map_name == "saint-helena" || map_name == "singapore" ||
            map_name == "sydney_australiia" || map_name == "tehran_iran" || map_name == "tokyo_japan" || map_name == "toronto_canada"){
        load_map(map_name);
    }
    
    ezgl::application::settings settings;

    // Include headers
    settings.main_ui_resource = "libstreetmap/resources/main.ui";
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    // Create EZGL application
    ezgl::application application(settings);
    intersections.resize(getNumIntersections());

    map_bounds();
    cout << "minLon: " << min_lon << " maxLon: " << max_lon << endl;
    cout << "minLat: " << min_lat << " maxLat: " << max_lat << endl;

    makeStreetsVector();
    makeStreetSizeTable();
    sortFeatures();
    
    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},{x_from_lon(max_lon), y_from_lat(max_lat)});

    application.add_canvas("MainCanvas", draw_main_canvas, initial_world, BACKGROUND);
    application.run(initial_setup, act_on_mouse_click, nullptr, act_on_key_press);
}

void draw_main_canvas(ezgl::renderer *g) {

    if (numTimesDrawn == 0) {

        full_map.m_first.x = x_from_lon(min_lon);
        full_map.m_first.y = min_lat;
        
        full_map.m_second.x = x_from_lon(max_lon);
        full_map.m_second.y = max_lat;
        
        full_screen = g->world_to_screen(full_map);
        numTimesDrawn++;
    }
    
    zoom(g);
    zoomFeatures(g);
    zoomStreets(g);
    drawSearchBar(g);
}

void initial_setup(ezgl::application *application, bool new_window){
          
  //GObject *searchBar = application->get_object("SearchBar");
  //g_signal_connect(searchBar, "key_press_event", G_CALLBACK(search_bar), application);
    // application->create_button("find", 6, find_button);

}

/*void search_bar(GtkWidget *widget, ezgl::application *application){
    GtkEntry *entry = (GtkEntry *)application->get_object("SearchBar");
    search_text = gtk_entry_get_text(entry);
    cout << search_text << endl;
    // application->refresh_drawing();
}
*/
/*
void find_button(GtkWidget *widget, ezgl::application *application){
    cout<< "find button is pressed."<< endl;
    cout<< "Enter 2 street names below to find an intersection." << endl;
    int street_id[2];
    vector<int> intersection_ids;
    
    for(int i=0; i< 2;i++){ //enter 2 street names to find intersection
        vector <int> street_ids;
        string str;
        getline(cin, str);
        cout<< str<<endl;
        street_ids = find_street_ids_from_partial_street_name(str);
        for(int j=0; j< street_ids.size(); j++){
            cout<<getStreetName(j)<<endl;
        }
        while(street_ids.size()!=1){
            cout <<"The partial street name does not uniquely identify a street." <<endl
                 <<"Please try again." << endl;
            getline(cin, str);
            street_ids = find_street_ids_from_partial_street_name(str);
        }
        street_id[i] = street_ids[0];
    }
    intersection_ids = find_intersections_of_two_streets(make_pair(street_id[0], street_id[1]));
    highlight_intersections(intersection_ids, application->get_renderer());
    
    
}*/
                                   
void highlight_intersections(vector<int> intersection_ids, ezgl::renderer *g){
    rectangle recover_screen = g->get_visible_screen();
    // this contains the screen when zoom_fit is clicked
    g->set_color(YELLOW);
    for(int i=0; i< intersection_ids.size(); i++){
        LatLon intersection_position = getIntersectionPosition(intersection_ids[i]);
        g->fill_arc({x_from_lon(intersection_position.lon()), y_from_lat(intersection_position.lat())}, 10, 0, 360);
        cout<< "*******************Intersection "<<i+1 <<" ******************"<<endl
            <<"Intersection id :           "<<endl<< intersection_ids[i]<< endl
            <<"Intersection street names:  ";
        vector<string> street_names =  find_street_names_of_intersection(intersection_ids[i]);
        for(int j=0;j<street_names.size();j++){
            cout<< street_names[j]<<endl;
        }
        cout<<"********************************************************"<< endl;
    }
    char YOrN;
    cout<<"Done browing? y/n"<<endl;       
    while(YOrN!='y'){
        cin>> YOrN;
    }
    if(YOrN =='Y'){
        g->set_visible_world(recover_screen);
    }
}