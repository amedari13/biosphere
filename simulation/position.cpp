#include "pch.h"
#include "position.h"

#include "global_scene.h"
#include <stdlib.h>

void position::move_to(global_scene* scene, position const& other, double speed)
{
	double dx = x - other.x;
	if (dx > scene->get_width() / 2)
		dx = scene->get_width() - dx;
	else if (-dx > scene->get_width() / 2)
		dx = -scene->get_width() + dx;

	double dy = y - other.y;
	if (dy > scene->get_height() / 2)
		dy = scene->get_height() - dy;
	else if (-dy > scene->get_height() / 2)
		dy = -scene->get_height() + dy;

	double d = sqrt(dx * dx + dy * dy);

	x -= dx / d * speed;
	y -= dy / d * speed;
}

void position::move_from(global_scene* scene, position const& other, double speed)
{
	double dx = x - other.x;
	if (dx > scene->get_width() / 2)
		dx = scene->get_width() - dx;
	else if (-dx > scene->get_width() / 2)
		dx = -scene->get_width() + dx;

	double dy = y - other.y;
	if (dy > scene->get_height() / 2)
		dy = scene->get_height() - dy;
	else if (-dy > scene->get_height() / 2)
		dy = -scene->get_height() + dy;

	double d = sqrt(dx * dx + dy * dy);

	x += dx / d * speed;
	y += dy / d * speed;
}


double position::distance(global_scene* scene, position const& rhs)
{
	double dx = abs(x - rhs.x);
	if (dx > scene->get_width() / 2)
		dx = scene->get_width() - dx;
	double dy = abs(y - rhs.y);
	if (dy > scene->get_height() / 2)
		dy = scene->get_height() - dy;

	return sqrt(dx * dx + dy * dy);
}

///////////////////////////////////////////////////////

#define TEST(n) struct n { public: n(); } _##n; n::n()
bool equals_(double a, double b) { return fabs(a - b) < 0.01; }

#include <assert.h>

namespace
{
	TEST(test_position)
	{
		global_scene g{ 100, 100 };
		position a{ 1, 1 };
		position b{ 2, 2 };
		assert(equals_(1.41, a.distance(&g, b)));

		a.move_to(&g, b, 1.41);
		assert(equals_(a.x, 2) && equals_(a.y, 2));

		position c{ 1, 1 };
		b.move_from(&g, c, 1.41);
		assert(equals_(b.x, 3) && equals_(b.y, 3));
	}

	TEST(test_position2)
	{
		global_scene g{ 100, 100 };
		position a{ 0, 0 };
		position b{ 99, 99 };
		assert(equals_(1.41, a.distance(&g, b)));

		//a.move_to(&g, b, 1.41);
		//assert(equals_(a.x, 2) && equals_(a.y, 2));
	}
}