#include <cinder\app\AppNative.h>
#include <cinder\Utilities.h>
#include <cinder\Font.h>
#include <cinder\gl\gl.h>

#include <cinder\Camera.h>
#include <cinder\MayaCamUI.h>
#include <cinder\gl\GlslProg.h>

#include <cinder\params\Params.h>
#include <cinder\gl\Light.h>

#include "ParticleController.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class YaynewstuffApp : public AppNative {
public:
	void setup();
	void prepareSettings(Settings *settings);

	//Pass all input to input class to be handed there.
	void mouseDown(MouseEvent event)	{ myCamera.mouseDown(event.getPos()); mainIO.mouseDown(event); }	
	void mouseUp(MouseEvent event)		{ mainIO.mouseUp(event); }	
	void keyUp(KeyEvent event)			{ mainIO.keyUp(event); }
	void keyDown(KeyEvent event)		{ mainIO.keyDown(event); }
	void mouseMove(MouseEvent event)	{ mainIO.mouseMove(event); }
	void mouseDrag(MouseEvent event)	{ myCamera.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() ); mainIO.mouseDrag(event); }
	void mouseWheel(MouseEvent event)	{ mainIO.mouseWheel(event); }

	void resize()
	{
	// adjust aspect ratio
		CameraPersp cam = myCamera.getCamera();
		cam.setAspectRatio( getWindowAspectRatio() );
		myCamera.setCurrentCam( cam );
	}


	void update();
	void draw();
protected:
	ParticleController parts;
	Font myFont;

	Input mainIO;
	
	MayaCamUI myCamera;

	gl::GlslProg	mShader;

	params::InterfaceGlRef myParams;
	Quatf mSceneRotation;
	float mCameraDistance; //quaternions yay!

	Vec3f mEye, mCenter, mUp;

	GLuint pmat, vmat;

	bool pause;
};

void YaynewstuffApp::prepareSettings(Settings *settings) {
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
}

void YaynewstuffApp::setup() {	
	myFont = Font("Arial", 12);
	pause = false;

	mCameraDistance = 500.0f;
	mEye = Vec3f( 0.0f, 0.0f, mCameraDistance ); //eye Position,
	mCenter = Vec3f::zero(); //Eye dir
	mUp = Vec3f::yAxis(); //camera up

	try {
		mShader = gl::GlslProg( loadResource( RES_SHADER_VERT ), loadResource( RES_SHADER_FRAG ) );
	} catch(gl::GlslProgCompileExc shaderError) {
		console() << "Shader Compile error:" << endl;
		console() << shaderError.what() << endl;
	}

	//vmat = mShader.getUniformLocation("viewMatrix");
	//pmat = mShader.getUniformLocation("projMatrix");


	CameraPersp cam;
	cam.setPerspective(75.0f, getWindowAspectRatio(), 5.0f, 2000.0f);
	cam.setEyePoint(mEye);
	cam.setCenterOfInterestPoint(Vec3f::zero());
	myCamera.setCurrentCam(cam);
		//Field of fiew, apsect ration, near clipping plane, far cipping plane

	//myLight = gl::Light(gl::Light::POINT, 1);
	//myLight.enable();

	parts = ParticleController(&mainIO);
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

	// UPDATE CAMERA
	/*mEye = Vec3f( 0.0f, 0.0f, mCameraDistance );
	myCamera.lookAt( mEye, mCenter, mUp );
	gl::setMatrices( myCamera );
	gl::rotate( mSceneRotation );*/

	//myLight.lookAt(Vec3f(0,100, 100), Vec3f::zero());
	//myLight.update(myCamera);

	//pmat = myCamera.getCamera().
}


void YaynewstuffApp::draw() {
	gl::clear( Colorf(0.0f, 0.0f, 0.0f) );
	gl::pushMatrices();
	gl::setMatrices( myCamera.getCamera() );

	gl::enableDepthRead();
	gl::enableDepthWrite();

	mShader.bind();

		parts.draw();

	mShader.unbind();
	glColor4f( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
	//gl::drawString("FPS: " + toString(getAverageFps()), Vec2f(0,0), Color(1.0, 1.0, 1.0), myFont);
	
}

CINDER_APP_NATIVE(YaynewstuffApp, RendererGl)
	