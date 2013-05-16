#ifndef __LOL__
#define __LOL__
#include <cinder\app\AppNative.h>
#include <vector>
#include "Particle.h"
#include "input.h"

//This is in m^3/(kg*s^2)
#define G_ 6.67384e-11 
//This modifier makes the force actualy noticble.
#define FORCE_MUL 1e6

using namespace ci;
using namespace ci::app;
using namespace std;

class ParticleController {
public:
	ParticleController(Input *l = NULL);
	void update(bool pausePhysics, Vec2f newPos = Vec2f());
	void addParticle(Vec2f pos);
	Particle* getParticle(Vec2f pos);
	void removeParticle(Particle *tar);
	void removeAllParticles();
	void resetTar();

	void draw();
protected:
	vector<Particle*> _particles;
	Particle *_target;
	Input *_mainIO;

	void _eraseParticle(unsigned int loc);
};


ParticleController::ParticleController(Input *l) {
	_target = NULL;
	_mainIO = l;
}

void ParticleController::update(bool pausePhysics, Vec2f newPos) {
	int size = _particles.size();
	pausePhysics = (_target ? true : pausePhysics);

	for (int i = 0; i < size; i++) {
		//Do collisions
		bool skip = false;
		Particle *part1 = _particles[i];
		for (int l = 0; l < size; l++) {
			Particle *part2 = _particles[l];
			if (part1 == part2)  { continue; };

			//Might wanna switch this up to do a random absorb, or bounse, if masses ==
			if (part1->isColliding(part2)) {
				if (part1->getMass() > part2->getMass()) {
					part1->absorb(part2);
					_eraseParticle(l);

					skip = true; //so we get out compleatly.
					break;
				} else if (part1->getMass() <= part2->getMass()) {
					part2->absorb(part1);
					_eraseParticle(i);
				
					skip = true;
					break; //skip to next itereation of parent loop
				} 
			}
			//TODO: optional collision with the walls, will be perfectly elastic, although could have some movement decay.
		}
		if (skip) break;
			
		//Do Force calculations
		if (!pausePhysics) {
			Vec2f AvgForce(0.0, 0.0);
			for (int l = 0; l < size; l++) {
				Particle *part2 = _particles[l];
				if (part1 == part2) continue; 
				//Calculate unit direction vector form particle 1, to particle 2.
				Vec2f dirVec = (part1->getPos() - part2->getPos()).safeNormalized();
				//Get force vector using Newtons law of universal gravitation in vector form (G_ is the gravitational constant);
				AvgForce = -G_ * ((part1->getMass() * part2->getMass()) / pow(dirVec.length(), 2)) * dirVec;
			}

			AvgForce *= FORCE_MUL; // to amke teh force actualy noticible.
			if (!part1->forceSet) {
				if (size > 1) 
					part1->force = (AvgForce / (size - 1));
				else
					part1->force = Vec2f(0.0,0.0);
			}else
				part1->forceSet = false;
				
		}
	}

	//Do member updates:
	size = _particles.size(); //incase we removed a particle.
	for (int i = 0; i < size; i++) {
		Particle *part1 = _particles[i];

		if (part1 == _target) {
			int massDelta = _mainIO->isKeyPressed(KeyEvent::KEY_LSHIFT) ? 100 : 10;
			float newMass = _target->getMass() + (_mainIO->getWheelSpin() * massDelta);
			part1->setMass(newMass);

			part1->force = (newPos - part1->getPos()) * 10;
			part1->forceSet = true;
			
		}
		part1->update(pausePhysics);
	}
}

void ParticleController::draw() {
	int size = _particles.size();
	for (int i = 0; i < size; i++)
		if (_particles[i] == _target)
			_particles[i]->draw(true);
		else
			_particles[i]->draw(false);
					
}

void ParticleController::removeAllParticles() {
	_particles.clear();
}

void ParticleController::addParticle(Vec2f pos) {
	_particles.push_back(new Particle(pos));
}

Particle* ParticleController::getParticle(Vec2f pos) {
	int size = _particles.size();
	for (int i = 0; i < size; i++) {
		if (_particles[i]->getPos().distance(pos) <= _particles[i]->getRadius()) {
			_target = _particles[i];
			return _target;
		}
	}
}

void ParticleController::removeParticle(Particle *tar) {
	int size = _particles.size();
	for (int i = 0; i < size; i++)
		if (_particles[i] == tar)
			_eraseParticle(i);
}

void ParticleController::resetTar() { _target = NULL; }

void ParticleController::_eraseParticle(unsigned int loc) {
	if (loc < 0 || loc >= _particles.size()) return;

	vector<Particle*>::iterator part = _particles.begin();
	advance(part, loc);
	_particles.erase(part);
}

#endif
