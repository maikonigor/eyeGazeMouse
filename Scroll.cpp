/*
 * Scroll.cpp
 *
 *  Created on: 10 de mar de 2017
 *      Author: maikon
 */

#include "Scroll.h"

Scroll::Scroll():
	_eyeExtractor(NULL),
	_moveMouse(NULL),
	_activeScroll(false),
	gazeY(0)
{

	display = XOpenDisplay(NULL);
}

Scroll::~Scroll() {}

void Scroll::process(){
	if(Application::status != Application::STATUS_CALIBRATED) {
		 return;
	}
	 if(_eyeExtractor == NULL) {
		_eyeExtractor = (EyeExtractor*) Application::getComponent("EyeExtractor");
	}

	 if(_moveMouse == NULL) {
	 		_moveMouse = (MoveMouse*) Application::getComponent("MoveMouse");
	 	}
	if(_eyeExtractor->isDoubleBlink()){
		scrollPage();
	}

//	if(_activeScroll){
//		scrollPage();
//	}

}

void Scroll::toggleScroll(){
	if(_activeScroll == false){
		std::cout << "scroll ativado" << std::endl;
		_activeScroll = true;
		gazeY = _moveMouse->yPoint;
	}else{
		_activeScroll = false;
	}
}

void Scroll::scrollPage(){
	if(_moveMouse->yPoint > MIDDLE_Y_POINT){
		scrollDown();
	}else{
		scrollUp();

	}
}

void Scroll::scrollUp(){
	std::cout << "up" << std::endl;
	keycode = XKeysymToKeycode(display, XK_Up);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
}

void Scroll::scrollDown(){
	std::cout << "down" << std::endl;
	keycode = XKeysymToKeycode(display, XK_Down);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
}




void Scroll::draw(){}

