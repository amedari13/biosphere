#include "pch.h"
#include "Being.h"
#include "species.h"

#include "global_scene.h"

#include <assert.h>


being::being(species_ptr s, global_scene* sc)
	: _species(s)
	, _scene(sc)
{
}

being::~being()
{
}

void being::append_capability(capability_ptr)
{
}

void being::calculate_physical_step(environment const & env)
{
	assert(_status != status::dead);

	// если холодно, теряем энергию на обогрева
	int delta_t = env.temperature - _temprature;
	if (delta_t < -10)
	{
		_energy -= delta_t / 10;
	}

	switch (_strategy)
	{
	case strategy::stampede:
	case strategy::hunt:
		_energy -= cfg::run_expence;
		break;
	case strategy::wander:
		_energy -= cfg::wander_expence;
		break;
	case strategy::rest:
		_energy += cfg::rest_bonus;
		break;
	}

	// получаем
	species_ptr spec = _species.lock();
	assert(spec);

	// если всё ещё есть свободная энергия -- растём
	if (_energy > spec->_mass_limit * cfg::mass_to_energy_factor / 10)
	{
		_mass += 1;
		_energy -= cfg::mass_to_energy_factor;
	}

	// если достигли критической массы -- размножаемся
	if (_mass > spec->_mass_limit)
	{
		_status = status::spawn;
	}
	else if (_energy <= 0)
	{
		_status = status::dead;
	}
	else {
		_status = status::ok;
	}
}

void being::calculate_activity(environment const & env)
{
	int run_speed = 3;
	int wander_speed = 1;

	auto aim = _aim.lock();
	switch (_strategy)
	{
	case strategy::rest:
		break;
	case strategy::stampede:
		_pos.move_from(_scene, aim->get_position(), run_speed);
		break;
	case strategy::hunt:
	{

		double d = _pos.distance(_scene, aim->get_position());
		bool success = false;
		if (d < run_speed)
		{
			_pos = aim->get_position();
			success = true;
		}
		else
		{ 
			_pos.move_to(_scene, aim->get_position(), run_speed);
			d = _pos.distance(_scene, aim->get_position());
			if (d <= 1)
				success = true;
		}
		if (success)
			fight(aim);

		break;
	}
	case strategy::wander:
		_pos.x += (rand() % (wander_speed * 2 + 1) - 1);
		_pos.y += (rand() % (wander_speed * 2 + 1) - 1);
		break;
	}
}

void being::fight(being_ptr other)
{
	int p = other->_mass * 100 / _mass;

	constexpr int lo_range = 70;
	constexpr int hi_range = 125;

	if (other->get_status() != status::dead)
	{
		p = (p - lo_range) * 100 / (hi_range - lo_range);
		if ((rand() % 100) > p)
			other->kill();
		else
		{ 
			kill();
			return;
		}
	}

	// eat it
	if (other->_mass)
	{
		other->_mass--;
		_energy += cfg::mass_to_energy_factor;
	}
}

void being::kill()
{
	_status = status::dead;
}

void being::rethink_strategy(environment const & env)
{
	_aim.reset();

	species_ptr sp = _species.lock();
	if (_energy < sp->_want_to_rest_limit)
	{
		_strategy = strategy::rest;
		return;
	}

	_strategy = strategy::wander; 
	for (auto& n : env.neighbours)
	{
		int p = n->_mass * 100 / _mass;
		if (p > sp->_treat_as_dangerous)
		{
			_strategy = strategy::stampede;
			_aim = n;
			return;
		}
		if (p < sp->_treat_as_yummy || n->get_status() == status::dead)
		{
			_strategy = strategy::hunt;
			_aim = n;
			return;
		}
	}
}

void being::set_position(position p)
{
	_pos = p;
}

position being::get_position() const
{
	return _pos;
}

status being::get_status() const
{
	return _status;
}

int being::get_mass() const
{
	return _mass;
}
