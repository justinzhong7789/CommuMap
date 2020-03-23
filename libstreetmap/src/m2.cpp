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
#include "m3_global.hpp"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "m1.h"
#include "MJ.hpp"
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

/*============== DRAWING OF MAP ==============*/
string map_name;
void draw_main_canvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);//add find button
void close_M2();

/*============== HELPER FUNCTIONS FOR CALLBACK FUNCTIONS ==============*/
void button_test(GtkWidget */*widget*/, ezgl::application *application );

//Determining the first time drawn
int numTimesDrawn = 0;
int num_intersections = 0;
string typed;
string POItext;
string locationText;
string destinationText;
vector<string> POItypesTable;
vector<int> found_intersections;
vector<int> found_street_segments;
vector<int> found_route_segments;
int destination_ID;
int location_ID;

//Created flags
bool night;
bool searchingPOI = 0;
bool searchingIntersections = 0;
bool searchingStreet = 0;
bool searchingRoute = true;
bool click_OnOff = 0;
bool text_OnOff = 0;
bool find_w_click = 0;
//void clean_map(ezgl::application *application);
GtkEntry *textboxGlobal;

//Global GTK WIDGETS
GtkEntry *LocationTextGlobal;
GtkEntry *DestinationTextGlobal;
GtkEntry *TurnPenaltyGlobal;

double turn_penalty_entry = 0;

void draw_map() {

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
    
    LatLon minMax (max_lat, min_lon);
    
    zoom(g);
    zoomFeatures(g);
    zoomStreets(g);
    if(searchingStreet){
        highlight_street(found_street_segments, g);
    }
    if(searchingRoute){
        cout<<"back here"<<endl;
        highlight_route(found_route_segments, g);
    }
    nameStreets(g);
    nameFeatures(g);
    
//    ezgl::surface * redDot = g->load_png("main/Images/redDot.png");
//    g->draw_surface(redDot, point2d_from_latlon(minMax));
    
//    g->sepoint2d_from_latlon(minMax), 0.2, 0, 360);
    
    if(searchingIntersections ){
        highlight_intersections(found_intersections, g);
    }
    if(searchingPOI){
        highlight_POI(g);
    }
}

void clean_map(ezgl::application *application)
{
    searchingIntersections = false;
    searchingPOI = false;
    searchingStreet = false;
    searchingRoute = false;
    found_intersections.clear();
    found_street_segments.clear();
    application->refresh_drawing();
}

void initial_setup(ezgl::application *application, bool /*new_window*/){  

  application->create_button("Find Intersections", 9, find_button);
  application->create_button("Load City",7,Load_Map);
  application->createButtonTester(application, button_test);
    GtkLabel *route = (GtkLabel*) application->get_object("Route");
    gtk_label_set_text(route, "\tRoute\t");
    
    GtkLabel *error = (GtkLabel*) application->get_object("ErrorOutput");
    gtk_label_set_text(error, "\nWelcome to CommuMaps!\n");
  
  
  GObject *openMj = application->get_object("Window");
  g_signal_connect(openMj, "clicked", G_CALLBACK(window_button),application);
  


  GObject *SearchButton = application->get_object("SearchButton");
  g_signal_connect(SearchButton, "clicked", G_CALLBACK(search_button), application);

  GObject *drawPOI = application->get_object("DrawPOI");
  g_signal_connect(drawPOI, "clicked", G_CALLBACK(draw_POI), application);

  GObject * NightMode = application->get_object("NightMode");
  g_signal_connect(NightMode, "clicked", G_CALLBACK(nightMode_button),application);  
  
  GtkWidget *test = (GtkWidget *) application->get_object("button");
  g_signal_connect(test, "clicked", G_CALLBACK(button_test),application);
  
//  GtkImage *image = (GtkImage *)gtk_image_new_from_file("main/Images/homeIcon.png");
//  gtk_image_set_pixel_size(image, 10);
//  gtk_button_set_image(GTK_BUTTON(test), (GtkWidget*)image);
  
   TurnPenaltyGlobal = (GtkEntry *)application->get_object("TurnPenalty");
  
}

void button_test(GtkWidget */*widget*/, ezgl::application *application )
{
    cout<<"WOAH I WAS JUST CLICKED"<<endl;
}
void window_button(GtkWidget */*widget*/, ezgl::application *application )
{
    
    clean_map(application);
    find_w_click = true; 
    
    GtkWindow *parent = (GtkWindow *) application->get_object("MainWindow");
    GtkWidget *dialog = (GtkWidget *)application->get_object("NavigationWindow");
    gtk_window_set_transient_for((GtkWindow *)dialog, parent);   
    
    
    //Plan to display Route
    GtkLabel *labeltext = (GtkLabel *)application->get_object("Label");
    gtk_label_set_text(labeltext, "Test");
    
    //Title: Enter two Intersections.
    GtkLabel *instructions = (GtkLabel *)application->get_object("Instructions");
    gtk_label_set_text(instructions, "Enter Two Intersections\n");
    
    //Title: Insert Location
    GtkLabel *locationLabel = (GtkLabel *)application->get_object("LocationLabel");
    gtk_label_set_text(locationLabel, "Type Location");
    
    //Title: Insert Destination
    GtkLabel *destinationLabel = (GtkLabel *)application->get_object("DestinationLabel");
    gtk_label_set_text(destinationLabel, "Type Destination");
    
    GtkLabel *turnPenaltyLabel = (GtkLabel *)application->get_object("TurnPenaltyLabel");
    gtk_label_set_text(turnPenaltyLabel, "      Turn Penalty:    ");
    
    gtk_widget_show(dialog);
    
    //Location -haven't implemented second search bar yet
    LocationTextGlobal = (GtkEntry *) application->get_object("Location");
    gtk_entry_set_text(LocationTextGlobal, "Location");
    g_signal_connect(LocationTextGlobal, "activate", G_CALLBACK(location_entry), application);
    
    DestinationTextGlobal = (GtkEntry *) application->get_object("Destination");
    gtk_entry_set_text(DestinationTextGlobal, "Destination");
    g_signal_connect(DestinationTextGlobal, "activate", G_CALLBACK(destination_entry), application);
    
    //If drive button is pressed
    GObject *driveButton = application->get_object("Drive");
    g_signal_connect(driveButton, "clicked", G_CALLBACK(drive_button), application);
    
    //If walk button is pressed
    GObject *walkButton = application->get_object("Walk");
    g_signal_connect(walkButton, "clicked", G_CALLBACK(walk_button), application);
    
    GObject *closeButton = application->get_object("Close");
    g_signal_connect(closeButton, "clicked", G_CALLBACK(close_button), application);
    
    
    //might not need
    g_signal_connect(GTK_DIALOG (dialog), "close", G_CALLBACK(on_dialog_response), LocationTextGlobal);
    
    //Close window
    std::cout<<"Here"<<std::endl;
    
}

void location_entry(GtkWidget */*widget*/, ezgl::application *application)
{
    //Doubles every time you reopen. (Not sure why this is happening)
    
    locationText = gtk_entry_get_text(LocationTextGlobal);
    cout<< "Pressed Enter for location"<<endl;
    
}

void destination_entry(GtkWidget */*widget*/, ezgl::application *application)
{
    //Doubles every time you reopen. (Not sure why this is happening)
    
    destinationText = gtk_entry_get_text(DestinationTextGlobal);
    cout<< "Pressed Enter for destination"<<endl;
    
}

void close_button(GtkWidget */*widget*/, ezgl::application *application)
{
    find_w_click = false;
    GtkWidget * dialog = (GtkWidget *) application -> get_object("NavigationWindow");
    GtkWidget *locationLabel = (GtkWidget *)application->get_object("LocationLabel");
    gtk_widget_hide_on_delete(locationLabel);
    gtk_widget_hide_on_delete(dialog);
}

void on_dialog_response(GtkDialog *dialog, gint /*response_id*/, gpointer /*user_data*/) {
    
    gtk_widget_hide_on_delete(GTK_WIDGET(dialog));
}



















void Load_Map(GtkWidget */*widget*/, ezgl::application *application){
    
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    std::string cityName = gtk_entry_get_text(textEntry);
    
    std::string map_path = "/cad2/ece297s/public/maps/" + cityName + ".streets.bin";
    
        close_M2();
        load_success = load_map(map_path);
        
        if(load_success){
            std::cout << "Successfully loaded map '" << map_path << "'\n";
        }else{
            std::cerr << "Usage: " << map_path << " [map_file_path]\n";
            std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
            load_success = load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
            
            if(load_success) std::cout << "Successfully loaded map '" << map_path << "'\n";
        }
    
    intersections.resize(getNumIntersections());
              
    map_bounds();

    makeStreetsVector();
    makeStreetSizeTable();
    sortFeatures();
    makePOITypesTable();

    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},{x_from_lon(max_lon), y_from_lat(max_lat)});
    application->change_canvas_world_coordinates("MainCanvas", initial_world);
    application->refresh_drawing();
    cout<<"Done reLoading"<<endl;
    
}

void nightMode_button(GtkWidget */*widget*/, ezgl::application *application){
    
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
    STREET_NAMES.color_change(D_STREET_NAMES);
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
    
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    std::string search_text = gtk_entry_get_text(textEntry);
    
    find_intersection (search_text,application,0);
}

void find_intersection(std::string search_text, ezgl::application *application, int intersectID)
{
    const char * errorLabel;
    const char * locChar;
    const char * destChar;
    string error;
    GtkLabel *errorOutput = (GtkLabel*)application->get_object("ErrorOutput");
    
    stringstream ss(search_text);
    string street1, street2;
    vector<int> street1_search_result, street2_search_result;

    getline(ss, street1, '&');//change to & test. 
    ss.ignore(256,' ');
    getline(ss, street2);

    error = "\nStreet 1: " + street1 + "     Street 2: " + street2 + "\n";

    street1_search_result = find_street_ids_from_partial_street_name(street1);
    street2_search_result = find_street_ids_from_partial_street_name(street2);
    //vector<int> intersections;
            
    
    //DEBUGGIN EDIT
    stringstream testss;
    testss<<intersectID;
    string text = testss.str();
    
    //CHECKING TO GET TWO INTERSECTIONS
    if(find_w_click){
        found_intersections.clear();
        
        if(num_intersections == 0 && found_intersections.size() == 0){
            
            //Outputting found intersection in search bar
            location_ID = intersectID; 
            locChar = text.c_str();
            //locChar = search_text.c_str();
            gtk_entry_set_text(LocationTextGlobal, locChar );
            
           
            found_intersections.push_back(intersectID);
            
            num_intersections++; //now on step 1 (num =1)
           
            
        }
        else if(num_intersections == 1 && found_intersections.size() == 0){
            
            num_intersections=0; //now on step 2 
           
             destination_ID = intersectID;
            
            //Outputting found intersection in search bar
            destChar = text.c_str();
//            destChar = search_text.c_str();
            gtk_entry_set_text(DestinationTextGlobal, destChar );
             
           
            
            //found_intersections.clear();
            found_intersections.push_back(location_ID);
            found_intersections.push_back(destination_ID);
            
           
        }
        searchingIntersections = true;
    }else{
        num_intersections = 0;
        
    
    
        //REGULARLY FINDING THE INTERSECTION
        //and error checking
        if(street1_search_result.size()==0 || street2_search_result.size()==0 ){
            if(street1_search_result.size()==0){error = "\ncannot find matching street for input 1\n";} 
            else if(street2_search_result.size()==0){error = "\ncannot find matching street for input 2\n";}
            searchingIntersections = false; //will not be drawn
        }
        else if(street1_search_result.size()>1 || street2_search_result.size()>1){
            found_intersections = find_intersections_of_two_streets(make_pair(street1_search_result[0], street2_search_result[0]));
            searchingIntersections = true; //will be drawn
        }
        else {
            found_intersections = find_intersections_of_two_streets(make_pair(street1_search_result[0], street2_search_result[0]));
            searchingIntersections = true; //will be drawn
        }
        if(found_intersections.size() == 0){
            error = "\nIntersection does not exist\n";
            searchingIntersections = false; //will not be drawn
        }

        //PRODUCTING ERROR OUTPUT or STREET NAMES OUTPUT
        errorLabel = error.c_str();
        gtk_label_set_text(errorOutput, errorLabel);
    }
    
    //Go into draw map so that the flags can actually activate
    application->refresh_drawing();
}

//void highlight_intersections(vector<int> intersection_ids, ezgl::application *application){
void highlight_intersections(vector<int> intersection_ids, ezgl::renderer *g){
    
    g->set_coordinate_system(ezgl::WORLD);
    g->set_color(YELLOW);
    
    if(find_w_click){
        for(int i=0; i< intersection_ids.size(); i++){
            
            
            
            LatLon intersection_position = getIntersectionPosition(intersection_ids[i]);
            g->fill_arc({x_from_lon(intersection_position.lon()), y_from_lat(intersection_position.lat())}, 0.001*(zooms.zcase)/(10-zooms.zcase), 0, 360);
        }
    }else{
        LatLon intersection_position = getIntersectionPosition(intersection_ids[0]);
        g->fill_arc({x_from_lon(intersection_position.lon()), y_from_lat(intersection_position.lat())}, 0.001*(zooms.zcase)/(10-zooms.zcase), 0, 360);
    }
}

void search_button(GtkWidget */*widget*/, ezgl::application *application){

    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    const char* search_text = gtk_entry_get_text(textEntry);
    
    std::vector<StreetIndex> street_index = find_street_ids_from_partial_street_name(search_text);
    // check if there are valid results
    if (street_index.size() == 0){
        cout << "No results found" << endl;
        searchingStreet = false; 
    }
    else {
        
        searchingStreet = true;
        // Make map go to zoom fit mode
       
        // Highlight street of first function
        StreetIndex street_id = street_index[0];
        found_street_segments = find_street_segments_of_street(street_id);
      
    }
    //Go into draw map so that the flags can actually activate    
       application->refresh_drawing();
}

void highlight_street(std::vector<int> street_seg_ids, ezgl::renderer *g)
{
    for(int i=0;i < street_seg_ids.size(); i++){
        InfoStreetSegment seg_info_start = getInfoStreetSegment(street_seg_ids[i]);
        g->set_color(ezgl::YELLOW);
        g->set_line_width(12);
        point2d from(x_from_lon(getIntersectionPosition(seg_info_start.from).lon()), y_from_lat(getIntersectionPosition(seg_info_start.from).lat()));
        point2d to(x_from_lon(getIntersectionPosition(seg_info_start.to).lon()), y_from_lat(getIntersectionPosition(seg_info_start.to).lat()));

        g->set_line_dash(ezgl::line_dash::none);
        g->set_line_cap(ezgl::line_cap::round);
        g->set_color(ezgl::YELLOW);
        g->set_line_width(6);

        g->draw_line(from, to);
             
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

void draw_POI(GtkWidget */*widget*/, ezgl::application *application){
    //ezgl::renderer *g = application->get_renderer();
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    POItext = gtk_entry_get_text(textEntry);
    
    if(elementAlreadyExists(POItext, POItypesTable)){
        searchingPOI = true;
    }
    else{
        cout<<"there is no matching POI type"<<endl;
        searchingPOI = false;
    }
    
    //Go into draw map so that the flags can actually activate
    application->refresh_drawing();
}

void highlight_POI(ezgl::renderer * g){
    
    g->set_coordinate_system(ezgl::WORLD);
    for(int i=0;i<getNumPointsOfInterest();i++){
        if(getPointOfInterestType(i)==POItext){
            g->set_color(RED);
            LatLon POIposition = getPointOfInterestPosition(i);
            point2d position(x_from_lon(POIposition.lon()), y_from_lat(POIposition.lat())); 
            g->fill_arc(position, 0.001*(zooms.zcase)/(10-zooms.zcase), 0, 360);//originally 0.002, and zoom in is 0.0007
            g->set_color(BLACK);
            //Change the font size too for each zoom level
            g->set_font_size(10);
            //Might want to change the position to be a little higher
            g->draw_text(position, getPointOfInterestName(i));
        }
    }
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

void act_on_key_press(ezgl::application *app, GdkEventKey* /*key*/, char* /*letter*/){   

    GtkEntry *textEntry = (GtkEntry *)app->get_object("SearchBar");
    // Get string from search bar
    typed = gtk_entry_get_text(textEntry);

    // lists possible inputs
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
    }
    g->set_coordinate_system(ezgl::WORLD);
}