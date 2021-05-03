#include "pch.h"
#include "global_scene.h"

#include <algorithm>

global_scene::global_scene(int w, int h)
	: _width(w)
	, _height(h)
{
}

void global_scene::add(int n, species_ptr s)
{
	_species_list.push_back(s);

	for (int index = n; index-- > 0;)
	{
		auto b = std::make_shared<being>(s, this);
		b->set_position(
			position{
				static_cast<double>(rand() % _width),
				static_cast<double>(rand() % _height) });
		_being_list.push_back(b);
	}
}

void global_scene::calculate()
{
	std::vector<being_ptr> to_delete;
	for (auto& b : _being_list)
	{
		if (b->get_status() == status::dead)
			continue;

		environment env;
		env.temperature = 20;

		constexpr int close_range = 6;

		auto base = b->get_position();
		for (auto& nei : _being_list)
		{
			if (base.distance(this, nei->get_position()) < close_range)
				env.neighbours.push_back(nei);
		}
		std::sort(std::begin(env.neighbours), std::end(env.neighbours),
			[](auto lhs, auto rhs)
			{
				return lhs->get_mass() > rhs->get_mass();
			});

		b->calculate_physical_step(env);
		if (b->get_status() == status::dead)
			continue;

		if (rand() % 10 == 0)
			b->rethink_strategy(env);
		b->calculate_activity(env);

		if (b->get_mass() == 0 && b->get_status() == status::dead)
			to_delete.push_back(b);
	}
	for (auto& b : to_delete)
	{
		_being_list.erase(
			std::remove(_being_list.begin(), _being_list.end(), b),
			_being_list.end());
	}
}

std::vector<being_ptr> const& global_scene::get_beings() const
{
	return _being_list;
}

int global_scene::get_width() const
{
	return _width;
}
int global_scene::get_height() const
{
	return _height;
}

