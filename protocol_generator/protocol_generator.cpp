#include <algorithm>
#include <iterator>
#include <fstream>
#include <random>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>

#define MAX_PROTOCOL_LENGTH 20

struct DataFrame {
  int sum;
  std::vector<int> coins;
  std::vector<int> avail_coins;
};

std::vector<int> factorials;

typedef std::map<int, std::vector<std::vector<int>>> protocol_sum;

void save_protocol_map(protocol_sum protocol_map) {
	// All of the unpermuted protocols
	std::ofstream f("protocol_map.csv");
	std::cout << "Save protocol map.\n";
	for (int target = 500; target > 50; target -= 25) {
	  	std::vector<std::vector<int>> sum_vector = protocol_map[target];
	  	std::cout << ':';
	  	f << target << ':';
		for (std::vector<std::vector<int>>::const_iterator i = sum_vector.begin(); 
			i != sum_vector.end(); ++i) {
			for (std::vector<int>::const_iterator val = (*i).begin(); val != (*i).end(); ++val) {
				f << *val << '.';
			}
			f << ',';
		}
		f << '\n';
	}
}

void calculate_factorials() {
	int fact = 1;
	for (int i = 2; i < MAX_PROTOCOL_LENGTH; i++) {
		factorials.push_back(fact);
		fact *= i;
	}
}

int main() {

	calculate_factorials();

  	protocol_sum protocol_map;
	int ways = 0;
	for (int target = 1000; target > 50; target -= 25) {
		std::stack<DataFrame> s;
		s.push({ target, {}, { 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900, 925, 950, 975, 1000 } });
			// , 1025, 1050, 1075, 1100, 1125, 1150, 1175, 1200, 1225, 1250, 1275, 1300, 1325, 1350, 1375, 1400, 1425, 1450, 1475, 1500, 1525, 1550, 1575, 1600, 1625, 1650, 1675, 1700, 1725, 1750, 1775, 1800, 1825, 1850, 1875, 1900, 1925, 1950, 1975, 2000, 2025, 2050, 2075, 2100, 2125, 2150, 2175, 2200, 2225, 2250, 2275, 2300, 2325, 2350, 2375, 2400, 2425, 2450, 2475, 2500, 2525, 2550, 2575, 2600, 2625, 2650, 2675, 2700, 2725, 2750, 2775, 2800, 2825, 2850, 2875, 2900, 2925, 2950, 2975, 3000, 3025, 3050, 3075, 3100, 3125, 3150, 3175, 3200, 3225, 3250, 3275, 3300, 3325, 3350, 3375, 3400, 3425, 3450, 3475, 3500, 3525, 3550, 3575, 3600, 3625, 3650, 3675, 3700, 3725, 3750, 3775, 3800, 3825, 3850, 3875, 3900, 3925, 3950, 3975, 4000, 4025, 4050, 4075, 4100, 4125, 4150, 4175, 4200, 4225, 4250, 4275, 4300, 4325, 4350, 4375, 4400, 4425, 4450, 4475, 4500, 4525, 4550, 4575, 4600, 4625, 4650, 4675, 4700, 4725, 4750, 4775, 4800, 4825, 4850, 4875, 4900, 4925, 4950, 4975, 5000, 5025, 5050, 5075, 5100, 5125, 5150, 5175, 5200, 5225, 5250, 5275, 5300, 5325, 5350, 5375, 5400, 5425, 5450, 5475, 5500, 5525, 5550, 5575, 5600, 5625, 5650, 5675, 5700, 5725, 5750, 5775, 5800, 5825, 5850, 5875, 5900, 5925, 5950, 5975, 6000, 6025, 6050, 6075, 6100, 6125, 6150, 6175, 6200, 6225, 6250, 6275, 6300, 6325, 6350, 6375, 6400, 6425, 6450, 6475, 6500, 6525, 6550, 6575, 6600, 6625, 6650, 6675, 6700, 6725, 6750, 6775, 6800, 6825, 6850, 6875, 6900, 6925, 6950, 6975, 7000, 7025, 7050, 7075, 7100, 7125, 7150, 7175, 7200, 7225, 7250, 7275, 7300, 7325, 7350, 7375, 7400, 7425, 7450, 7475, 7500, 7525, 7550, 7575, 7600, 7625, 7650, 7675, 7700, 7725, 7750, 7775, 7800, 7825, 7850, 7875, 7900, 7925, 7950, 7975, 8000, 8025, 8050, 8075, 8100, 8125, 8150, 8175, 8200, 8225, 8250, 8275, 8300, 8325, 8350, 8375, 8400, 8425, 8450, 8475, 8500, 8525, 8550, 8575, 8600, 8625, 8650, 8675, 8700, 8725, 8750, 8775, 8800, 8825, 8850, 8875, 8900, 8925, 8950, 8975, 9000, 9025, 9050, 9075, 9100, 9125, 9150, 9175, 9200, 9225, 9250, 9275, 9300, 9325, 9350, 9375, 9400, 9425, 9450, 9475, 9500, 9525, 9550, 9575, 9600, 9625, 9650, 9675, 9700, 9725, 9750, 9775, 9800, 9825, 9850, 9875, 9900, 9925, 9950, 9975, 10000 } });
		std::vector<std::vector<int>> chosen_protocols;
		while (!s.empty()) {
			DataFrame top = s.top();
			s.pop(); 
			if (top.sum < 0) continue;
			if (top.sum == 0) {
				std::cout << target << ":" << ways << ": ";
				for (int i = 0; i < top.coins.size(); i++) {
					std::cout << top.coins[i] << " ";
				}
				chosen_protocols.push_back(top.coins);
				std::cout << "\n";
				++ways;
				continue;
			}
			if (top.avail_coins.empty()) continue;
			DataFrame d = top;
			d.sum -= top.avail_coins[0];
			d.coins.push_back(top.avail_coins[0]);
			s.push(d);
			d = top;
			d.avail_coins.erase(std::begin(d.avail_coins));
			s.push(d);
		}
		std::cout << target << ":" << ways << std::endl;
		protocol_map[target] = chosen_protocols;
	}
	std::cout << ways << std::endl;
	std::cout << "Size " << protocol_map.size() << "\n";
  	std::cout << "Count " << protocol_map.count(200) << "\n";

  	// save_protocol_map(protocol_map);
  
	// 1. Losowanko
	// 1.1 losuj sumę - równa dystrybucja po wartości sumy
	// 1.2 losuj pozycję
	// 1.3 losuj permutacje
	// 1.4 losuj czasy

	std::set<std::vector<int>> protocol_set;

	std::random_device rd;
	std::mt19937 rng;
	rng.seed(rd());
	std::uniform_int_distribution<int> dist_sum(3, 40); 
	int i = 0;
	std::ofstream f("protocol_list.csv");

	while (i < 50000) {
		// Losuj sumę
		int sum = dist_sum(rng) * 25;

		std::vector<int> temp_protocol;
		int temp_sum = 0;

		std::cout << "Suma " << sum << "\n";
		std::cout << "Rozmiar " << protocol_map[sum].size() << "\n";
		// Losuj pozycję
		std::uniform_int_distribution<int> dist_pos(
			0, protocol_map[sum].size() - 1);
		int temp_pos = dist_pos(rng);
		std::cout << "Poz " << temp_pos << "\n";

		temp_protocol = (protocol_map[sum])[temp_pos];

		int temp_length = temp_protocol.size();

		std::cout << "Dlug " << temp_length << "\n";

		std::sort(temp_protocol.begin(), temp_protocol.begin() + temp_length);

		// Losuj permutację
		std::random_shuffle ( temp_protocol.begin(), temp_protocol.end() );

		// Zapisz jeśli nie było powtórek
		if (protocol_set.find(temp_protocol) == protocol_set.end()) {
			protocol_set.insert(temp_protocol);
			i++;
			std::cout << i << "... ";
			for (std::vector<int>::const_iterator val = temp_protocol.begin(); 
				val != temp_protocol.end(); ++val) {
					f << *val << ',';
			}
			f << '\n'; 
		}



	}

    return 0;
}