#include "input.h"


Input::Input() {
	_kbKeys.resize(KB_KEYS);
	for (int i = 0; i < KB_KEYS; i++) {
		_kbKeys[i].resize(3);
		for (int l = 0; l < 3; l++)
			_kbKeys[i][l] = false;
	}

	_mKeys.resize(M_KEYS);
	for (int i = 0; i < M_KEYS; i++) {
		_mKeys[i].resize(3);
		for (int l = 0; l < 3; l++)
			_mKeys[i][l] = false;
	}

	_wheel = 0;
}


int Input::_getMouseButton(MouseEvent event) {
	if (event.isLeft())
		return MouseEvent::LEFT_DOWN;
	else if(event.isRight())
		return MouseEvent::RIGHT_DOWN;
	else if(event.isMiddle())
		return MouseEvent::MIDDLE_DOWN;
	else 
		return -1;
}

void Input::keyDown(KeyEvent event) {
	int code = event.getCode();
	_kbKeys[code][0] = true; //is pressed
	_kbKeys[code][1] = true; //was pressed 
	_kbKeys[code][2] = false; //was released;
}

void Input::keyUp(KeyEvent event) {
	int code = event.getCode();
	_kbKeys[code][0] = false;
	_kbKeys[code][2] = true;
	
}

void Input::mouseDown(MouseEvent event) {
	int code = _getMouseButton(event);
	if (code == -1) return;
	_mKeys[code][0] = true;
	_mKeys[code][1] = true;
	_mKeys[code][2] = false;
}

void Input::mouseUp(MouseEvent event) {
	int code = _getMouseButton(event);
	if (code == -1) return;
	_mKeys[code][0] = false;
	_mKeys[code][2] = true;
}

void Input::mouseWheel(MouseEvent event) {
	_wheel = event.getWheelIncrement();
}

void Input::mouseDrag( MouseEvent event ) {
	mouseMove(event);
}

void Input::mouseMove(MouseEvent event) {
	_mouseVel = event.getPos() - _mousePos;
	_mousePos = event.getPos();
	//since fbos flip the y axis (0,0 is in bottm left corner) we gotta subtract form windwo hieght;
	_fboPos = Vec2f(_mousePos.x, app::getWindowHeight() - _mousePos.y); 
}

bool Input::wasKeyPressed(int key) {
	bool out = false;
	if (key >= 0 && key < _kbKeys.size()) {
		out = _kbKeys[key][1];
		_kbKeys[key][1] = false;
	}

	return out;
}

bool Input::wasKeyReleased(int key) {
	bool out = false;
	if (key >= 0 && key < _kbKeys.size()) {
		out = _kbKeys[key][2];
		_kbKeys[key][2] = false;
	}

	return out;
}

bool Input::isKeyPressed(int key) {
	bool out = false;
	if (key >= 0 && key < _kbKeys.size()) 
		out = _kbKeys[key][0];
	return out;
}

bool Input::wasMKeyPressed(int key) {
	bool out = false;
	if (key >= 0 && key < _mKeys.size()) {
		out = _mKeys[key][1];
		_mKeys[key][1] = false;
	}

	return out;
}

bool Input::isMKeyPressed(int key) {
	bool out = false;
	if (key >= 0 && key < _mKeys.size()) 
		out = _mKeys[key][0];

	return out;
}

bool Input::wasMKeyReleased(int key) {
	bool out = false;
	if (key >= 0 && key < _mKeys.size()) {
		out = _mKeys[key][2];
		_mKeys[key][2] = false;
	}

	return out;
}

float Input::getWheelSpin() { 
	float t = _wheel;
	_wheel = 0; 
	return t; 
}