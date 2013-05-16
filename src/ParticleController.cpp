#include "ParticleController.h"

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
		Particle *part1 = &_particles[i];
		for (int l = 0; l < size; l++) {
			Particle *part2 = &_particles[l];
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
				Particle *part2 = &_particles[l];
				if (part1 == part2) continue; 
				//Calculate unit direction vector form particle 1, to particle 2.
				Vec2f dirVec = (part1->getPos() - part2->getPos()).safeNormalized();
				//Get force vector using Newtons law of universal gravitation in vector form (G_ is the gravitational constant);
				AvgForce = -G_ * ((part1->getMass() * part2->getMass()) / pow(dirVec.length(), 2)) * dirVec;
			}

			AvgForce *= FORCE_MUL; // to amke teh force actualy noticible.
			if (!part1->test) {
				if (size > 1) 
					part1->_force = (AvgForce / (size - 1));
				else
					part1->_force = Vec2f(0.0,0.0);
			}else
				part1->test = false;
				
		}
	}

	//Do member updates:
	size = _particles.size(); //incase we removed a particle.
	for (int i = 0; i < size; i++) {
		Particle *part1 = &_particles[i];

		if (part1 == _target) {
			part1->_force = (newPos - part1->getPos()) * 10;
			part1->test = true;
			
		}
		part1->update(pausePhysics);
	}
}


void ParticleController::draw() {
	int size = _particles.size();
	for (int i = 0; i < size; i++)
		if (&_particles[i] == _target)
			_particles[i].draw(true);
		else
			_particles[i].draw(false);
					
}


void ParticleController::removeAllParticles() {
	_particles.clear();
}


void ParticleController::test(float w) {
	if (_target) 
		_target->setMass(_target->getMass() + (w * (_mainIO->isKeyPressed(KeyEvent::KEY_LSHIFT) ? 100 : 10)));
}

void ParticleController::addParticle(Vec2f pos) {
	_particles.push_back(Particle(pos));
}

Particle* ParticleController::getParticle(Vec2f pos) {
	int size = _particles.size();
	for (int i = 0; i < size; i++) {
		if (_particles[i].getPos().distance(pos) <= _particles[i]._radius) {
			_target = &_particles[i];
			return _target;
		}
	}
}

void ParticleController::removeParticle(Particle *tar) {
	int size = _particles.size();
	for (int i = 0; i < size; i++)
		if (&_particles[i] == tar)
			_eraseParticle(i);
}

void ParticleController::resetTar() { _target = NULL; }

void ParticleController::_eraseParticle(int loc) {
	if (loc < 0 || loc >= _particles.size()) return;

	vector<Particle>::iterator part = _particles.begin();
	advance(part, loc);
	_particles.erase(part);
}