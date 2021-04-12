#pragma once

#include <vector>

#include "Capability.h"
#include "position.h"
#include "environment.h"


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
	const int mass_to_energy_factor = 10;
	const int run_expence	 = 3;
	const int wander_expence = 1;
	const int rest_bonus     = 2;
}

using being_ptr = std::shared_ptr< class being >;
using species_ptr = std::shared_ptr< class species >;

class being
{
public:
	being(species_ptr s);
	virtual ~being();

	void append_capability(capability_ptr);

	void calculate_physical_step(environment env);
	void rethink_strategy(environment env);
	void calculate_activity(environment env);

	void set_position(position p);
	position get_position() const;

	status get_status() const;

private:
	std::weak_ptr<species> _species;

	// переменные, описывающие состояние организма
	position _pos;	    // положение существа в симуляции
	status	 _status{ status::ok };   // текущее состояние
	strategy _strategy{strategy::rest}; // текущая стратегия
	int		 _mass{ 1 };     // масса организма существа
	int		 _energy{1000};   // энергия, позволяющая организму 
	int	     _temprature{20};    // температура тела существа
};

