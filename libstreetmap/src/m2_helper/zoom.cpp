#include "zoom.hpp"

rectangle full_map;
rectangle full_screen;

std::vector <intersectionData> intersections;

Zoom zooms;

void zoom(ezgl::renderer *g){
    rectangle current_map = g->get_visible_world();
    rectangle current_screen = g->get_visible_screen();
    
    zooms.map = area_full_map/ findArea(current_map.m_first.x, current_map.m_first.y, current_map.m_second.x, current_map.m_second.y);
    zooms.screen = findArea(current_screen.m_first.x, current_screen.m_first.y, current_screen.m_second.x, current_screen.m_second.y) / area_full_screen;
    
    zooms.level = zooms.screen*zooms.map*100;
    
    zooms.zcase = zoomArraySize;
    
    for(int i = 0; i < zoomArraySize; i++){
        
        if(zooms.screen > ZOOM_SCREEN_INIT){
            zooms.small = 0.9*pow(2,zooms.level);
            if(zoomLevel[i] < zooms.small){
                zooms.zcase = i;
                break;
            }
        }else{
            if(zoomLevel[i] < zooms.level){
                zooms.zcase = i;
                break;
            }
        }
       
    }
    
    cout << "At level: "<< zooms.zcase << endl;
//     
//    cout<< "Screen values = X1: "<< current_screen.m_first.x << "Y1: " << current_screen.m_first.y 
//            << "// X2: " << current_screen.m_second.x << "Y1: " << current_screen.m_second.y << endl;
//    cout<< "Zoom Map: "<< zoom_map << endl;
//    cout<< "Zoom Screen: "<< zoom_screen << endl;
    cout<< "Zoom Level Full: "<< zooms.level << endl ;
//    cout<< "Zoom Level Small: "<< zoom_small << endl <<endl;
}

void zoomStreets(ezgl::renderer *g){
    
    int width = 12;
    
    switch (zooms.zcase){
        case 0:
            drawAllStreets(g, width*(zooms.level/ZOOM_ZERO));
            break;
        case 1:
            //drawAllStreets(g, width);
        case 2:
//            drawStreets(streetsizes.local, g , width);
        case 3:
            drawStreets(streetsizes.local, g , width-5);
            //drawStreets(streetsizes.minor, g , width);
        case 4:
            drawStreets(streetsizes.minor, g , width-3);
            drawStreets(streetsizes.major, g , width);
        case 5:
            drawStreets(streetsizes.highway, g, width);
            break;
        default: 
            drawStreets(streetsizes.highway, g, width);
            break;
    }
}

void drawStreets(vector<StreetData> streets, ezgl::renderer *g, int width ){
    
    for (int i=0; i<streets.size(); i++){
        // Iterate through every street segment in the street
        for (int j=0; j<streets[i].segments.size(); j++){
            // Iterate through every node in the street segment
            for (int k=1; k<streets[i].segments[j].node.size(); k++){
                std::pair <float, float> start = {x_from_lon(streets[i].segments[j].node[k-1].lon()), y_from_lat((streets[i].segments)[j].node[k-1].lat())};
                std::pair <float, float> end = {x_from_lon(streets[i].segments[j].node[k].lon()), y_from_lat(streets[i].segments[j].node[k].lat())};
                
                g->set_line_dash(ezgl::line_dash::none);
                
                //Outline colour
                g->set_color(OUTLINE);
                g->set_line_width(width+2);
                g->draw_line({start.first, start.second}, {end.first, end.second});
                
                g->set_line_cap(ezgl::line_cap::round);
                
                //Fill colour
                g->set_color(WHITE);
                g->set_line_width(width);
                g->draw_line({start.first, start.second}, {end.first, end.second});
                
                g->set_line_cap(ezgl::line_cap::butt);
                
            }
        }
    }
}

void drawAllStreets(renderer *g, int width){
       for (size_t i=0; i<streetSegments.size(); i++){
        for (size_t j=1; j<streetSegments[i].node.size(); j++){
            std::pair <float, float> start = {x_from_lon(streetSegments[i].node[j-1].lon()), y_from_lat(streetSegments[i].node[j-1].lat())};
            std::pair <float, float> end = {x_from_lon(streetSegments[i].node[j].lon()), y_from_lat(streetSegments[i].node[j].lat())};
            
            
            
            g->set_color(OUTLINE);
            g->set_line_width(width+2);
            g->draw_line({start.first, start.second}, {end.first, end.second});
            
            g->set_line_cap(ezgl::line_cap::round);
            
            g->set_line_width(width);
            g->set_color(ezgl::WHITE);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
            
            g->set_line_cap(ezgl::line_cap::butt);
        }
    }
}

void drawStreetNames(vector<StreetData> streets, renderer *g, int font_size){
    double angle = 0;
    InfoStreetSegment info;
    // Creates a vector of all the names that will be displayed; this checks for duplicates and prevents it from drawing names more than once
    // De-clutters the map a bit
    std::vector<std::string> check_names; 
    check_names.push_back(streets[0].name);
    // Loop through every street
    for (size_t i=0; i<streets.size(); i=i+2){
        std::string street_name = streets[i].name;
        float last_position = x_from_lon(getIntersectionPosition(getInfoStreetSegment(streets[i].segments[0].id).from).lon())-1;
        
        for (size_t j=0; j<streets[i].segments.size(); j++){
            
            info = getInfoStreetSegment(streets[i].segments[j].id);
            std::pair <float, float> start = {x_from_lon((getIntersectionPosition(info.from).lon())), y_from_lat(getIntersectionPosition(info.from).lat())};
            std::pair <float, float> end = {x_from_lon((getIntersectionPosition(info.to).lon())), y_from_lat(getIntersectionPosition(info.to).lat())};

            std::pair <float, float> center = {(end.first+start.first)/2, (end.second+start.second)/2};
            if (center.first > last_position + 1){
                last_position = center.first;
            angle = atan((end.second - start.second)/(end.first - start.first))*180/M_PI;
            
            for (int k=0; k<check_names.size(); k++){
                if (check_names[k] == street_name){
                    break;
                } 
                else if (k==check_names.size()-1){
                g->set_font_size(font_size);
                g->set_color(ezgl::BLACK);
                g->set_text_rotation(angle);
            //g->draw_text({center.first, center.second}, street_name);
            //g->draw_text({start.first, start.second}, street_name, 100, 100);
                g->draw_text({center.first, center.second}, street_name);
                check_names.push_back(street_name);
                }
            }
            }
        }
    }
}

void map_bounds(){
    
    intersections.resize(getNumIntersections());
    max_lat = -999999;
    min_lat = 9999999;
    max_lon = -9999999;
    min_lon = 9999999;
    for (int id=0; id<getNumIntersections(); id++){
        intersections[id].position = getIntersectionPosition(id);
        intersections[id].name = getIntersectionName(id);
        
        max_lat = std::max(max_lat, intersections[id].position.lat());
        min_lat = std::min(min_lat, intersections[id].position.lat());
        max_lon = std::max(max_lon, intersections[id].position.lon());
        min_lon = std::min(min_lon, intersections[id].position.lon());
    }
    
}


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

void drawFeatures(ezgl::renderer *g) {
    int numFeatures = getNumFeatures();
    //584158 features in toronto map
    //this loop draws all the features
    
    g->set_line_width(1);

    rectangle current_map = g->get_visible_world();
    LatLon top_left(current_map.m_first.y, lon_from_x(current_map.m_first.x));
    LatLon top_right(current_map.m_first.y, lon_from_x(current_map.m_second.x));
    LatLon bottom_left(current_map.m_second.y, lon_from_x(current_map.m_first.x));
    double current_area = abs(x_between_2_points(top_left, top_right)) * abs(y_between_2_points(top_right, bottom_left));

    for (FeatureIndex i = 0; i < numFeatures; i++) {
        if (find_feature_area(i) > 0.001 * current_area || find_feature_area(i) == 0) {
            if(getFeatureType(i) == Building){
                featuresizes.eight.push_back(i);
            }
            else if(getFeatureType(i) == Unknown) {
                    g->set_color(BUILDINGS);
            }
            else if (getFeatureType(i) == Park) {
                g->set_color(GRASS);
            }
            else if (getFeatureType(i) == Beach) {
                g->set_color(GRASS);
            }
            else if (getFeatureType(i) == Lake) {
                g->set_color(WATER);
            }
            else if (getFeatureType(i) == River) {
                g->set_color(WATER);
            }
            else if (getFeatureType(i) == Island) {
                g->set_color(GRASS);
            }
            else if (getFeatureType(i) == Building) {
                g->set_color(BUILDINGS);
            }
            else if (getFeatureType(i) == Greenspace) {
                g->set_color(GRASS);
            }
            else if (getFeatureType(i) == Golfcourse) {
                g->set_color(GRASS);
            }
            else if (getFeatureType(i) == Stream) {
                g->set_color(WATER);
            }
            
            double min_x = x_from_lon(getFeaturePoint(0,i).lon());
            double min_y = y_from_lat(getFeaturePoint(0,i).lat());
            double max_x = x_from_lon(getFeaturePoint(0,i).lon());
            double max_y = y_from_lat(getFeaturePoint(0,i).lat());
                //this condition checks for closed feature
            if (getFeaturePoint(0, i).lat() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lat() &&
                getFeaturePoint(0, i).lon() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lon()) {

                std::vector<ezgl::point2d> points;
                //put x-y coords of points that make up a closed feature into a vector
                for (int j = 0; j < getFeaturePointCount(i); j++) {
                    double x_coords = (double) x_from_lon(getFeaturePoint(j, i).lon());
                    double y_coords = (double) y_from_lat(getFeaturePoint(j, i).lat());
                    ezgl::point2d pointIn2D(x_coords, y_coords);
                    points.push_back(pointIn2D);
                    if(x_coords > max_x){max_x = x_coords;}
                    if(x_coords < min_x){min_x = x_coords;}
                    if(y_coords > max_y){max_y = y_coords;}
                    if(y_coords < min_y){min_y = y_coords;}
                }
                    //use the vector to draw
                if (points.size() > 1) {
                    g->fill_poly(points);
                    if(getFeatureName(i)!="<noname>"){
                        g->set_color(ezgl::BLACK);
                        g->draw_text({(min_x + max_x)/2, (min_y + max_y)/2}, getFeatureName(i));
                    }
                    
                }
            } 
            else {//open feature
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
}

void drawBuildings(std::vector<int> features, ezgl::renderer *g ){
    
    g->set_line_width(1);
    
    for (int i = 0; i< features.size() ; i++){
        
          if (getFeatureType(i) == Unknown) {
            g->set_color(BUILDINGS);
        } else if (getFeatureType(i) == Park) {
            g->set_color(GRASS);
        } else if (getFeatureType(i) == Beach) {
            g->set_color(GRASS);
        } else if (getFeatureType(i) == Lake) {
            g->set_color(WATER);
        } else if (getFeatureType(i) == River) {
            g->set_color(WATER);
        } else if (getFeatureType(i) == Island) {
            g->set_color(GRASS);
        } else if (getFeatureType(i) == Building) {
            g->set_color(BUILDINGS);
        } else if (getFeatureType(i) == Greenspace) {
            g->set_color(GRASS);
        } else if (getFeatureType(i) == Golfcourse) {
            g->set_color(GRASS);
        } else if (getFeatureType(i) == Stream) {
            g->set_color(WATER);
        }
        
        
        if (getFeaturePoint(0, i).lat() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lat() &&
                    getFeaturePoint(0, i).lon() == getFeaturePoint(getFeaturePointCount(i) - 1, i).lon()) {

                std::vector<ezgl::point2d> points;
                double maxX = x_from_lon(getFeaturePoint(0,i).lon());
                double minX = x_from_lon(getFeaturePoint(0,i).lon());
                double maxY = y_from_lat(getFeaturePoint(0,i).lat());
                double minY = y_from_lat(getFeaturePoint(0,i).lat());
                //put x-y coords of points that make up a closed feature into a vector
                for (int j = 0; j < getFeaturePointCount(i); j++) {
                    double x_coords = (double) x_from_lon(getFeaturePoint(j, i).lon());
                    double y_coords = (double) y_from_lat(getFeaturePoint(j, i).lat());
                    ezgl::point2d pointIn2D(x_coords, y_coords);
                    points.push_back(pointIn2D);
                    if(x_coords > maxX){maxX = x_coords;}
                    if(x_coords < minX){minX = x_coords;}
                    if(y_coords > maxY){maxY = y_coords;}
                    if(y_coords < maxY){minY = y_coords;}
                }
                //use the vector to draw
                if (points.size() > 1) {
                    g->fill_poly(points);
                }
                if(getFeatureName(i)!="<noname>"){
                    g->set_color(ezgl::BLACK);
                    g->draw_text({(maxX + minX)/2, (maxY + minY)/2}, getFeatureName(i));
                }
            } else {//open feature
                //open features are lines
                for (int k = 0; k + 1 < getFeaturePointCount(i); k++) {
                    //this process is to connect the lines between each feature point
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

void highlightStreet(ezgl::renderer* g, int street_id){
    vector<int> street_seg_ids = find_street_segments_of_street(street_id);
    for(int i=0;i < street_seg_ids.size(); i++){
        for(int j=i+1; j< street_seg_ids.size(); j++){
            g->set_color(ezgl::YELLOW);
            g->set_line_width(12);
            if(are_directly_connected(make_pair(i,j))){    
                ezgl::point2d start(x_from_lon(getIntersectionPosition(i).lon()), y_from_lat(getIntersectionPosition(i).lat()));
                ezgl::point2d end(x_from_lon(getIntersectionPosition(j).lon()), y_from_lat(getIntersectionPosition(j).lat()));
                
                g->draw_line(start, end);
            }
        }
    }
}
