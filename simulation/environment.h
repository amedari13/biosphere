#pragma once

#include <memory>
#include <vector>

class being;
using being_ptr = std::shared_ptr<being>;

struct environment
{
	int temperature;

	std::vector<being_ptr> neighbours;
};

