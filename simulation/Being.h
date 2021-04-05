#pragma once

#include <vector>

#include "Capability.h"
#include "position.h"
#include "environment.h"


enum class strategy
{
	rest,	// отдыхаем, набираемся сил
	wander, // бесцельно бродить
	afraid, // паника, убегаем
	hunt,   // пытаемся добраться до добычи
};

enum class status
{
	ok,		// продолжаем жить
	dead,	// умираем
	spawn,	// размножаться
};

const int mass_to_energy_factor = 10;

using being_ptr = std::shared_ptr< class being >;
using species_ptr = std::shared_ptr< class species >;

class being
{
public:
	being(species_ptr s);
	virtual ~being();

	void append_capability(capability_ptr);

	status calculate_next_step(environment env);

	position get_position() const;

private:	
	std::weak_ptr<species> _species;

	// переменные, описывающие состояние организма
	position _pos;	    // положение существа в симуляции
	strategy _strategy; // текущая стратегия
	int		 _mass;     // масса организма существа
	int		 _energy;   // энергия, позволяющая организму 
	int	     _temprature;    // температура тела существа

	// переменные, описывающие свойства организма
	int		 _mass_limit;
	int		 _speed;
	std::vector<capability_ptr> _cap_list;
};

