#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(path_with_walk_to_pickup_optimality_public) {
struct MapFixture {
    MapFixture() {
        rng = std::minstd_rand(4);
        rand_intersection = std::uniform_int_distribution<IntersectionIndex>(0, getNumIntersections()-1);
        rand_street = std::uniform_int_distribution<StreetIndex>(1, getNumStreets()-1);
        rand_segment = std::uniform_int_distribution<StreetSegmentIndex>(0, getNumStreetSegments()-1);
        rand_poi = std::uniform_int_distribution<POIIndex>(0, getNumPointsOfInterest()-1);
        rand_lat = std::uniform_real_distribution<double>(43.479999542, 43.919982910);
        rand_lon = std::uniform_real_distribution<double>(-79.789985657, -79.000000000);
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
    TEST(find_path_with_walk_to_pick_up_optimality) {
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;
        path = find_path_with_walk_to_pick_up(9088, 89818, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(9088, 89818, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 22.58759087598575732) <= 1761.93791051506400436);

        path = find_path_with_walk_to_pick_up(29171, 87192, 24.69242195727185418, 3.75261393716713698, 41.02582674660070694);
        CHECK(path_is_legal(29171, 87192, 24.69242195727185418, 3.75261393716713698, 41.02582674660070694, path));
        CHECK(compute_path_travel_time(path.second, 24.69242195727185418) <= 722.11612203662070897);

        path = find_path_with_walk_to_pick_up(54341, 36647, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(54341, 36647, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.74049806129059093) <= 1280.44753098342744124);

        path = find_path_with_walk_to_pick_up(55780, 7107, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(55780, 7107, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 17.38266612778162568) <= 757.27942027851383955);

        path = find_path_with_walk_to_pick_up(58939, 14931, 15.37633603142348804, 1.39999999999999991, 193.73271772287435510);
        CHECK(path_is_legal(58939, 14931, 15.37633603142348804, 1.39999999999999991, 193.73271772287435510, path));
        CHECK(compute_path_travel_time(path.second, 15.37633603142348804) <= 28.41103305705777515);

        path = find_path_with_walk_to_pick_up(61505, 85936, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(61505, 85936, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 18.40129796266718643) <= 1597.39043840076851666);

        path = find_path_with_walk_to_pick_up(90722, 92846, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(90722, 92846, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 22.04203310234851898) <= 1932.34234068334785661);

        path = find_path_with_walk_to_pick_up(137276, 131450, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(137276, 131450, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.80338191831117456) <= 2085.65133368438500838);

        path = find_path_with_walk_to_pick_up(40382, 94892, 17.46513984523502216, 4.65033172131618766, 270.99044672713131376);
        CHECK(path_is_legal(40382, 94892, 17.46513984523502216, 4.65033172131618766, 270.99044672713131376, path));
        CHECK(compute_path_travel_time(path.second, 17.46513984523502216) <= 2409.43487745407719558);

        path = find_path_with_walk_to_pick_up(42351, 40527, 18.22304086160225722, 3.38119942653586047, 211.19064763823004682);
        CHECK(path_is_legal(42351, 40527, 18.22304086160225722, 3.38119942653586047, 211.19064763823004682, path));
        CHECK(compute_path_travel_time(path.second, 18.22304086160225722) <= 1085.15507298325792362);

        path = find_path_with_walk_to_pick_up(48535, 50773, 18.40832381866542988, 1.39999999999999991, 81.16568401011527101);
        CHECK(path_is_legal(48535, 50773, 18.40832381866542988, 1.39999999999999991, 81.16568401011527101, path));
        CHECK(compute_path_travel_time(path.second, 18.40832381866542988) <= 6.44800023936008060);

        path = find_path_with_walk_to_pick_up(53214, 77703, 15.72791721664213505, 1.38253705856749232, 225.80856471219388482);
        CHECK(path_is_legal(53214, 77703, 15.72791721664213505, 1.38253705856749232, 225.80856471219388482, path));
        CHECK(compute_path_travel_time(path.second, 15.72791721664213505) <= 2041.41610950623066856);

        path = find_path_with_walk_to_pick_up(55591, 55588, 19.55163847051831993, 1.39999999999999991, 62.71106131057546662);
        CHECK(path_is_legal(55591, 55588, 19.55163847051831993, 1.39999999999999991, 62.71106131057546662, path));
        CHECK(compute_path_travel_time(path.second, 19.55163847051831993) <= 5.83406778664087167);

        path = find_path_with_walk_to_pick_up(30434, 96508, 17.16173232004141624, 3.74668330935078853, 239.74146350894687885);
        CHECK(path_is_legal(30434, 96508, 17.16173232004141624, 3.74668330935078853, 239.74146350894687885, path));
        CHECK(compute_path_travel_time(path.second, 17.16173232004141624) <= 1792.44647332072690915);

        path = find_path_with_walk_to_pick_up(70669, 49477, 22.60801995389195795, 2.41711713925994420, 184.62927526498486941);
        CHECK(path_is_legal(70669, 49477, 22.60801995389195795, 2.41711713925994420, 184.62927526498486941, path));
        CHECK(compute_path_travel_time(path.second, 22.60801995389195795) <= 2545.48570376817406213);

        path = find_path_with_walk_to_pick_up(77616, 106827, 19.73220619629803707, 4.90769176516309535, 56.81368682599703135);
        CHECK(path_is_legal(77616, 106827, 19.73220619629803707, 4.90769176516309535, 56.81368682599703135, path));
        CHECK(compute_path_travel_time(path.second, 19.73220619629803707) <= 2245.80071655071333225);

        path = find_path_with_walk_to_pick_up(83752, 128282, 20.58452655600576264, 3.07903788007991785, 299.07740911374781945);
        CHECK(path_is_legal(83752, 128282, 20.58452655600576264, 3.07903788007991785, 299.07740911374781945, path));
        CHECK(compute_path_travel_time(path.second, 20.58452655600576264) <= 1634.55976038561061614);

        path = find_path_with_walk_to_pick_up(84449, 111216, 18.12727336456592298, 3.22876315025370975, 257.46238926275731274);
        CHECK(path_is_legal(84449, 111216, 18.12727336456592298, 3.22876315025370975, 257.46238926275731274, path));
        CHECK(compute_path_travel_time(path.second, 18.12727336456592298) <= 1787.47578763171895844);

        path = find_path_with_walk_to_pick_up(89542, 14846, 22.92007129296922940, 1.39999999999999991, 51.15375519018232353);
        CHECK(path_is_legal(89542, 14846, 22.92007129296922940, 1.39999999999999991, 51.15375519018232353, path));
        CHECK(compute_path_travel_time(path.second, 22.92007129296922940) <= 1.24706053744370760);

        path = find_path_with_walk_to_pick_up(94563, 93624, 15.01240003546097590, 4.53142618213733783, 291.11655022631100564);
        CHECK(path_is_legal(94563, 93624, 15.01240003546097590, 4.53142618213733783, 291.11655022631100564, path));
        CHECK(compute_path_travel_time(path.second, 15.01240003546097590) <= 439.91585490490774646);

        path = find_path_with_walk_to_pick_up(26717, 108459, 23.46132436906174945, 1.39999999999999991, 197.57349445421218093);
        CHECK(path_is_legal(26717, 108459, 23.46132436906174945, 1.39999999999999991, 197.57349445421218093, path));
        CHECK(compute_path_travel_time(path.second, 23.46132436906174945) <= 8.78719983983949859);

        path = find_path_with_walk_to_pick_up(97802, 59077, 15.22913018357345294, 4.89882578064772467, 238.51367069200875903);
        CHECK(path_is_legal(97802, 59077, 15.22913018357345294, 4.89882578064772467, 238.51367069200875903, path));
        CHECK(compute_path_travel_time(path.second, 15.22913018357345294) <= 1601.32212793391295236);

        path = find_path_with_walk_to_pick_up(101057, 22145, 19.76185147836938683, 0.87654608249330834, 199.18584321802339332);
        CHECK(path_is_legal(101057, 22145, 19.76185147836938683, 0.87654608249330834, 199.18584321802339332, path));
        CHECK(compute_path_travel_time(path.second, 19.76185147836938683) <= 478.72082772015386354);

        path = find_path_with_walk_to_pick_up(112018, 85743, 15.29998809213639177, 2.65722607645164643, 235.80455158579394492);
        CHECK(path_is_legal(112018, 85743, 15.29998809213639177, 2.65722607645164643, 235.80455158579394492, path));
        CHECK(compute_path_travel_time(path.second, 15.29998809213639177) <= 1757.40161857244765997);

        path = find_path_with_walk_to_pick_up(125869, 87624, 22.93556533270106712, 4.27519230673719264, 218.71411202115464789);
        CHECK(path_is_legal(125869, 87624, 22.93556533270106712, 4.27519230673719264, 218.71411202115464789, path));
        CHECK(compute_path_travel_time(path.second, 22.93556533270106712) <= 3408.71781831599582802);

        path = find_path_with_walk_to_pick_up(91799, 46931, 19.37938493793083694, 1.39999999999999991, 238.63497263933146542);
        CHECK(path_is_legal(91799, 46931, 19.37938493793083694, 1.39999999999999991, 238.63497263933146542, path));
        CHECK(compute_path_travel_time(path.second, 19.37938493793083694) <= 20.00155966006685304);

        path = find_path_with_walk_to_pick_up(140662, 36923, 23.14467005467510319, 1.63106581700814646, 87.04559981341979835);
        CHECK(path_is_legal(140662, 36923, 23.14467005467510319, 1.63106581700814646, 87.04559981341979835, path));
        CHECK(compute_path_travel_time(path.second, 23.14467005467510319) <= 2725.93111248112518297);

        path = find_path_with_walk_to_pick_up(149206, 115090, 17.77525196070857660, 4.02200638279444611, 9.77595335217901429);
        CHECK(path_is_legal(149206, 115090, 17.77525196070857660, 4.02200638279444611, 9.77595335217901429, path));
        CHECK(compute_path_travel_time(path.second, 17.77525196070857660) <= 756.78848066312889387);

        path = find_path_with_walk_to_pick_up(150526, 134373, 21.77376355368367200, 1.67080190107441151, 187.07500683930976493);
        CHECK(path_is_legal(150526, 134373, 21.77376355368367200, 1.67080190107441151, 187.07500683930976493, path));
        CHECK(compute_path_travel_time(path.second, 21.77376355368367200) <= 1068.98786382012826834);

        path = find_path_with_walk_to_pick_up(124360, 67797, 20.69849316530155647, 4.42294530024311872, 267.74348821123129483);
        CHECK(path_is_legal(124360, 67797, 20.69849316530155647, 4.42294530024311872, 267.74348821123129483, path));
        CHECK(compute_path_travel_time(path.second, 20.69849316530155647) <= 1424.21020846237615842);

    } //find_path_with_walk_to_pick_up_optimality

} //path_with_walk_to_pickup_optimality_public

