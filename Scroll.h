/*
 * Scroll.h
 *
 *  Created on: 10 de mar de 2017
 *      Author: maikon
 */

#ifndef SCROLL_H_
#define SCROLL_H_

#define MIDDLE_Y_POINT 450

#include "Component.h"
#include "Application.h"
#include "EyeExtractor.h"
#include "BlinkDetector.h"
#include "MoveMouse.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

class Scroll: public Component {
private:
	EyeExtractor* _eyeExtractor;
	MoveMouse * _moveMouse;
	bool _activeScroll;
	int gazeY;

	Display *display;
	unsigned int keycode;

public:
	Scroll();
	virtual ~Scroll();
	void process();
	void draw();
	void toggleScroll();
	void scrollPage();
	void scrollDown();
	void scrollUp();
};

#endif /* SCROLL_H_ */
