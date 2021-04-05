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
		field[std::make_tuple(xy.x, xy.y)] = 1;
	}
}

int main()
{
	global_scene s{ 80, 25 };

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
