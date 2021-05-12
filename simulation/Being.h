#pragma once

#include <vector>

#include "Capability.h"
#include "position.h"
#include "environment.h"

class global_scene;

enum class strategy
{
	rest,	  // отдыхаем, набираемся сил
	wander,   // бесцельно бродить
	stampede, // паника, убегаем
	hunt,     // пытаемся добраться до добычи
};

enum class status
{
	ok,		// продолжаем жить
	dead,	// умираем
	spawn,	// размножаться
};

namespace cfg
{
	const int mass_to_energy_factor = 40;
	const int run_expence = 3;
	const int wander_expence = 1;
	const int rest_bonus = 50;
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
	void die();
	void decay();
	being_ptr clone();

	void set_position(position p);
	position get_position() const;

	status get_status() const;
	int get_mass() const;
	int get_energy() const;
	bool is_alive() const;

	species_ptr get_species() const;

private:
	// переменные, описывающие состояние организма
	status	 _status{ status::ok };   // текущее состояние
	strategy _strategy{ strategy::rest }; // текущая стратегия
	int		 _mass{ 1 };     // масса организма существа
	int		 _energy{ 1000 };   // энергия, позволяющая организму 
	int	     _temprature{ 20 };    // температура тела существа
	position _pos;	    // положение существа в симуляции
	std::weak_ptr<being> _aim; // существо, на которого завязана стратегия

	// 
	std::weak_ptr<species> _species;
	global_scene* _scene;

};

