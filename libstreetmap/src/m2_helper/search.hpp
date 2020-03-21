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
void act_on_key_press(ezgl::application *app, GdkEventKey* /*key*/, char* /*letter*/);
void search(StreetIndex streetSearchIndex);

bool char_pressed;
bool backspace_pressed;
bool enter_pressed;
bool space_pressed;
bool search_bar_clicked;
char* char_print;

extern std::string typed;