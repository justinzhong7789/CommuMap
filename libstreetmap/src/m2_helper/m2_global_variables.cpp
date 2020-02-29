#include "m2_global_variables.hpp"

// Define big streets as streets with length 1+ km
////std::vector <streetData> bigStreetsTable;

//std::vector<LatLon> add_nodes(StreetSegmentIndex id);

std::vector <StreetSegmentsData> streetSegments;

std::vector<std::vector<ezgl::point2d>> pointsOfFeatures;
//std::vector<std::vector<ezgl::point2d>> pointsOfFeatures;

StreetSize streetsizes;
FeatureClass featuretypes;


ezgl::point2d point2d_from_latlon(LatLon number){
    point2d point (x_from_lon(number.lon()),number.lat());
    return point;
}

LatLon latlon_from_point2d(ezgl::point2d point){
    
    LatLon number((float) lon_from_x(point.x),(float) point.y);
    return number;
}

double findArea(double x1, double y1, double x2, double y2){
    
    double xdiff = x2-x1;
    double ydiff = y2-y1;
    return xdiff*ydiff;
}

// add all street names length together
void makeStreetSizeTable(){
    StreetData street_data;
    StreetSize streets;
    StreetSegmentsData street_segment_data;

    
    //Starts at 1 so it doesn't include the <unknown> street
    for (StreetIndex i=0; i<getNumStreets(); i++){
        double street_length = 0;
        street_data.segments.clear();
        
        for (int j=0; j<segmentsOfStreets[i].size(); j++){
            StreetSegmentIndex id = segmentsOfStreets[i][j];
            //street_segment_data.id = id;
            street_length += find_street_segment_length(id);
            street_segment_data.node = add_nodes(id);
            street_segment_data.oneWay = getInfoStreetSegment(id).oneWay;
            
            
            street_segment_data.fromPos = getIntersectionPosition(getInfoStreetSegment(id).from);
            street_segment_data.toPos= getIntersectionPosition(getInfoStreetSegment(id).to);
            
            point2d fromPoint(point2d_from_latlon(street_segment_data.fromPos));
            point2d toPoint(point2d_from_latlon(street_segment_data.fromPos));
            
            rectangle rectSeg(fromPoint,toPoint);
            street_segment_data.midpoint = latlon_from_point2d(rectSeg.center());
            
            street_data.segments.push_back(street_segment_data);
        }
        
        
        street_data.id = i;
        street_data.name = getStreetName(i);
        street_data.length = street_length;
        
        
        if( i == 0){
            streetsizes.local.push_back(street_data);
        }else{
        
            float speed = getInfoStreetSegment(segmentsOfStreets[i][0]).speedLimit; 

    //        //Identifying highways
            if(120 > speed && speed > 80 && street_length > 15000) {
                streetsizes.highway.push_back(street_data);
            }
            else if (80 >= speed && speed >= 60 && street_length > 2000){          
                streetsizes.major.push_back(street_data);

            }
            else if (60 >= speed && speed >= 50 && street_length > 500){
                streetsizes.minor.push_back(street_data);
            }
            else if ( street_length > 80 && street_length > 100){
                streetsizes.local.push_back(street_data);
            }
        }
    }
}

std::vector<LatLon> add_nodes(StreetSegmentIndex id){
    InfoStreetSegment info = getInfoStreetSegment(id);
    std::vector<LatLon> node_list;
    node_list.resize(info.curvePointCount+2);
    // Find LatLon of beginning of intersection
    node_list[0] = getIntersectionPosition(info.from);
    // If no curve points, find end of intersection
    if (info.curvePointCount == 0){
            node_list[1] = getIntersectionPosition(info.to);
    }
    else {
        for (int i=1; i<=info.curvePointCount+1; i++){
            if (i>info.curvePointCount){
                node_list[i] = getIntersectionPosition(info.to);
            } else {
                node_list[i] = getStreetSegmentCurvePoint(i-1, id);
            }
        }
    }
    return node_list;
}

void makeStreetsVector(){
    
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

void makePointsOfFeatures(){
    
    for(int i = 0; i<getNumFeatures(); i++){
        for(int j = 0; j<getFeaturePointCount(i); j++){
            double x_coords = (double) x_from_lon(getFeaturePoint(j, i).lon());
            double y_coords = (double) y_from_lat(getFeaturePoint(j, i).lat());
            ezgl::point2d pointIn2D(x_coords, y_coords);
            pointsOfFeatures[i].push_back(pointIn2D);
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


void sortFeatures() {
    int numFeatures = getNumFeatures();

    
  //  vector<float> check;
    for (FeatureIndex i = 0; i < numFeatures; i++) {
        
//        for(int j = 0; j<getFeaturePointCount(i); j++){
//            float x_coords = (float) x_from_lon(getFeaturePoint(j, i).lon());
//            float y_coords = (float) y_from_lat(getFeaturePoint(j, i).lat());
//            ezgl::point2d pointIn2D(x_coords, y_coords);
//            pointsOfFeatures[j].push_back(pointIn2D);
//            cout<<"help me"<<endl;
//        }
       
//        
        FeatureType type = getFeatureType(i);
        FeatureType type2;
        //info featInfo;
        int featInfo;
        featInfo = i;
        //featInfo.ID = i;
       // featInfo.name = getFeatureName(i);
        
        int typeCase = 10;
       
        
        for(int k = 0 ; k<10 ; k++){
            type2 = featureTypeList[k];
            if(type == type2){
                typeCase = k;
                break;
            }
        }
        
        switch(typeCase){
            case 0:
                featuretypes.unknownFeatures.push_back(featInfo);
                //std::cout<<"Im here"<<endl;
                break;
            case 1:
                featuretypes.bigparks.push_back(featInfo);
                break;    
            case 2:
                featuretypes.beaches.push_back(featInfo);
                break;
            case 3:
                featuretypes.lakes.push_back(featInfo);
                break;
            case 4:
                featuretypes.rivers.push_back(featInfo);
                break;
            case 5:
                featuretypes.islands.push_back(featInfo);
                break;
            case 6:
                featuretypes.buildings.push_back(featInfo);
                break;
            case 7:
                featuretypes.greenspaces.push_back(featInfo);
                break;
            case 8:
                featuretypes.golfcourses.push_back(featInfo);
                break;
            case 9:
                featuretypes.streams.push_back(featInfo);
                break;
            default:
                break;
        }
}
}