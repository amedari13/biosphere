#pragma once

#include "species.h"
#include "being.h"

#include <vector>
#include <map>

struct species_stat_entry
{
	int count{ 0 };
	int dead{ 0 };
	int avr_energy{ 0 };
	int max_energy{ 0 };
	int avr_mass{ 0 };
	int max_mass{ 0 };
};

struct global_scene_stats
{
    std::map<species_ptr, species_stat_entry > sstat;
};

class global_scene
{
public:
	global_scene(int w, int h);

	void add(int n, species_ptr s);
	void add(being_ptr b);
	void calculate();

	std::vector<being_ptr> const &get_beings() const;
	std::vector<species_ptr> const& get_species() const;
    global_scene_stats get_statistics() const;

	int get_width() const;
	int get_height() const;

private:
	std::vector<species_ptr> _species_list;
	std::vector<being_ptr> _being_list;

	int _width;
	int _height;
};

