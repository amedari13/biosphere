// biosphere.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>

#include "simulation/global_scene.h"
#include "simulation/species.h"

#include <map>

void print_scene(global_scene& s)
{
	std::map<std::tuple<int, int>, int> field;
	auto const& vec = s.get_beings();
	for (auto b : vec)
	{
		position xy = b->get_position();
		field[std::make_tuple(
			static_cast<int>(fmod(xy.x, s.get_width())),
			static_cast<int>(fmod(xy.y, s.get_height())))] = 1;
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
		std::cout << "|\n";
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

	s.add(10, species::random());
	s.add(10, species::random());
	s.add(10, species::random());

	while (1)
	{
		print_scene(s);
		s.calculate();

		::Sleep(100);
	}
}
