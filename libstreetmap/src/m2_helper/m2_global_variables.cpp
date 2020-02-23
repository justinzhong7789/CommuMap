#include "m2_global_variables.hpp"

// Define big streets as streets with length 1+ km
////std::vector <streetData> bigStreetsTable;

//std::vector<LatLon> add_nodes(StreetSegmentIndex id);

std::vector <streetSegmentsData> streetSegments;

StreetSize streetsizes;

FeatureSize featuresizes;

// add all street names length together
void makeStreetSizeTable(){
    StreetData street_data;
    StreetSize streets;
    streetSegmentsData street_segment_data;

    
    //Starts at 1 so it doesn't include the <unknown> street
    for (StreetIndex i=1; i<getNumStreets(); i++){
        double street_length = 0;
        street_data.segments.clear();
        
        for (int j=0; j<segmentsOfStreets[i].size(); j++){
            StreetSegmentIndex id = segmentsOfStreets[i][j];
            street_segment_data.id = id;
            
            street_length += find_street_segment_length(id);
            street_segment_data.node = add_nodes(id);
            street_data.segments.push_back(street_segment_data);
        }
        street_data.name = getStreetName(i);
        street_data.length = street_length;
        
        
        float speed = getInfoStreetSegment(segmentsOfStreets[i][0]).speedLimit; 

//        //Identifying highways
        if(120 > speed && speed > 80 && street_length > 1000) {
            streetsizes.highway.push_back(street_data);
        }
        else if (street_length > 5000){          
            streetsizes.major.push_back(street_data);
            
        }
        else if (50 == speed && street_length > 500){
            streetsizes.minor.push_back(street_data);
        }
        else if (40 >= speed && street_length < 500 && street_length > 100 ){
            streetsizes.local.push_back(street_data);
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
//
//void sortFeatures() {
//    int numFeatures = getNumFeatures();
//    //584158 features in toronto map
//    //this loop draws all the features
//    
//
//    for (FeatureIndex i = 0; i < numFeatures; i++) {
//
//        //cout << "The feature area of "<<i<<" is: "<<find_feature_area(i)<< endl;
////        if (find_feature_area(i) > 5000) {
////            featuresizes.eight.push_back(i);
////        }
////        else if (find_feature_area(i) > 3000) {
////            featuresizes.seven.push_back(i);
////        }
////        else if (find_feature_area(i) > 2000) {
////            featuresizes.six.push_back(i);
////        }
////        else if (find_feature_area(i) > 1000) {
////            featuresizes.five.push_back(i);
////        }
////        else if (find_feature_area(i) > 500) {
////            featuresizes.four.push_back(i);
////        }
////        else if (find_feature_area(i) > 400) {
////            featuresizes.three.push_back(i);
////        }
////        else if (find_feature_area(i) > 300) {
////            featuresizes.two.push_back(i);
////        }
////        else {
////            featuresizes.one.push_back(i);
////        }
////    }
// 
//        if (getFeatureType(i) == Park) {
//            featuresizes.two.push_back(i);
//            featuresizes.three.push_back(i);
//            featuresizes.four.push_back(i);
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == Beach) {
//            featuresizes.three.push_back(i);
//              featuresizes.four.push_back(i);
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == Lake) {
//            featuresizes.one.push_back(i);
//            featuresizes.two.push_back(i);
//            featuresizes.three.push_back(i);
//            featuresizes.four.push_back(i);
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == River) {
//            featuresizes.four.push_back(i);
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == Island) {
//            featuresizes.one.push_back(i);
//                featuresizes.two.push_back(i);
//            featuresizes.three.push_back(i);
//            featuresizes.four.push_back(i);
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == Building) {
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == Greenspace) {
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == Golfcourse) {
//            featuresizes.two.push_back(i);
//            featuresizes.three.push_back(i);
//            featuresizes.four.push_back(i);
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//            featuresizes.eight.push_back(i);
//        } else if (getFeatureType(i) == Stream) {
//            featuresizes.three.push_back(i);
//               featuresizes.four.push_back(i);
//            featuresizes.five.push_back(i);
//            featuresizes.six.push_back(i);
//            featuresizes.seven.push_back(i);
//        }
//        else{
//            featuresizes.eight.push_back(i);
//        }
//    }
//    
//}

