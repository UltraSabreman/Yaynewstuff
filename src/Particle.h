#include <cinder\app\AppNative.h>
#include <cinder\gl\gl.h>
#include <cinder\Path2d.h>
#include <cinder\Rand.h>

#define PI 3.14159
#define DENSITY 1

using namespace ci;
using namespace ci::app;
using namespace std;

class Particle {
public:
	Particle(Vec2f pos, float mass = 500, Vec2f vel = Vec2f(), Vec2f force = Vec2f());
	void update(bool Paused);//
	void draw(bool isSelected);//
	bool isColliding(Particle *part2);//
	void absorb(Particle *part2);//
	void setMass(float mass);//
	void setPos(Vec2f pos);//

	Vec2f getPos();//
	float getMass();//
	float getRadius();//

	//could use setters and getters, but there are not extra checks that need to run on this.
	//so it doesnt matter.
	Vec2f force; 
	bool forceSet;
protected:
	Vec2f _pos, _oldPos, _accel, _vel;
	Color _col;
	float _mass, _radius;
	Path2d _path;
};



Particle::Particle(Vec2f pos, float mass, Vec2f vel, Vec2f inforce) {
	_pos = pos; 
	_oldPos = pos;
	_vel = vel;
	_mass = mass;
	force = inforce;
		
	_col = hsvToRGB(Vec3f(Rand::randFloat(), 1.0, 1.0));
	_accel = Vec2f();
	_path.moveTo(_pos);
	_radius = 0;
	forceSet = false;
}

void Particle::update(bool Paused) {
	_radius = (pow(_mass, 1.0/3.0)/DENSITY) * (3.0/4.0) * PI;

	if (!Paused) {
		_accel = (force / _mass);
		_vel = (_pos - _oldPos);
		Vec2f newPos = _pos + _vel + _accel;
		_oldPos = _pos;
		_pos = newPos;
	}

	if (_pos.distance(_oldPos) >= 0.5)
		_path.lineTo(_pos);
}

void Particle::draw(bool isSelected) {
	gl::color(_col);
	gl::drawSolidCircle(_pos, _radius);
	gl::draw(_path);
	if (isSelected) {
		gl::color(Color(1.0, 1.0, 1.0));
		gl::drawLine(_pos, _pos + force/10);
		gl::drawStrokedCircle(_pos, _radius+10);
	}
}

bool Particle::isColliding(Particle *part2) {
	if ((_pos.distance(part2->getPos()) - _radius - part2->_radius) <= 0) 
		return true;
	return false;
}

void Particle::absorb(Particle *part2) {
	//perfectly inelestic collision, might change later to be partially inelestic.
	_vel = (_mass*_vel + part2->getMass()*part2->_vel) / (_mass + part2->getMass());

	_mass += part2->getMass();
}

void Particle::setMass(float mass) { 
	_mass = (mass > 0 ? mass : 0); 
}

void Particle::setPos(Vec2f pos) { 
	_pos.x = constrain(pos.x, 0.0f, (float)app::getWindowWidth()); 
	_pos.y = constrain(pos.y, 0.0f, (float)app::getWindowHeight()); 
}

float Particle::getRadius() {
	return _radius; 
}

float Particle::getMass() {
	return _mass;
}


Vec2f Particle::getPos() {
	return _pos;
}