#include "Particle.h"
#include <vector>
//This is in m^3/(kg*s^2)
#define G_ 6.67384e-11 
//This modifier makes the force actualy noticble.
#define ACCEL_MUL 1.0e7
using namespace ci;
using namespace ci::app;
using namespace std;

class ParticleController {
public:
	void update(bool pausePhysics, Vec2f newPos = Vec2f(0,0)) {
		vector<Vec2f> accelBuffer;
		pausePhysics = (_target ? true : pausePhysics);

		for (vector<Particle>::iterator part = _particles.begin(); part != _particles.end(); part++)
			accelBuffer.push_back(Vec2f(0,0));
		int vec = 0;
		for (vector<Particle>::iterator part = _particles.begin(); part != _particles.end(); vec++) {
			//Do collisions
			bool skip = false;
			for (vector<Particle>::iterator part2 = _particles.begin(); part2 != _particles.end();) {
				if (part == part2)  { part2++; continue; };

				//Might wanna switch this up to do a random absorb, or bounse, if masses ==
				if (part->isColliding(part2._Ptr)) {
					if (part->getMass() > part2->getMass()) {
						part->absorb(part2._Ptr);
						_particles.erase(part2);

						//this is probably a really silly solution
						skip = true;
						//continue;
						break;
					} else if (part->getMass() <= part2->getMass()) {
						part2->absorb(part._Ptr);
						_particles.erase(part);
				
						skip = true;
						break; //skip to next itereation of parent loop
					} 
				}else
					part2++;

				//TODO: optional collision with the walls, will be perfectly elastic, although could have some movement decay.
			}
			if (skip) break;
			
			//Do acceleration calculations.
			if (!pausePhysics) {
				Vec2f AvgAccel(0.0, 0.0);
				int numOfVec = 0;
				for (vector<Particle>::iterator part2 = _particles.begin(); part2 != _particles.end(); part2++) {
					if (part == part2) continue; 
					//Calculate unit direction vector form particle 1, to particle 2.
					Vec2f dirVec = (part->getPos() - part2->getPos()).safeNormalized();
					//Get force vector using Newtons law of universal gravitation in vector form (G_ is the gravitational constant);
					Vec2f force = -G_ * ((part->getMass() * part2->getMass()) / pow(dirVec.length(), 2)) * dirVec;

					//get particle1 acceleration using f=ma in vector form (we are calculating stuff for part 1 atm.
					AvgAccel += (force / part->getMass());
					numOfVec++;
				}
				if (numOfVec != 0) {
					AvgAccel /= numOfVec; //get avarage acceleration vector based on ALL planets acting on this one.
					AvgAccel *= ACCEL_MUL; // to amke teh force actualy noticible.
					accelBuffer[vec] = AvgAccel;//push a new one so we dont loose daa.
				}
			}
			part++; //if we skip an iteration by erasing, we dont want to mess up our iterator.
		}

		//Do memebr updates:
		int i = 0;
		for (vector<Particle>::iterator part = _particles.begin(); part != _particles.end(); part++, i++) {
			part->_accel = accelBuffer[i];
			if (part._Ptr == _target) {
				part->_accel = Vec2f(0,0);
				part->_vel =  newPos - part->getPos();
			}
			part->update(pausePhysics);
		}
	}

	void draw() {
		int size = _particles.size();
		for (int i = 0; i < size; i++)
			if (&_particles[i] == _target)
				_particles[i].draw(true);
			else
				_particles[i].draw(false);
					
	}

	void addParticles(Vec2f pos) {

		_particles.push_back(Particle(pos, Rand::randVec2f() * 0));
	}

	Particle* getParticle(Vec2f pos) {
		int size = _particles.size();
		for (int i = 0; i < size; i++) {
			if (_particles[i].getPos().distance(pos) <= _particles[i]._radius) {
				_target = &_particles[i];
				return _target;
			}
		}
	}
	void resetTar() { _target = NULL; }

	void removeParticles (int num) {
		for (int i = 0; i < num; i++)
			_particles.pop_back();
	}


protected:
	vector<Particle> _particles;
	Particle *_target;

};