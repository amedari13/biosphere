#pragma once

#include <memory>

using species_ptr = std::shared_ptr< class species >;

class species
{
public:
	static species_ptr random()
	{
		return std::make_shared<species>();
	}
};

