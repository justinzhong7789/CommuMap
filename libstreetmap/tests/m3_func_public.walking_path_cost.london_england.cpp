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
    TEST(compute_path_travel_time) {
        std::vector<StreetSegmentIndex> path;
        double turn_penalty;
        double walking_speed;
        double expected;
        double actual;

        path = {288116, 288115, 288114, 225481, 9447, 196110, 225487, 225486};
        turn_penalty = 18.26087682029633541;
        walking_speed = 2.43167678446974200;
        expected = 14.59444826854667809;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {159853, 159852, 159851, 27587, 294539, 7628, 7627};
        turn_penalty = 34.51993359273959072;
        walking_speed = 0.86994370304351953;
        expected = 23.54687532004745520;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {155349, 155348, 276395, 209547, 46287, 219003};
        turn_penalty = 20.52818021603572873;
        walking_speed = 3.33517607336110089;
        expected = 27.52281052639915870;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {54907, 99186, 99185, 234937, 12045, 12046, 234940, 14818, 157384, 157385, 157386, 12031, 12032, 12033};
        turn_penalty = 34.66734537616928691;
        walking_speed = 2.11775904418693184;
        expected = 28.94101919263060196;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {141270, 141097, 141098, 141099, 141079, 141080, 141081, 141082, 141083, 141084, 141085, 141086, 141071, 141072, 247888, 245257, 220664, 167404, 167410, 167411};
        turn_penalty = 29.64832794497377222;
        walking_speed = 3.43498870222091934;
        expected = 38.03895516680690037;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {270492, 270493, 270424, 270425, 208514, 208513, 208512, 208511, 238995, 238981, 238982, 238983, 238984, 238985, 238986, 238987, 238988, 238989, 158273};
        turn_penalty = 33.54275001978079729;
        walking_speed = 2.71194833105055011;
        expected = 38.23919331779395492;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {130843, 88264, 88263, 134383, 17616, 132542, 132543, 132544, 232464, 285523, 285533, 285528, 285529, 153966, 285534, 183975, 183982, 4565};
        turn_penalty = 43.62876494617410117;
        walking_speed = 2.00392001764460481;
        expected = 40.91848063762039089;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {42272, 42271, 42270, 42269, 42268, 42267, 42266, 42265, 42264, 109465, 109464, 109463};
        turn_penalty = 26.39729671158957203;
        walking_speed = 4.12556905952097619;
        expected = 45.60522719588807661;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {28662, 28663, 28664, 28665, 28666, 28667, 28668, 28669, 42304, 42303};
        turn_penalty = 12.97117716583002434;
        walking_speed = 3.36902423514803484;
        expected = 50.56724010081633480;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {192228, 192227, 192226, 192225, 192224, 192223, 192222, 192221, 192220, 193451, 193450, 193449, 193448, 193447, 193446, 193445, 193444, 193443, 193442, 193441, 193440, 193439, 193438, 193437, 193436, 193435, 193434, 193433, 193432, 193431, 193430, 193429, 193515, 193514, 193513, 193512, 193511, 193510, 193509, 193508, 193507, 193506, 193505, 193504, 254198, 254197, 254196, 254195, 254194, 254193, 254192, 254191, 254190, 254189, 254188, 254187, 254186, 254185, 254184, 254183, 192638, 192637, 192636, 192635, 192634, 192633, 192632, 192631, 192630, 192629, 192628, 192627, 192626, 192625, 192624, 192623, 192622, 192621, 192620, 192619, 192618, 192617, 192616, 192615, 192614, 192613, 192612, 192611, 192610, 192609, 192608, 192607, 192606, 192605, 192604, 192603, 192602, 192601, 192600, 192599, 192598, 192597, 192596, 192595, 192594, 192593, 192592, 192591, 192590, 192589};
        turn_penalty = 37.53834279945513686;
        walking_speed = 2.70788313432656125;
        expected = 55.18814720897591997;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {68141, 68142, 68143, 277177, 277178, 277179, 277180, 277181, 277182, 277183, 277184, 277185, 277186, 277187, 277188, 277189, 199307, 66132, 66133, 66134, 66135, 66136, 297834, 66131, 158536, 158537, 292336, 68144};
        turn_penalty = 35.73388314152779088;
        walking_speed = 4.25714570821250859;
        expected = 66.31216505211689594;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {189462, 189466, 74720, 74719, 261581, 261580, 261579, 261578, 261577, 261576, 308932, 74697};
        turn_penalty = 10.52221169261282618;
        walking_speed = 3.24321991146385713;
        expected = 79.59170544401047209;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {279745, 291877, 35461, 238286, 238287, 3618, 35460, 237286, 151666, 151667, 151688, 177833, 14619, 279738, 279740, 234274, 14621, 239824, 46760, 216979, 216980, 220731, 172056, 172057, 172058, 172059, 9319, 9320, 9321, 9322, 9323, 9324, 9325, 9326, 95019, 95015, 95016, 95017, 95018, 245707};
        turn_penalty = 20.85227712661118815;
        walking_speed = 3.20877666847904308;
        expected = 87.58799756642928003;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {135111, 135104, 135105, 135106, 135107, 135108, 135109, 135099, 135098, 135097, 135096, 187369, 123644, 123645, 123646, 123647, 123648, 123649, 123650, 123651, 123652, 123629, 123628, 241123};
        turn_penalty = 26.65500735658102016;
        walking_speed = 2.90148829288311383;
        expected = 88.04606251077647983;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {279661, 279662, 285230, 279658, 181620, 181621, 181622, 67104, 284076, 67102, 18958, 278814, 278813, 149327, 168978, 168977, 168976, 168975};
        turn_penalty = 20.48237311733243615;
        walking_speed = 3.61309987964352253;
        expected = 88.39164010247957037;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {236889, 236890, 236891, 236892, 236893, 236894, 236895, 236896, 236897, 236898, 234720, 291057, 291056, 291055, 233625, 125310, 125309, 125308, 71944, 125311, 281507, 281509, 149515, 156079, 114677, 239034};
        turn_penalty = 16.81407069785912611;
        walking_speed = 2.18940457641120290;
        expected = 95.22613216484013776;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {310567, 310568, 198766, 280851, 198761, 99232, 99231, 99230, 99229, 99228, 99227, 99226, 99225, 99224, 99223, 99222, 99221, 198759, 68031, 308288, 308287, 3712, 3713, 3714, 3715, 3716, 3717, 3718, 3719, 209718, 289173, 8536, 280339, 280338, 280337, 65607, 169550, 223000, 65529};
        turn_penalty = 33.99182029046458098;
        walking_speed = 3.46933916344308280;
        expected = 100.64147758166454594;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {243631, 173874, 173873, 173872, 173871, 173870, 173869, 173868, 233836, 233835, 233834, 233837, 288445, 233822, 233821, 233825, 233824, 233823, 233820, 306261, 279637, 224120, 288446, 238481, 138913, 138914, 224119, 224095, 224094, 224098, 224107, 224106, 224110, 159522, 224111, 224113, 288439, 224108, 224109, 224104, 224105, 272250, 130590, 130589, 264051, 264052, 130619, 130620, 244028, 244029, 252370, 244025, 244026};
        turn_penalty = 15.11409834525278484;
        walking_speed = 2.76050899288038876;
        expected = 116.09263851404324441;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {102609, 102608, 234242, 234241, 234240, 102600, 102599, 41472, 41471, 41470, 41469, 74320, 74321, 74322, 138962, 138966, 138965, 138964, 138963, 140257, 141894, 48618, 48619, 224461, 224462, 102594, 102595, 102596, 102597, 278266, 278248, 278249, 74225, 74226};
        turn_penalty = 30.62471546934541422;
        walking_speed = 4.89973621411523919;
        expected = 129.01923204468693029;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {213843, 213842, 237728, 237729, 237730, 224587, 171410, 171411, 268824, 268825, 224576, 224592, 224588, 263480, 224596, 231289, 299170, 299169, 299168, 299167, 299166, 292022, 292021, 292020, 292019, 40251, 40250, 40249, 40248, 40247, 259797, 40246};
        turn_penalty = 15.33745718399443092;
        walking_speed = 3.99376719618737219;
        expected = 134.61130575281214306;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {190148, 190131, 190162, 190163, 190164, 211005, 190175, 190174, 190173, 254928, 254929, 254930, 254931, 254932, 254933, 274507, 274508, 274505, 274506, 274509, 274510, 274494, 274496, 274503, 274499, 274500, 274497, 274498, 254934, 254935, 254936, 254937, 254938, 254939, 254940, 254941, 254942, 254943, 254944, 254945, 254946, 191270, 274513, 274514, 191267, 193107, 188533};
        turn_penalty = 16.63331716611783051;
        walking_speed = 2.63934167393095143;
        expected = 141.77884900460304607;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {277848, 277847, 81081, 62006, 62007, 62008, 158600, 158601, 70958, 70959, 70960, 70961, 297339, 231309, 265786, 265787, 265785, 136785, 136786, 136787, 136788, 287938, 287939, 123103, 123104, 123105, 222805, 222806, 269220, 222809, 294242};
        turn_penalty = 9.40665919658631999;
        walking_speed = 4.14471597040462481;
        expected = 170.44242568980834562;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {297826, 303927, 303928, 280628, 280626, 73034, 73035, 255144, 255145, 288970, 288969, 250650, 250649, 250648, 250647, 250646, 250645, 179291, 271891, 250634, 234827, 234826, 234823, 285520, 285519, 285518, 285517, 285516, 255143, 247312, 247311, 247310, 247309, 133510, 133509, 133508, 247303};
        turn_penalty = 16.54538144767878549;
        walking_speed = 2.92744955311141464;
        expected = 174.93344194979414397;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {78789, 80525, 80524, 200632, 47407, 47408, 47409, 47410, 47411, 24898, 24899, 24900, 14877, 24629, 229010, 232552, 229509, 229508, 248000, 24644, 24643, 295221, 24650, 69256, 69255, 69254, 69253, 69252, 69259, 69258, 69257, 69262};
        turn_penalty = 7.31448838274191271;
        walking_speed = 1.63954236367476947;
        expected = 216.97429412299902651;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {25781, 25780, 30168, 30169, 30170, 25809, 30173, 30174, 25735, 25736, 113841, 113842, 113843, 113844, 268544, 25799, 175017, 175016, 110751, 110752, 110753, 22216, 110766, 260986, 300338, 238465, 260987, 230387, 230378, 110459, 22265, 22231, 22230};
        turn_penalty = 15.36062383199156578;
        walking_speed = 4.20088107805602018;
        expected = 219.94160188006185308;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {154048, 38542, 288693, 297200, 38891, 38544, 225388, 225389, 225390, 225391, 225392, 225393, 225394, 225395, 225396, 225397, 225398, 225399, 225400, 225401, 112893, 112894, 112895, 79490, 79491, 79492, 79493, 79494, 79495, 79496, 282884, 282885, 282886, 282887, 282888, 154323};
        turn_penalty = 7.82606251372725037;
        walking_speed = 1.86915130961948428;
        expected = 222.52703142728987018;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {18, 19, 261494, 149055, 149056, 149057, 149058, 149059, 149060, 149061, 149062, 149063, 149064, 149065, 149066, 149067, 149068, 149069, 308909, 308908, 272513, 272512, 64236, 64235, 154917, 3174, 3175, 3176, 3177, 3178, 3179, 3180, 3181, 159497, 159498, 62456, 115350, 232262, 158489};
        turn_penalty = 18.24346895180306305;
        walking_speed = 4.46165579394548306;
        expected = 225.64862540037555050;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {247953, 247952, 292108, 292107, 25986, 233346, 233347, 123385, 123386, 123387, 6922, 6470, 125225, 125224, 125223, 6464, 6461, 6462, 6463, 157306, 134312};
        turn_penalty = 8.47841745985060946;
        walking_speed = 3.31156119423954554;
        expected = 329.59026113012765791;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {12589, 12590, 265303, 265307, 108673, 246923, 245584};
        turn_penalty = 2.32310977358730941;
        walking_speed = 2.30440253117113114;
        expected = 508.45032576125862533;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

        path = {267359, 267358, 59263, 59262, 229595, 229606, 229605, 229604, 229603, 229602, 229601, 229600, 229599, 229598, 229597, 272745, 131828, 131827, 131826, 131825, 59155, 59165, 59166, 59167, 59168, 59178, 59177, 59176, 59175, 285238, 285266, 285267, 285271, 285272, 285273, 285274, 285275, 285276};
        turn_penalty = 1.30596128124808364;
        walking_speed = 4.26384373645593939;
        expected = 1655.80994365115452638;
        actual = compute_path_walking_time(path, turn_penalty, walking_speed);
        CHECK(relative_error(expected, actual) < 0.001000000);

    } //compute_path_travel_time

} //walking_path_cost_public

