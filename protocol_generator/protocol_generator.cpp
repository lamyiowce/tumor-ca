#include <algorithm>
#include <iterator>
#include <fstream>
#include <random>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <stack>

struct DataFrame {
  float sum;
  std::vector<float> coins;
  std::vector<float> avail_coins;
};

typedef std::map<float, std::vector<std::vector<float>>> protocol_sum;


int main() {
  	protocol_sum protocol_map;
	int ways = 0;
	for (float target = 10.0; target > 0.5; target -= 0.25) {
		std::stack<DataFrame> s;
		std::vector<float> max_doses;
		for (float max_dose = 0.5; max_dose < 10.25; max_dose += 0.25) {
			max_doses.push_back(max_dose);
		}
		s.push({ target, {}, max_doses });
		std::vector<std::vector<float>> chosen_protocols;
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
  	std::cout << "Count " << protocol_map.count(2.0) << "\n";

	// 1. Losowanko
	// 1.1 losuj sumę - równa dystrybucja po wartości sumy
	// 1.2 losuj pozycję
	// 1.3 losuj permutacje
	// 1.4 losuj czasy

	std::set<std::vector<float>> protocol_set;

	std::random_device rd;
	std::mt19937 rng;
	rng.seed(rd());
	std::uniform_int_distribution<int> dist_sum(3, 40); 
	int i = 0;
	std::ofstream f("protocol_list_1.csv");
	std::ofstream t("protocol_times_1.csv");
	int file_count = 1;
	while (i < 50000) {
		// Ile linii na jeden plik
		if (i > file_count * 1570) {
			file_count++;
			f.close();
			t.close();
			f.open("protocol_list_" + std::to_string(file_count) + ".csv");
			t.open("protocol_list_" + std::to_string(file_count) + ".csv");
		}
		// Losuj sumę
		float sum = dist_sum(rng) * 0.25;

		std::vector<float> temp_protocol;

		std::cout << "Suma " << sum << "\n";
		std::cout << "Rozmiar " << protocol_map[sum].size() << "\n";
		// Losuj pozycję
		std::uniform_int_distribution<int> dist_pos(
			0, protocol_map[sum].size() - 1);
		int temp_pos = dist_pos(rng);
		std::cout << "Poz " << temp_pos << "\n";

		temp_protocol = (protocol_map[sum])[temp_pos];

		int temp_length = temp_protocol.size();

		std::cout << "Dlugosc " << temp_length << "\n";

		std::sort(temp_protocol.begin(), temp_protocol.begin() + temp_length);

		// Losuj permutację
		std::random_shuffle ( temp_protocol.begin(), temp_protocol.end() );

		// Losuj 2 czasy
		// 72000 kroków symulacji co 6 sekund
		// zaczynamy w pierwszym kroku nr 1
		// między dawkami 30min = 300 kroków
		// czyli:
		// 1.1 weź długość dawek. dawkki mogą zacząć się najpóźniej 
		//     w 72000 - długość * 300 kroku = 72000 - koniec
		// 1.2 losuj pierwszy termin dawki z 1..72000-koniec
		// 1.3 dodaj do niego 300 i losuj kolejny termin dawki z 
		//     (poprzedni + 300, 14400-koniec + 300)
		// 1.4 itd, aż nie dojdziesz do końca długości wektorów

		std::vector<int> random_time_1;
		std::vector<int> random_time_2;

		int time_length = 0;

		while (time_length < temp_length) {
			std::uniform_int_distribution<int> dist_time(1, 72000);
			int temp_time = dist_time(rng);
			bool good_time = true;
			for (int t = 0; t < random_time_1.size(); ++t) {
				if ((std::abs(random_time_1[t] - temp_time)) < 300) {
					good_time = false;
				}
			}
			if (good_time) {
				random_time_1.push_back(temp_time);
				time_length++;
			}
		}

		time_length = 0;

		while (time_length < temp_length) {
			std::uniform_int_distribution<int> dist_time(1, 72000);
			int temp_time = dist_time(rng);
			bool good_time = true;
			for (int t = 0; t < random_time_2.size(); ++t) {
				if ((std::abs(random_time_2[t] - temp_time)) < 300) {
					good_time = false;
				}
			}
			if (good_time) {
				random_time_2.push_back(temp_time);
				time_length++;
			}
		}

		std::cout << "Good times.\n";

		std::sort(random_time_1.begin(), random_time_1.begin() + temp_length);
		std::sort(random_time_2.begin(), random_time_2.begin() + temp_length);

		// Zapisz jeśli nie było powtórek
		if (protocol_set.find(temp_protocol) == protocol_set.end()) {
			std::cout << "Zapis.\n";
			protocol_set.insert(temp_protocol);
			i++;
			std::cout << i << "...\n";
			for (std::vector<float>::const_iterator val = temp_protocol.begin(); 
				val != temp_protocol.end(); ++val) {
					t << *val << ' ';
			}
			t << '\n';
			for (std::vector<int>::const_iterator val = random_time_1.begin(); 
				val != random_time_1.end(); ++val) {
					f << *val << ' ';
					t << *val << ' ';
			}
			f << '\n'; 
			t << '\n';
			for (std::vector<int>::const_iterator val = random_time_2.begin(); 
				val != random_time_2.end(); ++val) {
					f << *val << ' ';
					t << *val << ' ';
			}
			f << '\n'; 
			t << '\n'; 
			if (temp_length <= 5) { 
				// Protocol chosen for the benchmarking:
				// every 24h = every 14400 steps, for 5 days
				int benchmark_protocol1 = 1;
				int position = 0;
				for (int benchmark_protocol1 = 1; 
					(benchmark_protocol1 < 72000) && (position < temp_length); 
					benchmark_protocol1 += 14400) {
						t << benchmark_protocol1 << ' ';
						f << benchmark_protocol1 << ' ';
						position++;
				}
				f << '\n'; 
				t << '\n'; 
			}

			if (temp_length <= 8) {
				// Protocol chosen for the benchmarking:
				// every 6 + every18h = every 14400 steps, for 5 days
				// => time % 24 = 6 or time % 24 = 18
				int benchmark_protocol2 = 1;
				int position = 0;
				for (int benchmark_protocol2 = 1; 
					(benchmark_protocol2 < 72000) && (position < temp_length); 
					benchmark_protocol2 += 3600) {
						if (benchmark_protocol2 % 14400 == 1) {
							t << benchmark_protocol2 << ' ';
							f << benchmark_protocol2 << ' ';
							position++;
						} else if (benchmark_protocol2 % 14400 == 10801) {
							t << benchmark_protocol2 << ' ';
							f << benchmark_protocol2 << ' ';
							position++;
						}
				}
				f << '\n'; 
				t << '\n'; 
			}
		}
	}
    return 0;
}