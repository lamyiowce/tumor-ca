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
	// 1.2 losuj pozycję - czyli losowy rozkład dawek o danej sumie
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
		// Ile linii na jeden plik: 50000 / 32 = 1562.5 ~ 1570, 
		// po przekroczeniu tego otwieramy kolejny plik
		if (i > file_count * 1570) {
			file_count++;
			f.close();
			t.close();
			f.open("protocol_list_" + std::to_string(file_count) + ".csv");
			t.open("protocol_times_" + std::to_string(file_count) + ".csv");
		}
		// Losuj sumę - od 0.75 do 10.00
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

		int temp_protocol_length = temp_protocol.size();

		std::cout << "Dlugosc " << temp_protocol_length << "\n";

		std::sort(temp_protocol.begin(), temp_protocol.begin() + temp_protocol_length);

		// Losuj permutację
		std::random_shuffle ( temp_protocol.begin(), temp_protocol.end() );

		
		// Zapisz jeśli nie było powtórek
		if (protocol_set.find(temp_protocol) == protocol_set.end()) {
			std::cout << "Zapis.\n";
			protocol_set.insert(temp_protocol);
			i++;
			std::cout << i << "...\n";

			// Losuj 2 czasy
			// _doses_slots_ kroków symulacji co 6 sekund
			// zaczynamy w pierwszym kroku nr 1
			// między dawkami 30 min = _doses_interval_ kroków
			// 1.1 losuj aż nie będzie odpowiedniej długości

			std::vector<int> random_time_1;
			std::vector<int> random_time_2;

			int time_length = 0;

			// 72000 = 5 dni co 6 sekund, interwał 300, dzień 14400, 8h to 4800
			// 720 = 5 dni co 10 minut, interwał 3, dzień 144, 6h to 36
			// 240 = 5 dni co 30 minut, interwał 1, dzień 48, 6h to 12
			int doses_slots = 720;
			int doses_min_interval = 3;
			int doses_day_interval = 144;
			int doses_6h_interval = 36;

			while (time_length < temp_protocol_length) {
				std::uniform_int_distribution<int> dist_time(1, doses_slots);
				int temp_time = dist_time(rng);
				bool good_time = true;
				for (int t = 0; t < random_time_1.size(); ++t) {
					if ((std::abs(random_time_1[t] - temp_time)) < doses_min_interval) {
						good_time = false;
					}
				}
				if (good_time) {
					random_time_1.push_back(temp_time);
					time_length++;
				}
			}

			time_length = 0;

			while (time_length < temp_protocol_length) {
				std::uniform_int_distribution<int> dist_time(1, doses_slots);
				int temp_time = dist_time(rng);
				bool good_time = true;
				for (int t = 0; t < random_time_2.size(); ++t) {
					if ((std::abs(random_time_2[t] - temp_time)) < doses_min_interval) {
						good_time = false;
					}
				}
				if (good_time) {
					random_time_2.push_back(temp_time);
					time_length++;
				}
			}

			std::cout << "Good times.\n";

			std::sort(random_time_1.begin(), random_time_1.begin() + temp_protocol_length);
			std::sort(random_time_2.begin(), random_time_2.begin() + temp_protocol_length);



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
			for (std::vector<float>::const_iterator val = temp_protocol.begin(); 
				val != temp_protocol.end(); ++val) {
					t << *val << ' ';
			}
			t << '\n';
			for (std::vector<int>::const_iterator val = random_time_2.begin(); 
				val != random_time_2.end(); ++val) {
					f << *val << ' ';
					t << *val << ' ';
			}
			f << '\n'; 
			t << '\n'; 
			if (temp_protocol_length <= 5) { 
				// Protocol chosen for the benchmarking:
				// every 24h = every _doses_day_interval_ steps, for 5 days
				for (std::vector<float>::const_iterator val = temp_protocol.begin(); 
				val != temp_protocol.end(); ++val) {
					t << *val << ' ';
				}
				t << '\n';
				int benchmark_protocol1 = 1;
				int position = 0;
				for (int benchmark_protocol1 = 1; 
					(benchmark_protocol1 < doses_slots) && (position < temp_protocol_length); 
					benchmark_protocol1 += doses_day_interval) {
						t << benchmark_protocol1 << ' ';
						f << benchmark_protocol1 << ' ';
						position++;
				}
				f << '\n'; 
				t << '\n'; 
			}

			if (temp_protocol_length <= 8) {
				// Protocol chosen for the benchmarking:
				// every 6h + every 18h = every _doses_day_interval steps, for 4 days,
				// hence 8 times
				// => time % 24 = 6 or time % 24 = 18
				for (std::vector<float>::const_iterator val = temp_protocol.begin(); 
						val != temp_protocol.end(); ++val) {
					t << *val << ' ';
				}
				t << '\n';
				int benchmark_protocol2 = 1;
				int position = 0;
				for (int benchmark_protocol2 = 1; 
					(benchmark_protocol2 < doses_slots) && (position < temp_protocol_length); 
					benchmark_protocol2 += doses_6h_interval) {
						if (benchmark_protocol2 % doses_day_interval == 1) {
							t << benchmark_protocol2 << ' ';
							f << benchmark_protocol2 << ' ';
							position++;
						} else if (benchmark_protocol2 % doses_day_interval == (3 * doses_6h_interval + 1)) {
							t << benchmark_protocol2 << ' ';
							f << benchmark_protocol2 << ' ';
							position++;
						}
				}
				f << '\n'; 
				t << '\n'; 
			}
			// Nowa linia oddziela osobne linie protokołów i ich czasów 
			// (każdy protokół dawek może mieć 2, 3 lub 4 czasy)
		}
	}
    return 0;
}