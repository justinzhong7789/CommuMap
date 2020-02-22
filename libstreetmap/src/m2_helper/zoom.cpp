#include "zoom.hpp"

double max_lat;
double min_lat;
double max_lon;
double min_lon;

rectangle full_map;
rectangle full_screen;

//ezgl::color GRASS(192,211,192);
//ezgl::color WATER(182,196,201);
//ezgl::color HIGHWAY(131,133,134);
//ezgl::color OUTLINE(197,197,197);
//ezgl::color BUILDINGS(224,224,224);

//ezgl::color GRASS.;
//ezgl::color BACKGROUND;
//ezgl::color WATER;
//ezgl::color HIGHWAY;
//ezgl::color OUTLINE;

//double area_full_map = findArea(full_map.m_first.x, full_map.m_first.y, full_map.m_second.x, full_map.m_second.y);

std::vector <intersectionData> intersections;

double findArea(double x1, double y1, double x2, double y2){
    
    double xdiff = x2-x1;
    double ydiff = y2-y1;
    return xdiff*ydiff;
}



void zoom(ezgl::renderer *g){

    rectangle current_map = g->get_visible_world();
    rectangle current_screen = g->get_visible_screen();

    double zoom_map = area_full_map/ findArea(current_map.m_first.x, current_map.m_first.y, current_map.m_second.x, current_map.m_second.y);
    double zoom_screen = findArea(current_screen.m_first.x, current_screen.m_first.y, current_screen.m_second.x, current_screen.m_second.y) / area_full_screen;
    
    double zoom_level = zoom_screen*zoom_map*100;
    //check when zoom is close to 0
    //int width = ceil(zoom_level*pow(2,1/zoom_level));
    
    
    cout<< "Zoom Map: "<< zoom_map << endl;
    cout<< "Screen values = X1: "<< current_screen.m_first.x << "Y1: " << current_screen.m_first.y 
            << "// X2: " << current_screen.m_second.x << "Y1: " << current_screen.m_second.y << endl;
    
    cout<< "Zoom Screen: "<< zoom_screen << endl;
    
    cout<< "Zoom Level: "<< zoom_level << endl << endl;
    
  
    int zoom_adjust_two = 1;
    int zoom_adjust_three = 1;
    double zoom_adjust_four = 1;
    int zoom_adjust_five = 1;
    
    if(zoom_screen > 1.5 ){
        zoom_adjust_two = 2;
        zoom_adjust_three = 3;
        zoom_adjust_four = 4.5;
        zoom_adjust_five = 5;
    }
    int width = 12;
    
    if ( ZOOM_EIGHT < zoom_adjust_two*zoom_level){
        cout << "Zoom 8 : all : buildings" << endl;
        width = ceil(zoom_adjust_two*zoom_level/103100);
        drawAllStreets(g, width);
        drawStreets(streetsizes.local, g, width );
        drawStreets(streetsizes.minor, g, width);//3
        drawStreets(streetsizes.major, g, width);//3
        drawStreets(streetsizes.highway, g, width);
    cout << "width: " << width << endl;
    }
    else if ( ZOOM_SEVEN < zoom_adjust_two*zoom_level ){
        cout << "Zoom 7 : local : playgrounds " << endl;
        //width = ceil(high_zoom_adjust*zoom_level/9170);
        drawStreets(streetsizes.local, g, width );
        drawStreets(streetsizes.minor, g, width);//3
        drawStreets(streetsizes.major, g, width);//3
        drawStreets(streetsizes.highway, g, width);
    cout << "width: " << width << endl;
    }
    else if ( ZOOM_SIX < zoom_adjust_three*zoom_level){
        cout << "Zoom 6 : ponds and rivers" << endl;
        //width = ceil(high_zoom_adjust*zoom_level/3200);
        drawStreets(streetsizes.minor, g, width);//3
        drawStreets(streetsizes.major, g, width);//3
        drawStreets(streetsizes.highway, g, width);
    cout << "width: " << width << endl;
    }
    else if ( ZOOM_FIVE < zoom_adjust_three*zoom_level){
        cout << "Zoom 5 : minor : marshes" << endl;
        //width = ceil(low_zoom_adjust*zoom_level/260);
        drawStreets(streetsizes.minor, g, width);//3
        drawStreets(streetsizes.major, g, width);//3
        drawStreets(streetsizes.highway, g, width);
    cout << "width: " << width << endl;
    }
    else if ( ZOOM_FOUR < zoom_adjust_four*zoom_level){
        cout << "Zoom 4 : parks and farms" << endl;
        //width = ceil(low_zoom_adjust*zoom_level/147);
        drawStreets(streetsizes.major, g, width);//3
        drawStreets(streetsizes.highway, g, width);
    cout << "width: " << width << endl;
    }
    else if ( ZOOM_THREE < zoom_adjust_four *zoom_level){
        cout << "Zoom 3 : major : golf club" << endl;
        //width = ceil(low_zoom_adjust*zoom_level/60);
        drawStreets(streetsizes.major, g, width);//3
        drawStreets(streetsizes.highway, g, width);
    cout << "width: " << width << endl;
    }
    else if ( ZOOM_TWO < zoom_adjust_five*zoom_level){
        cout << "Zoom 2 : big features" << endl;
        //width = ceil(zoom_level/19);
        drawStreets(streetsizes.highway, g, width);
    cout << "width: " << width << endl;
    }
    else if ( ZOOM_ONE < zoom_adjust_five * zoom_level){
        cout << "Zoom 1 : highway" << endl;
        //width = ceil(zoom_level/15);
        drawStreets(streetsizes.highway, g, width);
        //CURRENTLY NO SPECIFIC DRAWING FOR HIGHWAY
        cout << "width: " << width << endl;
//        drawStreets(streetsizes.highway, g, 5);//5
//        if (6 < zoom_level){
//            drawStreetNames(streetsizes.highway, g, 6);
//        }
//        if (10 < zoom_level){
//            drawStreetNames(streetsizes.major, g, 6);
//        }
//        if (15 < zoom_level){
//            drawStreetNames(streetsizes.minor, g, 6);
//        }
//        if (25 < zoom_level){
//            drawStreetNames(streetsizes.local, g, 6);
//        }
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
                
                //Fill colour
                g->set_color(WHITE);
                g->set_line_width(width);
                g->draw_line({start.first, start.second}, {end.first, end.second});
                
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
            
            g->set_line_width(width);
            g->set_color(ezgl::WHITE);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
        }
    }
}

void drawStreetNames(vector<StreetData> streets, renderer *g, int font_size){
    double angle = 0;
    InfoStreetSegment info;
    //const float textsquare_width = 100;
    //ezgl::rectangle textsquare = {{100, 400}, textsquare_width, textsquare_width};
    
    for (size_t i=0; i<streets.size(); i++){
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
            
            g->set_font_size(font_size);
            g->set_color(ezgl::BLACK);
            g->set_text_rotation(angle);
            //g->draw_text({center.first, center.second}, street_name);
            //g->draw_text({start.first, start.second}, street_name, 100, 100);
            g->draw_text({center.first, center.second}, street_name);
            }
        }
    }
}

float x_from_lon(float lon){
    double latAvg = (max_lat + min_lat)/2;
    double x = lon*cos(latAvg * 3.1415926535 /180);
    return x;
}

float y_from_lat(float lat){
    return lat;
}

float lon_from_x(float x){
    double latAvg = (max_lat + min_lat)/2;
    double lon = x / cos(latAvg * 3.1415926535 /180);
    return lon;
}

float lat_from_y(float y){
    return y;
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


        if (find_feature_area(i) > 0.001 * current_area || find_feature_area(i) == 0) {
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
                }
                //use the vector to draw
                if (points.size() > 1) {
                    g->fill_poly(points);
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

