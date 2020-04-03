#include "directions.hpp"

struct Direction{
    string Name;
    double rad;
};

//NEEDED THESE CUZ FOR SOME REASON IT WOULDNT CALCULATE IT PROPERLY
double pi2 = M_PI*2;
double pi1_2 = M_PI*0.5;
double pi3_2 = M_PI*1.5;
double pi5_2 = M_PI*2.5;
double pi1_4 = M_PI*0.25;
double pi3_4 = M_PI*0.75;
double pi5_4 = M_PI*1.25;
double pi7_4 = M_PI*1.75;

void delivery_button(GtkWidget */*widget*/, ezgl::application *application) {
    
    clean_map(application);
    make_deliveries = true;

    GtkWindow *parent = (GtkWindow *) application->get_object("MainWindow");
    GtkWidget *deliveryWindow = (GtkWidget *) application->get_object("DeliveryWindow");
    gtk_window_set_transient_for((GtkWindow *) deliveryWindow, parent);

    gtk_widget_show(dialog);

    //Location -haven't implemented second search bar yet
    DeliveryTextGlobal = (GtkEntry *) application->get_object("DeliveryEntry");
    g_signal_connect(DeliveryTextGlobal, "activate", G_CALLBACK(delivery_entry), application);

    GtkEntry* numDepotsEntry = (GtkEntry *) application->get_object("NumDepotsEntry");
    g_signal_connect(numDepotsEntry, "activate", G_CALLBACK(num_depots_entry), application);
    GtkEntry* numDeliveryEntry = (GtkEntry *) application->get_object("NumDeliveryEntry");
    g_signal_connect(numDeliveryEntry, "activate", G_CALLBACK(num_delivery_entry), application);

    TurnPenaltyGlobal = (GtkEntry *) application->get_object("DeliveryTurnPenaltyEntry");
    TruckCapacityGlobal = (GtkEntry *) application->get_object("TruckCapacity");


    GObject *depotsDone = application->get_object("DepotsDone");
    g_signal_connect(depotsDone, "clicked", G_CALLBACK(depots_done), application);

    GObject *pickUpsDone = application->get_object("PickUpsDone");
    g_signal_connect(pickUpsDone, "clicked", G_CALLBACK(pickups_done), application);

    GObject *dropOffsDone = application->get_object("DropOffsDone");
    g_signal_connect(dropOffsDone, "clicked", G_CALLBACK(dropoffs_done), application);
    
    GObject *weightDone = application->get_object("WeightDone");
    g_signal_connect(weightDone, "clicked", G_CALLBACK(weight_done), application);
    
    GObject *deliverButton = application->get_object("MakeDelivery");
    g_signal_connect(deliverButton, "clicked", G_CALLBACK(make_delivery), application);
    
    GObject *closeButton = application->get_object("DeliveryClose");
    g_signal_connect(closeButton, "clicked", G_CALLBACK(close_button), application);
    
}

void delivery_entry(GtkWidget */*widget*/, ezgl::application *application)
{
    stringstream ss;
    const char* print;
    int intersectionID;
    
    string text; 
    GtkLabel * label;
    //Input in the necessary places
    if(set_depots)
    {
        if(numDepots > 0){
            label = (GtkLabel*) application->get_object("ChosenDepots");        
            
         
            text = gtk_entry_get_text(DeliveryTextGlobal);
            ss<<text;
            ss>>intersectionID;
            
            //Starts creating the vector of depots
            depotsIntersection.push_back(intersectionID);
            allDepots += intersectionID + "\t" + getIntersectionName(intersectionID) + "\n"; 
            
            print = allDepots.c_str();
            gtk_entry_set_text(label, print);
            numDepots--;
        }
        else set_depots = false;
    }
    else if(set_deliveries)
    {
        if(numDeliveries>0){
            if(set_pickups){
                
                label = (GtkLabel*) application->get_object("PickUps");
                
                text = gtk_entry_get_text(DeliveryTextGlobal);
                ss<<text;
                ss>>pickUp;

                //Starts creating the vector of depots
                allPickUps += intersectionID + "\t" + getIntersectionName(pickUp) + "\n"; 

                print = allPickUps.c_str();
                gtk_entry_set_text(label, print);
                
                //Setting up next
                set_pickups = false;
                set_dropoffs = true;
            }
            else if(set_dropoffs){
                
                //Setting up next
                set_dropoffs = false;
                set_weight = true;
                
            }else if(set_weight){
                //Make deliveryInfo variable
                
                //Setting up next
                set_weight = false;
                numDeliveries--;
                if(numDeliveries >0 ){
                    set_pickups = true;
                }
            }
        }
    }
}

//Starts the recording of depots, and writes number of depots to be inputted in a label beside the entry bar
void num_depots_entry(GtkWidget */*widget*/, ezgl::application *application)
{
    //clear it before writing into it
    depotsIntersection.clear();
    allDepots = "";
    set_depots = true;
    
    GtkEntry* numDepotsEntry = (GtkEntry *) application->get_object("NumDepotsEntry");
    string numDepotsText = gtk_entry_get_text(numDepotsEntry);
    stringstream ss(numDepotsText);
    ss>>numDepots;
    GtkLabel* numDepotsLabel = (GtkLabel*) application->get_object("NumDepots");
    const char* print = numDepotsText.c_str();
    gtk_label_set_text(numDepotsLabel, print);
}

//Starts the recording of deliveries and writes number of deliveries to be inputted in a label beside the entry bar
void num_delivery_entry(GtkWidget */*widget*/, ezgl::application *application){
    
    allPickUps = "";
    allDropOffs = "";
    allWeight = "";
    allIndex = "";
    set_pickups = true;
    set_deliveries = true;
    
    GtkEntry* numDeliveriesEntry = (GtkEntry *) application->get_object("NumDeliveriesEntry");
    string numDeliveriesText = gtk_entry_get_text(numDeliveriesEntry);
    stringstream ss(numDeliveriesText);
    ss>>numDeliveries;
    GtkLabel* numDeliveriesLabel = (GtkLabel*) application->get_object("NumDeliveries");
    const char* print = numDeliveriesText.c_str();
    gtk_label_set_text(numDeliveriesLabel, print);
}

void depots_done(GtkComboBox *widget, ezgl::application * application)
{
    set_depots = false;
}

void pickups_done(GtkWidget */*widget*/,ezgl::application *application);
void dropoffs_done(GtkWidget */*widget*/,ezgl::application *application);
void weight_done(GtkWidget */*widget*/,ezgl::application *application);
void make_deliveries(GtkWidget */*widget*/,ezgl::application *application);

