#pragma once

#include "ImageWindow.h"

class DebugWindow {
public:	
	cv::Mat screenImage;
	
	DebugWindow();
	~DebugWindow();
	
	// Display the debug info
	void display();
	void raise();
	
private:
	ImageWindow _window;
};


