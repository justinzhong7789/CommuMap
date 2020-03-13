#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_with_walk_to_pickup_perf_medium_public) {
struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(6);
        rand_intersection = std::uniform_int_distribution<IntersectionIndex>(0, getNumIntersections()-1);
        rand_street = std::uniform_int_distribution<StreetIndex>(1, getNumStreets()-1);
        rand_segment = std::uniform_int_distribution<StreetSegmentIndex>(0, getNumStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<POIIndex>(0, getNumPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<double>(51.280006409, 51.699996948);
        rand_lon = std::uniform_real_distribution<double>(-0.599989712, 0.419978589);
        rand_turn_penalty = std::uniform_real_distribution<double>(0., 30.);
        rand_walking_speed = std::uniform_real_distribution<double>(0.8, 5);
        rand_walking_time_limit = std::uniform_real_distribution<double>(0.0, 300);
    }

    std::minstd_rand rng;
    std::uniform_int_distribution<IntersectionIndex> rand_intersection;
    std::uniform_int_distribution<StreetSegmentIndex> rand_street;
    std::uniform_int_distribution<StreetSegmentIndex> rand_segment;
    std::uniform_int_distribution<POIIndex> rand_poi;
    std::uniform_real_distribution<double> rand_lat;
    std::uniform_real_distribution<double> rand_lon;
    std::uniform_real_distribution<double> rand_turn_penalty;
    std::uniform_real_distribution<double> rand_walking_speed;
    std::uniform_real_distribution<double> rand_walking_time_limit;
};
    TEST_FIXTURE(MapFixture, find_path_with_walk_to_pick_up_perf_medium) {
        //Verify Functionality
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;
        path = find_path_with_walk_to_pick_up(68434, 65486, 22.03968825460766823, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(68434, 65486, 22.03968825460766823, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 22.03968825460766823) <= 2798.39885243821890981);

        path = find_path_with_walk_to_pick_up(82912, 229014, 20.34600716370009010, 1.97175785393010061, 35.43952806306187142);
        CHECK(path_is_legal(82912, 229014, 20.34600716370009010, 1.97175785393010061, 35.43952806306187142, path));
        CHECK(compute_path_travel_time(path.second, 20.34600716370009010) <= 1712.62293281778534038);

        path = find_path_with_walk_to_pick_up(123587, 37648, 15.28342392282279150, 1.39999999999999991, 107.47982700690523927);
        CHECK(path_is_legal(123587, 37648, 15.28342392282279150, 1.39999999999999991, 107.47982700690523927, path));
        CHECK(compute_path_travel_time(path.second, 15.28342392282279150) <= 30.23830590306116051);

        path = find_path_with_walk_to_pick_up(136459, 179710, 23.58207964209191232, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(136459, 179710, 23.58207964209191232, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 23.58207964209191232) <= 4132.20573820808294840);

        path = find_path_with_walk_to_pick_up(162105, 141762, 18.12727336456592298, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(162105, 141762, 18.12727336456592298, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 18.12727336456592298) <= 977.65240912006959206);

        path = find_path_with_walk_to_pick_up(163296, 35783, 21.63952810726744502, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(163296, 35783, 21.63952810726744502, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 21.63952810726744502) <= 1982.71622314894807459);

        path = find_path_with_walk_to_pick_up(195669, 152715, 23.34796063664652976, 0.83387156804978357, 155.32803237317455114);
        CHECK(path_is_legal(195669, 152715, 23.34796063664652976, 0.83387156804978357, 155.32803237317455114, path));
        CHECK(compute_path_travel_time(path.second, 23.34796063664652976) <= 2963.15233586415934042);

        path = find_path_with_walk_to_pick_up(200350, 80452, 22.25185801364911597, 0.86717086695056966, 34.72157557019876606);
        CHECK(path_is_legal(200350, 80452, 22.25185801364911597, 0.86717086695056966, 34.72157557019876606, path));
        CHECK(compute_path_travel_time(path.second, 22.25185801364911597) <= 2398.40644034802471651);

        path = find_path_with_walk_to_pick_up(228788, 4467, 19.76185147836938683, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(228788, 4467, 19.76185147836938683, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 19.76185147836938683) <= 4280.64558587007650203);

        path = find_path_with_walk_to_pick_up(230574, 150660, 18.22304086160225722, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(230574, 150660, 18.22304086160225722, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 18.22304086160225722) <= 2842.09354876509723908);

        path = find_path_with_walk_to_pick_up(2761, 2763, 16.38699299658926734, 1.39999999999999991, 48.28180620021811364);
        CHECK(path_is_legal(2761, 2763, 16.38699299658926734, 1.39999999999999991, 48.28180620021811364, path));
        CHECK(compute_path_travel_time(path.second, 16.38699299658926734) <= 5.88460733166481287);

        path = find_path_with_walk_to_pick_up(13220, 54027, 24.21035745579012044, 3.44390952690365726, 160.26426178017953816);
        CHECK(path_is_legal(13220, 54027, 24.21035745579012044, 3.44390952690365726, 160.26426178017953816, path));
        CHECK(compute_path_travel_time(path.second, 24.21035745579012044) <= 1543.67024288888114825);

        path = find_path_with_walk_to_pick_up(21294, 232768, 18.42042950963095649, 3.58339357959991833, 47.71863144714528460);
        CHECK(path_is_legal(21294, 232768, 18.42042950963095649, 3.58339357959991833, 47.71863144714528460, path));
        CHECK(compute_path_travel_time(path.second, 18.42042950963095649) <= 4489.78356161917145073);

        path = find_path_with_walk_to_pick_up(22173, 26171, 19.32077848575661960, 3.34082782117972688, 124.89542660425773590);
        CHECK(path_is_legal(22173, 26171, 19.32077848575661960, 3.34082782117972688, 124.89542660425773590, path));
        CHECK(compute_path_travel_time(path.second, 19.32077848575661960) <= 2150.68348734856635929);

        path = find_path_with_walk_to_pick_up(26426, 122785, 23.03881532165408075, 2.58370725189438843, 281.57967187387822605);
        CHECK(path_is_legal(26426, 122785, 23.03881532165408075, 2.58370725189438843, 281.57967187387822605, path));
        CHECK(compute_path_travel_time(path.second, 23.03881532165408075) <= 837.35220488781305903);

        path = find_path_with_walk_to_pick_up(27187, 91395, 23.67510436632049675, 1.05869936068625070, 177.68501314906845323);
        CHECK(path_is_legal(27187, 91395, 23.67510436632049675, 1.05869936068625070, 177.68501314906845323, path));
        CHECK(compute_path_travel_time(path.second, 23.67510436632049675) <= 1063.86253882325559061);

        path = find_path_with_walk_to_pick_up(106582, 180323, 22.00646888533273682, 2.29633103575633690, 97.88134551527502936);
        CHECK(path_is_legal(106582, 180323, 22.00646888533273682, 2.29633103575633690, 97.88134551527502936, path));
        CHECK(compute_path_travel_time(path.second, 22.00646888533273682) <= 1369.26795932807158351);

        path = find_path_with_walk_to_pick_up(141987, 97183, 19.94198246742107727, 1.39999999999999991, 87.21325420133806006);
        CHECK(path_is_legal(141987, 97183, 19.94198246742107727, 1.39999999999999991, 87.21325420133806006, path));
        CHECK(compute_path_travel_time(path.second, 19.94198246742107727) <= 8.17234033456697873);

        path = find_path_with_walk_to_pick_up(143780, 100677, 19.27152593262500702, 2.76749419536576147, 192.52917806828008906);
        CHECK(path_is_legal(143780, 100677, 19.27152593262500702, 2.76749419536576147, 192.52917806828008906, path));
        CHECK(compute_path_travel_time(path.second, 19.27152593262500702) <= 1787.58156351717821053);

        path = find_path_with_walk_to_pick_up(162901, 112777, 19.84066396180578806, 3.46774055151517313, 170.99499396088469894);
        CHECK(path_is_legal(162901, 112777, 19.84066396180578806, 3.46774055151517313, 170.99499396088469894, path));
        CHECK(compute_path_travel_time(path.second, 19.84066396180578806) <= 3684.63663397988830184);

        path = find_path_with_walk_to_pick_up(167379, 14080, 18.87124374718368713, 4.23081812903309995, 90.89732617029939377);
        CHECK(path_is_legal(167379, 14080, 18.87124374718368713, 4.23081812903309995, 90.89732617029939377, path));
        CHECK(compute_path_travel_time(path.second, 18.87124374718368713) <= 3201.23712596672885411);

        path = find_path_with_walk_to_pick_up(201727, 111927, 22.82953435589690727, 3.93366597380373673, 202.16376015952195644);
        CHECK(path_is_legal(201727, 111927, 22.82953435589690727, 3.93366597380373673, 202.16376015952195644, path));
        CHECK(compute_path_travel_time(path.second, 22.82953435589690727) <= 3508.37537170892073846);

        path = find_path_with_walk_to_pick_up(202323, 225770, 21.92677455233223327, 3.82089906429046611, 160.39335409411330602);
        CHECK(path_is_legal(202323, 225770, 21.92677455233223327, 3.82089906429046611, 160.39335409411330602, path));
        CHECK(compute_path_travel_time(path.second, 21.92677455233223327) <= 2799.47799230822693062);

        path = find_path_with_walk_to_pick_up(220163, 70950, 18.74009106118970180, 1.39999999999999991, 153.56921843142052353);
        CHECK(path_is_legal(220163, 70950, 18.74009106118970180, 1.39999999999999991, 153.56921843142052353, path));
        CHECK(compute_path_travel_time(path.second, 18.74009106118970180) <= 6.59232936763346533);

        path = find_path_with_walk_to_pick_up(223226, 184691, 24.29011830635839075, 4.81119276024805931, 297.88015941245572549);
        CHECK(path_is_legal(223226, 184691, 24.29011830635839075, 4.81119276024805931, 297.88015941245572549, path));
        CHECK(compute_path_travel_time(path.second, 24.29011830635839075) <= 1685.29950415066991809);

        path = find_path_with_walk_to_pick_up(184057, 20956, 19.67877331825037146, 3.66160132621009282, 276.71739233210843167);
        CHECK(path_is_legal(184057, 20956, 19.67877331825037146, 3.66160132621009282, 276.71739233210843167, path));
        CHECK(compute_path_travel_time(path.second, 19.67877331825037146) <= 2575.48318315516735311);

        path = find_path_with_walk_to_pick_up(237181, 163202, 15.29998809213639177, 1.39999999999999991, 20.43800182100934748);
        CHECK(path_is_legal(237181, 163202, 15.29998809213639177, 1.39999999999999991, 20.43800182100934748, path));
        CHECK(compute_path_travel_time(path.second, 15.29998809213639177) <= 0.89537821355266611);

        path = find_path_with_walk_to_pick_up(237598, 196961, 21.28518966563932580, 4.53292466824396811, 167.23065124455794717);
        CHECK(path_is_legal(237598, 196961, 21.28518966563932580, 4.53292466824396811, 167.23065124455794717, path));
        CHECK(compute_path_travel_time(path.second, 21.28518966563932580) <= 3114.55983296872955179);

        path = find_path_with_walk_to_pick_up(242350, 123431, 21.03732454890024783, 1.99869653649096168, 170.81677571948142713);
        CHECK(path_is_legal(242350, 123431, 21.03732454890024783, 1.99869653649096168, 170.81677571948142713, path));
        CHECK(compute_path_travel_time(path.second, 21.03732454890024783) <= 2563.73375855858193972);

        path = find_path_with_walk_to_pick_up(239758, 78379, 23.32528392383026627, 1.39999999999999991, 56.81368682599703135);
        CHECK(path_is_legal(239758, 78379, 23.32528392383026627, 1.39999999999999991, 56.81368682599703135, path));
        CHECK(compute_path_travel_time(path.second, 23.32528392383026627) <= 4.93849121864229534);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        std::vector<double> walking_speeds;
        std::vector<double> walking_time_limits;
        for(size_t i = 0; i < 25; i++) {
            intersection_ids1.push_back(rand_intersection(rng));
            turn_penalties.push_back(rand_turn_penalty(rng));
            walking_speeds.push_back(rand_walking_speed(rng));
            walking_time_limits.push_back(rand_walking_time_limit(rng));

           double radius = walking_speeds[i]*walking_time_limits[i];

           IntersectionIndex id2 = -1;
           double dist = 0.0;
           do {
               id2 = rand_intersection(rng);
               dist = find_distance_between_two_points(std::make_pair(getIntersectionPosition(intersection_ids1[i]),
                   getIntersectionPosition(id2)));
           } while (dist <= radius);

            intersection_ids2.push_back(id2);
        }
        {
            //Timed Test
            ECE297_TIME_CONSTRAINT(322739);
            std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> result;
            for(size_t i = 0; i < 25; i++) {
                result = find_path_with_walk_to_pick_up(intersection_ids1[i], intersection_ids2[i], turn_penalties[i],
                        walking_speeds[i], walking_time_limits[i]);
            }
        }
    } //find_path_with_walk_to_pick_up_perf_medium

} //path_with_walk_to_pickup_perf_medium_public

