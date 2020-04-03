#include "directions.hpp"

//Global drawing deliveries
bool making_deliveries_show;
bool set_depots_show;
bool set_deliveries_show;
bool set_pickups_show;
bool set_dropoffs_show;
bool set_weight_show;
int numDepotsShow;
int numDeliveriesShow;
int weightShow;
int pickUpShow;
int dropOffShow;
int indexShow =1;
std::vector<int> depotsShow;
std::vector<DeliveryInfo> deliveriesShow;
//std::vector<int> pickupsIntersectionShow; //store all intersections to display on the
//std::vector<int> dropoffsIntersectionShow; //store all drop off intersections
std::vector<CourierSubpath> subpathShow;
string allDepotsShow;
string allPickUpsShow;
string allDropOffsShow;
string allWeightShow;
string allIndexShow;
string indexString;
float turnPenaltyShow;
float truckCapacityShow;
//GtkEntries for deliveries
GtkEntry *TruckCapacityGlobal;
GtkEntry *DeliveryTextGlobal;
GtkEntry *TurnPenaltyDeliveryGlobal;

void delivery_button(GtkWidget */*widget*/, ezgl::application *application)
{
    cout<<"Delivery Button!"<<endl;
    clean_map(application);
    making_deliveries_show = true;

    GtkWindow *parent = (GtkWindow *) application->get_object("MainWindow");
    GtkWidget *deliveryWindow = (GtkWidget *) application->get_object("DeliveryWindow");
    gtk_window_set_transient_for((GtkWindow *) deliveryWindow, parent);

    gtk_widget_show(deliveryWindow);
    //cout<<"Got these objects"<<endl;
    //Location -haven't implemented second search bar yet
    DeliveryTextGlobal = (GtkEntry *) application->get_object("DeliveryEntry");
    g_signal_connect(DeliveryTextGlobal, "activate", G_CALLBACK(delivery_entry), application);

    GtkEntry* numDepotsEntry = (GtkEntry *) application->get_object("NumDepotsEntry");
    g_signal_connect(numDepotsEntry, "activate", G_CALLBACK(num_depots_entry), application);
    GtkEntry* numDeliveryEntry = (GtkEntry *) application->get_object("NumDeliveriesEntry");
    g_signal_connect(numDeliveryEntry, "activate", G_CALLBACK(num_delivery_entry), application);
    TurnPenaltyDeliveryGlobal = (GtkEntry *) application->get_object("DeliveryTurnPenalty");
     cout<<"Got these objects"<<endl;
    TruckCapacityGlobal = (GtkEntry *) application->get_object("TruckCapacity");

    GObject *depotsDone = application->get_object("DepotsDone");
    g_signal_connect(depotsDone, "clicked", G_CALLBACK(depots_done), application);

    GObject *pickUpsDone = application->get_object("PickUpsDone");
    g_signal_connect(pickUpsDone, "clicked", G_CALLBACK(pickups_done), application);

//    GObject *dropOffsDone = application->get_object("DropOffsDone");
//    g_signal_connect(dropOffsDone, "clicked", G_CALLBACK(dropoffs_done), application);
    
//    GObject *weightDone = application->get_object("WeightDone");
//    g_signal_connect(weightDone, "clicked", G_CALLBACK(weight_done), application);
    
    GObject *deliverButton = application->get_object("MakeDelivery");
    g_signal_connect(deliverButton, "clicked", G_CALLBACK(make_deliveries), application);
    
    GObject *closeButton = application->get_object("DeliveryClose");
    g_signal_connect(closeButton, "clicked", G_CALLBACK(close_button_delivery), application);
    
}

void close_button_delivery(GtkWidget */*widget*/, ezgl::application *application)
{
    GtkWidget * dialog = (GtkWidget *) application -> get_object("DeliveryWindow");
    gtk_widget_hide_on_delete(dialog);

    clean_map(application);
}


void delivery_entry(GtkWidget */*widget*/, ezgl::application *application)
{
    stringstream ss;
    const char* print;
    int intersectionID;
    
    string text; 
    GtkLabel * label;
    //Input in the necessary places
    if(set_depots_show)
    {
        if(numDepotsShow > 0){
            label = (GtkLabel*) application->get_object("ChosenDepots");
            
            text = gtk_entry_get_text(DeliveryTextGlobal);
            ss<<text;
            ss>>intersectionID;
            
            //Starts creating the vector of depots
            depotsShow.push_back(intersectionID);
            
            allDepotsShow = allDepotsShow + text + "\t" + getIntersectionName(intersectionID) + "\n"; 
            
            print = allDepotsShow.c_str();
            gtk_label_set_text(label, print);
            numDepotsShow--;
        }
        else set_depots_show = false;
    }
    else if(set_deliveries_show)
    {
        if(numDeliveriesShow>0){
            if(set_pickups_show){
                
                label = (GtkLabel*) application->get_object("PickUps");
                
                text = gtk_entry_get_text(DeliveryTextGlobal);
                ss<<text;
                ss>>pickUpShow;

                //Starts creating the vector of depots
                allPickUpsShow += text + "\t" + getIntersectionName(pickUpShow) + "\n"; 

                print = allPickUpsShow.c_str();
                gtk_label_set_text(label, print);
                
                //Setting up next
                set_pickups_show = false;
                set_dropoffs_show = true;
                
//                label = (GtkLabel*) application->get_object("DeliveryIndex");
//                ss<<indexShow;
//                cout<<indexShow<<endl;
//                ss>>indexString;
//                cout<<"String: "<<indexString<<endl;
//                allIndexShow = allIndexShow + indexString + "\n";
//                print = allIndexShow.c_str();
//                gtk_label_set_text(label, print);
//                indexShow++;
            }
            else if(set_dropoffs_show){
                
                
                label = (GtkLabel*) application->get_object("DropOffs");
                
                text = gtk_entry_get_text(DeliveryTextGlobal);
                ss<<text;
                ss>>dropOffShow;

                //Starts creating the vector of depots
                allDropOffsShow += text + "\t" + getIntersectionName(dropOffShow) + "\n"; 

                print = allDropOffsShow.c_str();
                gtk_label_set_text(label, print);
                
                //Setting up next
                set_dropoffs_show = false;
                set_weight_show = true;
                
            }else if(set_weight_show){
                
                label = (GtkLabel*) application->get_object("Weight");
                
                text = gtk_entry_get_text(DeliveryTextGlobal);
                ss<<text;
                ss>>weightShow;

                //Starts creating the vector of depots
                allWeightShow += text + "\n";

                print = allWeightShow.c_str();
                gtk_label_set_text(label, print);
                
                //Make deliveryInfo variable
                DeliveryInfo delivery(pickUpShow, dropOffShow, weightShow);
                deliveriesShow.push_back(delivery);
                
                //Setting up next
                set_weight_show = false;
                numDeliveriesShow--;
                if(numDeliveriesShow >0 ){
                    set_pickups_show = true;
                }
            }
        }
    }
    else{
        text = gtk_entry_get_text(TruckCapacityGlobal);
        ss<<text;
        ss>>truckCapacityShow;
        
        text = gtk_entry_get_text(TurnPenaltyDeliveryGlobal);
        ss<<text;
        ss>>turnPenaltyShow;
    }
}

//Starts the recording of depots, and writes number of depots to be inputted in a label beside the entry bar
void num_depots_entry(GtkWidget */*widget*/, ezgl::application *application)
{
    //clear it before writing into it
    depotsShow.clear();
    allDepotsShow = "";
    set_depots_show = true;
    
    GtkEntry* numDepotsEntry = (GtkEntry *) application->get_object("NumDepotsEntry");
    string numDepotsText = gtk_entry_get_text(numDepotsEntry);
    stringstream ss(numDepotsText);
    ss>>numDepotsShow;
    GtkLabel* numDepotsLabel = (GtkLabel*) application->get_object("NumDepots");
    const char* print = numDepotsText.c_str();
    gtk_label_set_text(numDepotsLabel, print);
}

//Starts the recording of deliveries and writes number of deliveries to be inputted in a label beside the entry bar
void num_delivery_entry(GtkWidget */*widget*/, ezgl::application *application){
    
    deliveriesShow.clear();
    allPickUpsShow = "";
    allDropOffsShow = "";
    allWeightShow = "";
    allIndexShow = "";
    indexShow = 1;
    set_pickups_show = true;
    set_deliveries_show = true;
    
    cout<<"Here"<<endl;
    
    GtkEntry* numDeliveriesEntry = (GtkEntry *) application->get_object("NumDeliveriesEntry");
    string numDeliveriesText = gtk_entry_get_text(numDeliveriesEntry);
    
    stringstream ss(numDeliveriesText);
    ss>>numDeliveriesShow;
    
    GtkLabel* numDeliveriesLabel = (GtkLabel*) application->get_object("NumDeliveries");
    const char* print = numDeliveriesText.c_str();
    gtk_label_set_text(numDeliveriesLabel, print);
}

void depots_done(GtkComboBox /**widget*/, ezgl::application * /*application*/)
{
    set_depots_show = false;
}

void pickups_done(GtkWidget */*widget*/,ezgl::application */*application*/)
{
    set_pickups_show = false;
    set_dropoffs_show = false;
    set_weight_show = false;
    set_deliveries_show = false;
}

void make_deliveries(GtkWidget */*widget*/,ezgl::application */*application*/)
{
   //subpathShow = traveling_courier(deliveriesShow, depotsShow, turnPenaltyShow, truckCapacityShow);
    cout<<"Making deliveries now"<<endl;
}

