#include "pch.h"
#include "global_scene.h"


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
		auto b = std::make_shared<being>(s);
		b->set_position(position{ rand() % _width, rand() % _height });
		_being_list.push_back(b);
	}
}

void global_scene::calculate()
{
	for (auto& b : _being_list)
	{
		if (b->get_status() == status::dead)
			continue;

		environment env;
		env.temperature = 20;

		b->calculate_physical_step(env);
		if (b->get_status() == status::dead)
			continue;

		if (rand() % 10 == 0)
			b->rethink_strategy(env);
		b->calculate_activity(env);
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
