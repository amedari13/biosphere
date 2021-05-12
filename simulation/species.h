#pragma once

#include "capability.h"

#include <memory>
#include <vector>

using species_ptr = std::shared_ptr< class species >;

inline void make_random(int& v)
{
	v = v * (rand() % 100 + 50) / 100;
}

class species
{
public:
	static species_ptr random()
	{
		species_ptr self = std::make_shared<species>();
		make_random(self->_treat_as_dangerous);
		make_random(self->_treat_as_yummy);
		make_random(self->_want_to_rest_limit);
		make_random(self->_mass_limit);
		make_random(self->_speed);
		return self;
	}

public:
	int		 _treat_as_dangerous = 150;
	int		 _treat_as_yummy = 70;
	int		 _want_to_rest_limit = 100;
	int		 _mass_limit{100};
	int		 _speed{3};
	std::vector<capability_ptr> _cap_list;
};

