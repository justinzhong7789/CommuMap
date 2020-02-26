#include "features.hpp"

//distance in meters

double x_between_2_points(LatLon first, LatLon second) {
    double LatAvg = (first.lat() + second.lat()) * DEGREE_TO_RADIAN / 2;
    double x1 = first.lon() * DEGREE_TO_RADIAN * cos(LatAvg);
    double x2 = second.lon() * DEGREE_TO_RADIAN * cos(LatAvg);
    return EARTH_RADIUS_METERS * (x2 - x1);
}



// Find the distance in the y component of two points using the formula provided in M1 Instructions

double y_between_2_points(LatLon first, LatLon second) {
    double y1 = first.lat() * DEGREE_TO_RADIAN;
    double y2 = second.lat() * DEGREE_TO_RADIAN;
    return EARTH_RADIUS_METERS * (y2 - y1);
}

void drawFeatures(std::vector<int> feature, ezgl::renderer *g) {
    //584158 features in toronto map
    //this loop draws all the features
    
    g->set_line_width(1);
    
    
    
    for (FeatureIndex i = 0; i < feature.size() ; i++) {
        
         ezgl::color featureColour = getFeatureColour(feature[i]);
        g->set_color(featureColour);
       
        //this condition checks for closed feature
        if (getFeaturePoint(0, i).lat() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lat() &&
                getFeaturePoint(0, i).lon() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lon()) {

            //use the vector to draw
            if (pointsOfFeatures[i].size() > 1) {
                g->fill_poly(pointsOfFeatures[i]);
            }
        } else {//open feature
            //open features are lines
            for (int k = 0; k + 1 < getFeaturePointCount(i); k++) {

                double start_x = x_from_lon(getFeaturePoint(k, i).lon());
                double start_y = y_from_lat(getFeaturePoint(k, i).lat());
                double end_x = x_from_lon(getFeaturePoint(k + 1, i).lon());
                double end_y = y_from_lat(getFeaturePoint(k + 1, i).lat());

                ezgl::point2d start_point(start_x, start_y);
                ezgl::point2d end_point(end_x, end_y);

                g->set_line_dash(ezgl::line_dash::none);
                g->draw_line(start_point, end_point);
            }
        }
        
        
    }
}

ezgl::color getFeatureColour(int i){
    
    auto type = getFeatureType(i);
    ezgl::color featureColour;
    
     if (type == Unknown || type == Building) {
         featureColour = BUILDINGS;
    } else if (type == Park || type == Golfcourse || type == Greenspace) {
        featureColour = GRASS;
    } else if (type == Beach) {
        featureColour = SAND;
    } else if (type == Lake || type == River || type == Stream) {
        featureColour = WATER;
    } else if (type == Island) {
        featureColour = BACKGROUND;
    }
    
    return featureColour;
}





//void drawBuildings(std::vector<int> features, ezgl::renderer *g ){
//    
//    g->set_line_width(1);
//    
//    for (int i = 0; i< features.size() ; i++){
//        
//          if (getFeatureType(i) == Unknown) {
//            g->set_color(BUILDINGS);
//        } else if (getFeatureType(i) == Park) {
//            g->set_color(GRASS);
//        } else if (getFeatureType(i) == Beach) {
//            g->set_color(GRASS);
//        } else if (getFeatureType(i) == Lake) {
//            g->set_color(WATER);
//        } else if (getFeatureType(i) == River) {
//            g->set_color(WATER);
//        } else if (getFeatureType(i) == Island) {
//            g->set_color(GRASS);
//        } else if (getFeatureType(i) == Building) {
//            g->set_color(BUILDINGS);
//        } else if (getFeatureType(i) == Greenspace) {
//            g->set_color(GRASS);
//        } else if (getFeatureType(i) == Golfcourse) {
//            g->set_color(GRASS);
//        } else if (getFeatureType(i) == Stream) {
//            g->set_color(WATER);
//        }
//        
//        
//        if (getFeaturePoint(0, i).lat() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lat() &&
//                    getFeaturePoint(0, i).lon() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lon()) {
//
//                std::vector<ezgl::point2d> points;
//                //put x-y coords of points that make up a closed feature into a vector
//                for (int j = 0; j < getFeaturePointCount(i); j++) {
//                    double x_coords = (double) x_from_lon(getFeaturePoint(j, i).lon());
//                    double y_coords = (double) y_from_lat(getFeaturePoint(j, i).lat());
//                    ezgl::point2d pointIn2D(x_coords, y_coords);
//                    points.push_back(pointIn2D);
//                }
//                //use the vector to draw
//                if (points.size() > 1) {
//                    g->fill_poly(points);
//                }
//            } else {//open feature
//                //open features are lines
//                for (int k = 0; k + 1 < getFeaturePointCount(i); k++) {
//                    double start_x = x_from_lon(getFeaturePoint(k, i).lon());
//                    double start_y = y_from_lat(getFeaturePoint(k, i).lat());
//                    double end_x = x_from_lon(getFeaturePoint(k + 1, i).lon());
//                    double end_y = y_from_lat(getFeaturePoint(k + 1, i).lat());
//                    ezgl::point2d start_point(start_x, start_y);
//                    ezgl::point2d end_point(end_x, end_y);
//                    g->set_line_dash(ezgl::line_dash::none);
//                    g->draw_line(start_point, end_point);
//                }
//            }
//    }
//}

