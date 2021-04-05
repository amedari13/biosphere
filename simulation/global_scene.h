#pragma once

#include "species.h"
#include "being.h"

#include <vector>

class global_scene
{
public:
	global_scene(int w, int h);

	void add(int n, species_ptr s);
	void calculate();

	std::vector<being_ptr> const &get_beings() const;

private:
	std::vector<species_ptr> _species_list;
	std::vector<being_ptr> _being_list;

	int _width;
	int _height;
};

