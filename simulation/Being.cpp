#include "pch.h"
#include "Being.h"

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

status being::calculate_next_step(environment env)
{
	// если холодно, теряем энергию на обогрева
	int delta_t = env.temperature - _temprature;
	if (delta_t < -10)
	{
		_energy -= delta_t / 10;
	}

	// если всё ещё есть свободная энергия -- растём
	if (_energy > _mass_limit * mass_to_energy_factor / 2)
	{
		_mass += 1;
		_energy -= mass_to_energy_factor;
	}

	// если достигли критической массы -- размножаемся
	if (_mass > _mass_limit)
	{
		return status::spawn;
	} 
	else if (_energy <= 0)
	{ 
		return status::dead;
	}
	return status::ok;
}

position being::get_position() const
{
	return _pos;
}
