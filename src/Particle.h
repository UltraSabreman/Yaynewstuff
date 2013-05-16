#pragma once
#include <cinder\app\AppNative.h>
#include <cinder\gl\gl.h>
#include <cinder\Path2d.h>
#include <cinder\Rand.h>
#include "input.h"

#define PI 3.14159
#define DENSITY 1

using namespace ci;
using namespace ci::app;
using namespace std;


class Particle {
public:
	Particle(Vec2f pos, float mass = 500, Vec2f vel = Vec2f(0,0), Vec2f force = Vec2f(0,0));
	void update(bool Paused);
	void draw(bool isSelected);
	bool isColliding(Particle *part2);
	void absorb(Particle *part2);
	void setMass(float mass);
	void setPos(Vec2f pos);

	Vec2f getPos() { return _pos; }
	float getMass() { return _mass; }

	float _radius;
	Vec2f force;
	bool test;
protected:
	Vec2f _pos, _oldPos, _accel, _vel; //not public because we need to constrain it.
	Color _col;
	float _mass;
	Path2d _path;
};

