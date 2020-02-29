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
<<<<<<< HEAD
void search_bar(GtkWidget *widget, ezgl::application *application);
=======
string drawFindSearchBar(ezgl::renderer *g);
>>>>>>> 4efb4071071d2d9c0a9fbe2c3cb893fec509e927
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
<<<<<<< HEAD
    application.run(initial_setup, act_on_mouse_click, nullptr, act_on_key_press);
=======


    application.run(initial_setup, act_on_mouse_click, nullptr, nullptr);
    application.run(nullptr, act_on_mouse_click, nullptr, act_on_key_press);
    application.destroy_button("find");
>>>>>>> 4efb4071071d2d9c0a9fbe2c3cb893fec509e927
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
   // nameStreets(g);
    nameFeatures(g);
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
    ezgl::renderer *g = application->get_renderer();
    string two_streets[2];
    int two_streets_id[2];
    vector<int> intersection_ids;
    
    for(int i=0; i< 2;i++){ //enter 2 street names to find intersection
        
 //MJ COMMENTED THIS OUT CUZ IT WONT LET ME BUILD       
//        two_streets[i] = drawFindSearchBar(g);
    }
<<<<<<< HEAD
    intersection_ids = find_intersections_of_two_streets(make_pair(street_id[0], street_id[1]));
    highlight_intersections(intersection_ids, application->get_renderer());
    
    
}*/
=======
    for(int i=0; i< getNumStreets();i++){
        if(getStreetName(i)==two_streets[0]){two_streets_id[0]=1;}
        else if(getStreetName(i)==two_streets[1]){two_streets_id[1]=i;}
    }
    intersection_ids = find_intersections_of_two_streets(make_pair(two_streets_id[0], two_streets_id[1]));
    highlight_intersections(intersection_ids, g);
}
>>>>>>> 4efb4071071d2d9c0a9fbe2c3cb893fec509e927
                                   
void highlight_intersections(vector<int> intersection_ids, ezgl::renderer *g){
    rectangle recover_screen = g->get_visible_screen();
    // this contains the screen when zoom_fit is clicked
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
    cout<<"Done browing? y/n"<<endl;       
    while(YOrN!='y'){
        cin>> YOrN;
    }
    if(YOrN =='Y'){
        g->set_visible_world(recover_screen);
    }
}

string drawFindSearchBar(ezgl::application *application){
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
}
