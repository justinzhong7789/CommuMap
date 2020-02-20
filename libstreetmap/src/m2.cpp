/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "m1.h"
#include <string>
#include <iostream>
#include "m2.h"
#include "m2_helper/zoom.hpp"
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

using namespace std;
using namespace ezgl; 

void draw_main_canvas(ezgl::renderer *g);

void makeFeaturesVector();
void drawFeatures(ezgl::renderer *g);


void draw_map(){
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
    makeSegments_OfStreets();    
    makeStreetSizeTable();

    ezgl::rectangle initial_world({x_from_lon(min_lon),y_from_lat(min_lat)}, {x_from_lon(max_lon),y_from_lat(max_lat)});

    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    

    application.run(nullptr, nullptr, nullptr, nullptr);
}

void draw_main_canvas(ezgl::renderer *g){
    
    g->draw_rectangle({x_from_lon(min_lon), y_from_lat(min_lat)}, {x_from_lon(max_lon),y_from_lat(max_lat)});
    for (size_t i=0; i<intersections.size(); i++){
        
        float x = x_from_lon(intersections[i].position.lon());
        float y = y_from_lat(intersections[i].position.lat());
        
        float width = 0.001;
        float height = width;
        
        g->fill_rectangle({x,y}, {x+width, y+height});
    }

//    
//    for (size_t i=0; i<streetSegments.size(); i++){
//        for (size_t j=1; j<streetSegments[i].node.size(); j++){
//            std::pair <float, float> start = {x_from_lon(streetSegments[i].node[j-1].lon()), y_from_lat(streetSegments[i].node[j-1].lat())};
//            std::pair <float, float> end = {x_from_lon(streetSegments[i].node[j].lon()), y_from_lat(streetSegments[i].node[j].lat())};
//            g->set_color(ezgl::BLACK);
//            g->set_line_dash(ezgl::line_dash::none);
//            g->draw_line({start.first, start.second}, {end.first, end.second});
//        }
//    }
    drawFeatures(g);

    //draw the diagonal line across the 
    zoom(g);
}

/*
void draw_streets(){
    streetSegments.resize(getNumStreetSegments());
    for (StreetSegmentIndex id=0; id<streetSegments.size(); id++){
        InfoStreetSegment info = getInfoStreetSegment(id);
        streetSegments[id].node.resize(info.curvePointCount + 2);
        // Find LatLon of beginning of intersection
        streetSegments[id].node[0] = getIntersectionPosition(info.from);
        // If no curve points, find LatLon of end of intersection
        if (info.curvePointCount == 0){
            streetSegments[id].node[1] = getIntersectionPosition(info.to);
            // Connect the begin and end to form a street 
        }
        // Find all LatLons of each curve point
        else {
            for (int i=1; i<=info.curvePointCount+1; i++){
                if (i>info.curvePointCount){
                    streetSegments[id].node[i] = getIntersectionPosition(info.to);
                } else {
                streetSegments[id].node[i] = getStreetSegmentCurvePoint(i-1, id);
                }
            }
        }
    }
}
*/

void drawFeatures(ezgl::renderer *g){
    int numFeatures=getNumFeatures();
    //584158 features in toronto map
    //this loop draws all the features
    
    //fault at 5625
    for(FeatureIndex i=0;i<numFeatures;i++){
        if(getFeatureType(i) == Unknown){g->set_color(ezgl::BLACK);}
        else if(getFeatureType(i) == Park){g->set_color(ezgl::SADDLE_BROWN);}
        else if(getFeatureType(i)== Beach){g->set_color(ezgl::BISQUE);}
        else if(getFeatureType(i)== Lake){g->set_color(ezgl::BLUE);}
        else if(getFeatureType(i)== River){g->set_color(ezgl::BLUE);}
        else if(getFeatureType(i) == Island){g->set_color(ezgl::KHAKI);}
        else if(getFeatureType(i) == Building){g->set_color(ezgl::GREY_55);}
        else if(getFeatureType(i) == Greenspace){g->set_color(ezgl::GREEN);}
        else if(getFeatureType(i) == Golfcourse){g->set_color(ezgl::TURQUOISE);}
        else if(getFeatureType(i) == Stream){g->set_color(ezgl::BLUE);}
        //this condition checks for closed feature
        // closed features are polygons
        if(getFeaturePoint(0,i).lat() == getFeaturePoint(getFeaturePointCount(i)-1,i).lat() &&
           getFeaturePoint(0,i).lon() == getFeaturePoint(getFeaturePointCount(i)-1,i).lon() )  {
            
            std::vector<ezgl::point2d> points;
            //put x-y coords of points that make up a closed feature into a vector
            for(int j=0;j< getFeaturePointCount(i);j++){  
                double x_coords = (double) x_from_lon(getFeaturePoint(j,i).lon());
                double y_coords = (double) y_from_lat(getFeaturePoint(j,i).lat());
                ezgl::point2d pointIn2D(x_coords, y_coords);
                points.push_back(pointIn2D);
            }
            //use the vector to draw
            if(points.size()>1){
                g->fill_poly(points);
            }
        }
        else{//open feature
            //open features are lines
            for(int k=0;k+1<getFeaturePointCount(i);k++){
                double start_x = x_from_lon(getFeaturePoint(k,i).lon());
                double start_y = y_from_lat(getFeaturePoint(k,i).lat());
                double end_x = x_from_lon(getFeaturePoint(k+1,i).lon());
                double end_y = y_from_lat(getFeaturePoint(k+1,i).lat());                
                ezgl::point2d start_point(start_x, start_y);
                ezgl::point2d end_point(end_x, end_y);
                g->set_line_dash(ezgl::line_dash::none);
                g->draw_line(start_point,end_point);
            }
        }
    }
}