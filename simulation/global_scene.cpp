#include "pch.h"
#include "global_scene.h"


global_scene::global_scene(int w, int h)
	: _width(w)
	, _height(h)
{
}

void global_scene::add(int n, species_ptr s)
{
	_species_list.push_back(s);

	for (int index = n; index-- > 0;)
	{
		_being_list.push_back(std::make_shared<being>(s));
	}
}

std::vector<being_ptr> const& global_scene::get_beings() const
{
	return _being_list;
}