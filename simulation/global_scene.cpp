#include "pch.h"
#include "global_scene.h"

#include <algorithm>

global_scene::global_scene(int w, int h)
	: _width(w)
	, _height(h)
{
}

void global_scene::add(being_ptr b)
{
	b->set_position(
		position{
			static_cast<double>(rand() % _width),
			static_cast<double>(rand() % _height) });
	_being_list.push_back(b);
}

void global_scene::add(int n, species_ptr s)
{
	_species_list.push_back(s);

	for (int index = n; index-- > 0;)
	{
		auto b = std::make_shared<being>(s, this);
		add(b);
	}
}

void global_scene::calculate()
{
	auto bs = _being_list;
	for (auto& b : bs)
	{
		environment env;
		env.temperature = 20;

		if (!b->is_alive())
		{
			if (b->get_mass() == 0)
			{
				_being_list.erase(
					std::remove(_being_list.begin(), _being_list.end(), b),
					_being_list.end());
			}
			else
			{
				b->decay();
			}
			continue;
		}

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

		if (b->get_status() == status::spawn)
		{
			auto clone = b->clone();
			add(clone);		
		}

		if (rand() % 10 == 0)
			b->rethink_strategy(env);
		b->calculate_activity(env);
	}
}

std::vector<being_ptr> const& global_scene::get_beings() const
{
	return _being_list;
}

std::vector<species_ptr> const& global_scene::get_species() const
{
	return _species_list;
}

global_scene_stats global_scene::get_statistics() const
{
    global_scene_stats result;
	for (auto& sp : _species_list)
	{
        result.sstat[sp] = species_stat_entry{};
	}

	for (auto& b : _being_list)
	{
        auto& stat = result.sstat[b->get_species()];

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

    return result;
}

int global_scene::get_width() const
{
	return _width;
}
int global_scene::get_height() const
{
	return _height;
}

