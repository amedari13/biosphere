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
	int		 _mass_limit{1000};
	int		 _speed{3};
	std::vector<capability_ptr> _cap_list;
};

