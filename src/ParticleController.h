#include "Particle.h"
#include <vector>

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
	void update(bool pausePhysics, Vec2f newPos = Vec2f(0,0));
	void changeMass(float w);
	void addParticle(Vec2f pos);
	Particle* getParticle(Vec2f pos);
	void removeParticle(Particle *tar);
	void removeAllParticles();
	void resetTar();

	void draw();
protected:
	vector<Particle> _particles;
	Particle *_target;
	Input *_mainIO;

	void _eraseParticle(int loc);
};