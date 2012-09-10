#include "input.h"


Keyboard Keys::down;
Keyboard Keys::wasDown;
Keyboard Keys::pressed;
Keyboard Keys::released;
bool Keys::anyKeyDown;
bool Keys::anyKeyPressed;

void Keys::process()
{
	anyKeyDown = false;
	anyKeyPressed = false;
	for(int i=0; i<256; i++){
		if(down.keys[i]){
			//logf ("key down %x\n", i);
			anyKeyDown = true;
		}
		pressed.keys[i] = down.keys[i] && !wasDown.keys[i];
		released.keys[i] = !down.keys[i] && wasDown.keys[i];
		if(pressed.keys[i]){
			//lastKeyPressed = i;
			anyKeyPressed = true;
		}
		wasDown.keys[i] = down.keys[i];
	}
}



int Mouse::x, Mouse::y, Mouse::movX, Mouse::movY;
bool Mouse::rightDown, Mouse::rightWasDown, Mouse::rightPressed, Mouse::rightReleased;
bool Mouse::leftDown, Mouse::leftWasDown, Mouse::leftPressed, Mouse::leftReleased;

void Mouse::process()
{
	leftPressed = !leftWasDown && leftDown;
	leftReleased = leftWasDown && !leftDown;
	leftWasDown = leftDown;
	rightPressed = !rightWasDown && rightDown;
	rightReleased = rightWasDown && !rightDown;
	rightWasDown = rightDown;
}


