
#pragma once

#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"

#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"

#include <map>
#include <list>
#include <unordered_map>
#include <math.h>
#include <cctype>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

#include "m3.h"
#include "m2.h"

/*
 * pass in 2 street segment ids.
 * return true is both streets belong to the same street,
 * false if not
 */
bool there_is_turn(int from_seg_id, int to_seg_id);
//bool bfs_find_walk_path(Node* sourceNode, int destID, double turn_penalty);