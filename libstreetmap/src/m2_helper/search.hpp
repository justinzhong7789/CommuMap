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

#include "gdk/gdkkeysyms.h"

void drawSearchBar(ezgl::renderer *g);
void act_on_key_press(ezgl::application *app, GdkEventKey* key, char* letter);

bool char_pressed;
//bool char_released = false;
bool backspace_pressed;
char* char_print;

std::string typed = "";

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
    
    if (char_pressed == true){
        // Convert character to a length one string
        std::string temp(1, *char_print);
        typed += temp;
        
        ezgl::point2d text_start(start_point.x + offset, start_point.y + search_bar_width * 0.4); 
        
        
        g->set_font_size(14);
        g->set_color(ezgl::BLACK);
        g->draw_text(text_start, typed);
        
        char_pressed = false;
    }
    // Set coordinate system back for safety
    g->set_coordinate_system(ezgl::WORLD);
}


void act_on_key_press(ezgl::application *app, GdkEventKey* key, char* letter){

    cout << "Key pressed: " << letter << endl;
    char_pressed = true;
    char_print = letter;

    // If backspace was pressed
    if (key->keyval == GDK_KEY_BackSpace){
        backspace_pressed = true;
        cout << "Backspace pressed" << endl;
    }
    app->refresh_drawing();
}
