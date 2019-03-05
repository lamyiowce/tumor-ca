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

int main() {

	calculate_factorials();

  	protocol_sum protocol_map;
	int ways = 0;
	for (int target = 1000; target > 50; target -= 25) {
		std::stack<DataFrame> s;
		s.push({ target, {}, { 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300, 325, 350, 375, 400, 425, 450, 475, 500, 525, 550, 575, 600, 625, 650, 675, 700, 725, 750, 775, 800, 825, 850, 875, 900, 925, 950, 975, 1000 } });
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