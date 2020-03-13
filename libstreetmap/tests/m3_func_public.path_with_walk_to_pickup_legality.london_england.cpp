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
    TEST(find_path_with_walk_to_pick_up_legality) {
        std::pair<std::vector<StreetSegmentIndex>, std::vector<StreetSegmentIndex>> path;

        path = find_path_with_walk_to_pick_up(14685, 145134, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(14685, 145134, 22.58759087598575732, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(90134, 11485, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(90134, 11485, 17.38266612778162568, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(99384, 138862, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(99384, 138862, 18.40129796266718643, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(99479, 99478, 16.82374630314258113, 1.39999999999999991, 116.54834174783871958);
        CHECK(path_is_legal(99479, 99478, 16.82374630314258113, 1.39999999999999991, 116.54834174783871958, path));

        path = find_path_with_walk_to_pick_up(108402, 67706, 20.56643390083450385, 1.58021018804951030, 21.76115375595257717);
        CHECK(path_is_legal(108402, 67706, 20.56643390083450385, 1.58021018804951030, 21.76115375595257717, path));

        path = find_path_with_walk_to_pick_up(146595, 150028, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(146595, 150028, 22.04203310234851898, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(87809, 59217, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(87809, 59217, 24.74049806129059093, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(159375, 138824, 23.24724699156175944, 1.39999999999999991, 53.25736922532109219);
        CHECK(path_is_legal(159375, 138824, 23.24724699156175944, 1.39999999999999991, 53.25736922532109219, path));

        path = find_path_with_walk_to_pick_up(163296, 163304, 21.25477987116730105, 1.39999999999999991, 20.41798773437117376);
        CHECK(path_is_legal(163296, 163304, 21.25477987116730105, 1.39999999999999991, 20.41798773437117376, path));

        path = find_path_with_walk_to_pick_up(172338, 165028, 18.50759507368418610, 2.21649998556084338, 87.00691725350289119);
        CHECK(path_is_legal(172338, 165028, 18.50759507368418610, 2.21649998556084338, 87.00691725350289119, path));

        path = find_path_with_walk_to_pick_up(221820, 212406, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000);
        CHECK(path_is_legal(221820, 212406, 24.80338191831117456, 1.39999999999999991, 0.00000000000000000, path));

        path = find_path_with_walk_to_pick_up(239241, 47818, 24.92187560353409026, 1.65216581887766800, 10.29795875189352117);
        CHECK(path_is_legal(239241, 47818, 24.92187560353409026, 1.65216581887766800, 10.29795875189352117, path));

        path = find_path_with_walk_to_pick_up(34001, 37727, 20.11605397431454634, 4.31994584277146831, 218.75953695706866142);
        CHECK(path_is_legal(34001, 37727, 20.11605397431454634, 4.31994584277146831, 218.75953695706866142, path));

        path = find_path_with_walk_to_pick_up(50827, 51281, 22.38363928283910553, 2.23074402792155890, 190.21487440152219506);
        CHECK(path_is_legal(50827, 51281, 22.38363928283910553, 2.23074402792155890, 190.21487440152219506, path));

        path = find_path_with_walk_to_pick_up(59664, 59652, 17.54559835623686581, 1.39999999999999991, 73.95419535705060810);
        CHECK(path_is_legal(59664, 59652, 17.54559835623686581, 1.39999999999999991, 73.95419535705060810, path));

        path = find_path_with_walk_to_pick_up(70126, 69655, 16.92280555852369517, 3.44279620503049522, 294.78125216537381448);
        CHECK(path_is_legal(70126, 69655, 16.92280555852369517, 3.44279620503049522, 294.78125216537381448, path));

        path = find_path_with_walk_to_pick_up(94388, 167016, 21.23306273583789050, 4.75835585385030679, 201.93108722199818317);
        CHECK(path_is_legal(94388, 167016, 21.23306273583789050, 4.75835585385030679, 201.93108722199818317, path));

        path = find_path_with_walk_to_pick_up(133023, 48886, 24.83484050724447911, 4.86353184250799675, 20.43800182100934748);
        CHECK(path_is_legal(133023, 48886, 24.83484050724447911, 4.86353184250799675, 20.43800182100934748, path));

        path = find_path_with_walk_to_pick_up(141762, 1517, 17.79158648691273470, 3.73022132235690673, 198.37735872276729765);
        CHECK(path_is_legal(141762, 1517, 17.79158648691273470, 3.73022132235690673, 198.37735872276729765, path));

        path = find_path_with_walk_to_pick_up(141987, 91369, 22.60549692457038518, 2.66296360064370585, 107.91932539141981806);
        CHECK(path_is_legal(141987, 91369, 22.60549692457038518, 2.66296360064370585, 107.91932539141981806, path));

        path = find_path_with_walk_to_pick_up(171992, 125489, 19.65814383955234490, 2.61380348252008510, 269.42699505188778630);
        CHECK(path_is_legal(171992, 125489, 19.65814383955234490, 2.61380348252008510, 269.42699505188778630, path));

        path = find_path_with_walk_to_pick_up(188054, 188057, 23.46132436906174945, 1.39999999999999991, 4.99597878882282131);
        CHECK(path_is_legal(188054, 188057, 23.46132436906174945, 1.39999999999999991, 4.99597878882282131, path));

        path = find_path_with_walk_to_pick_up(188062, 202542, 21.44660135770386944, 4.64346223684140469, 125.62056185421472776);
        CHECK(path_is_legal(188062, 202542, 21.44660135770386944, 4.64346223684140469, 125.62056185421472776, path));

        path = find_path_with_walk_to_pick_up(202840, 175558, 20.52046685992282704, 2.37083824569967527, 230.22411451605856314);
        CHECK(path_is_legal(202840, 175558, 20.52046685992282704, 2.37083824569967527, 230.22411451605856314, path));

        path = find_path_with_walk_to_pick_up(211319, 58729, 24.44644791758729951, 1.64997535417071628, 283.91495119879840558);
        CHECK(path_is_legal(211319, 58729, 24.44644791758729951, 1.64997535417071628, 283.91495119879840558, path));

        path = find_path_with_walk_to_pick_up(211464, 209692, 23.29661237256773632, 4.71770614686478496, 58.90407864412429717);
        CHECK(path_is_legal(211464, 209692, 23.29661237256773632, 4.71770614686478496, 58.90407864412429717, path));

        path = find_path_with_walk_to_pick_up(217796, 156028, 23.19714671248527083, 2.21215233136955636, 256.86781979822910671);
        CHECK(path_is_legal(217796, 156028, 23.19714671248527083, 2.21215233136955636, 256.86781979822910671, path));

        path = find_path_with_walk_to_pick_up(130847, 193094, 21.69284068351273831, 3.80033717800303528, 211.06793157310116271);
        CHECK(path_is_legal(130847, 193094, 21.69284068351273831, 3.80033717800303528, 211.06793157310116271, path));

        path = find_path_with_walk_to_pick_up(148336, 148338, 21.61594626068436753, 1.39999999999999991, 238.63497263933146542);
        CHECK(path_is_legal(148336, 148338, 21.61594626068436753, 1.39999999999999991, 238.63497263933146542, path));

        path = find_path_with_walk_to_pick_up(239758, 22712, 17.00609809812324968, 3.34095808144363549, 249.75851771490800957);
        CHECK(path_is_legal(239758, 22712, 17.00609809812324968, 3.34095808144363549, 249.75851771490800957, path));

    } //find_path_with_walk_to_pick_up_legality

} //path_with_walk_to_pickup_legality_public

