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


void print_scene(global_scene& s)
{
	std::map<std::tuple<int, int>, int> field;
	auto const& vec = s.get_beings();
	for (auto& b : vec)
	{
		position xy = b->get_position();
		field[std::make_tuple(
			static_cast<int>(fmod(xy.x, s.get_width())),
			static_cast<int>(fmod(xy.y, s.get_height())))] = 1;
	}

	auto const& sstat = s.get_statistics();

	std::vector<std::string> stat_lines;
	int index = 0;
	for (auto & [sp, stat] : sstat.by_species)
	{
		int avr_energy = stat.count ? stat.total_energy / stat.count : 0;
		int avr_mass = stat.count ? stat.total_mass / stat.count : 0;

		std::ostringstream ss;
		ss << "sp#" << ++index << "  : " << stat.count << " alive, " << stat.dead << " dead";
		stat_lines.push_back(ss.str());

		ss.str("");
		ss << "energy: " << avr_energy << " (" << stat.max_energy << " max)";
		stat_lines.push_back(ss.str());

		ss.str("");
		ss << "  mass: " << avr_mass << " (" << stat.max_mass << " max)";
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
			int b = static_cast<int>(
				field.count(std::make_tuple(c, r)));
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
		for (size_t c = 30 - line.size(); c-- > 0;)
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
