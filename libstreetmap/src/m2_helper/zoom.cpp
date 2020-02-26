#include "zoom.hpp"

rectangle full_map;
rectangle full_screen;

std::vector <intersectionData> intersections;

void zoom(ezgl::renderer *g){

    rectangle current_map = g->get_visible_world();
    rectangle current_screen = g->get_visible_screen();

    double zoom_map = area_full_map/ findArea(current_map.m_first.x, current_map.m_first.y, current_map.m_second.x, current_map.m_second.y);
    double zoom_screen = findArea(current_screen.m_first.x, current_screen.m_first.y, current_screen.m_second.x, current_screen.m_second.y) / area_full_screen;
    
    double zoom_level = zoom_screen*zoom_map*100;
    //check when zoom is close to 0
    //int width = ceil(zoom_level*pow(2,1/zoom_level));
    
    double zoom_small = 0.0;
    
    int width = 12;
    
    int zoomCase = zoomArraySize;
    
    for(int i = 0; i < zoomArraySize; i++){
        
        if(zoom_screen > ZOOM_SCREEN_INIT){
            zoom_small = 0.9*pow(2,zoom_level);
            if(zoomLevel[i] < zoom_small){
                zoomCase = i;
                break;
            }
        }else{
            if(zoomLevel[i] < zoom_level){
                zoomCase = i;
                break;
            }
        }
       
    }
    
     cout << "At level: "<< zoomCase << endl;
     
     cout<< "Screen values = X1: "<< current_screen.m_first.x << "Y1: " << current_screen.m_first.y 
            << "// X2: " << current_screen.m_second.x << "Y1: " << current_screen.m_second.y << endl;
    cout<< "Zoom Map: "<< zoom_map << endl;
    cout<< "Zoom Screen: "<< zoom_screen << endl;
    cout<< "Zoom Level Full: "<< zoom_level << endl ;
    cout<< "Zoom Level Small: "<< zoom_small << endl <<endl;
     
    switch (zoomCase){
        case 0:
            drawFeatures(featuretypes.unknownFeatures, g);
            //drawAllStreets(g, width*(zoom_level/ZOOM_ZERO));
            
            break;
        case 1:
            drawFeatures(featuretypes.buildings, g);
            //drawAllStreets(g, width*(zoom_level/ZOOM_ONE));
            
        case 2:
            drawFeatures(featuretypes.streams, g);
            //drawStreets(streetsizes.local, g , width);
            
        case 3:
            //drawStreets(streetsizes.minor, g , width);
        case 4:
            drawFeatures(featuretypes.golfcourses, g);
            drawFeatures(featuretypes.beaches, g);
            //drawStreets(streetsizes.major, g , width);
        case 5:
            drawFeatures(featuretypes.islands, g);
            drawFeatures(featuretypes.lakes, g);
            drawFeatures(featuretypes.rivers, g);
            drawFeatures(featuretypes.bigparks, g);
            //drawStreets(streetsizes.highway, g, width);
            break;
        default: 
            //drawStreets(streetsizes.highway, g, width);
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
