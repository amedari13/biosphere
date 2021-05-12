#include "pch.h"

#include "position.h"
#include "global_scene.h"

#include <stdlib.h>
#include <assert.h>

double fmod_positive(double x, double w)
{
	x = fmod(x, w);
	if (x < 0) return x + w;
	return x;
}

double zero_based_fmod(double x, double w)
{
	return fmod(fmod_positive(x, w) + w / 2, w) - w / 2;
}


void position::move_to(global_scene* scene, position const& other, double speed)
{
	double dx = zero_based_fmod(x - other.x, scene->get_width());
	double dy = zero_based_fmod(y - other.y, scene->get_height());
	double d = sqrt(dx * dx + dy * dy);
	if (d > 0)
		move_by(scene,
			-dx / d * speed,
			-dy / d * speed);
}

void position::move_from(global_scene* scene, position const& other, double speed)
{
	double dx = zero_based_fmod(x - other.x, scene->get_width());
	double dy = zero_based_fmod(y - other.y, scene->get_height());
	double d = sqrt(dx * dx + dy * dy);
	if (d>0)
		move_by(scene,
			dx / d * speed,
			dy / d * speed);
}

void position::move_by(global_scene* scene, double dx, double dy)
{
	x = fmod_positive(x + dx, scene->get_width());
	y = fmod_positive(y + dy, scene->get_height());

	assert(!_isnan(x) && !_isnan(y));
}

double position::distance(global_scene* scene, position const& other)
{
	double dx = zero_based_fmod(x - other.x, scene->get_width());
	double dy = zero_based_fmod(y - other.y, scene->get_height());

	return sqrt(dx * dx + dy * dy);
}

///////////////////////////////////////////////////////

#define TEST(n) struct n { public: n(); } _##n; n::n()
bool equals_(double a, double b) { return fabs(a - b) < 0.01; }
void assert_equals(double a, double b) { assert(equals_(a, b)); }

///////////////////////////////////////////////////////

namespace
{
	TEST(test_zero_based_fmod)
	{
		assert_equals(1, zero_based_fmod(1, 100));
		assert_equals(-1, zero_based_fmod(-1, 100));
		assert_equals(1, zero_based_fmod(101, 100));
		assert_equals(-1, zero_based_fmod(99, 100));
		assert_equals(1, zero_based_fmod(-99, 100));
		assert_equals(-1, zero_based_fmod(-101, 100));
	}
	TEST(test_position1)
	{
		global_scene g{ 100, 100 };
		position a{ 1, 1 };
		position b{ 2, 2 };
		assert_equals(sqrt(2), a.distance(&g, b));

		a.move_to(&g, b, sqrt(2));
		assert_equals(a.x, 2);
		assert_equals(a.y, 2);

		position c{ 1, 1 };
		b.move_from(&g, c, sqrt(2));
		assert_equals(b.x, 3);
		assert_equals(b.y, 3);
	}

	TEST(test_position2)
	{
		global_scene g{ 100, 100 };
		position a{ 0, 0 };
		position b{ 99, 99 };
		assert_equals(sqrt(2), a.distance(&g, b));

		a.move_to(&g, b, sqrt(2));
		assert_equals(a.x, 99);
		assert_equals(a.y, 99);

		position c{ 0, 0 };
		c.move_from(&g, b, sqrt(2));
		assert_equals(c.x, 1);
		assert_equals(c.y, 1);
	}
}