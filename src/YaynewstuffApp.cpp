#include <cinder/gl/Fbo.h>
#include "ParticleController.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class YaynewstuffApp : public AppNative {
public:
	void setup();
	void prepareSettings(Settings *settings);

	//Pass all input to input class to be handed there.
	void mouseDown(MouseEvent event) { mainIO.mouseDown(event); }	
	void mouseUp(MouseEvent event) { mainIO.mouseUp(event); }	
	void keyUp(KeyEvent event) { mainIO.keyUp(event); }
	void keyDown(KeyEvent event) { mainIO.keyDown(event); }
	void mouseMove(MouseEvent event) { mainIO.mouseMove(event); }
	void mouseDrag( MouseEvent event) { mainIO.mouseDrag(event); }

	void update();
	void draw();
protected:
	ParticleController parts;
	gl::Fbo mainFBO;

	Input mainIO;
	bool pause;
};

void YaynewstuffApp::setup()
{
	mainFBO = gl::Fbo(800,600);
	pause = false;
}

void YaynewstuffApp::prepareSettings(Settings *settings){
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
}

void YaynewstuffApp::update()
{
	if (mainIO.wasKeyPressed(KeyEvent::KEY_SPACE))
		pause = !pause;

	if (mainIO.wasMKeyPressed(MouseEvent::RIGHT_DOWN))
		parts.getParticle(mainIO.getFboPos());

	if (mainIO.wasMKeyReleased(MouseEvent::RIGHT_DOWN))
		parts.resetTar();

	if (mainIO.isMKeyPressed(MouseEvent::RIGHT_DOWN)) {
		parts.update(pause, mainIO.getFboPos());	
	} else
		parts.update(pause);	

	

	if (mainIO.wasMKeyPressed(MouseEvent::LEFT_DOWN))
		parts.addParticle(mainIO.getFboPos());
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
