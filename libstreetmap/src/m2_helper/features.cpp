#include "features.hpp"

//distance in meters
// Find the distance in the y component of two points using the formula provided in M1 Instructions
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

//When each feature will be drawn depending on the zoom level
void zoomFeatures(ezgl::renderer *g){
     
    switch (zooms.zcase){
        case 0:
            drawFeatures(featuretypes.unknownFeatures, g,BUILDINGS);
        case 1:
            
        case 2:
            drawFeatures(featuretypes.buildings, g, BUILDINGS);
            drawFeatures(featuretypes.streams, g, WATER);    
        case 3:
            drawFeatures(featuretypes.greenspaces, g, WATER);
        case 4:
            drawFeatures(featuretypes.golfcourses, g, GRASS);
            drawFeatures(featuretypes.beaches, g, SAND);
        case 5:
            drawFeatures(featuretypes.bigparks, g, GRASS);

            drawFeatures(featuretypes.lakes, g, WATER);
            drawFeatures(featuretypes.islands, g, BACKGROUND);
            break;
        default:
            drawFeatures(featuretypes.unknownFeatures, g, BUILDINGS);
            drawFeatures(featuretypes.buildings, g, BUILDINGS);
            drawFeatures(featuretypes.streams, g, WATER); 
            drawFeatures(featuretypes.golfcourses, g, GRASS);
            drawFeatures(featuretypes.beaches, g, SAND);
            drawFeatures(featuretypes.islands, g, BACKGROUND);
            drawFeatures(featuretypes.lakes, g, WATER);
            drawFeatures(featuretypes.rivers, g, WATER);
            drawFeatures(featuretypes.bigparks, g, GRASS);
            break;
    }
}

void drawFeatures(std::vector<int> feature, ezgl::renderer *g, ezgl::color colour) {
    
//    rectangle current_map = g->get_visible_world();
    g->set_line_width(1);
    
    for (FeatureIndex i = 0; i < feature.size() ; i++) {
        
        int feat = feature[i];
        
        //ezgl::color featureColour = getFeatureColour(feat);
        g->set_color(colour);

        //Closed features
        if (getFeaturePoint(0, feat).lat() == getFeaturePoint(getFeaturePointCount(feat) - 1, feat).lat() &&
            getFeaturePoint(0, feat).lon() == getFeaturePoint(getFeaturePointCount(feat) - 1, feat).lon()) {

            std::vector<ezgl::point2d> points;
            //put x-y coords of points that make up a closed feature into a vector
            for (int j = 0; j < getFeaturePointCount(feat); j++) {
                LatLon coor =  getFeaturePoint(j, feat);
                ezgl::point2d pointIn2D(point2d_from_latlon(coor));
                points.push_back(pointIn2D);
            }

            if (points.size() > 1) {
                g->fill_poly(points);
            }
            
        //Open features are lines
        } else {
            
            for (int k = 0; k + 1 < getFeaturePointCount(feat); k++) {

                LatLon coorStart = getFeaturePoint(k, feat);
                LatLon coorEnd = getFeaturePoint(k + 1, feat);

                ezgl::point2d start_point(point2d_from_latlon(coorStart));
                ezgl::point2d end_point(point2d_from_latlon(coorEnd));

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

void drawFeatureNames(std::vector<int> vec, ezgl::renderer *g){
    
    for (FeatureIndex i = 0; i < vec.size(); i++){ 
        int feat = vec[i];
        ezgl::rectangle featSize = findHighLowPoint(feat);
        
        if ((find_feature_area(feat) > 0.01* zooms.mapArea) && getFeatureName(feat)!="<noname>" ){
            g->set_text_rotation(0);
            g->set_color(FEATURE_NAMES);
            g->draw_text({(featSize.m_first.x + featSize.m_second.x)/2, (featSize.m_first.y + featSize.m_second.y)/2}, getFeatureName(feat));
        }
    }
}

//When to draw the names 
void nameFeatures(ezgl::renderer *g){
    switch (zooms.zcase){
        case 0:
            drawFeatureNames(featuretypes.unknownFeatures, g);
        case 1:
        case 2:
            drawFeatureNames(featuretypes.buildings, g);
            drawFeatureNames(featuretypes.streams, g);    
        case 3:
            drawFeatureNames(featuretypes.greenspaces, g);
        case 4:
            drawFeatureNames(featuretypes.golfcourses, g);
            drawFeatureNames(featuretypes.beaches, g);
        case 5:
            drawFeatureNames(featuretypes.bigparks, g);

            drawFeatureNames(featuretypes.lakes, g);
            drawFeatureNames(featuretypes.islands, g);
            break;
        default:
            break;
    }
}

ezgl::rectangle findHighLowPoint(FeatureIndex feat){
    int numPoints = getFeaturePointCount(feat);
    double high_lat = -999999;
    double low_lat = 9999999;
    double high_lon = -9999999;
    double low_lon = 9999999;
    for (int i =0; i <numPoints; i++){ 
        high_lat = std::max(high_lat,getFeaturePoint(i, feat).lat() );
        low_lat = std::min(low_lat, getFeaturePoint(i, feat).lat());
        high_lon = std::max(high_lon, getFeaturePoint(i, feat).lon());
        low_lon = std::min(low_lon, getFeaturePoint(i, feat).lon());
    }
    ezgl::rectangle rect({x_from_lon(low_lon), low_lat},{x_from_lon(high_lon),high_lat});
    return rect;
}