#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include <cinder/Rand.h>
#include <cinder/Perlin.h>
#define CIRCLE_DETAIL 50
using namespace ci;
using namespace ci::app;
using namespace std;


class Particle {
public:
	Particle(Vec2f pos, Vec2f vel) {
		_pos = pos;
		_vel = vel;//Rand::randVec2f();//Rand::randFloat();
		_col = Color(1.0f, 1.0f, 1.0f);
		_mass = 2.0f;//cos(_pos.y * 0.1f ) + sin(_pos.x * 0.1f ) + 2.0f;
		_age = 0;
		_life = Rand::randInt( 50, 250 );
		_decay = Rand::randFloat(0.95, 0.995);
		_isDead = false;
	}
	void update(Vec2f mousePos, Channel32f &chan, Perlin &perlin) {
		_age++;
		if (_age >= _life) 
			_isDead = true;

		//_pos += _vel;
		//_vel *= _decay; //velocity decay
		float noise = perlin.fBm( Vec3f( _pos * 0.005f, app::getElapsedSeconds() * 0.1f ) );
		float angle = noise * 15.0f;
		_vel += Vec2f( cos( angle ), sin( angle ) ) * 0.2f * ( 1.0f - ((float)_age / (float)_life) );
		_pos += _vel;
	
		_col = hsvToRGB(Vec3f(((float)_age / (float)_life), 1.0, 1.0));
		/*_dirToMouse = mousePos - _pos;
		float time = app::getElapsedSeconds() * 4.0f;
		float dist = _dirToMouse.length() * 0.05f;
		float sinOffset = sin( dist - time );
		_dirToMouse.safeNormalize();

		_newPos = _pos + _dirToMouse * sinOffset;
		_newPos.x = constrain( _newPos.x, 0.0f, chan->getWidth() - 1.0f );
		_newPos.y = constrain( _newPos.y, 0.0f, chan->getHeight() - 1.0f );

		//float grey = chan->getValue(_pos);
		_mass = *chan->getData(_pos) * 7.0;
		_dirToMouse *= sinOffset * 15.0f;
		_col = Color(1, 1, 1);*/

	}

	void draw() {
		gl::color(_col);
		gl::drawSolidCircle(_pos + _dirToMouse, _mass * (1.0f-((float)_age / (float)_life)), 7);
	}

	bool _isDead;
protected:
	Vec2f _pos, _vel, _dirToMouse, _newPos;
	Color _col;
	float _mass, _decay;
	int _age, _life;
	
};

