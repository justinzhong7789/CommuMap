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
    TEST(compute_path_travel_time) {
        std::vector<StreetSegmentIndex> path;
        double turn_penalty;
        double walking_speed;
        double expected;
        double actual;

        path = {81515, 81514, 81513, 81512, 188759, 188751, 188757, 188756, 188755, 188754};
        turn_penalty = 20.52818021603572873;
        walking_speed = 3.33517607336110089;
        expected = 9.81712566799964925;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {106203, 106204, 106205, 187655, 4799};
        turn_penalty = 18.26087682029633541;
        walking_speed = 2.43167678446974200;
        expected = 17.78219916488789920;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {132584, 36554, 36553, 184194, 184195, 184193, 36528, 36529, 36530};
        turn_penalty = 34.66734537616928691;
        walking_speed = 2.11775904418693184;
        expected = 20.73508950106115023;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {87353, 205895, 80193, 80214, 80215, 80216, 205934, 148889, 183207, 118104, 162930};
        turn_penalty = 34.51993359273959072;
        walking_speed = 0.86994370304351953;
        expected = 23.17310532493628727;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {198746, 198747, 198723, 73067, 73066, 73065, 73064, 73063, 73062, 73090, 73089, 51729, 73084, 47783, 154779, 50013, 47764, 50613, 154786};
        turn_penalty = 33.54275001978079729;
        walking_speed = 2.71194833105055011;
        expected = 30.84612459332506162;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {62879, 128233, 62695, 62694, 62985, 62651, 62944, 63211, 217137, 169234, 216971, 159134, 217138, 63240, 62842, 63116, 63230, 177995, 62785, 177993, 62810, 115382};
        turn_penalty = 43.62876494617410117;
        walking_speed = 2.00392001764460481;
        expected = 36.31864736740627109;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {106259, 17277, 60360, 60364, 60365, 116710, 116711, 116712, 116717, 145968};
        turn_penalty = 29.64832794497377222;
        walking_speed = 3.43498870222091934;
        expected = 44.82167480322575415;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {144096, 144095, 144094, 144093, 144092, 144102, 9918, 9917, 9911, 136205, 136206, 136207, 136208, 136209, 136210, 210650, 210651, 186074, 186075, 186077, 186076, 162322, 162321, 162320, 163280, 210867, 210866, 210865, 81238, 160243, 12148, 211402, 211403};
        turn_penalty = 37.53834279945513686;
        walking_speed = 2.70788313432656125;
        expected = 56.33805448059601417;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {180955, 180956, 180957, 180958, 180959, 180951, 180949, 180950, 180952, 180953, 180954, 180948, 27325, 27326, 27327, 64023, 63980, 63981, 63982};
        turn_penalty = 26.39729671158957203;
        walking_speed = 4.12556905952097619;
        expected = 61.46991945743497610;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {18375, 5904, 5903, 5902, 805, 5901, 5900, 5899, 5898, 612, 613, 9944, 185268};
        turn_penalty = 12.97117716583002434;
        walking_speed = 3.36902423514803484;
        expected = 67.69707951377263555;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {45810, 45809, 45808, 45807, 86996, 67292, 67309, 64349, 150186, 132631, 207594, 4928, 134527, 4926, 4927, 134529, 176644, 181537, 129815, 67508, 67507, 67504, 67506, 159130};
        turn_penalty = 35.73388314152779088;
        walking_speed = 4.25714570821250859;
        expected = 70.06258380165586175;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {181299, 181298, 172646, 172649, 21661, 172650, 172652, 172653, 184649, 23814, 144784, 144783, 144782, 166285, 166284, 149386, 23829, 23828, 109004, 109003};
        turn_penalty = 20.48237311733243615;
        walking_speed = 3.61309987964352253;
        expected = 85.08725213036186119;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {101678, 101676, 211570, 25412, 25411, 80325, 80385, 80386, 100717, 32126, 32127, 100684, 87827, 87826, 87825, 87824, 81881, 81882, 81883, 81884, 81885, 81886};
        turn_penalty = 26.65500735658102016;
        walking_speed = 2.90148829288311383;
        expected = 88.66559053306168892;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {124316, 124315, 124314, 124309, 22835, 176301, 22769, 82980, 82979, 22767, 22766, 82983, 82982, 22768, 116548, 116552, 189210, 191209, 116546, 181155, 181154, 181153, 181152, 181151, 207754, 181149, 181150, 181304, 181309, 181308, 181307, 181306, 86903, 86904, 86905, 86908, 86909, 86910};
        turn_penalty = 20.85227712661118815;
        walking_speed = 3.20877666847904308;
        expected = 90.78126661954125609;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {184555, 184556, 184529, 184530, 184540, 184541, 184542, 184548, 184534, 184535, 184536, 184537, 184538, 184539, 179985, 112990, 179987, 112999, 112998, 112997, 76879, 76877, 76883, 76878};
        turn_penalty = 15.11409834525278484;
        walking_speed = 2.76050899288038876;
        expected = 95.90866761825250819;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {131401, 111157, 25634, 131403, 131404, 132710, 132711, 132712, 132713, 32818, 32811, 32817, 32825, 32824, 32806, 131262, 131274, 165669, 165670, 131273, 131272, 131271, 131270, 131269, 131268, 131267, 131266, 131265, 131264, 131249, 131260};
        turn_penalty = 15.33745718399443092;
        walking_speed = 3.99376719618737219;
        expected = 100.12258216099547781;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {202274, 86700, 86701, 86702, 202263, 202264, 202260, 94346, 94310, 94186, 94230, 94323, 202322, 202321, 202315, 202314, 202316, 202309, 202319, 202318, 202330, 202329, 202324, 202323, 202326, 202313, 94260, 94259, 94258, 94257, 94256, 94255, 94254, 94253, 94252, 94251, 94250, 94249, 94248, 94247, 94319, 94318, 94317, 94316, 94315, 94218, 94217, 94216, 94215, 94214, 94213, 94212, 94211, 94210, 94209, 160457, 160456, 160455, 160454, 160453, 160452, 160451, 160450, 160449, 160448, 160447, 160446, 160445, 160444, 160443, 160442, 160441, 130124, 195207, 195206, 195205, 195204, 195203, 195216, 195208, 195184, 195183, 195182, 195181, 195180, 195141, 195142, 195139, 195163, 195162, 195161, 195160, 195159, 195158, 195157, 195156, 195155, 195154};
        turn_penalty = 33.99182029046458098;
        walking_speed = 3.46933916344308280;
        expected = 100.73281947765534028;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {50429, 48146, 50233, 48306, 207507, 207506, 207505, 93432, 93473, 93444, 71613, 71612, 71611};
        turn_penalty = 16.81407069785912611;
        walking_speed = 2.18940457641120290;
        expected = 104.75385020568073458;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {167726, 142407, 142406, 184329, 184324, 167871, 167870, 180866, 167727, 167876, 167875, 167874, 167873, 213266, 213273, 213274, 167341, 167340, 167345, 167338, 206492, 167335, 167334, 167333, 167332, 167331, 167495};
        turn_penalty = 16.63331716611783051;
        walking_speed = 2.63934167393095143;
        expected = 134.74514812483801052;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {139363, 132803, 132804, 132805, 132371, 132372, 99506, 99507, 99508, 99509, 132611, 208384};
        turn_penalty = 10.52221169261282618;
        walking_speed = 3.24321991146385713;
        expected = 136.24661107897648549;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {75317, 75320, 75314, 75231, 75232, 75205, 75215, 98334, 98335, 98336, 98337, 98338, 168253, 168251, 168252, 97335, 97336, 168516, 168514, 168512, 168513, 168515, 78413, 78414, 98248, 98247, 98246, 98245, 168681, 168683, 168682, 168680, 98397, 98396, 98395};
        turn_penalty = 30.62471546934541422;
        walking_speed = 4.89973621411523919;
        expected = 137.58146720592992551;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {55945, 55946, 55925, 55927, 55928, 55913, 140924, 81243, 19393, 140926, 61474, 140765};
        turn_penalty = 9.40665919658631999;
        walking_speed = 4.14471597040462481;
        expected = 148.93639312501329641;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {50328, 60331, 18280, 155574, 155575, 159853, 159852, 159851, 83977, 83978, 82647, 82648, 82649, 82650, 82651, 82652, 82653, 82654, 83985, 83986, 83987, 83988, 83989, 83990, 83991, 83992, 178547, 178549, 145727};
        turn_penalty = 16.54538144767878549;
        walking_speed = 2.92744955311141464;
        expected = 175.49498482076779737;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {53699, 152973, 53622, 53611, 53614, 53615, 84055, 53617, 53616, 84052, 84053, 53625, 53685, 53686, 45954, 45953, 45952, 45951, 45950, 45949};
        turn_penalty = 7.31448838274191271;
        walking_speed = 1.63954236367476947;
        expected = 214.02389792685320913;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {137, 138, 44958, 139, 7809, 122391, 213372, 213373, 567, 5054, 9049, 9050, 123410, 123411, 123414, 135506, 123422, 135496, 135498, 135500, 135499, 135497, 98427, 121956};
        turn_penalty = 18.24346895180306305;
        walking_speed = 4.46165579394548306;
        expected = 218.96587630830842386;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {115269, 33059, 33060, 33061, 38248, 123274, 123275, 160409, 160410, 99506};
        turn_penalty = 7.82606251372725037;
        walking_speed = 1.86915130961948428;
        expected = 225.39213721800453527;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {16440, 188558, 161884, 161886, 161885, 213933, 213934, 213935, 213936, 61593, 61603, 61577, 61578, 61579, 61570, 61571, 161959, 161960, 61652, 61498, 61584, 61543, 61614, 61526, 61527, 61528, 61529, 61565, 61566, 61547, 61548, 61720, 61721, 155181, 155182, 155175, 155176, 155177, 155179, 155178, 155180, 65129, 152481, 65140, 181592};
        turn_penalty = 15.36062383199156578;
        walking_speed = 4.20088107805602018;
        expected = 234.79216811683420474;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {188322, 188321, 181052, 181051, 183871, 181048, 181046, 102510, 102488, 160431, 160430, 102460, 102472, 102513, 102506, 102449, 153681, 152443, 50384, 50383, 206855, 206856, 206857, 206858, 173870, 112767, 102442, 173869, 48340, 51622, 49690};
        turn_penalty = 8.47841745985060946;
        walking_speed = 3.31156119423954554;
        expected = 317.52022509554836915;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {211191, 109285, 156531, 176640, 176641, 81398, 114151, 81438, 125092, 8484, 8483, 8482, 113924, 113923};
        turn_penalty = 2.32310977358730941;
        walking_speed = 2.30440253117113114;
        expected = 554.51211524085101701;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {197290, 197289, 25333, 25334, 25335, 25336, 25337, 25338, 25339, 25340, 197305, 197325, 197324, 197323, 197322, 25297, 25298, 25299, 197094, 197095, 197090, 197089, 197083, 197088, 120734, 156859, 196345, 196347, 196346, 196344, 120761, 120762, 120763, 120764, 183556, 182665, 182666, 182667, 89973, 89974, 89975, 89976, 89977, 89978, 89979, 89980, 122254, 146759, 122257, 122258, 122259, 122260, 122261, 122262, 122263, 19372, 19373, 19374, 182635, 182636, 182637, 182638, 182639, 182640, 182641, 182642, 182643, 182644, 182645, 182646, 182647, 182648, 182649, 182650, 182651, 182652, 182653};
        turn_penalty = 1.30596128124808364;
        walking_speed = 4.26384373645593939;
        expected = 1895.51308787032303371;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

    } //compute_path_travel_time

} //walking_path_cost_public

