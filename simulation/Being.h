#pragma once

#include <vector>

#include "Capability.h"
#include "position.h"
#include "environment.h"

class global_scene;

enum class strategy
{
	rest,	  // ��������, ���������� ���
	wander,   // ��������� �������
	stampede, // ������, �������
	hunt,     // �������� ��������� �� ������
};

enum class status
{
	ok,		// ���������� ����
	dead,	// �������
	spawn,	// ������������
};

namespace cfg
{
	const int mass_to_energy_factor = 40;
	const int run_expence = 3;
	const int wander_expence = 1;
	const int rest_bonus = 10;
}

using being_ptr = std::shared_ptr< class being >;
using species_ptr = std::shared_ptr< class species >;

class being
{
public:
	being(species_ptr s, global_scene* sc);
	virtual ~being();

	void append_capability(capability_ptr);

	void calculate_physical_step(environment const& env);
	void rethink_strategy(environment const& env);
	void calculate_activity(environment const& env);
	void fight(being_ptr other);
	void kill();

	void set_position(position p);
	position get_position() const;

	status get_status() const;
	int get_mass() const;

private:
	std::weak_ptr<species> _species;
	global_scene* _scene;

	// ����������, ����������� ��������� ���������
	position _pos;	    // ��������� �������� � ���������
	status	 _status{ status::ok };   // ������� ���������
	strategy _strategy{ strategy::rest }; // ������� ���������
	std::weak_ptr<being> _aim; // ��������, �� �������� �������� ���������
	int		 _mass{ 1 };     // ����� ��������� ��������
	int		 _energy{ 1000 };   // �������, ����������� ��������� 
	int	     _temprature{ 20 };    // ����������� ���� ��������
};

