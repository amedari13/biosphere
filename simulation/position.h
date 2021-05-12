#pragma once

class global_scene;

struct position
{
	double x;
	double y;

	double distance(global_scene*, position const& rhs);
	void move_by(global_scene*, double dx, double dy);
	void move_to(global_scene*, position const& other, double speed);
	void move_from(global_scene*, position const& other, double speed);
};

