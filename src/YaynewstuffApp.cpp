
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
	void keyDown(KeyEvent event);
	void mouseMove(MouseEvent event);
	void mouseDrag( MouseEvent event);
	void update();
	void draw();
protected:
	Channel32f chan;
	ParticleController parts;
	Vec2f mousePos, mouseVel;
	Perlin _perlin;
	gl::Texture myText;

	bool isMouseDown;

	void getRandomPic();
	
};

void YaynewstuffApp::getRandomPic() {
	console() << "Finding valid image" << endl;
	srand(time(NULL));
	bool exit = false;

	while (!exit) {
		string lol = "http://i.imgur.com/";
		//int times = rand() % 4 + 6;
		for (int i = 0; i < 5; i++) {
			switch (rand() % 3) {
				case 0: {
					lol += (char)(rand() % 10 + 48);
					break;
				} case 1: {
					lol += (char)(rand() % 26 + 65);
					break;
				} case 2: {
					lol += (char)(rand() % 26 + 97);
					break;
				}
				default: break;
			}
		}
		lol += ".jpg";
		console() << "New URL: " << lol << endl;
		exit = true;
		try {
			myText = gl::Texture(loadImage(loadUrl(lol)));
			setWindowSize(myText.getWidth(), myText.getHeight());
		}
		catch( ... ) {
			console() << "Failed to load" << std::endl;
			exit = false;
		}
	}
}
void YaynewstuffApp::keyDown(KeyEvent event) {

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
	_perlin = Perlin();
	isMouseDown = false;
	//Url url( "http://www.flight404.com/_images/paris.jpg");
	//chan = Channel32f(loadImage(loadUrl(url)));
}

void YaynewstuffApp::prepareSettings(Settings *settings){
	settings->setWindowSize(800, 600);
	settings->setFrameRate(60.0f);
}

void YaynewstuffApp::mouseDown(MouseEvent event)
{
	//getRandomPic();
	isMouseDown = true;
}

void YaynewstuffApp::mouseUp(MouseEvent event)
{
	getRandomPic();
	isMouseDown = false;
}

void YaynewstuffApp::update()
{
//	parts.update(mousePos, chan, _perlin);
//	if(isMouseDown)
//		parts.addParticles(5, mousePos, mouseVel);
}


void YaynewstuffApp::draw()
{
	gl::clear();

	if (myText)
		gl::draw(myText, getWindowBounds());

//	parts.draw();
}

CINDER_APP_NATIVE(YaynewstuffApp, RendererGl)
