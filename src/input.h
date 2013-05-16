#include <cinder/app/AppNative.h>

#define KB_KEYS 350
#define M_KEYS 5

using namespace ci;
using namespace ci::app;
using namespace std;

class Input {
public:
	Input();
	
	void mouseDown(MouseEvent event);	
	void mouseUp(MouseEvent event);	
	void mouseMove(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void mouseWheel(MouseEvent event);
	
	void keyUp(KeyEvent event);
	void keyDown(KeyEvent event);

	bool wasKeyPressed(int key);
	bool wasKeyReleased(int key);
	bool isKeyPressed(int key);

	bool wasMKeyPressed(int key);
	bool wasMKeyReleased(int key);
	bool isMKeyPressed(int key);

	float getWheelSpin();

	Vec2f getMousePos() { return _mousePos; }
	Vec2f getMouseVel() { return _mouseVel; }
	Vec2f getFboPos() { return _fboPos; }

protected:
	Vec2f _mousePos, _mouseVel, _fboPos;
	bool _onScreen;
	float _wheel;

	vector<vector<bool>> _kbKeys;
	vector<vector<bool>> _mKeys;


	int _getMouseButton(MouseEvent event);
};
