#include "pch.h"
#include "species.h"

static std::vector<std::string> names1 =
{
    "Black", "White", "Noble", "Wild", "Regular", "European", "Long-tailed",
    "Dwarf", "Giant", "Golden", "Brown", "Sabre-toothed", "Cavern", "Australian"
};

static std::vector<std::string> names2 =
{
    "wolf", "fox", "bear", "deer", "hare", "rabbit", "eagle", "chicken", "duck",
    "elephant", "hippo", "lion", "grasshopper", "fish", "fly", "sloth"
};

species_ptr species::random()
{
    species_ptr self = std::make_shared<species>();
    make_random(self->_treat_as_dangerous);
    make_random(self->_treat_as_yummy);
    make_random(self->_want_to_rest_limit);
    make_random(self->_mass_limit);
    make_random(self->_speed);
    self->set_color( rand() % 16 );
    self->set_name( names1[ rand() % names1.size()]
                    + " "
                    + names2[ rand() % names2.size()]);
    return self;
}

int species::get_id() const
{
    return _id;
}

void species::set_id(int id)
{
    _id = id;
}

void species::set_name(const std::string &name)
{
    _name = name;
}

int species::get_color() const
{
    return _color;
}

void species::set_color(int color)
{
    _color = color;
}

std::string species::get_name() const
{
    return _name;
}
