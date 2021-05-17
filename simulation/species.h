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
    static species_ptr random();

    int get_id() const;
    void set_id(int);

    std::string get_name() const;
    void set_name(std::string const & n);

    int get_color() const;
    void set_color(int color);

private:
    int _id{0};
    int _color{0};
    std::string _name;

public:
	int		 _treat_as_dangerous = 150;
	int		 _treat_as_yummy = 70;
	int		 _want_to_rest_limit = 100;
	int		 _mass_limit{100};
	int		 _speed{3};
	std::vector<capability_ptr> _cap_list;
};

