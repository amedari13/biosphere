#pragma once

#include "capability.h"

#include <memory>
#include <vector>

using species_ptr = std::shared_ptr< class species >;

class species
{
public:
	static species_ptr random()
	{
		return std::make_shared<species>();
	}

public:
	int		 _treat_as_dangerous = 150;
	int		 _treat_as_yummy = 70;
	int		 _want_to_rest_limit = 100;
	int		 _mass_limit{100};
	int		 _speed{3};
	std::vector<capability_ptr> _cap_list;
};

