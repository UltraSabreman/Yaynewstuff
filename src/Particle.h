#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include <cinder/Rand.h>
#include <cinder/Perlin.h>
#define CIRCLE_DETAIL 7
#define MASS_TO_RAD 1
using namespace ci;
using namespace ci::app;
using namespace std;


class Particle {
public:
	Particle(Vec2f pos, Vec2f vel = Vec2f(0,0), Vec2f accel = Vec2f(0,0), float mass = 5.0) {
		_pos = Vec2f(pos.x, app::getWindowHeight() - pos.y); //silly fbo, origin in lower left.
		_vel = vel;
		_col = Color(1.0f, 1.0f, 1.0f);//hsvToRGB(Vec3f(Rand::randFloat(), 1.0, 1.0));
		_mass = mass;
		_radius = _mass / MASS_TO_RAD;
		_accel = accel;
		_isDead = false;
	}
	void update(bool Paused) {
		_radius = _mass / MASS_TO_RAD;
		if (!Paused) {
			_vel += _accel;
			_pos += _vel;
		}
	}

	void draw(bool test) {
		gl::color(_col);
		gl::drawSolidCircle(_pos, _radius, CIRCLE_DETAIL);
		if (test) {
			gl::drawLine(_pos, _pos + _vel);
		}
	}

	bool isColliding(Particle *part2) {
		if ((_pos.distance(part2->getPos()) - _radius - part2->_radius) <= 0) 
			return true;
		return false;
	}

	void absorb(Particle *part2) {
		//perfectly inelestic collision, might change later to be partially inelestic.
		_vel = (_mass*_vel + part2->getMass()*part2->_vel) / (_mass + part2->getMass());

		_mass += part2->getMass();
		part2->_isDead = true;
	}


	void setMass(float mass) { _mass = (mass > 0 ? mass : 0); }
	void setPos(Vec2f pos) { 
		_pos.x = constrain(pos.x, 0.0f, (float)app::getWindowWidth()); 
		_pos.y = constrain(pos.y, 0.0f, (float)app::getWindowHeight()); 
	}
	
	Vec2f getPos() { return _pos; }
	float getMass() { return _mass; }

	bool _isDead;
	float _radius;
	Vec2f _vel, _accel;
protected:
	Vec2f _pos; //not public because we need to constrain it.
	Color _col;
	float _mass;
};

