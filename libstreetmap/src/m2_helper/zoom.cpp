#include "zoom.hpp"

rectangle full_map;
rectangle full_screen;

std::vector <intersectionData> intersections;

Zoom zooms;

void zoom(ezgl::renderer *g){
    zooms.current = g->get_visible_world();
    rectangle current_screen = g->get_visible_screen();
    
    //Finding Area
    LatLon first(zooms.current.m_first.x, zooms.current.m_first.y);
    LatLon second(zooms.current.m_second.x, zooms.current.m_second.y);
    double mapX = x_distance_between_2_points(first, second);
    double mapY = y_distance_between_2_points(first,second);
    
    //Level Calculations for percentage of area shown on map compared to original
    zooms.mapArea = mapX*mapY;
    zooms.map = area_full_map/zooms.current.area();
    zooms.screen = current_screen.area()/ area_full_screen;
    zooms.level = zooms.screen*zooms.map*100;
    zooms.zcase = zoomArraySize;
    
    //Adjustments for window screen size
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
    
 //   cout<<"The zoom level: "<< zooms.level << endl ;
    
     g->set_color(BACKGROUND);
     g->fill_rectangle(zooms.current.m_first, zooms.current.m_second);
     
}

void zoomStreets(ezgl::renderer *g){
    
    int width = 6; //Used to be 10
    
    switch (zooms.zcase){
        case 0:
            drawStreets(streetsizes.local, g , width*(zooms.level/ZOOM_ZERO), STREETS);
            break;
        case 1: //empty case for future purposes
        case 2:
        case 3:
            drawStreets(streetsizes.local, g , width-5, STREETS);
            
        case 4:
            drawStreets(streetsizes.minor, g , width-3, STREETS);
            drawStreets(streetsizes.major, g , width, STREETS);
            
        case 5:
            drawStreets(streetsizes.highway, g, width, ezgl::GREY_75);
            break;
            
        default: 
            drawStreets(streetsizes.highway, g, width, STREETS);
            break;
    }
}

void drawStreets(vector<StreetData> streets, ezgl::renderer *g, int width, ezgl::color colour ){
    
    for (int i=0; i<streets.size(); i++){
        // Iterate through every street segment in the street
        for (int j=0; j<streets[i].segments.size(); j++){
            // Iterate through every node in the street segment
            for (int k=1; k<streets[i].segments[j].node.size(); k++){
                std::pair <float, float> start = {x_from_lon(streets[i].segments[j].node[k-1].lon()), y_from_lat((streets[i].segments)[j].node[k-1].lat())};
                std::pair <float, float> end = {x_from_lon(streets[i].segments[j].node[k].lon()), y_from_lat(streets[i].segments[j].node[k].lat())};
                
                g->set_line_dash(ezgl::line_dash::none);
                
                //Draw Outline
                
                g->set_line_cap(ezgl::line_cap::round);
                
                //Draw Fill Colour
                g->set_color(colour);
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
            
            g->set_line_cap(ezgl::line_cap::round);
            
            g->set_line_width(width);
            g->set_color(STREETS);
            g->set_line_dash(ezgl::line_dash::none);
            g->draw_line({start.first, start.second}, {end.first, end.second});
            
//            g->set_line_cap(ezgl::line_cap::butt);
        }
    }
}


void drawOneWay(ezgl::renderer *g){
    int angle = 0;
    std::string arrow = "   ->   ";
    for( int i = 0; i<getNumStreetSegments(); i++){
        int seg = i;
         
        auto info = getInfoStreetSegment(seg);
        if(info.oneWay){
           
            std::pair <float, float> start = {x_from_lon((getIntersectionPosition(info.from).lon())), y_from_lat(getIntersectionPosition(info.from).lat())};
            std::pair <float, float> end = {x_from_lon((getIntersectionPosition(info.to).lon())), y_from_lat(getIntersectionPosition(info.to).lat())};

            std::pair <float, float> center = {(end.first+start.first)/2, (end.second+start.second)/2};
            if((end.first - start.first) != 0.0){
                angle = atan((end.second - start.second)/(end.first - start.first))*180/M_PI;
            }
            
              
            double length;
            double textWidth = g->textWidth({center.first,center.second}, arrow);
            
            if((angle != 0)||(angle != 180) ||(angle != 360)){
            length = (getIntersectionPosition(info.to).lat()- getIntersectionPosition(info.from).lat())/sin(angle);
            }
                    
            if((textWidth<length)&& (length > 0.0007)){
            
                g->set_font_size(10);
                g->set_color(ONE_WAY);
                g->set_text_rotation(angle);
                g->draw_text({center.first, center.second}, arrow);
            }
        }
    }
}

void nameStreets(ezgl::renderer *g){
    int width = 10;
    switch (zooms.zcase){
        case 0:
        case 1: //empty case for future purposes
        case 2:
        case 3:
            drawStreetNames(streetsizes.local, g , width,10);
        case 4:
           // drawStreetNames(streetsizes.local, g , width,10);
            drawStreetNames(streetsizes.minor, g , width,40);
            drawStreetNames(streetsizes.major, g , width,60);
            
        case 5:
            drawStreetNames(streetsizes.highway, g, width,100);
            if(zooms.zcase <= 3) drawOneWay(g);
            break;
            
        default: 
            drawStreetNames(streetsizes.highway, g, width, 100);
            break;
    }
}


void drawStreetNames(vector<StreetData> streets, renderer *g, int font_size, int /*distance*/){
    

    
    for (size_t i=0; i<streets.size(); i++){
        int drawn = 0;
        string street_name = streets[i].name;
       
        int j, next;
        for (j=0, next = 1; (j<streets[i].segments.size()) && (drawn <1) &&( next<streets[i].segments.size()); j++, next = next+2){//used to be 10
            if(street_name == "<unknown>"){
                break;
            }
            
            StreetSegmentsData segData = streets[i].segments[j];
            //ASSUMING THAT THE SEGDATA IS IN ORDER
            
            StreetSegmentsData segDataNext = streets[i].segments[next];
            
            int angle = 0;
            int from = getInfoStreetSegment(segData.id).from;
            int to = getInfoStreetSegment(segData.id).to;

            std::pair <float, float> start = {x_from_lon((getIntersectionPosition(from).lon())), y_from_lat(getIntersectionPosition(from).lat())};
            std::pair <float, float> end = {x_from_lon((getIntersectionPosition(to).lon())), y_from_lat(getIntersectionPosition(to).lat())};

            if((end.first - start.first) != 0.0){
                angle = atan((end.second - start.second)/(end.first - start.first))*180/M_PI;
            }
            auto lonM = segData.midpoint.lon(); //already in point 2d
            auto latM = segData.midpoint.lat();
            point2d segmentMid(lonM, latM);
            
            double length;
            double textWidth = g->textWidth(segmentMid, street_name);
            
            if((angle != 0)||(angle != 180) ||(angle != 360)){
                length = (segDataNext.toPos.lat() - segData.fromPos.lat())/sin(angle);
            }
                    
            if(textWidth>length){
                
                drawSegmentName(segData, g, font_size, street_name, angle, segmentMid);
                drawn++;
            }
        }
    }
}

//////////////////

//void drawStreetNames(ezgl::renderer *g) {
//    if (map.currentXYDiagonal < CURRENT_XY_DIAGONAL_STREET_NAMES) {
//        g->format_font("sans serif", ezgl::font_slant::normal, ezgl::font_weight::normal);
//        g->set_font_size(10);
//        g->set_color(STREET_NAMES);
//
//        for (int streetIdx = 1; streetIdx < getNumStreets(); ++streetIdx) {
//            std::vector<StreetSegmentIndex> streetSegmentsOfStreet = find_street_segments_of_street(streetIdx);
//
//            for (int streetSegIdx = 0; streetSegIdx < streetSegmentsOfStreet.size(); ++streetSegIdx) {
//                drawStreetNameOnStreetSegment(g, streetIdx, streetSegmentsOfStreet[streetSegIdx]);
//            }
//        }
//    }
//}
//
//void drawStreetNameOnStreetSegment(ezgl::renderer *g, StreetIndex streetIdx, StreetSegmentIndex streetSegIdx) {
//    double streetSegLength = find_street_segment_length(streetSegIdx);
//
//    if (streetSegLength > map.currentXYDiagonal / 18.0) {
//        InfoStreetSegment segInfo = getInfoStreetSegment(streetSegIdx);
//
//        LatLon fromLatLon = getIntersectionPosition(segInfo.from);
//        LatLon toLatLon = getIntersectionPosition(segInfo.to);
//
//        if (withinCurrentMapLatLon(fromLatLon) || withinCurrentMapLatLon(toLatLon)) {
//            std::string name = getStreetName(streetIdx);
//
//            ezgl::point2d from = point2dFromLatLon(fromLatLon);
//            ezgl::point2d to = point2dFromLatLon(toLatLon);
//
//            double xDistance = from.x - to.x;
//            double yDistance = from.y - to.y;
//
//            double textRotationAngle = atan(yDistance / xDistance) * RADIAN_TO_DEGREE;
//           
////            g->set_text_rotation(textRotationAngle);
//            g->set_text_rotation(0);
//
//            ezgl::point2d textLocation = {(from.x + to.x) / 2, (from.y + to.y) / 2};
//            g->draw_text(textLocation, name);
//        }
//    }
//}

/////////////////



void drawSegmentName(StreetSegmentsData segData, renderer *g, int font_size, string street_name, int angle, point2d segmentMid){

    
    
    if ((segData.fromPos.lat() != segData.toPos.lat() )&& (segData.fromPos.lon() != segData.toPos.lon())){
        
        g->set_font_size(font_size);
        g->set_color(STREET_NAMES);
        g->set_text_rotation(angle);
        g->draw_text(segmentMid, street_name);
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

void highlightStreet(ezgl::renderer* g, int street_id){
    vector<int> street_seg_ids = find_street_segments_of_street(street_id);
    g->set_color(ezgl::YELLOW);
    g->set_line_width(12);
    g->draw_line({x_from_lon(min_lon), y_from_lat(min_lat)},{x_from_lon(max_lon), y_from_lat(max_lat)});
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
