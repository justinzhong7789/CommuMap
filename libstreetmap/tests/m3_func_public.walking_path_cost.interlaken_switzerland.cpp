#include <random>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"

#include "unit_test_util.h"
#include "path_verify.h"

using ece297test::relative_error;
using ece297test::path_is_legal;


SUITE(walking_path_cost_public) {
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
    TEST(compute_path_travel_time) {
        std::vector<StreetSegmentIndex> path;
        double turn_penalty;
        double walking_speed;
        double expected;
        double actual;

        path = {62, 1914, 1913};
        turn_penalty = 18.26087682029633541;
        walking_speed = 2.43167678446974200;
        expected = 9.08654868371561264;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {798, 799, 772, 1649};
        turn_penalty = 20.52818021603572873;
        walking_speed = 3.33517607336110089;
        expected = 16.96846335291809638;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {804, 805, 806, 984};
        turn_penalty = 26.39729671158957203;
        walking_speed = 4.12556905952097619;
        expected = 18.28784034761609334;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {844, 845, 155, 1451, 1338, 156, 1861, 1862, 1863, 1864, 1865};
        turn_penalty = 34.51993359273959072;
        walking_speed = 0.86994370304351953;
        expected = 23.30994490821158038;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {377, 378, 379, 1950, 421, 422, 267, 1962, 1963, 1964, 357, 358};
        turn_penalty = 34.66734537616928691;
        walking_speed = 2.11775904418693184;
        expected = 24.42048756744876314;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1384, 1383, 282, 789, 788, 787, 786, 785, 784, 783, 782, 781, 780, 779, 778, 777, 776, 775, 1509, 1508};
        turn_penalty = 37.53834279945513686;
        walking_speed = 2.70788313432656125;
        expected = 29.11235406850717311;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1909, 1887, 1888, 1889, 1890, 1891, 1892, 1893, 1894, 1895, 1833, 1821, 1022, 1023, 1024, 1025, 2059, 59, 207, 208, 209, 210, 58, 1546, 1533, 1532};
        turn_penalty = 33.54275001978079729;
        walking_speed = 2.71194833105055011;
        expected = 31.72536752509349256;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1799, 1851, 1852, 1853, 1854, 1855, 1343, 1342, 1341, 1340, 1339, 1338, 221, 1440, 2151};
        turn_penalty = 29.64832794497377222;
        walking_speed = 3.43498870222091934;
        expected = 36.60514440778566581;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {611, 610, 609, 639, 638, 637, 618, 605, 1787, 1786, 199, 743, 82, 176, 746, 45};
        turn_penalty = 43.62876494617410117;
        walking_speed = 2.00392001764460481;
        expected = 38.83041862555835877;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2004, 124, 123, 126, 127, 98, 99, 100, 101};
        turn_penalty = 12.97117716583002434;
        walking_speed = 3.36902423514803484;
        expected = 50.83773127812303017;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1422, 1421, 901, 1033, 1042, 1041, 1034, 1025, 2059, 59, 207, 208, 209, 210, 58, 1546, 1533, 1532, 2114, 1389, 1846, 998, 2162};
        turn_penalty = 35.73388314152779088;
        walking_speed = 4.25714570821250859;
        expected = 52.18539464500018710;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {989, 990, 991, 777, 776, 775, 774, 773, 772, 799, 798, 797, 796, 795, 794, 793, 771, 770, 769, 768, 1087, 1088, 1089, 1090};
        turn_penalty = 26.65500735658102016;
        walking_speed = 2.90148829288311383;
        expected = 63.68164106035077765;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {240, 239, 238, 1451, 1338, 1339, 1340, 1341, 1342, 1343, 1855, 1883, 1884, 1885, 1886, 157, 1348, 1349, 1350, 219};
        turn_penalty = 20.85227712661118815;
        walking_speed = 3.20877666847904308;
        expected = 73.31414778351293648;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1657, 960, 959, 958, 827, 764, 1806, 1805, 1804, 1816, 1815, 1814, 1820, 1819, 790, 789};
        turn_penalty = 20.48237311733243615;
        walking_speed = 3.61309987964352253;
        expected = 76.09253263434325731;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1352, 2175, 998, 1846, 1389, 2114, 1532, 214, 215, 216, 217, 218, 219, 1344, 1345};
        turn_penalty = 10.52221169261282618;
        walking_speed = 3.24321991146385713;
        expected = 77.02896212147138044;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {2012, 343, 342, 341, 340, 339, 109, 1307, 1306, 1995, 1994, 1993, 1992, 1991, 1990, 1989, 897, 900, 2028, 2027, 1336, 1337, 231, 193, 192, 80, 79, 78, 1545, 1531, 1532, 1533, 1546, 58, 210, 209, 208, 207, 59, 2059, 1025, 1024, 1023, 1022, 1821, 206, 205, 818, 817, 531, 530, 529, 20, 21, 22, 23, 24, 1139};
        turn_penalty = 33.99182029046458098;
        walking_speed = 3.46933916344308280;
        expected = 99.32268804241142846;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {470, 742, 657, 658, 669, 670, 671, 621, 639, 638, 637, 615, 558, 953};
        turn_penalty = 16.81407069785912611;
        walking_speed = 2.18940457641120290;
        expected = 100.39456370006112707;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1876, 1840, 1841, 721, 1049, 1158, 1159, 1160, 829, 828, 166, 293, 731, 730, 729, 296, 295};
        turn_penalty = 15.33745718399443092;
        walking_speed = 3.99376719618737219;
        expected = 104.44042638985844462;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {750, 82, 176, 1936, 1937, 2014, 1788, 1789, 1790, 1791, 1792, 1793, 1794, 1795, 104, 940, 939, 938, 1720, 1719, 1849, 368, 367, 1965, 1968, 1969, 1314, 1313, 1312, 332, 139, 1304, 1773, 1775, 1609, 1610};
        turn_penalty = 30.62471546934541422;
        walking_speed = 4.89973621411523919;
        expected = 118.22217816072243579;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1672, 854, 852, 851, 850, 474, 473, 2106, 472, 400, 488, 1222, 653, 1747, 1746};
        turn_penalty = 15.11409834525278484;
        walking_speed = 2.76050899288038876;
        expected = 125.55019657929243238;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1374, 2226, 4, 455, 1409, 1410, 1411, 1412, 1413, 1414, 1415, 1416, 1417, 1134, 1135, 17, 18, 19, 12, 11, 1905, 1904, 1903, 1756, 1887, 211, 212};
        turn_penalty = 16.63331716611783051;
        walking_speed = 2.63934167393095143;
        expected = 135.35867493612633439;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {556, 557, 412, 413, 414, 415, 944, 945, 1722, 1723, 399, 710, 711, 712, 713, 628, 629, 2086, 857, 856};
        turn_penalty = 9.40665919658631999;
        walking_speed = 4.14471597040462481;
        expected = 153.82407023579574457;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {488, 290, 1233, 1234, 1235, 1236, 1058, 369, 370};
        turn_penalty = 16.54538144767878549;
        walking_speed = 2.92744955311141464;
        expected = 175.76951599443574992;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {181, 1791, 1792, 1793, 1794, 1795, 1796, 1797, 1798, 911, 912, 1961, 1960, 1959, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 109, 1307, 1306, 1995, 1994, 1993, 1992, 1991, 1990, 1989, 897, 900, 2028, 2027, 1336, 1337, 231, 193, 192, 157, 1348, 1349, 1350, 1344, 1345, 1346, 1347, 274, 1335, 1559, 464, 465, 275, 1721, 1832, 1824, 1130};
        turn_penalty = 15.36062383199156578;
        walking_speed = 4.20088107805602018;
        expected = 187.73603241186725654;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1736, 1735, 1734, 1733, 1732, 1731, 1730, 1729, 1728, 1727, 1726, 946, 1910, 1909, 1887, 1888, 1889, 1890, 1891, 1892, 1893, 1894, 1895, 1833, 1821, 1022, 1023, 1024, 1025, 2059, 59, 207, 208, 209, 210, 58, 1546, 1533, 1532, 1531, 1545, 78, 79, 80, 192, 193, 231, 1337, 1336, 2027, 2028, 900, 897, 1989, 1990, 1991, 1992, 1993, 1994, 1995, 77, 76, 93};
        turn_penalty = 18.24346895180306305;
        walking_speed = 4.46165579394548306;
        expected = 192.73219237158721739;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {538, 1026, 1707, 1706, 1705, 1708, 1709, 2158, 2159, 2097, 2046};
        turn_penalty = 7.31448838274191271;
        walking_speed = 1.63954236367476947;
        expected = 204.66083910615250829;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1547, 2174, 2023, 1352, 2175, 998, 1846, 1389, 2115, 158, 1544, 78, 79, 80, 1886, 1885, 1884, 1883, 1855, 1343, 1342, 1341, 1340, 1339, 1338, 221, 1440, 1441, 1442, 1443, 1444, 1445, 1446, 1447, 1448, 1449, 1450, 1039};
        turn_penalty = 7.82606251372725037;
        walking_speed = 1.86915130961948428;
        expected = 214.74943911000320895;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1699, 1702, 649, 650, 651, 652, 656, 1744, 1743, 291, 605, 618, 637, 638, 639, 621, 671, 670, 669, 658, 657};
        turn_penalty = 8.47841745985060946;
        walking_speed = 3.31156119423954554;
        expected = 326.74251461821506837;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {83, 87, 88, 89, 90, 91, 92, 93, 94, 95, 339, 340, 341, 107, 106, 105, 357};
        turn_penalty = 2.32310977358730941;
        walking_speed = 2.30440253117113114;
        expected = 399.72145315646577046;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {1854, 1883, 1884, 1885, 1886, 80, 79, 78, 1545, 1531, 1532, 1533, 1546, 58, 210, 209, 208, 207, 59, 2059, 1025, 1024, 1023, 1022, 1821, 206, 205, 204, 961, 532, 531, 530, 529, 20, 21, 22, 23, 1140, 19};
        turn_penalty = 1.30596128124808364;
        walking_speed = 4.26384373645593939;
        expected = 935.46527171794343758;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

    } //compute_path_travel_time

} //walking_path_cost_public

