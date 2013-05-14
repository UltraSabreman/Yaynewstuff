#include <cinder/gl/Fbo.h>
#include "ParticleController.h"
#include "cinder\Utilities.h"
#define SAMPLES 100

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

	float test[SAMPLES];
	int index;
	float getFps() {
		float total = 0; 
		for (int i = 0; i < SAMPLES; i++) 
			total += test[i];
		return (total / SAMPLES);
	}
};

void YaynewstuffApp::setup()
{
	mainFBO = gl::Fbo(800,600);
	pause = false;
	index = 0;

	for (int i = 0; i < SAMPLES; i++)
		test[i] = 0;
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
		//for (int i = 0; i < 1000; i++)
			parts.update(pause);		

	if (mainIO.wasMKeyPressed(MouseEvent::LEFT_DOWN))
		parts.addParticle(mainIO.getFboPos());
}


void YaynewstuffApp::draw()
{
	float start = clock();
	mainFBO.bindFramebuffer();
	gl::clear(Color(0.0,0.0,0.0), true);

	parts.draw();
	

	mainFBO.unbindFramebuffer();

	gl::draw(mainFBO.getTexture());
	gl::drawString("FPS: " + toString(getFps()), Vec2f(0,0), Color(1.0, 1.0, 1.0));
	test[index++] = ((clock() - start) / CLOCKS_PER_SEC);
	if (index == 100) index = 0;

}

CINDER_APP_NATIVE(YaynewstuffApp, RendererGl)
