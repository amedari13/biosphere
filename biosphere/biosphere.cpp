// biosphere.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define NOMINMAX
#include <Windows.h>

#include "simulation/global_scene.h"
#include "simulation/species.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <map>

namespace
{
	struct species_stat
	{
		int count{ 0 };
		int dead{ 0 };
		int avr_energy{ 0 };
		int max_energy{ 0 };
		int avr_mass{ 0 };
		int max_mass{ 0 };
	};
}

void print_scene(global_scene& s)
{
	auto const& sps = s.get_species();
	std::map<species_ptr, species_stat> sstat;
	for (auto& sp : sps)
	{
		sstat[sp] = species_stat{};
	}

	std::map<std::tuple<int, int>, int> field;
	auto const& vec = s.get_beings();
	for (auto& b : vec)
	{
		position xy = b->get_position();
		field[std::make_tuple(
			static_cast<int>(fmod(xy.x, s.get_width())),
			static_cast<int>(fmod(xy.y, s.get_height())))] = 1;


		species_stat& stat = sstat[b->get_species()];

		if (!b->is_alive())
		{
			stat.dead += 1;
			continue;
		}

		stat.count += 1;
		stat.avr_energy += b->get_energy();
		stat.max_energy = std::max(stat.max_energy, b->get_energy());
		stat.avr_mass += b->get_mass();
		stat.max_mass = std::max(stat.max_mass, b->get_mass());
	}

	std::vector<std::string> stat_lines;
	int index = 0;
	for (auto& sp : sps)
	{
		species_stat& stat = sstat[sp];
		if (stat.count == 0)
			continue;

		stat.avr_energy /= stat.count;
		stat.avr_mass /= stat.count;

		std::ostringstream ss;
		ss << "sp#" << ++index << "   : " << stat.count << " alive, " << stat.dead << " dead";
		stat_lines.push_back(ss.str());

		ss.str("");
		ss << "energy: " << stat.avr_energy << " (" << stat.max_energy << " max)";
		stat_lines.push_back(ss.str());

		ss.str("");
		ss << "  mass: " << stat.avr_mass << " (" << stat.max_mass << " max)";
		stat_lines.push_back(ss.str());
	}

	std::cout << "\x1b[?25l";
	std::cout << "+";
	for (int c = 0; c < s.get_width(); ++c)
		std::cout << "-";
	std::cout << "+\n";
	for (int r = 0; r < s.get_height(); ++r)
	{
		std::cout << "|";
		for (int c = 0; c < s.get_width(); ++c)
		{
			int b = field.count(std::make_tuple(c, r));
			std::cout << (b ? "#" : " ");
		}
		std::cout << "|";

		std::string line;
		if (stat_lines.size())
		{
			line = stat_lines.front();
			stat_lines.erase(stat_lines.begin());
		}

		std::cout << " " << line;
		for (int c = 30 - line.size(); c-- > 0;)
			std::cout << ' ';

		std::cout << "\n";
	}
	std::cout << "+";
	for (int c = 0; c < s.get_width(); ++c)
		std::cout << "-";
	std::cout << "+\n";

	for (int r = 0; r < s.get_height() + 2; ++r)
		std::cout << "\x1b[A";
}

int main()
{
	global_scene s{ 80, 20 };

	for (int c = 6; c-- > 0; )
		s.add(5, species::random());

	while (1)
	{
		print_scene(s);
		s.calculate();

		::Sleep(100);
	}
}
