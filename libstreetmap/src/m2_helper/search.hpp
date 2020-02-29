/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   search.h
 * Author: dengpris
 *
 * Created on February 24, 2020, 3:41 PM
 */

#pragma once 

#include "m1.h"
#include "m2.h"
#include "global_variables.hpp"
#include "m2_global_variables.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include "m2_helper/zoom.hpp"
#include "gdk/gdkkeysyms.h"

void drawSearchBar(ezgl::renderer *g);
void act_on_key_press(ezgl::application *app, GdkEventKey* key, char* letter);
void search(StreetIndex streetSearchIndex);

bool char_pressed;
//bool char_released = false;
bool backspace_pressed;
bool enter_pressed;
bool space_pressed;
bool search_bar_clicked;
char* char_print;

std::string typed = "";

/*
struct find_id : std::unary_function <StreetData, bool> {
    StreetIndex index;
    find_id(StreetIndex id):index(id){}
    bool operator()(StreetData const& s) const {
        return s.id == index;
    }
};*/

void drawSearchBar(ezgl::renderer *g){
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
    
    // Determine position of text box
    ezgl::point2d text_start(start_point.x + offset, start_point.y + search_bar_width * 0.4); 
        
    
    if (char_pressed == true){
       // If backspace is pressed, delete last index of string
        if (backspace_pressed == true){
            if (typed.length() > 0){
                typed.pop_back();
                g->set_font_size(14);
                g->set_color(ezgl::BLACK);
                g->draw_text(text_start, typed);
                // Reset backspace
                backspace_pressed = false;
                // Print results after backspace as well
                vector<int> results = find_street_ids_from_partial_street_name(typed);
                int results_num = results.size();
                for (int i = 0; i < std::min(5, results_num); i++){
                    g->set_color(ezgl::WHITE);
                    g->fill_rectangle({start_point.x, start_point.y+30*(i+1)+3}, {end_point.x, end_point.y+30*(i+1)+3});
                    g->set_color(ezgl::BLACK);
                    g->draw_text({text_start.x, text_start.y+30*(i+1)+3}, getStreetName(results[i]));
                }
            }
        } 
        else if (enter_pressed == true){
            if (typed.length() > 0){
                vector<int> results = find_street_ids_from_partial_street_name(typed);
                if (results.size() == 0){
                    enter_pressed = false;
                } else {
                for (int id = 0; id < results.size(); id++){
                    cout << getStreetName(results[id]) << endl;
                }
                search(results[0]);
                enter_pressed = false;
                }
            }
        }
        else if (space_pressed == true){
            typed += " ";
            space_pressed = false;
            g->draw_text(text_start, typed);
        }
        else {
        // Convert character to a length one string
            std::string temp(1, *char_print);
            typed += temp;

            g->set_font_size(14);
            g->set_color(ezgl::BLACK);
            g->draw_text(text_start, typed);
            vector<int> results = find_street_ids_from_partial_street_name(typed);
            int results_num = results.size();
            for (int i = 0; i < std::min(5, results_num); i++){
                g->set_color(ezgl::WHITE);
                g->fill_rectangle({start_point.x, start_point.y+30*(i+1)+3}, {end_point.x, end_point.y+30*(i+1)+3});
                g->set_color(ezgl::BLACK);
                g->draw_text({text_start.x, text_start.y+30*(i+1)+3}, getStreetName(results[i]));
            }
        }
        char_pressed = false;
    }
    if (typed.empty()){
        g->set_color(197,197,197);
        g->set_font_size(15);
        g->draw_text(text_start, "Start typing. Press enter to search.");
    }
    // Set coordinate system back for safety
    g->set_coordinate_system(ezgl::WORLD);
}

void search(StreetIndex streetSearchIndex){
    for (std::vector<StreetData>::iterator it = streetsizes.highway.begin(); it < streetsizes.highway.end(); it++){
        if ((*it).id == streetSearchIndex){
            cout << "highway detected" << endl;
            // Pseudo code:
            // Set zoom to highway zoom level
            // NEED HELP
            // Pan map to highway center 
            // Highlight
        }
    }
}

void act_on_key_press(ezgl::application *app, GdkEventKey* key, char* letter){

    char_pressed = true;
    char_print = letter;
    // ignore characters that are not a-z

    // If backspace was pressed
    if (key->keyval == GDK_KEY_BackSpace){
        backspace_pressed = true;
    }
    else if (key->keyval == GDK_KEY_Return){
        enter_pressed = true;
    }
    else if (key->keyval == GDK_KEY_space){
        space_pressed = true;
    }
    else if (isupper(*char_print)){
        char_pressed = false;
        return;
    }
    app->refresh_drawing();
}
