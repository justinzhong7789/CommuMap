#include "zoom.hpp"

double max_lat;
double min_lat;
double max_lon;
double min_lon;

rectangle full_map;
rectangle full_screen;


ezgl::color GRASS(192,211,192);
ezgl::color WATER(182,196,201);
ezgl::color HIGHWAY(131,133,134);
ezgl::color OUTLINE(197,197,197);

//ezgl::color GRASS.;
//ezgl::color BACKGROUND;
//ezgl::color WATER;
//ezgl::color HIGHWAY;
//ezgl::color OUTLINE;

std::vector <intersectionData> intersections;

void zoom(ezgl::renderer *g){

    rectangle current_map = g->get_visible_world();
    rectangle current_screen = g->get_visible_screen();

    double zoom_map = ((full_map.m_second.x)-(full_map.m_first.x))/((current_map.m_second.x)-(current_map.m_first.x));
    double zoom_screen = ((full_screen.m_second.x)-(full_screen.m_first.x))/((current_screen.m_second.x)-(current_screen.m_first.x));
    
    double zoom_level = zoom_screen;
    //check when zoom is close to 0
    int width = ceil(zoom_level*pow(2,1/zoom_level));
    
    
    cout<< "Zoom Map: "<< zoom_map << endl;
    cout<< "Zoom Screen: "<< zoom_screen << endl;
    cout<< "width: " << width << endl;
    cout<< "Zoom Level: "<< zoom_level << endl << endl;
    
    if (20 < zoom_level){
        cout << "all"<< endl;
        drawAllStreets(g, 1);
    }
    if (7 < zoom_level ){
        cout << "Local" << endl;
        drawStreets(streetsizes.local, g, 1 );//1
    }
    if (4 < zoom_level){ 
        cout << "Minor" << endl;
       drawStreets(streetsizes.minor, g, 2);//2
    }
    if ( 2 < zoom_level){
        cout << "Major" << endl;
        drawStreets(streetsizes.major, g, 3);//3
    }
    if ( 0.1 < zoom_level){
        cout << "Highway" << endl;
        //CURRENTLY NO SPECIFIC DRAWING FOR HIGHWAY

        drawStreets(streetsizes.highway, g, 5);//5
        if (6 < zoom_level){
            drawStreetNames(streetsizes.highway, g, 6);
        }
        if (10 < zoom_level){
            drawStreetNames(streetsizes.major, g, 6);
        }
        if (15 < zoom_level){
            drawStreetNames(streetsizes.minor, g, 6);
        }
        if (25 < zoom_level){
            drawStreetNames(streetsizes.local, g, 6);
        }

        drawStreets(streetsizes.highway, g, width);//5

        drawStreets(streetsizes.highway, g, 5);//5
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
            g->set_color(ezgl::BLACK);
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
            g->set_color(ezgl::GREY_55);
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

