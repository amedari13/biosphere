#include "pch.h"
#include "Being.h"
#include "species.h"

#include <assert.h>


being::being(species_ptr s)
	: _species(s)
{
}

being::~being()
{
}

void being::append_capability(capability_ptr)
{
}

void being::calculate_physical_step(environment env)
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
	case  strategy::hunt:
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
	if (_energy > spec->_mass_limit * cfg::mass_to_energy_factor / 2)
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

void being::calculate_activity(environment env)
{
	switch (_strategy)
	{
	case strategy::rest:
		break;
	case strategy::stampede:
	case strategy::hunt:
		_pos.x += rand() % 5 - 2;
		_pos.y += rand() % 5 - 2;
		break;
	case strategy::wander:
		_pos.x += rand() % 3 - 1;
		_pos.y += rand() % 3 - 1;
		break;
	}
}

void being::rethink_strategy(environment env)
{
	switch (rand() % 4)
	{
	case 0: _strategy = strategy::rest; return;
	case 1: _strategy = strategy::hunt; return;
	case 2: _strategy = strategy::stampede; return;
	case 3: _strategy = strategy::wander; return;
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
