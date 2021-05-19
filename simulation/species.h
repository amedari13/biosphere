#pragma once

#include "capability.h"

#include <memory>
#include <vector>

using species_ptr = std::shared_ptr< class species >;

inline void make_random(int& v)
{
	v = v * (rand() % 100 + 50) / 100;
}

constexpr int fear_default = 70;
constexpr int greed_default = 150;
constexpr int lazyness_default = 100;
constexpr int mass_limit_default = 100;
constexpr int speed_default = 3;

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
    int		 _fear = 150;
    int		 _greed = 70;
    int		 _lazyness = 100;
	int		 _mass_limit{100};
	int		 _speed{3};
	std::vector<capability_ptr> _cap_list;
};

