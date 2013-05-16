#include <cinder\app\AppNative.h>
#include <cinder\Utilities.h>
#include <cinder\Font.h>
#include <cinder\gl\gl.h>
#include "ParticleController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class YaynewstuffApp : public AppNative {
public:
	void setup();
	void prepareSettings(Settings *settings);

	//Pass all input to input class to be handed there.
	void mouseDown(MouseEvent event)	{ mainIO.mouseDown(event); }	
	void mouseUp(MouseEvent event)		{ mainIO.mouseUp(event); }	
	void keyUp(KeyEvent event)			{ mainIO.keyUp(event); }
	void keyDown(KeyEvent event)		{ mainIO.keyDown(event); }
	void mouseMove(MouseEvent event)	{ mainIO.mouseMove(event); }
	void mouseDrag(MouseEvent event)	{ mainIO.mouseDrag(event); }
	void mouseWheel(MouseEvent event)	{ mainIO.mouseWheel(event); }

	void update();
	void draw();
protected:
	ParticleController parts;
	Font myFont;

	Input mainIO;
	bool pause;
};

void YaynewstuffApp::setup() {	
	myFont = Font("Arial", 12);
	pause = false;

	parts = ParticleController(&mainIO);
}

void YaynewstuffApp::prepareSettings(Settings *settings) {
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
}

void YaynewstuffApp::update() {
	if (mainIO.wasKeyPressed(KeyEvent::KEY_SPACE))
		pause = !pause;

	if (mainIO.wasMKeyPressed(MouseEvent::RIGHT_DOWN))
		 parts.getParticle(mainIO.getMousePos());

	if (mainIO.wasMKeyReleased(MouseEvent::RIGHT_DOWN))
		parts.resetTar();

	if (mainIO.isMKeyPressed(MouseEvent::RIGHT_DOWN)) 
		parts.update(pause, mainIO.getMousePos());	
	else
		parts.update(pause);

	if (mainIO.wasMKeyPressed(MouseEvent::LEFT_DOWN))
		parts.addParticle(mainIO.getMousePos());

	if (mainIO.wasMKeyPressed(MouseEvent::MIDDLE_DOWN))
		parts.removeParticle(parts.getParticle(mainIO.getMousePos()));

	if (mainIO.wasKeyPressed(KeyEvent::KEY_RETURN))
		parts.removeAllParticles();
}


void YaynewstuffApp::draw() {
	gl::clear();
	parts.draw();

	gl::drawString("FPS: " + toString(getAverageFps()), Vec2f(0,0), Color(1.0, 1.0, 1.0), myFont);
}

CINDER_APP_NATIVE(YaynewstuffApp, RendererGl)
