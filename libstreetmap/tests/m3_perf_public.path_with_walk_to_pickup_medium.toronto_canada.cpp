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
    TEST_FIXTURE(MapFixture, find_path_with_walk_to_pick_up_perf_medium) {
        //Verify Functionality
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;
        path = find_path_with_walk_to_pick_up(42326, 42327, 22.03559771910337162, 1.39999999999999991, 35.43952806306187142);
        CHECK(path_is_legal(42326, 42327, 22.03559771910337162, 1.39999999999999991, 35.43952806306187142, path));
        CHECK(compute_path_travel_time(path.second, 22.03559771910337162) <= 10.91348456350643126);

        path = find_path_with_walk_to_pick_up(43398, 43107, 24.82604173884579524, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(43398, 43107, 24.82604173884579524, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.82604173884579524) <= 1970.49068014809199667);

        path = find_path_with_walk_to_pick_up(87192, 95462, 16.92280555852369517, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(87192, 95462, 16.92280555852369517, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 16.92280555852369517) <= 2074.54440027342616304);

        path = find_path_with_walk_to_pick_up(97731, 88004, 20.17182549212847675, 0.99465713279713786, 36.04427610336737331);
        CHECK(path_is_legal(97731, 88004, 20.17182549212847675, 0.99465713279713786, 36.04427610336737331, path));
        CHECK(compute_path_travel_time(path.second, 20.17182549212847675) <= 2165.49353481595926496);

        path = find_path_with_walk_to_pick_up(106654, 102129, 17.90023057511676186, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(106654, 102129, 17.90023057511676186, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 17.90023057511676186) <= 1660.07752300618153640);

        path = find_path_with_walk_to_pick_up(1708, 108377, 21.69284068351273831, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(1708, 108377, 21.69284068351273831, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 21.69284068351273831) <= 1528.30940514377675754);

        path = find_path_with_walk_to_pick_up(130777, 36345, 24.46383170662661399, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(130777, 36345, 24.46383170662661399, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.46383170662661399) <= 565.51688821292134435);

        path = find_path_with_walk_to_pick_up(138146, 138144, 15.01183230438437022, 1.39999999999999991, 3.70359655163481882);
        CHECK(path_is_legal(138146, 138144, 15.01183230438437022, 1.39999999999999991, 3.70359655163481882, path));
        CHECK(compute_path_travel_time(path.second, 15.01183230438437022) <= 2.40228305517055141);

        path = find_path_with_walk_to_pick_up(143933, 30702, 24.44644791758729951, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(143933, 30702, 24.44644791758729951, 1.39999999999999991, 0.00000000000000000, path));
        CHECK(compute_path_travel_time(path.second, 24.44644791758729951) <= 1735.14600500764254321);

        path = find_path_with_walk_to_pick_up(9375, 140038, 17.03745712321989103, 3.43447944824956064, 196.72098233849041549);
        CHECK(path_is_legal(9375, 140038, 17.03745712321989103, 3.43447944824956064, 196.72098233849041549, path));
        CHECK(compute_path_travel_time(path.second, 17.03745712321989103) <= 776.90958325521251027);

        path = find_path_with_walk_to_pick_up(48270, 60140, 21.67519896478087560, 3.86521214689720694, 189.60650191258073960);
        CHECK(path_is_legal(48270, 60140, 21.67519896478087560, 3.86521214689720694, 189.60650191258073960, path));
        CHECK(compute_path_travel_time(path.second, 21.67519896478087560) <= 2031.69355358072630224);

        path = find_path_with_walk_to_pick_up(65938, 13677, 24.45437769244494319, 2.77490814491235582, 41.93471005470038193);
        CHECK(path_is_legal(65938, 13677, 24.45437769244494319, 2.77490814491235582, 41.93471005470038193, path));
        CHECK(compute_path_travel_time(path.second, 24.45437769244494319) <= 557.11836129496020931);

        path = find_path_with_walk_to_pick_up(41850, 128701, 16.71504981361658437, 2.09773067571534222, 87.70654758349419922);
        CHECK(path_is_legal(41850, 128701, 16.71504981361658437, 2.09773067571534222, 87.70654758349419922, path));
        CHECK(compute_path_travel_time(path.second, 16.71504981361658437) <= 1687.55605060403036077);

        path = find_path_with_walk_to_pick_up(81264, 64439, 18.17953959676264120, 1.93533970074921680, 88.16806559754864736);
        CHECK(path_is_legal(81264, 64439, 18.17953959676264120, 1.93533970074921680, 88.16806559754864736, path));
        CHECK(compute_path_travel_time(path.second, 18.17953959676264120) <= 1949.56384768110183359);

        path = find_path_with_walk_to_pick_up(86181, 131510, 19.00171824583044966, 3.90974697206861954, 187.03508151671948667);
        CHECK(path_is_legal(86181, 131510, 19.00171824583044966, 3.90974697206861954, 187.03508151671948667, path));
        CHECK(compute_path_travel_time(path.second, 19.00171824583044966) <= 936.35903665767341408);

        path = find_path_with_walk_to_pick_up(92061, 91486, 19.89102086808226488, 4.56848485600839016, 27.53582984287076130);
        CHECK(path_is_legal(92061, 91486, 19.89102086808226488, 4.56848485600839016, 27.53582984287076130, path));
        CHECK(compute_path_travel_time(path.second, 19.89102086808226488) <= 1830.98990782271607713);

        path = find_path_with_walk_to_pick_up(111906, 146930, 16.65614495382273930, 2.75862476886342334, 151.58627932028602459);
        CHECK(path_is_legal(111906, 146930, 16.65614495382273930, 2.75862476886342334, 151.58627932028602459, path));
        CHECK(compute_path_travel_time(path.second, 16.65614495382273930) <= 1822.42648982840341887);

        path = find_path_with_walk_to_pick_up(114941, 86106, 24.30747583831690406, 2.87589492436569127, 275.95300059268697623);
        CHECK(path_is_legal(114941, 86106, 24.30747583831690406, 2.87589492436569127, 275.95300059268697623, path));
        CHECK(compute_path_travel_time(path.second, 24.30747583831690406) <= 1636.07490944073356332);

        path = find_path_with_walk_to_pick_up(115971, 48658, 23.53722743820737406, 2.71072767974519957, 146.73259977531978393);
        CHECK(path_is_legal(115971, 48658, 23.53722743820737406, 2.71072767974519957, 146.73259977531978393, path));
        CHECK(compute_path_travel_time(path.second, 23.53722743820737406) <= 789.26138511115800611);

        path = find_path_with_walk_to_pick_up(119431, 19503, 20.45841070499538716, 1.20695364002606320, 126.82637949508593067);
        CHECK(path_is_legal(119431, 19503, 20.45841070499538716, 1.20695364002606320, 126.82637949508593067, path));
        CHECK(compute_path_travel_time(path.second, 20.45841070499538716) <= 772.52489880249743237);

        path = find_path_with_walk_to_pick_up(13665, 3565, 19.41666903591372417, 1.39999999999999991, 57.91168903398340717);
        CHECK(path_is_legal(13665, 3565, 19.41666903591372417, 1.39999999999999991, 57.91168903398340717, path));
        CHECK(compute_path_travel_time(path.second, 19.41666903591372417) <= 7.24801511236401819);

        path = find_path_with_walk_to_pick_up(122505, 122504, 15.53929282185655403, 1.39999999999999991, 26.05881062328641562);
        CHECK(path_is_legal(122505, 122504, 15.53929282185655403, 1.39999999999999991, 26.05881062328641562, path));
        CHECK(compute_path_travel_time(path.second, 15.53929282185655403) <= 3.71577810567041134);

        path = find_path_with_walk_to_pick_up(129151, 115497, 16.94758728063605702, 1.67440756558571580, 57.51808304799190807);
        CHECK(path_is_legal(129151, 115497, 16.94758728063605702, 1.67440756558571580, 57.51808304799190807, path));
        CHECK(compute_path_travel_time(path.second, 16.94758728063605702) <= 1102.42932439451283244);

        path = find_path_with_walk_to_pick_up(134791, 55194, 18.52929843128670484, 1.89185028781409170, 107.21274765032889320);
        CHECK(path_is_legal(134791, 55194, 18.52929843128670484, 1.89185028781409170, 107.21274765032889320, path));
        CHECK(compute_path_travel_time(path.second, 18.52929843128670484) <= 1637.35238768000704113);

        path = find_path_with_walk_to_pick_up(136065, 81899, 19.75095321567011908, 1.39999999999999991, 71.70695938727332930);
        CHECK(path_is_legal(136065, 81899, 19.75095321567011908, 1.39999999999999991, 71.70695938727332930, path));
        CHECK(compute_path_travel_time(path.second, 19.75095321567011908) <= 6.79831855136581087);

        path = find_path_with_walk_to_pick_up(136723, 134005, 16.36906750429234236, 4.54090149687136524, 194.82443623747644779);
        CHECK(path_is_legal(136723, 134005, 16.36906750429234236, 4.54090149687136524, 194.82443623747644779, path));
        CHECK(compute_path_travel_time(path.second, 16.36906750429234236) <= 1527.63479892833947815);

        path = find_path_with_walk_to_pick_up(138966, 124388, 15.41652320998716696, 1.70539632939149310, 91.38353873260361127);
        CHECK(path_is_legal(138966, 124388, 15.41652320998716696, 1.70539632939149310, 91.38353873260361127, path));
        CHECK(compute_path_travel_time(path.second, 15.41652320998716696) <= 1250.30384268046555007);

        path = find_path_with_walk_to_pick_up(133400, 100474, 16.58850420671487313, 1.71777433561344273, 274.02713409085180274);
        CHECK(path_is_legal(133400, 100474, 16.58850420671487313, 1.71777433561344273, 274.02713409085180274, path));
        CHECK(compute_path_travel_time(path.second, 16.58850420671487313) <= 1260.58153652473424700);

        path = find_path_with_walk_to_pick_up(147662, 106028, 20.41577714410001221, 3.02242961318037118, 22.44809494514823101);
        CHECK(path_is_legal(147662, 106028, 20.41577714410001221, 3.02242961318037118, 22.44809494514823101, path));
        CHECK(compute_path_travel_time(path.second, 20.41577714410001221) <= 2870.84714741131301707);

        path = find_path_with_walk_to_pick_up(10050, 114313, 19.24692202831997179, 1.39999999999999991, 70.03409454965564862);
        CHECK(path_is_legal(10050, 114313, 19.24692202831997179, 1.39999999999999991, 70.03409454965564862, path));
        CHECK(compute_path_travel_time(path.second, 19.24692202831997179) <= 15.10474733545629533);

        //Generate random inputs
        std::vector<IntersectionIndex> intersection_ids1;
        std::vector<IntersectionIndex> intersection_ids2;
        std::vector<double> turn_penalties;
        std::vector<double> walking_speeds;
        std::vector<double> walking_time_limits;
        for(size_t i = 0; i < 40; i++) {
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
            ECE297_TIME_CONSTRAINT(181939);
            std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> result;
            for(size_t i = 0; i < 40; i++) {
                result = find_path_with_walk_to_pick_up(intersection_ids1[i], intersection_ids2[i], turn_penalties[i],
                        walking_speeds[i], walking_time_limits[i]);
            }
        }
    } //find_path_with_walk_to_pick_up_perf_medium

} //path_with_walk_to_pickup_perf_medium_public

