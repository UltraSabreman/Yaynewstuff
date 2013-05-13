#include <cinder/gl/Fbo.h>
#include "ParticleController.h"


using namespace ci;
using namespace ci::app;
using namespace std;


class YaynewstuffApp : public AppNative {
public:
	void setup();
	void prepareSettings(Settings *settings);
	void mouseDown(MouseEvent event);	
	void mouseUp(MouseEvent event);	
	void keyUp(KeyEvent event);
	void mouseMove(MouseEvent event);
	void mouseDrag( MouseEvent event);
	void update();
	void draw();
protected:
	ParticleController parts;
	Vec2f mousePos, mouseVel, test;
	gl::Fbo mainFBO;
	Particle *lol;
	bool isMouseDown;
	bool pause;
	
};

void YaynewstuffApp::keyUp(KeyEvent event) {
	if (event.getCode() == KeyEvent::KEY_SPACE) 
		pause = !pause;
}

void YaynewstuffApp::mouseDrag( MouseEvent event ) {
	mouseMove(event);
}

void YaynewstuffApp::mouseMove(MouseEvent event) {
	Vec2f newPos = event.getPos();
	mouseVel = newPos - mousePos;
	mousePos = newPos;
}

void YaynewstuffApp::setup()
{
	mainFBO = gl::Fbo(800,600);
	isMouseDown = false;
	pause = false;
}

void YaynewstuffApp::prepareSettings(Settings *settings){
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
}

void YaynewstuffApp::mouseDown(MouseEvent event)
{
	if (event.isLeft()) {
		if (!isMouseDown) 
			lol = parts.getParticle(mousePos);
		isMouseDown = true;
	
	}
}

void YaynewstuffApp::mouseUp(MouseEvent event)
{
	if (event.isLeft()) {
		if (isMouseDown) {
			parts.resetTar();
			lol = NULL;
		}
		isMouseDown = false;

	}
	if (!lol)
		parts.addParticles(mousePos);
}

void YaynewstuffApp::update()
{
	if (isMouseDown)
		parts.update(pause, test);	
	else
		parts.update(pause);	


}


void YaynewstuffApp::draw()
{
	mainFBO.bindFramebuffer();
	gl::clear();

	parts.draw();

	mainFBO.unbindFramebuffer();
	gl::draw(mainFBO.getTexture());

}

CINDER_APP_NATIVE(YaynewstuffApp, RendererGl)
