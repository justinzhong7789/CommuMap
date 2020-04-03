#include "m1.h"
#include <string>
#include <iostream>
#include "m2.h"
#include "m4.h"

//HEADER FILES
#include "m2_helper/directions.hpp"
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
void draw_main_canvas(ezgl::renderer *g);
void initial_setup(ezgl::application *application, bool new_window); //add find button
void close_M2();

//Global Variables
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
vector<int> found_walk_segments;
int destination_ID;
int location_ID;
int pickUp_ID;
int start_ID;
int end_ID;
double turn_penalty_entry = 0;
double walking_speed_entry = 0;
double walking_time_limit_entry = 0;

//Created flags
bool night;
bool searchingPOI = false;
bool searchingIntersections = false;
bool searchingStreet = false;
bool searchingRoute = false;
bool searchingWalkPath = false;
bool click_OnOff = false;
bool text_OnOff = false;
bool find_w_click = false;
bool make_deliveries = false;
bool load_success;
bool usingIDs = false;
bool nightHover = false;
bool open = false;

//global variables to draw deliveries
bool set_depots = false;
bool set_deliveries = false;
int numDepots;
int numDeliveries;
bool set_pickups = false;
bool set_dropoffs = false;
bool set_weight = false;
std::vector<int> depotsIntersection;
std::vector<DeliveryInfo> depotsIntersection;
std::vector<int> pickupsIntersection; //store all intersections to display on the
std::vector<int> dropoffsIntersection; //store all drop off intersections
std::vector<float> weight; //weight?
string allDepots;
string allPickUps;
string allDropOffs;
string allWeight;
string allIndex;



//GTK global variables
GtkEntry *textboxGlobal;
GtkEntry *LocationTextGlobal;
GtkEntry *DestinationTextGlobal;
GtkEntry *LocationIDGlobal;
GtkEntry *DestinationIDGlobal;
GtkEntry *TurnPenaltyGlobal;
GtkEntry *WalkingSpeedGlobal;
GtkEntry *WalkingTimeLimitGlobal;
GtkEntry *LocationComboEntry;
GtkEntry *DestinationComboEntry;
GtkEntry *DeliveryTextGlobal;
GtkEntry *TruckCapacityGlobal;

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

    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},
    {
        x_from_lon(max_lon), y_from_lat(max_lat)
    });

    application.add_canvas("MainCanvas", draw_main_canvas, initial_world, BACKGROUND);

    application.run(initial_setup, act_on_mouse_click, nullptr, act_on_key_press);

}

void draw_main_canvas(ezgl::renderer *g) {

    if (numTimesDrawn == 0) {

        full_map.m_first.x = x_from_lon(min_lon);
        full_map.m_first.y = min_lat;
        full_map.m_second.x = x_from_lon(max_lon);
        full_map.m_second.y = max_lat;

        point2d botL(x_from_lon(min_lon), min_lat);
        point2d topR(x_from_lon(max_lon), max_lat);

        zooms.full.m_first = botL;
        zooms.full.m_second = topR;
        full_screen = g->world_to_screen(full_map);
        numTimesDrawn++;
    }

    LatLon minMax(max_lat, min_lon);

    zoom(g);
    zoomFeatures(g);
    zoomStreets(g);
    if (searchingStreet) {
        highlight_street(found_street_segments, g);
    }
    if (searchingRoute) {
        highlight_route(found_route_segments, g, BLUE);

    }
    if (searchingWalkPath) {
        highlight_route(found_walk_segments, g, CYAN);
    }
    nameStreets(g);
    nameFeatures(g);

    if (searchingIntersections) {
        highlight_intersections(found_intersections, g);
    }
    if (searchingPOI) {
        highlight_POI(g);
    }
    if (searchingRoute || searchingWalkPath) {
        start_ID = location_ID;
        end_ID = destination_ID;
        drawStartEndPoints(g);
    }
}

void drawStartEndPoints(ezgl::renderer *g) {

    if (numTimesDrawn != 0) {
        if (searchingWalkPath && found_walk_segments.size() != 0) {
            LatLon pickUpPos = getIntersectionPosition(pickUp_ID);
            point2d pickUpCentre(x_from_lon(pickUpPos.lon()), y_from_lat(pickUpPos.lat()));
            point2d pickUpText(x_from_lon(pickUpPos.lon()), y_from_lat(pickUpPos.lat()) + 0.002 * (zooms.zcase) / (10 - zooms.zcase));

            point2d pickUpBackStart(x_from_lon(pickUpPos.lon()) - 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(pickUpPos.lat()) + 0.001 * (zooms.zcase) / (10 - zooms.zcase));
            point2d pickUpBackEnd(x_from_lon(pickUpPos.lon()) + 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(pickUpPos.lat()) + 0.003 * (zooms.zcase) / (10 - zooms.zcase));

            rectangle pickUpRect(pickUpBackStart, pickUpBackEnd);
            g->set_color(WHITE);
            g->fill_rectangle(pickUpRect);

            g->set_color(RED);
            g->fill_arc(pickUpCentre, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360);
            g->draw_text(pickUpText, "Pick up Location");

            LatLon locPos = getIntersectionPosition(start_ID);
            point2d locText(x_from_lon(locPos.lon()), y_from_lat(locPos.lat()) + 0.002 * (zooms.zcase) / (10 - zooms.zcase));
            point2d locCentre(x_from_lon(locPos.lon()), y_from_lat(locPos.lat()));


            point2d locBackStart(x_from_lon(locPos.lon()) - 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(locPos.lat()) + 0.001 * (zooms.zcase) / (10 - zooms.zcase));
            point2d locBackEnd(x_from_lon(locPos.lon()) + 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(locPos.lat()) + 0.003 * (zooms.zcase) / (10 - zooms.zcase));

            rectangle locRect(locBackStart, locBackEnd);
            g->set_color(WHITE);
            g->fill_rectangle(locRect);


            g->set_color(CYAN);
            g->fill_arc(locCentre, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360);
            g->draw_text(locText, "Start");

        } else if (searchingWalkPath && found_walk_segments.size() == 0) {
            LatLon pickUpPos = getIntersectionPosition(pickUp_ID);
            point2d pickUpCentre(x_from_lon(pickUpPos.lon()), y_from_lat(pickUpPos.lat()));
            point2d pickUpText(x_from_lon(pickUpPos.lon()), y_from_lat(pickUpPos.lat()) + 0.002 * (zooms.zcase) / (10 - zooms.zcase));

            point2d pickUpBackStart(x_from_lon(pickUpPos.lon()) - 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(pickUpPos.lat()) + 0.001 * (zooms.zcase) / (10 - zooms.zcase));
            point2d pickUpBackEnd(x_from_lon(pickUpPos.lon()) + 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(pickUpPos.lat()) + 0.003 * (zooms.zcase) / (10 - zooms.zcase));

            rectangle pickUpRect(pickUpBackStart, pickUpBackEnd);
            g->set_color(WHITE);
            g->fill_rectangle(pickUpRect);


            g->set_color(RED);
            g->fill_arc(pickUpCentre, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360);
            g->draw_text(pickUpText, "Pick up Location");

        } else {
            LatLon locPos = getIntersectionPosition(start_ID);
            point2d locText(x_from_lon(locPos.lon()), y_from_lat(locPos.lat()) + 0.002 * (zooms.zcase) / (10 - zooms.zcase));
            point2d locCentre(x_from_lon(locPos.lon()), y_from_lat(locPos.lat()));

            point2d locBackStart(x_from_lon(locPos.lon()) - 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(locPos.lat()) + 0.001 * (zooms.zcase) / (10 - zooms.zcase));
            point2d locBackEnd(x_from_lon(locPos.lon()) + 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(locPos.lat()) + 0.003 * (zooms.zcase) / (10 - zooms.zcase));

            rectangle locRect(locBackStart, locBackEnd);
            g->set_color(WHITE);
            g->fill_rectangle(locRect);

            g->set_color(CYAN);
            g->fill_arc(locCentre, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360);
            g->draw_text(locText, "Start");

        }
        LatLon destPos = getIntersectionPosition(end_ID);
        point2d destCentre(x_from_lon(destPos.lon()), y_from_lat(destPos.lat()));
        point2d destText(x_from_lon(destPos.lon()), y_from_lat(destPos.lat()) + 0.002 * (zooms.zcase) / (10 - zooms.zcase));

        point2d destBackStart(x_from_lon(destPos.lon()) - 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(destPos.lat()) + 0.001 * (zooms.zcase) / (10 - zooms.zcase));
        point2d destBackEnd(x_from_lon(destPos.lon()) + 0.005 * (zooms.zcase) / (10 - zooms.zcase), y_from_lat(destPos.lat()) + 0.003 * (zooms.zcase) / (10 - zooms.zcase));

        rectangle destRect(destBackStart, destBackEnd);
        g->set_color(WHITE);
        g->fill_rectangle(destRect);

        g->set_color(BLUE);
        g->fill_arc(destCentre, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360);
        g->draw_text(destText, "Destination");
    }
}

void clean_map(ezgl::application *application) {
    searchingIntersections = false;
    searchingPOI = false;
    searchingStreet = false;
    searchingRoute = false;
    searchingWalkPath = false;
    found_intersections.clear();
    found_street_segments.clear();
    application->refresh_drawing();
}

void initial_setup(ezgl::application *application, bool /*new_window*/) {

    application->create_button("Find Intersections", 9, find_button);
    application->create_button("Load City", 7, Load_Map);
    application->colourNavigationButton(application);
    application->colourDeliveryButton(application);
    application->colourUpButton(application);
    application->colourDownButton(application);
    application->colourRightButton(application);
    application->colourLeftButton(application);
    application->colourZoomInButton(application);
    application->colourZoomOutButton(application);
    application->colourZoomFitButton(application);
    application->colourGrid(application);

    GtkLabel *error = (GtkLabel*) application->get_object("ErrorOutput");
    gtk_label_set_text(error, "\nWelcome to CommuMaps!\n");

    GObject *navigationWindow = application->get_object("Window");
    g_signal_connect(navigationWindow, "clicked", G_CALLBACK(window_button), application);
    
    GObject *deliveryButton = application->get_object("Window");
    g_signal_connect(deliveryButton, "clicked", G_CALLBACK(delivery_button), application);
    
    GtkEntry *SearchBar = (GtkEntry *) application->get_object("SearchBar");
    g_signal_connect(SearchBar, "activate", G_CALLBACK(search_bar), application);

    GObject *SearchButton = application->get_object("SearchButton");
    g_signal_connect(SearchButton, "clicked", G_CALLBACK(search_button), application);

    GObject *drawPOI = application->get_object("DrawPOI");
    g_signal_connect(drawPOI, "clicked", G_CALLBACK(draw_POI), application);

    GObject * NightMode = application->get_object("NightMode");
    g_signal_connect(NightMode, "clicked", G_CALLBACK(nightMode_button), application);

    TurnPenaltyGlobal = (GtkEntry *) application->get_object("TurnPenalty");
    WalkingSpeedGlobal = (GtkEntry *) application->get_object("WalkingSpeed");
    WalkingTimeLimitGlobal = (GtkEntry *) application->get_object("TimeLimit");
    GObject* help = application->get_object("Help");
    g_signal_connect(help, "clicked", G_CALLBACK(instructions), application);

    GtkWidget *locationComboBox = (GtkWidget*) application->get_object("LocationComboBox");
    GtkWidget *destinationComboBox = (GtkWidget*) application->get_object("DestinationComboBox");

    LocationComboEntry = (GtkEntry *) application->get_object("LocationComboEntry");
    DestinationComboEntry = (GtkEntry *) application->get_object("DestinationComboEntry");

    g_signal_connect(locationComboBox, "changed", G_CALLBACK(location_combo), NULL);
    g_signal_connect(destinationComboBox, "changed", G_CALLBACK(destination_combo), NULL);
}

void instructions(GtkWidget */*widget*/, ezgl::application *application) {

    string instructions;
    if (open == 0) {
        instructions = "\nHow to operate Navigation:\n 1)\tType in two intersections, \n\tand separate streets by an '&' sign";
        instructions += "\n\n2)\tType in beginning of intersection and find any known \n\tstreets in the drop down box.";
        instructions += "\n\tSelect and continue to type the second street\n\tYou can continue using the drop down box for the second street";
        instructions += "\n\n3)\tInput intersection Ids for the city \n\t(Only for people who created CommuMaps)";
        instructions += "\n\tPress the 'Use Id's' button before pressing \n\tchosing the method of navigation";
        instructions += "\n\n4)\tYou have the choice to choose different methods \n\t(driving or driving and walking)";
        instructions += "\n\tInput the walking speed, turning time \n\tand walking time limit in the following boxes";
        open = 1;
    } else {
        instructions = "";
        open = 0;
    }
    GtkLabel* instructionsLabel = (GtkLabel*) application->get_object("ScrollLabel");
    const char* print;
    print = instructions.c_str();
    gtk_label_set_text(instructionsLabel, print);
    nightHover = false;
}

void location_combo(GtkComboBox */*widget*/, ezgl::application * /*application*/) {
    string textEntry = gtk_entry_get_text(LocationComboEntry);
    cout << textEntry << endl;
    string locationEntry = gtk_entry_get_text(LocationTextGlobal);
    if (locationEntry.find("&") != std::string::npos) {
        stringstream ss(locationEntry);
        string street1;
        getline(ss, street1, '&');
        textEntry = street1 + " & " + textEntry;
    }
    const char * printText = textEntry.c_str();
    gtk_entry_set_text(LocationTextGlobal, printText);

}

void destination_combo(GtkComboBox */*widget*/, ezgl::application * /*application*/) {
    string textEntry = gtk_entry_get_text(DestinationComboEntry);
    cout << textEntry << endl;
    string destinationEntry = gtk_entry_get_text(DestinationTextGlobal);
    if (destinationEntry.find("&") != std::string::npos) {
        stringstream ss(destinationEntry);
        string street1;
        getline(ss, street1, '&');
        textEntry = street1 + " & " + textEntry;
    }
    const char * printText = textEntry.c_str();
    gtk_entry_set_text(DestinationTextGlobal, printText);
}

void search_bar(GtkWidget */*widget*/, ezgl::application *application) {
    GtkEntry *textEntry = (GtkEntry *) application->get_object("SearchBar");
    const char* search_text = gtk_entry_get_text(textEntry);

    GtkLabel *errorOutput = (GtkLabel*) application->get_object("ErrorOutput");
    std::string error;
    const char * print;

    std::vector<StreetIndex> street_index = find_street_ids_from_partial_street_name(search_text);

    find_w_click = false;
    // SEARCH
    if (find_street(application, search_text)) {
        error = "\nFound a Street!\n";

    } else if (find_intersection(search_text, application, 0)) {
        error = "\nFound an Intersection!\n";
    }

    print = error.c_str();
    gtk_label_set_text(errorOutput, print);
}

bool find_street(ezgl::application *application, std::string search_text) {

    bool found_street;
    const char * print;
    GtkLabel *errorOutput = (GtkLabel*) application->get_object("ErrorOutput");
    std::string error;

    std::vector<StreetIndex> street_index = find_street_ids_from_partial_street_name(search_text);
    // check if there are valid results
    if (street_index.size() == 0) {
        error = "\nNo streets Found\n";
        searchingStreet = false;
        found_street = false;
    } else {

        searchingStreet = true;
        // Make map go to zoom fit mode
        // Highlight street of first function
        StreetIndex street_id = street_index[0];
        found_street_segments = find_street_segments_of_street(street_id);
        found_street = true;
    }
    //Go into draw map so that the flags can actually activate
    application->refresh_drawing();
    print = error.c_str();
    gtk_label_set_text(errorOutput, print);
    return found_street;
}

void window_button(GtkWidget */*widget*/, ezgl::application *application) {
    application->colourGrid(application);
    clean_map(application);
    find_w_click = true;

    GtkWindow *parent = (GtkWindow *) application->get_object("MainWindow");
    GtkWidget *dialog = (GtkWidget *) application->get_object("NavigationWindow");
    gtk_window_set_transient_for((GtkWindow *) dialog, parent);



    GtkLabel *route = (GtkLabel*) application->get_object("Route");
    gtk_label_set_text(route, "\tDirections:\t");


    //Plan to display Route
    GtkLabel *labeltext = (GtkLabel *) application->get_object("ScrollLabel");
    gtk_label_set_text(labeltext, "\n\n\n\n\nWaiting for \nNavigation Input ... ");


    gtk_widget_show(dialog);

    //Location -haven't implemented second search bar yet
    LocationTextGlobal = (GtkEntry *) application->get_object("Location");

    g_signal_connect(LocationTextGlobal, "activate", G_CALLBACK(location_entry), application);

    DestinationTextGlobal = (GtkEntry *) application->get_object("Destination");

    g_signal_connect(DestinationTextGlobal, "activate", G_CALLBACK(destination_entry), application);


    LocationIDGlobal = (GtkEntry *) application->get_object("LocationID");
    DestinationIDGlobal = (GtkEntry *) application->get_object("DestinationID");


    //If drive button is pressed
    GObject *driveButton = application->get_object("Drive");
    g_signal_connect(driveButton, "clicked", G_CALLBACK(drive_button), application);

    //If walk button is pressed
    GObject *walkButton = application->get_object("Walk");
    g_signal_connect(walkButton, "clicked", G_CALLBACK(walk_button), application);

    GObject *useIDsButton = application->get_object("UseIDs");
    g_signal_connect(useIDsButton, "clicked", G_CALLBACK(useIDs_button), application);

    GObject *closeButton = application->get_object("Close");
    g_signal_connect(closeButton, "clicked", G_CALLBACK(close_button), application);


    //Close window
    g_signal_connect(GTK_DIALOG(dialog), "close", G_CALLBACK(on_dialog_response), LocationTextGlobal);

}

void useIDs_button(GtkWidget */*widget*/, ezgl::application */*application*/) {
    usingIDs = true;
}

void location_entry(GtkWidget *widget, ezgl::application *application) {
    //Doubles every time you reopen. (Not sure why this is happening)
    show_location_autofill(widget, application);
}

void destination_entry(GtkWidget *widget, ezgl::application *application) {
    //Doubles every time you reopen. (Not sure why this is happening)
    show_destination_autofill(widget, application);
}

void close_button(GtkWidget */*widget*/, ezgl::application *application) {
    find_w_click = false;
    GtkWidget * dialog = (GtkWidget *) application -> get_object("NavigationWindow");
    GtkWidget *locationLabel = (GtkWidget *) application->get_object("LocationLabel");
    GtkLabel * scrollLabel = (GtkLabel *) application->get_object("ScrollLabel");
    gtk_label_set_text(scrollLabel, "");
    GtkLabel * routeLabel = (GtkLabel *) application->get_object("Route");
    gtk_label_set_text(routeLabel, "");
    gtk_widget_hide_on_delete(locationLabel);
    gtk_widget_hide_on_delete(dialog);

    clean_map(application);
}

void on_dialog_response(GtkDialog *dialog, gint /*response_id*/, gpointer /*user_data*/) {

    gtk_widget_hide_on_delete(GTK_WIDGET(dialog));
}

void show_location_autofill(GtkWidget */*widget*/, ezgl::application *application) {
    const char * fill;
    string streetName;
    string currentText = gtk_entry_get_text(LocationTextGlobal);
    std::vector<int> streetIds;

    GtkListStore *listStoreLoc = (GtkListStore *) application->get_object("LocationListstore");
    GtkTreeIter iter;
    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(listStoreLoc), &iter);

    stringstream ss(currentText);
    string street1, street2;
    getline(ss, street1, '&');
    street2 = street1;
    ss.ignore(256, ' ');
    getline(ss, street2);

    streetIds = find_street_ids_from_partial_street_name(street2);

    for (int i = 0; i < 5; i++) {
        if (streetIds.size() == 0) {
            gtk_list_store_set(listStoreLoc, &iter, 0, "No options Found", -1);
            gtk_tree_model_iter_next(GTK_TREE_MODEL(listStoreLoc), &iter);
        }
        if (i >= streetIds.size()) {
            gtk_list_store_set(listStoreLoc, &iter, 0, "", -1);
            gtk_tree_model_iter_next(GTK_TREE_MODEL(listStoreLoc), &iter);
        } else {
            streetName = getStreetName(streetIds[i]);
            fill = streetName.c_str();
            gtk_list_store_set(listStoreLoc, &iter, 0, fill, -1);
            gtk_tree_model_iter_next(GTK_TREE_MODEL(listStoreLoc), &iter);
        }
    }

}

void show_destination_autofill(GtkWidget */*widget*/, ezgl::application *application) {
    const char * fill;
    string streetName;
    string currentText = gtk_entry_get_text(DestinationTextGlobal);
    std::vector<int> streetIds;

    GtkListStore *listStoreDest = (GtkListStore *) application->get_object("DestinationListstore");
    GtkTreeIter iter;
    gtk_tree_model_get_iter_first(GTK_TREE_MODEL(listStoreDest), &iter);

    stringstream ss(currentText);
    string street1, street2;
    getline(ss, street1, '&');
    street2 = street1;
    ss.ignore(256, ' ');
    getline(ss, street2);

    streetIds = find_street_ids_from_partial_street_name(street2);

    for (int i = 0; i < 5; i++) {
        if (streetIds.size() == 0) {
            gtk_list_store_set(listStoreDest, &iter, 0, "No options Found", -1);
            gtk_tree_model_iter_next(GTK_TREE_MODEL(listStoreDest), &iter);
        }
        if (i >= streetIds.size()) {
            gtk_list_store_set(listStoreDest, &iter, 0, "", -1);
            gtk_tree_model_iter_next(GTK_TREE_MODEL(listStoreDest), &iter);
        } else {
            streetName = getStreetName(streetIds[i]);
            fill = streetName.c_str();
            gtk_list_store_set(listStoreDest, &iter, 0, fill, -1);
            gtk_tree_model_iter_next(GTK_TREE_MODEL(listStoreDest), &iter);
        }
    }

}

void Load_Map(GtkWidget */*widget*/, ezgl::application *application) {

    GtkEntry *textEntry = (GtkEntry *) application->get_object("SearchBar");
    // Get string from search bar
    std::string cityName = gtk_entry_get_text(textEntry);

    std::string map_path = "/cad2/ece297s/public/maps/" + cityName + ".streets.bin";

    close_M2();
    load_success = load_map(map_path);

    if (load_success) {
        std::cout << "Successfully loaded map '" << map_path << "'\n";
    } else {
        std::cerr << "Usage: " << map_path << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        load_success = load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");

        if (load_success) std::cout << "Successfully loaded map '" << map_path << "'\n";
    }

    intersections.resize(getNumIntersections());

    map_bounds();

    makeStreetsVector();
    makeStreetSizeTable();
    sortFeatures();
    makePOITypesTable();

    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)},
    {
        x_from_lon(max_lon), y_from_lat(max_lat)
    });
    application->change_canvas_world_coordinates("MainCanvas", initial_world);
    application->refresh_drawing();
    cout << "Done reLoading" << endl;

}

void nightMode_button(GtkWidget */*widget*/, ezgl::application *application) {

    if (!night) {
        setNight();
    } else {
        setLight();
    }
    application->refresh_drawing();
}

void setNight() {
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

void setLight() {
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

void find_button(GtkWidget */*widget*/, ezgl::application *application) {

    GtkEntry *textEntry = (GtkEntry *) application->get_object("SearchBar");
    // Get string from search bar
    std::string search_text = gtk_entry_get_text(textEntry);

    find_intersection(search_text, application, 0);
}

bool find_intersection(std::string search_text, ezgl::application *application, int intersectID) {
    bool intersection_found = true;

    const char * locChar;
    const char * destChar;

    stringstream ss(search_text);
    string street1, street2;
    vector<int> street1_search_result, street2_search_result;

    getline(ss, street1, '&'); //change to & test. 
    ss.ignore(256, ' ');
    getline(ss, street2);

    street1_search_result = find_street_ids_from_partial_street_name(street1);
    street2_search_result = find_street_ids_from_partial_street_name(street2);


    //DEBUGGIN EDIT
    stringstream testss;
    testss << intersectID;
    string text = testss.str();

    //CHECKING TO GET TWO INTERSECTIONS
    if (find_w_click) {
        found_intersections.clear();

        if (num_intersections == 0 && found_intersections.size() == 0) {

            //Outputting found intersection in search bar
            location_ID = intersectID;
            locChar = text.c_str();

            gtk_entry_set_text(LocationIDGlobal, locChar);

            locChar = search_text.c_str();
            gtk_entry_set_text(LocationTextGlobal, locChar);

            found_intersections.push_back(intersectID);

            num_intersections++; //now on step 1 (num =1)


        } else if (num_intersections == 1 && found_intersections.size() == 0) {

            num_intersections = 0; //now on step 2 

            destination_ID = intersectID;

            //Outputting found intersection in search bar
            destChar = text.c_str();
            gtk_entry_set_text(DestinationIDGlobal, destChar);

            destChar = search_text.c_str();
            gtk_entry_set_text(DestinationTextGlobal, destChar);

            //found_intersections.clear();
            found_intersections.push_back(location_ID);
            found_intersections.push_back(destination_ID);


        }
        searchingIntersections = true;
        intersection_found = true;
    } else {
        num_intersections = 0;

        intersection_found = validIntersection(street1, street2, application);
    }

    //Go into draw map so that the flags can actually activate
    application->refresh_drawing();
    return intersection_found;
}

bool validIntersection(std::string street1, std::string street2, ezgl::application *application) {
    //REGULARLY FINDING THE INTERSECTION
    bool intersection_found;
    const char * errorLabel;
    string error;
    GtkLabel *errorOutput = (GtkLabel*) application->get_object("ErrorOutput");

    vector<int> street1_search_result, street2_search_result;
    street1_search_result = find_street_ids_from_partial_street_name(street1);
    street2_search_result = find_street_ids_from_partial_street_name(street2);
    //and error checking
    if (street1_search_result.size() == 0 || street2_search_result.size() == 0) {
        if (street1_search_result.size() == 0) {
            error = "\ncannot find matching street for input 1\n";
        }
        else if (street2_search_result.size() == 0) {
            error = "\ncannot find matching street for input 2\n";
        }
        searchingIntersections = false; //will not be drawn
        intersection_found = false;

    } else if (street1_search_result.size() > 1 || street2_search_result.size() > 1) {
        found_intersections = find_intersections_of_two_streets(make_pair(street1_search_result[0], street2_search_result[0]));
        searchingIntersections = true; //will be drawn
        intersection_found = true;
    } else {
        found_intersections = find_intersections_of_two_streets(make_pair(street1_search_result[0], street2_search_result[0]));
        searchingIntersections = true; //will be drawn
        intersection_found = true;
    }
    if (found_intersections.size() == 0) {
        error = "\nIntersection does not exist\n";
        searchingIntersections = false; //will not be drawn
        intersection_found = false;
    }

    //PRODUCTING ERROR OUTPUT or STREET NAMES OUTPUT
    errorLabel = error.c_str();
    gtk_label_set_text(errorOutput, errorLabel);

    return intersection_found;
}

void highlight_intersections(vector<int> intersection_ids, ezgl::renderer *g) {

    g->set_coordinate_system(ezgl::WORLD);
    g->set_color(YELLOW);

    if (find_w_click) {
        for (int i = 0; i < intersection_ids.size(); i++) {



            LatLon intersection_position = getIntersectionPosition(intersection_ids[i]);
            g->fill_arc({x_from_lon(intersection_position.lon()), y_from_lat(intersection_position.lat())}, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360);
        }
    } else {
        LatLon intersection_position = getIntersectionPosition(intersection_ids[0]);
        g->fill_arc({x_from_lon(intersection_position.lon()), y_from_lat(intersection_position.lat())}, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360);
    }
}

void search_button(GtkWidget */*widget*/, ezgl::application *application) {

    GtkEntry *textEntry = (GtkEntry *) application->get_object("SearchBar");
    const char* search_text = gtk_entry_get_text(textEntry);

    const char * errorLabel;
    string error;
    GtkLabel *errorOutput = (GtkLabel*) application->get_object("ErrorOutput");

    std::vector<StreetIndex> street_index = find_street_ids_from_partial_street_name(search_text);
    // check if there are valid results
    if (street_index.size() == 0) {
        error = "\nNo results found\n";
        searchingStreet = false;
    } else {

        searchingStreet = true;
        StreetIndex street_id = street_index[0];
        found_street_segments = find_street_segments_of_street(street_id);

    }

    errorLabel = error.c_str();
    gtk_label_set_text(errorOutput, errorLabel);

    //Go into draw map so that the flags can actually activate    
    application->refresh_drawing();
}

void highlight_street(std::vector<int> street_seg_ids, ezgl::renderer *g) {
    for (int i = 0; i < street_seg_ids.size(); i++) {
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

void close_M2() {
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

void draw_POI(GtkWidget */*widget*/, ezgl::application *application) {

    GtkEntry *textEntry = (GtkEntry *) application->get_object("SearchBar");
    const char * errorLabel;
    string error;
    GtkLabel *errorOutput = (GtkLabel*) application->get_object("ErrorOutput");
    // Get string from search bar
    POItext = gtk_entry_get_text(textEntry);

    if (elementAlreadyExists(POItext, POItypesTable)) {
        searchingPOI = true;
    } else {
        error = "\nThere is no matching POI type\n";
        searchingPOI = false;
    }

    errorLabel = error.c_str();
    gtk_label_set_text(errorOutput, errorLabel);

    //Go into draw map so that the flags can actually activate
    application->refresh_drawing();
}

void highlight_POI(ezgl::renderer * g) {

    g->set_coordinate_system(ezgl::WORLD);
    for (int i = 0; i < getNumPointsOfInterest(); i++) {
        if (getPointOfInterestType(i) == POItext) {
            g->set_color(RED);
            LatLon POIposition = getPointOfInterestPosition(i);
            point2d position(x_from_lon(POIposition.lon()), y_from_lat(POIposition.lat()));
            point2d textposition(x_from_lon(POIposition.lon()), y_from_lat(POIposition.lat()) + 0.002 * (zooms.zcase) / (10 - zooms.zcase));
            g->fill_arc(position, 0.001 * (zooms.zcase) / (10 - zooms.zcase), 0, 360); //originally 0.002, and zoom in is 0.0007
            g->set_color(BLACK);
            //Change the font size too for each zoom level
            g->set_font_size(0.02 * (zooms.zcase) / (10 - zooms.zcase));
            //Might want to change the position to be a little higher
            g->draw_text(textposition, getPointOfInterestName(i));
        }
    }
}

void makePOITypesTable() {
    int numPOI = getNumPointsOfInterest();
    for (int i = 0; i < numPOI; i++) {
        if (!elementAlreadyExists(getPointOfInterestType(i), POItypesTable)) {
            POItypesTable.push_back(getPointOfInterestType(i));
        }
    }

}

bool elementAlreadyExists(string st, vector<string> vec) {
    bool exist = false;
    for (int i = 0; i < vec.size(); i++) {
        if (st == vec[i]) {
            exist = true;
            break;
        }
    }
    return exist;
}

void act_on_key_press(ezgl::application *app, GdkEventKey* /*key*/, char* /*letter*/) {

    GtkEntry *textEntry = (GtkEntry *) app->get_object("SearchBar");
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

    vector<int> results;
    int results_num;

    stringstream ss(typed);
    string street1, street2;
    getline(ss, street1, '&');
    ss.ignore(256, ' ');
    street2 = street1;
    getline(ss, street2);

    results = find_street_ids_from_partial_street_name(street2);
    results_num = results.size();
    app->refresh_drawing();
    for (int i = 0; i < std::min(5, results_num); i++) {
        g->set_color(ezgl::WHITE);
        g->fill_rectangle({start_point.x, start_point.y + 30 * (i) + 3},
        {
            end_point.x, end_point.y + 30 * (i) + 3
        });
        g->set_color(ezgl::BLACK);
        g->draw_text({text_start.x, text_start.y + 30 * (i) + 3}, getStreetName(results[i]));
    }


    g->set_coordinate_system(ezgl::WORLD);
}
