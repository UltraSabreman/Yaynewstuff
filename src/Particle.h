#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Path2d.h"
//#include "cinder/ImageIo.h"
//#include "cinder/gl/Texture.h"
#include "input.h"

#define CIRCLE_DETAIL 7
#define MASS_TO_RAD 1
using namespace ci;
using namespace ci::app;
using namespace std;


class Particle {
public:
	Particle(Vec2f pos, float mass = 50000, Vec2f vel = Vec2f(0,0), Vec2f force = Vec2f(0,0)) {
		_pos = pos;
		_vel = vel;
		_col = Color(1.0f, 1.0f, 1.0f);//hsvToRGB(Vec3f(Rand::randFloat(), 1.0, 1.0));
		_mass = mass;
		_radius = log(_mass);
		_force = force;
		_accel = Vec2f(0.0,0.0);
		_path.moveTo(_pos);
	}
	void update(bool Paused) {
		_path.lineTo(_pos);
		_accel = (_force / _mass);
		_radius = log(_mass);
		if (!Paused) {
			_vel += _accel;
			_pos += _vel;
		}
	}

	void draw(bool test) {
		gl::color(_col);
		gl::drawSolidCircle(_pos, _radius, CIRCLE_DETAIL);
		gl::draw(_path);
		if (test) 
			gl::drawLine(_pos, _pos + _vel * 10);
		
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
	}


	void setMass(float mass) { _mass = (mass > 0 ? mass : 0); }
	void setPos(Vec2f pos) { 
		_pos.x = constrain(pos.x, 0.0f, (float)app::getWindowWidth()); 
		_pos.y = constrain(pos.y, 0.0f, (float)app::getWindowHeight()); 
	}
	
	Vec2f getPos() { return _pos; }
	float getMass() { return _mass; }

	float _radius;
	Vec2f _vel, _accel, _force;
protected:
	Vec2f _pos; //not public because we need to constrain it.
	Color _col;
	float _mass;
	Path2d _path;
};

