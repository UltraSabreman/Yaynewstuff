#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Path2d.h"
#include "cinder\Utilities.h"
#include "cinder\Font.h"
//#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "input.h"

#define CIRCLE_DETAIL 50
#define MASS_TO_RAD 1
#define PI 3.14159
#define DENCITY 1
using namespace ci;
using namespace ci::app;
using namespace std;


class Particle {
public:
	Particle(Vec2f pos, float mass = 500, Vec2f vel = Vec2f(0,0), Vec2f force = Vec2f(0,0)) {
		_pos = pos;
		_vel = vel;
		_col = hsvToRGB(Vec3f(Rand::randFloat(), 1.0, 1.0));
		_mass = mass;
		_radius = (pow(_mass, 1.0/3.0)/1) * (3.0/4.0) * PI;//log(_mass);
		_force = force;
		_accel = Vec2f(0.0,0.0);
		_path.moveTo(_pos);
		_fbo = gl::Fbo(800,600);
		_oldPos = pos;
		test = false;
	}
	void update(bool Paused) {
		_radius = (pow(_mass, 1.0/3.0)/DENCITY) * (3.0/4.0) * PI; //log(_mass);
		if (!Paused) {
			_accel = (_force / _mass);
			//_vel += _accel;
			//_pos += _vel;
			_vel = (_pos - _oldPos);
			Vec2f newPos = _pos + _vel + _accel;
			_oldPos = _pos;
			_pos = newPos;
		}
		if (_pos.distance(_oldPos) >= 0.5)//sds
			_path.lineTo(_pos);

	}

	void draw(bool test) {
		gl::color(_col);
		gl::drawSolidCircle(_pos, _radius);
		gl::draw(_path);
		//gl::drawStringCentered(toString(_mass), _pos, Color(1.0, 0.0, 0.0), Font("Arial", 12));
		if (test) {
			gl::color(Color(1.0, 1.0, 1.0));
			gl::drawLine(_pos, _pos + _force/10);
			gl::drawStrokedCircle(_pos, _radius+10);
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
	}


	void setMass(float mass) { _mass = (mass > 0 ? mass : 0); }
	void setPos(Vec2f pos) { 
		_pos.x = constrain(pos.x, 0.0f, (float)app::getWindowWidth()); 
		_pos.y = constrain(pos.y, 0.0f, (float)app::getWindowHeight()); 
	}
	
	Vec2f getPos() { return _pos; }
	float getMass() { return _mass; }

	float _radius;
	Vec2f _vel, _accel, _force, _oldPos;
	bool test;
protected:
	Vec2f _pos; //not public because we need to constrain it.
	Color _col;
	float _mass;
	Path2d _path;

	gl::Fbo _fbo;
};

