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

string map_name;
void draw_main_canvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window);//add find button
void find_button(GtkWidget */*widget*/, ezgl::application *application);
void search_button(GtkWidget */*widget*/, ezgl::application *application);
void nightMode_button(GtkWidget *widget, ezgl::application *application);
void Load_Map(GtkWidget */*widget*/, ezgl::application *application);
void Open_Window(GtkWidget */*widget*/, ezgl::application *application);
void on_dialog_response(GtkDialog *dialog, gint /*response_id*/, gpointer /*user_data*/);
void window_button(GtkWidget */*widget*/, ezgl::application *application, gpointer user_data);
void open_dialog (GtkWidget *widget, gpointer data);
void drive_button(GtkWidget */*widget*/, ezgl::application *application);
void walk_button(GtkWidget */*widget*/, ezgl::application *application);

vector<string> POItypesTable;
//const char* search_text;

void highlight_intersections(vector<int> intersection_ids, ezgl::application *application);
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

GtkEntry *textboxGlobal;

std::string typed;

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
    
    zoom(g);
    zoomFeatures(g);
    zoomStreets(g);
    nameStreets(g);
    nameFeatures(g);
    //drawSearchBar(g);
}

void initial_setup(ezgl::application *application, bool /*new_window*/){  
//  application->create_button("find", 6, find_button);
  application->create_button("find", 6, find_button);
  application->create_button("draw POI", 7, drawPOI);
  application->create_button("load map",8,Load_Map);
  application->create_button("open window",9,Open_Window);
  
//  GtkWidget *window;
//  window = (GtkWidget *) application->get_object("ApplicationWindow");
  
  GObject *openMj = application->get_object("Window");
  g_signal_connect(openMj, "clicked", G_CALLBACK(window_button),application);
//  g_signal_connect(openMj, "clicked", G_CALLBACK(open_dialog),window);
  
  GObject *SearchButton = application->get_object("SearchButton");
  g_signal_connect(SearchButton, "clicked", G_CALLBACK(search_button), application);

  GObject * NightMode = application->get_object("NightMode");
  g_signal_connect(NightMode, "clicked", G_CALLBACK(nightMode_button),application);
//  gtk_widget_show_all(window);
  
  
}


void window_button(GtkWidget */*widget*/, ezgl::application *application, gpointer user_data){
    
    GtkWindow *parent = (GtkWindow *) application->get_object("MainWindow");
    GtkWidget *dialog = (GtkWidget *)application->get_object("NavigationWindow");
    gtk_window_set_transient_for((GtkWindow *)dialog, parent);
    
    GtkWidget *label; // the label we will create to display a message in the content area
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
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
    
    //Text bar1 -haven't implemented second search bar yet
    textboxGlobal = (GtkEntry *) application->get_object("Location");
    gtk_entry_set_text(textboxGlobal, "Location");
    
    gtk_widget_show_all(dialog);
    g_signal_connect(GTK_DIALOG (dialog), "response", G_CALLBACK(on_dialog_response), textboxGlobal);
    
    //If drive button is pressed
    GObject *DriveButton = application->get_object("Drive");
    g_signal_connect(DriveButton, "clicked", G_CALLBACK(drive_button), application);
    
    //If walk button is pressed
    GObject *WalkButton = application->get_object("Walk");
    g_signal_connect(WalkButton, "clicked", G_CALLBACK(walk_button), application);
    
    //Close window
    g_signal_connect(GTK_DIALOG (dialog), "response", G_CALLBACK(on_dialog_response), textboxGlobal);
    std::cout<<"Here"<<std::endl;
    
}

void drive_button(GtkWidget */*widget*/, ezgl::application *application)
{
    
    cout<<"Drive was pressed!"<<endl;
}

void walk_button(GtkWidget */*widget*/, ezgl::application *application)
{

    cout<<"Walk was pressed"<<endl;
}

void Open_Window(GtkWidget */*widget*/, ezgl::application *application) {


        //Pop up the window with the warning
        GObject *window; // the parent window over which to add the dialog
        GtkWidget *content_area; // the content area of the dialog
        GtkWidget *label; // the label we will create to display a message in the content area
        GtkWidget *dialog; // the dialog box we will create

        // get a pointer to the main application window
        window = application->get_object(application->get_main_window_id().c_str());
        
        // Create the dialog window.
        // Modal windows prevent interaction with other windows in the same application
        //GTK_DIALOG_MODAL closes the 
        dialog = gtk_dialog_new_with_buttons("Navigate", (GtkWindow*) window, GTK_DIALOG_MODAL, ("Done"), GTK_RESPONSE_ACCEPT, NULL);

        // Create a label and attach it to the content area of the dialog
        content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        label = gtk_label_new("Enter Two Intersections");
        gtk_container_add(GTK_CONTAINER(content_area), label);

        // The main purpose of this is to show dialog’s child widget, label
        gtk_widget_show_all(dialog);

        // Connecting the "response" signal from the user to the associated callback function
        g_signal_connect(GTK_DIALOG(dialog), "response", G_CALLBACK(on_dialog_response), NULL);
   
        
}

void on_dialog_response(GtkDialog *dialog, gint /*response_id*/, gpointer /*user_data*/) {
    // This will cause the dialog to be destroyed and close
    // without this line the dialog remains open unless the
    // response_id is GTK_RESPONSE_DELETE_EVENT which
    // automatically closes the dialog without the following line.
    
    
    //Will print out line when dialog is closed (Doesn't mean that it doesnt get the input still tho.. I just can't print it)
    std::string text = gtk_entry_get_text(textboxGlobal);
    cout<< text<<endl;
    gtk_widget_destroy(GTK_WIDGET(dialog));
}



















void Load_Map(GtkWidget */*widget*/, ezgl::application *application){
    
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    std::string map_path = gtk_entry_get_text(textEntry);

        cout<<"load_map was clicked"<<endl;
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
    
    cout<< "the find button is pressed."<<endl;
        // Get Object
    GtkEntry *textEntry = (GtkEntry *)application->get_object("SearchBar");
    // Get string from search bar
    std::string search_text = gtk_entry_get_text(textEntry);
    stringstream ss(search_text);
    string street1, street2;
    vector<int> street1_search_result, street2_search_result, found_intersections;
    getline(ss, street1, ',');
    ss.ignore(5,' ');
    getline(ss, street2);
    street1_search_result = find_street_ids_from_partial_street_name(street1);
    street2_search_result = find_street_ids_from_partial_street_name(street2);
    if(street1_search_result.size()==0 || street2_search_result.size()==0 ){
        if(street1_search_result.size()==0){cout<<"cannot find matching street for input 1"<<endl;}
        if(street2_search_result.size()==0){cout<<"cannot find matching street for input 2"<<endl;}
    }
    else if(street1_search_result.size()>1 || street2_search_result.size()>1){
        if(street1_search_result.size()>1){cout<<"input 1 does not uniquely identify a street"<<endl;}
        if(street2_search_result.size()>1){cout<<"input 2 does not uniquely identify a street"<<endl;}
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
                g->fill_arc(position, 0.001*(zooms.zcase)/(10-zooms.zcase), 0, 360);//originally 0.002, and zoom in is 0.0007
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

void act_on_key_press(ezgl::application *app, GdkEventKey* /*key*/, char* /*letter*/){   

    GtkEntry *textEntry = (GtkEntry *)app->get_object("SearchBar");
    // Get string from search bar
    typed = gtk_entry_get_text(textEntry);
    //cout << typed << endl;
//    
//    GtkEntry *textLocation = (GtkEntry *)app->get_object("Location");
//    // Get string from search bar
//    std::string location = gtk_entry_get_text(textLocation);
//    cout << typed << endl;
//    cout<<"Location: "<<location<<endl;
//    
    
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