#include "Particle.h"
#include <vector>
using namespace ci;
using namespace ci::app;
using namespace std;

class ParticleController {
public:
	//ParticleController();
	void update(Vec2f mousePos, Channel32f &chan, Perlin &perlin) {
		//using namespace std;
		for (vector<Particle>::iterator part = _particles.begin(); part != _particles.end();) {
			if (part->_isDead) 
				part = _particles.erase(part);
			else {
				part->update(mousePos, chan, perlin);
				part++;
			}
		}
	}

	void draw() {
		int size = _particles.size();
		for (int i = 0; i < size; i++)
			_particles[i].draw();
	}

	void addParticles(int num, Vec2f pos, Vec2f vel) {
		for (int i = 0; i < num; i++) {
			Vec2f randVec = Rand::randVec2f() * 10.0f;
			Vec2f velOffset = Rand::randVec2f() * Rand::randFloat( 1.0f, 3.0f );
			Vec2f vel2 = vel * 0.25f + velOffset;
			_particles.push_back(Particle(pos + randVec, vel2 * 0.25));
		}
	}

	/*void ParticleController::addParticle( int xi, int yi) {
		float x = ( xi + 0.5f ) * 10;
		float y = ( yi + 0.5f ) * 10;
		_particles.push_back(Particle(Vec2f(x, y)));
	}*/

	void removeParticles (int num) {
		for (int i = 0; i < num; i++)
			_particles.pop_back();
	}


protected:
	vector<Particle> _particles;

};