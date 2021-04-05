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
	// ���� �������, ������ ������� �� ��������
	int delta_t = env.temperature - _temprature;
	if (delta_t < -10)
	{
		_energy -= delta_t / 10;
	}

	// ���� �� ��� ���� ��������� ������� -- �����
	if (_energy > _mass_limit * mass_to_energy_factor / 2)
	{
		_mass += 1;
		_energy -= mass_to_energy_factor;
	}

	// ���� �������� ����������� ����� -- ������������
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
