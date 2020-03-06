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

std::string typed;

void act_on_key_press(ezgl::application *app, GdkEventKey* /*key*/, char* /*letter*/){     
    GtkEntry *textEntry = (GtkEntry *)app->get_object("SearchBar");
    // Get string from search bar
    typed = gtk_entry_get_text(textEntry);
    cout << typed << endl;
    text_just write_text_left = text_just::left;
    renderer *g = app->get_renderer();
    
    rectangle map_full_screen = g->get_visible_screen();
    double offset = 10;
    double search_bar_width = 40;
    
    const ezgl::point2d start_point(map_full_screen.m_first.x + offset, map_full_screen.m_first.y + offset);
    const ezgl::point2d end_point(400, map_full_screen.m_first.y + search_bar_width);
    
    // Determine position of text box
    ezgl::point2d text_start(start_point.x + offset, start_point.y + search_bar_width * 0.4); 
    // Draw using screen coordinates    
    g->set_coordinate_system(ezgl::SCREEN);
    g->set_horiz_text_just(write_text_left);    
    g->set_color(ezgl::BLACK);
    g->set_font_size(14);
    vector<int> results = find_street_ids_from_partial_street_name(typed);
    int results_num = results.size();
    app->refresh_drawing();    
    for (int i = 0; i < std::min(5, results_num); i++){
        g->set_color(ezgl::WHITE);
        g->fill_rectangle({start_point.x, start_point.y+30*(i)+3}, {end_point.x, end_point.y+30*(i)+3});
        g->set_color(ezgl::BLACK);
        g->draw_text({text_start.x, text_start.y+30*(i)+3}, getStreetName(results[i]));
//    char_pressed = true;
//    char_print = letter;
//    // ignore characters that are not a-z
//
//    // If backspace was pressed
//    if (key->keyval == GDK_KEY_BackSpace){
//        backspace_pressed = true;
//    }
//    else if (key->keyval == GDK_KEY_Return){
//        enter_pressed = true;
//    }
//    else if (key->keyval == GDK_KEY_space){
//        space_pressed = true;
//    }
//    else if (isupper(*char_print)){
//        char_pressed = false;
//        return;
//    }
    // Set coordinate system back for safety
    }
    g->set_coordinate_system(ezgl::WORLD);
}
