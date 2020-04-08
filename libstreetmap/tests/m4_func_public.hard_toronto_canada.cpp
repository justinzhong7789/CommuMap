#include <random>
#include <iostream>
#include <unittest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include "m4.h"

#include "unit_test_util.h"
#include "courier_verify.h"

using ece297test::relative_error;
using ece297test::courier_path_is_legal_with_capacity;


SUITE(hard_toronto_canada_public) {
    TEST(hard_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<IntersectionIndex> depots;
        float turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(59070, 9735, 34.13857), DeliveryInfo(109872, 97382, 102.87185), DeliveryInfo(106953, 45459, 127.22393), DeliveryInfo(9859, 135785, 26.73004), DeliveryInfo(115701, 98871, 85.55891), DeliveryInfo(144407, 66165, 13.94950), DeliveryInfo(54045, 27684, 156.23058), DeliveryInfo(58301, 48324, 5.90659), DeliveryInfo(146133, 20951, 116.91071), DeliveryInfo(140358, 73665, 196.70969), DeliveryInfo(36915, 16947, 173.25085), DeliveryInfo(41222, 43881, 191.73979), DeliveryInfo(149796, 33350, 72.03912), DeliveryInfo(89654, 149085, 0.40323), DeliveryInfo(57092, 46215, 64.45849), DeliveryInfo(47717, 86944, 75.88801), DeliveryInfo(61584, 49074, 189.90866), DeliveryInfo(121858, 83583, 81.14774), DeliveryInfo(52238, 137574, 82.33183), DeliveryInfo(49995, 143641, 40.19408), DeliveryInfo(93875, 88323, 8.27145), DeliveryInfo(83177, 112099, 71.23460), DeliveryInfo(36732, 140976, 165.86661), DeliveryInfo(101173, 96713, 146.92995), DeliveryInfo(72669, 135300, 55.38988), DeliveryInfo(56810, 46005, 125.04338), DeliveryInfo(35858, 5844, 168.88548), DeliveryInfo(71710, 34031, 71.35764), DeliveryInfo(73047, 3982, 104.14169), DeliveryInfo(28042, 135329, 23.42255), DeliveryInfo(94406, 132530, 29.90889), DeliveryInfo(65919, 49987, 132.08820), DeliveryInfo(82026, 125583, 29.32549), DeliveryInfo(127968, 92209, 170.53413), DeliveryInfo(141605, 116127, 53.31223), DeliveryInfo(51482, 66663, 34.63387), DeliveryInfo(16484, 104730, 11.67235), DeliveryInfo(44209, 141866, 35.96309), DeliveryInfo(102363, 133399, 174.03258), DeliveryInfo(146164, 3674, 126.73223), DeliveryInfo(142776, 58244, 91.26589), DeliveryInfo(125320, 1757, 96.04809), DeliveryInfo(8292, 7386, 137.12761), DeliveryInfo(116622, 48180, 86.25928), DeliveryInfo(92345, 100726, 22.19036), DeliveryInfo(137755, 118340, 150.79965), DeliveryInfo(147964, 92254, 50.05916), DeliveryInfo(29398, 36839, 5.78693), DeliveryInfo(126158, 108143, 140.86339), DeliveryInfo(100926, 3481, 16.40904), DeliveryInfo(12164, 7896, 80.75494), DeliveryInfo(52963, 25118, 121.60390), DeliveryInfo(112616, 148428, 141.49138), DeliveryInfo(100640, 42350, 130.24796), DeliveryInfo(99694, 34887, 198.80614), DeliveryInfo(55182, 22226, 170.81590), DeliveryInfo(27898, 71383, 54.52250), DeliveryInfo(65904, 62560, 55.33802), DeliveryInfo(23733, 70103, 21.78803), DeliveryInfo(101774, 42803, 130.18661), DeliveryInfo(22574, 24432, 38.46646), DeliveryInfo(1607, 77224, 14.27020), DeliveryInfo(48413, 134847, 36.67271), DeliveryInfo(6971, 139862, 28.46946), DeliveryInfo(27680, 61371, 49.42979), DeliveryInfo(104149, 137430, 25.17506), DeliveryInfo(78370, 133505, 25.41876), DeliveryInfo(33710, 148793, 189.14767), DeliveryInfo(108425, 69505, 147.26413), DeliveryInfo(87409, 70805, 187.30554), DeliveryInfo(142733, 56478, 26.07329), DeliveryInfo(3416, 120288, 183.69395), DeliveryInfo(59671, 124946, 91.14422), DeliveryInfo(54679, 49647, 22.82865), DeliveryInfo(5192, 109185, 161.54930), DeliveryInfo(102523, 108802, 146.45059), DeliveryInfo(58630, 53105, 116.50365), DeliveryInfo(71625, 22215, 147.75230), DeliveryInfo(93690, 7271, 151.02028), DeliveryInfo(59690, 129818, 99.64669), DeliveryInfo(120429, 139639, 45.52915), DeliveryInfo(109357, 126042, 137.66962), DeliveryInfo(119085, 73617, 50.61726), DeliveryInfo(46993, 24468, 146.05179), DeliveryInfo(30579, 72777, 65.82381), DeliveryInfo(58468, 116357, 181.47511), DeliveryInfo(130545, 20945, 185.19974), DeliveryInfo(17687, 87030, 176.71071), DeliveryInfo(106329, 43952, 2.69646), DeliveryInfo(115363, 31143, 161.01949), DeliveryInfo(46104, 35757, 171.41914), DeliveryInfo(95925, 140111, 173.39388), DeliveryInfo(3419, 38280, 95.33434), DeliveryInfo(127296, 61489, 84.06668), DeliveryInfo(41037, 60799, 182.82811), DeliveryInfo(43960, 86318, 96.11957), DeliveryInfo(24631, 129893, 187.54022), DeliveryInfo(127169, 24771, 154.27420), DeliveryInfo(46520, 110442, 170.08232), DeliveryInfo(136510, 131158, 43.37552)};
        depots = {2336, 47652, 136248, 117990, 89996, 91893, 12973, 104502, 25135, 68554};
        turn_penalty = 15.000000000;
        truck_capacity = 1131.576782227;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_toronto_canada: INVALID" << std::endl;
        }

    } //hard_toronto_canada

    TEST(hard_multi_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<IntersectionIndex> depots;
        float turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(102627, 14543, 187.31200), DeliveryInfo(111347, 92005, 137.59500), DeliveryInfo(100605, 123798, 48.40664), DeliveryInfo(131838, 48937, 36.90132), DeliveryInfo(102881, 55360, 63.51763), DeliveryInfo(109379, 44430, 59.62290), DeliveryInfo(60946, 22176, 56.99833), DeliveryInfo(27597, 32707, 166.45079), DeliveryInfo(53014, 90918, 146.00418), DeliveryInfo(49569, 142766, 167.56726), DeliveryInfo(68032, 105926, 38.71244), DeliveryInfo(35374, 50635, 88.06906), DeliveryInfo(111347, 141611, 181.68616), DeliveryInfo(13943, 89146, 171.94859), DeliveryInfo(73520, 33894, 130.29910), DeliveryInfo(133754, 87762, 68.27714), DeliveryInfo(144217, 57748, 5.74371), DeliveryInfo(81532, 128510, 54.44788), DeliveryInfo(127539, 98181, 53.46008), DeliveryInfo(147881, 115087, 171.11781), DeliveryInfo(47467, 140555, 27.89900), DeliveryInfo(32969, 41890, 112.46114), DeliveryInfo(113620, 65893, 11.81318), DeliveryInfo(50270, 140206, 33.82141), DeliveryInfo(96827, 7349, 193.41937), DeliveryInfo(10385, 3515, 146.50169), DeliveryInfo(120784, 11688, 183.47961), DeliveryInfo(129004, 79015, 144.07825), DeliveryInfo(16584, 87905, 0.80647), DeliveryInfo(51836, 58797, 128.91698), DeliveryInfo(127539, 79691, 151.77602), DeliveryInfo(95305, 96361, 179.81734), DeliveryInfo(6832, 89146, 162.29547), DeliveryInfo(6838, 79691, 164.66367), DeliveryInfo(25946, 20925, 80.38815), DeliveryInfo(111347, 5029, 16.54290), DeliveryInfo(116603, 121599, 142.46921), DeliveryInfo(132809, 135571, 131.73322), DeliveryInfo(56587, 106211, 93.85989), DeliveryInfo(114185, 12340, 110.77976), DeliveryInfo(111347, 132021, 50.08675), DeliveryInfo(137108, 117982, 137.77098), DeliveryInfo(28280, 80543, 142.71527), DeliveryInfo(137102, 119342, 8.28338), DeliveryInfo(49262, 117261, 46.84510), DeliveryInfo(53334, 49741, 59.81778), DeliveryInfo(123169, 119342, 64.17639), DeliveryInfo(73831, 79691, 58.65097), DeliveryInfo(127539, 14773, 141.06827), DeliveryInfo(40139, 92208, 106.62446), DeliveryInfo(98928, 145339, 69.26775), DeliveryInfo(82340, 127566, 23.34470), DeliveryInfo(146094, 123148, 71.92617), DeliveryInfo(32979, 100373, 148.06516), DeliveryInfo(82445, 64575, 53.46445), DeliveryInfo(127539, 76561, 182.53178), DeliveryInfo(65805, 24329, 192.09618), DeliveryInfo(56084, 89146, 74.25520), DeliveryInfo(45149, 85607, 172.51857), DeliveryInfo(125660, 131809, 44.38072), DeliveryInfo(47676, 37101, 101.59931), DeliveryInfo(108090, 145555, 100.11832), DeliveryInfo(95435, 32796, 11.57386), DeliveryInfo(145824, 15455, 81.72677), DeliveryInfo(111347, 98149, 32.81808), DeliveryInfo(35668, 116488, 161.50987), DeliveryInfo(143420, 123437, 43.20779), DeliveryInfo(131499, 145144, 82.98276), DeliveryInfo(50141, 15792, 60.49590), DeliveryInfo(102965, 122743, 197.61226), DeliveryInfo(4672, 88865, 141.63181), DeliveryInfo(32074, 133841, 109.04499), DeliveryInfo(3215, 118889, 110.67605), DeliveryInfo(84268, 46030, 43.57607), DeliveryInfo(93987, 68062, 60.37325), DeliveryInfo(67420, 6962, 76.93291), DeliveryInfo(93040, 79691, 28.54039), DeliveryInfo(118140, 92431, 73.34542), DeliveryInfo(82074, 147331, 56.93892), DeliveryInfo(19719, 145875, 98.85957), DeliveryInfo(127539, 89146, 50.35012), DeliveryInfo(116937, 121309, 50.83753), DeliveryInfo(67002, 108075, 178.29533), DeliveryInfo(55797, 79691, 94.52828), DeliveryInfo(92573, 99974, 174.61108), DeliveryInfo(36038, 66700, 52.14658), DeliveryInfo(14643, 125120, 167.38791), DeliveryInfo(121087, 81002, 182.28844), DeliveryInfo(109359, 112957, 45.65729), DeliveryInfo(71717, 130241, 123.09861), DeliveryInfo(104224, 118946, 92.90118), DeliveryInfo(104477, 61158, 33.00730), DeliveryInfo(106323, 119342, 95.50459), DeliveryInfo(65139, 99991, 102.04055), DeliveryInfo(86459, 23106, 199.29338), DeliveryInfo(119380, 62194, 91.05830), DeliveryInfo(87921, 99294, 75.33926), DeliveryInfo(88418, 72487, 101.23453), DeliveryInfo(57293, 50237, 92.10358), DeliveryInfo(143251, 107924, 131.64763)};
        depots = {129258, 116379, 4090};
        turn_penalty = 15.000000000;
        truck_capacity = 1100.685058594;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_multi_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_multi_toronto_canada: INVALID" << std::endl;
        }

    } //hard_multi_toronto_canada

} //hard_toronto_canada_public

