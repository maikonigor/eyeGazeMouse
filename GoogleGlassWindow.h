#pragma once

#include "ImageWindow.h"
#include "Point.h"
#include "Component.h"

#define picturesX 3
#define picturesY 3

class GoogleGlassWindow: public Component {
public:
	GoogleGlassWindow();
	~GoogleGlassWindow();
	
	// Main processing function
	void process();
	
	// Calibration API functions (for future use)
	void start();
	void abortGoogleGlassWindow();
	void draw();
	
	bool isActive();
	
private:
	ImageWindow _window;
	ImageWindow _window2;


	cv::Mat _interfaceStimuli;
	cv::Mat _interfaceStimuliWithOverlay;
	cv::Mat _pictureResized;
	
	cv::Mat _interfaceText;
	
	cv::Rect roi_rect;

	std::vector<cv::Mat> _textImages;


};


