#pragma once

#include <vector>

#include "Capability.h"
#include "position.h"

using being_ptr = std::shared_ptr<class being>;

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

class being
{
public:
	being();
	virtual ~being();

	void appendCapability(capability_ptr);

	status calculate_next_step(environment env);
	status calculate_next_step(environment env);

private:	
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

