#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_with_walk_to_pickup_legality_public) {
struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(4);
        rand_intersection = std::uniform_int_distribution<IntersectionIndex>(0, getNumIntersections()-1);
        rand_street = std::uniform_int_distribution<StreetIndex>(1, getNumStreets()-1);
        rand_segment = std::uniform_int_distribution<StreetSegmentIndex>(0, getNumStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<POIIndex>(0, getNumPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<double>(46.650653839, 46.711387634);
        rand_lon = std::uniform_real_distribution<double>(7.800767422, 7.915423393);
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
    TEST(find_path_with_walk_to_pick_up_legality) {
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;

        path = find_path_with_walk_to_pick_up(109, 1086, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(109, 1086, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(246, 247, 21.50123978084875986, 1.39999999999999991, 86.47451443886683364);
        CHECK(path_is_legal(246, 247, 21.50123978084875986, 1.39999999999999991, 86.47451443886683364, path));

        path = find_path_with_walk_to_pick_up(549, 550, 23.24724699156175944, 1.39999999999999991, 3.71509002689571588);
        CHECK(path_is_legal(549, 550, 23.24724699156175944, 1.39999999999999991, 3.71509002689571588, path));

        path = find_path_with_walk_to_pick_up(657, 443, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(657, 443, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(674, 85, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(674, 85, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(743, 1039, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(743, 1039, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(1660, 1589, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(1660, 1589, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(1457, 902, 21.45775725742914375, 1.39999999999999991, 64.21662972540707415);
        CHECK(path_is_legal(1457, 902, 21.45775725742914375, 1.39999999999999991, 64.21662972540707415, path));

        path = find_path_with_walk_to_pick_up(1097, 1123, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(1097, 1123, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(1804, 1392, 17.77525196070857660, 4.02200638279444611, 9.77595335217901429);
        CHECK(path_is_legal(1804, 1392, 17.77525196070857660, 4.02200638279444611, 9.77595335217901429, path));

        path = find_path_with_walk_to_pick_up(488, 1147, 17.46513984523502216, 4.65033172131618766, 270.99044672713131376);
        CHECK(path_is_legal(488, 1147, 17.46513984523502216, 4.65033172131618766, 270.99044672713131376, path));

        path = find_path_with_walk_to_pick_up(587, 1657, 16.39187402126722048, 1.39999999999999991, 81.16568401011527101);
        CHECK(path_is_legal(587, 1657, 16.39187402126722048, 1.39999999999999991, 81.16568401011527101, path));

        path = find_path_with_walk_to_pick_up(643, 939, 15.72791721664213505, 1.38253705856749232, 225.80856471219388482);
        CHECK(path_is_legal(643, 939, 15.72791721664213505, 1.38253705856749232, 225.80856471219388482, path));

        path = find_path_with_walk_to_pick_up(938, 1292, 19.73220619629803707, 4.90769176516309535, 56.81368682599703135);
        CHECK(path_is_legal(938, 1292, 19.73220619629803707, 4.90769176516309535, 56.81368682599703135, path));

        path = find_path_with_walk_to_pick_up(1013, 1551, 20.58452655600576264, 3.07903788007991785, 299.07740911374781945);
        CHECK(path_is_legal(1013, 1551, 20.58452655600576264, 3.07903788007991785, 299.07740911374781945, path));

        path = find_path_with_walk_to_pick_up(352, 1054, 24.69242195727185418, 3.75261393716713698, 41.02582674660070694);
        CHECK(path_is_legal(352, 1054, 24.69242195727185418, 3.75261393716713698, 41.02582674660070694, path));

        path = find_path_with_walk_to_pick_up(1021, 1345, 18.12727336456592298, 3.22876315025370975, 257.46238926275731274);
        CHECK(path_is_legal(1021, 1345, 18.12727336456592298, 3.22876315025370975, 257.46238926275731274, path));

        path = find_path_with_walk_to_pick_up(854, 598, 22.60801995389195795, 2.41711713925994420, 184.62927526498486941);
        CHECK(path_is_legal(854, 598, 22.60801995389195795, 2.41711713925994420, 184.62927526498486941, path));

        path = find_path_with_walk_to_pick_up(1143, 1132, 15.01240003546097590, 4.53142618213733783, 291.11655022631100564);
        CHECK(path_is_legal(1143, 1132, 15.01240003546097590, 4.53142618213733783, 291.11655022631100564, path));

        path = find_path_with_walk_to_pick_up(1182, 714, 15.22913018357345294, 4.89882578064772467, 238.51367069200875903);
        CHECK(path_is_legal(1182, 714, 15.22913018357345294, 4.89882578064772467, 238.51367069200875903, path));

        path = find_path_with_walk_to_pick_up(1222, 267, 19.76185147836938683, 0.87654608249330834, 199.18584321802339332);
        CHECK(path_is_legal(1222, 267, 19.76185147836938683, 0.87654608249330834, 199.18584321802339332, path));

        path = find_path_with_walk_to_pick_up(1354, 1037, 15.29998809213639177, 2.65722607645164643, 235.80455158579394492);
        CHECK(path_is_legal(1354, 1037, 15.29998809213639177, 2.65722607645164643, 235.80455158579394492, path));

        path = find_path_with_walk_to_pick_up(298, 290, 19.66787855447711664, 1.39999999999999991, 59.96731169105495241);
        CHECK(path_is_legal(298, 290, 19.66787855447711664, 1.39999999999999991, 59.96731169105495241, path));

        path = find_path_with_walk_to_pick_up(1504, 820, 20.69849316530155647, 4.42294530024311872, 267.74348821123129483);
        CHECK(path_is_legal(1504, 820, 20.69849316530155647, 4.42294530024311872, 267.74348821123129483, path));

        path = find_path_with_walk_to_pick_up(1522, 1059, 22.93556533270106712, 4.27519230673719264, 218.71411202115464789);
        CHECK(path_is_legal(1522, 1059, 22.93556533270106712, 4.27519230673719264, 218.71411202115464789, path));

        path = find_path_with_walk_to_pick_up(512, 490, 18.22304086160225722, 3.38119942653586047, 211.19064763823004682);
        CHECK(path_is_legal(512, 490, 18.22304086160225722, 3.38119942653586047, 211.19064763823004682, path));

        path = find_path_with_walk_to_pick_up(1701, 446, 23.14467005467510319, 1.63106581700814646, 87.04559981341979835);
        CHECK(path_is_legal(1701, 446, 23.14467005467510319, 1.63106581700814646, 87.04559981341979835, path));

        path = find_path_with_walk_to_pick_up(368, 1167, 17.16173232004141624, 3.74668330935078853, 239.74146350894687885);
        CHECK(path_is_legal(368, 1167, 17.16173232004141624, 3.74668330935078853, 239.74146350894687885, path));

        path = find_path_with_walk_to_pick_up(1779, 198, 23.46132436906174945, 1.39999999999999991, 85.99428697461463855);
        CHECK(path_is_legal(1779, 198, 23.46132436906174945, 1.39999999999999991, 85.99428697461463855, path));

        path = find_path_with_walk_to_pick_up(1820, 1625, 21.77376355368367200, 1.67080190107441151, 187.07500683930976493);
        CHECK(path_is_legal(1820, 1625, 21.77376355368367200, 1.67080190107441151, 187.07500683930976493, path));

    } //find_path_with_walk_to_pick_up_legality

} //path_with_walk_to_pickup_legality_public

