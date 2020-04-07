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


SUITE(easy_toronto_canada_public) {
    TEST(easy_toronto_canada) {
        std::vector<DeliveryInfo> deliveries;
        std::vector<IntersectionIndex> depots;
        float turn_penalty;
        float truck_capacity;
        std::vector<CourierSubpath> result_path;
        bool is_legal;

        deliveries = {DeliveryInfo(106896, 96006, 75.05315), DeliveryInfo(65038, 87076, 90.76368), DeliveryInfo(135221, 28262, 53.31088), DeliveryInfo(95563, 109313, 169.31749), DeliveryInfo(82672, 101772, 124.38162), DeliveryInfo(74966, 56035, 25.56255), DeliveryInfo(134204, 51749, 129.98366), DeliveryInfo(103341, 37007, 41.05344), DeliveryInfo(105969, 10229, 90.72692), DeliveryInfo(102380, 42614, 79.29823), DeliveryInfo(42199, 117913, 4.35320), DeliveryInfo(119658, 32615, 133.52373), DeliveryInfo(86334, 72388, 123.70870), DeliveryInfo(113281, 81132, 142.89053), DeliveryInfo(121985, 133821, 69.00546), DeliveryInfo(29435, 48326, 162.74165), DeliveryInfo(145491, 82634, 102.08238), DeliveryInfo(81186, 87321, 18.80107), DeliveryInfo(137127, 125686, 146.41916), DeliveryInfo(34975, 45024, 198.81140), DeliveryInfo(27101, 140389, 24.81209), DeliveryInfo(40756, 122694, 104.19133), DeliveryInfo(117691, 82535, 19.50759), DeliveryInfo(149474, 121164, 50.89673), DeliveryInfo(136515, 38216, 36.02774)};
        depots = {10, 38701};
        turn_penalty = 15.000000000;
        truck_capacity = 2427.750000000;
        {
        	ECE297_TIME_CONSTRAINT(45000);
        	
        	result_path = traveling_courier(deliveries, depots, turn_penalty, truck_capacity);
        }

        is_legal = courier_path_is_legal_with_capacity(deliveries, depots, result_path, truck_capacity);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR easy_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR easy_toronto_canada: INVALID" << std::endl;
        }

    } //easy_toronto_canada

} //easy_toronto_canada_public

