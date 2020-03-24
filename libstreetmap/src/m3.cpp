/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m3_global.hpp"
#include <vector>
#include <string>
#include <iostream>

bool there_is_turn(int from_seg_id, int to_seg_id){
    int from_street_id = getInfoStreetSegment(from_seg_id).streetID;
    int to_street_id = getInfoStreetSegment(to_seg_id).streetID;
    if(from_street_id == to_street_id){return false;}
    else {return true;}
}
