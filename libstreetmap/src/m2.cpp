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
void initial_setup(ezgl::application *application, bool new_window);//add find button
void find_button(GtkWidget */*widget*/, ezgl::application *application);
void highlight_intersections(vector<int> intersection_ids, ezgl::renderer* g);
void search_bar(GtkWidget *widget, ezgl::application *application);
string drawFindSearchBar(ezgl::application *application);
void close_M2();
string drawFindSearchBar(ezgl::application *application);
//Determining the first time drawn
int numTimesDrawn = 0;
StreetIndex search_street_highlight = 0;

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

    makeStreetsVector();
    makeStreetSizeTable();
    sortFeatures();
    
    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},{x_from_lon(max_lon), y_from_lat(max_lat)});

    application.add_canvas("MainCanvas", draw_main_canvas, initial_world, BACKGROUND);

    application.run(initial_setup, act_on_mouse_click, nullptr, nullptr);
//    application.run(nullptr, act_on_mouse_click, nullptr, act_on_key_press);
    
  //  close_M2();
}

void draw_main_canvas(ezgl::renderer *g) {
    
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
  //  drawStreetNames(streetsizes.highway, g, 10);
    nameStreets(g);
    nameFeatures(g);
    drawSearchBar(g);
}
void search_button(GtkWidget */*widget*/, ezgl::application *application);
void initial_setup(ezgl::application *application, bool new_window){  
  application->create_button("find", 6, find_button);
  GObject *SearchButton = application->get_object("SearchButton");
  g_signal_connect(SearchButton, "clicked", G_CALLBACK(search_button), application);

  GObject * test_Button = application->get_object("test_button");
  
  g_signal_connect(test_Button, "clicked", G_CALL_BACK(test_button),application);
}

void test_button(GtkWidget *widget, ezgl::application *application){
    \
    cout<<"LOOK THE TEST BUTTON IS HERE"<<endl;
}

void find_button(GtkWidget */*widget*/, ezgl::application *application){
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
        getline(cin, two_streets[i]);
        while(find_street_ids_from_partial_street_name(two_streets[i]).size()!=1){
            cout<< "Your input does not uniquely identify a street."<<endl<<"Please try again"<<endl;
            getline(cin, two_streets[i]);
        }
        two_streets_id[i] = find_street_ids_from_partial_street_name(two_streets[i])[0];
    }
    intersection_ids = find_intersections_of_two_streets(make_pair(street_id[0], street_id[1]));
    highlight_intersections(intersection_ids, application->get_renderer());
    
    
}
                              
void highlight_intersections(vector<int> intersection_ids, ezgl::renderer *g){
    // this contains the screen when zoom_fit is clicked
    g->set_coordinate_system(ezgl::SCREEN);
    g->set_color(YELLOW);
    for(int i=0; i< intersection_ids.size(); i++){
        LatLon intersection_position = getIntersectionPosition(intersection_ids[i]);
        g->fill_arc({x_from_lon(intersection_position.lon()), y_from_lat(intersection_position.lat())}, 10, 0, 360);
        cout<< "*******************Intersection "<<i+1 <<" ******************"<<endl
            <<"Intersection id :           "<<endl<< intersection_ids[i]<< endl
            <<"Intersection street names:  "<<endl;
        vector<string> street_names =  find_street_names_of_intersection(intersection_ids[i]);
        for(int j=0;j<street_names.size();j++){
            cout<< street_names[j]<<endl;
        }
        cout<<"********************************************************"<< endl;
    }
    char YOrN;
    cout<<"Done browsing? y/n"<<endl;       
    while(YOrN!='y'){
        cin>> YOrN;
    }
    if(YOrN =='Y'){
        g->set_visible_world(recover_screen);
    }
}
void search_button(GtkWidget */*widget*/, ezgl::application *application){
    // Get Object
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    std::string search_text = gtk_entry_get_text(textEntry);
    std::vector<StreetIndex> street_index = find_street_ids_from_partial_street_name(search_text);
    // check if there are valid results
    if (street_index.size() == 0){
        cout << "No results found" << endl;
    }
    else {
        // Highlight street of first function
        search_street_highlight = street_index[0];
    }
}
/*string drawFindSearchBar(ezgl::application *application){
    ezgl::renderer *g = application->get_renderer();
    string find_typed = "";
    g->set_coordinate_system(ezgl::SCREEN);
    rectangle map_full_screen = g->get_visible_screen();
    double search_bar_width = 40;
    double offset = 10;
    
    const ezgl::point2d start_point(map_full_screen.m_first.x + offset, map_full_screen.m_first.y + offset);
    //const ezgl::point2d end_point(map_full_screen.m_second.x - offset, map_full_screen.m_first.y + search_bar_width);
    const ezgl::point2d end_point(400, map_full_screen.m_first.y + search_bar_width);
    
    g->set_color(197,197,197);
    g->draw_rectangle(start_point, end_point);
    g->set_color(ezgl::WHITE);
    g->fill_rectangle(start_point, end_point);
    
    // Sets text box to write from left instead of center
    // This may need to be moved into main function
    text_just write_text_left = text_just::left;
    g->set_horiz_text_just(write_text_left);    
    
    if (char_pressed == true){
        // Determine position of text box
        ezgl::point2d text_start(start_point.x + offset, start_point.y + search_bar_width * 0.4); 
        
        // If backspace is pressed, delete last index of string
        if (backspace_pressed == true){
            if (find_typed.length() > 0){
                find_typed.pop_back();
                g->set_font_size(14);
                g->set_color(ezgl::BLACK);
                g->draw_text(text_start, typed);
                // Reset backspace
                backspace_pressed = false;
            }
        } 
        else if (enter_pressed == true){
            if (find_typed.length() > 0){
                vector<int> results = find_street_ids_from_partial_street_name(typed);
                if(results.size()!=1){
                    application->update_message( "Your input does not uniquely identify a street. Try again.");
                }
                else if(results.size()==1){
                    return find_typed;
                }
                enter_pressed = false;
            }
        }
        else if (space_pressed == true){
            find_typed += " ";
            space_pressed = false;
            g->draw_text(text_start, find_typed);
        }
        else {
        // Convert character to a length one string
            std::string temp(1, *char_print);
            find_typed += temp;

            g->set_font_size(14);
            g->set_color(ezgl::BLACK);
            g->draw_text(text_start, find_typed);
            vector<int> results = find_street_ids_from_partial_street_name(find_typed);
            int results_num = results.size();
            if(results.size()!=1){
                    application->update_message( "Your input does not uniquely identify a street. Try again.");
            }
            else if(results.size()==1){
                return find_typed;
                
            }
            for (int i = 0; i < std::max(5, results_num); i++){
                g->set_color(ezgl::WHITE);
                g->fill_rectangle({start_point.x, start_point.y+15*(i+1)}, {end_point.x, end_point.y+15*(i+1)});
                g->set_color(ezgl::BLACK);
                g->draw_text({text_start.x, text_start.y+15*(i+1)}, getStreetName(results[i]));
            }
        }
        char_pressed = false;
    }
    // Set coordinate system back for safety
    g->set_coordinate_system(ezgl::WORLD);
    return 0;
}*/

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
}

