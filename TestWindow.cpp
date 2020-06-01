#include "TestWindow.h"
#include "Application.h"

TestWindow::TestWindow(const std::vector<Point> &points) : 
	_window(2, false)	// Create this window in the second screen
{
	_points = points;
	_frameNumber = -1;
	
	_screenImage.create(cv::Size(_window.size().width(), _window.size().height()), CV_8UC3);
	_targetImage = cv::imread("./target.png", CV_LOAD_IMAGE_COLOR);
	_window.setWindowTitle("Opengazer");
}

TestWindow::~TestWindow() {}

// Main processing function
void TestWindow::process() {
	if(Application::Signals::initiateTestingFrameNo == Application::Components::videoInput->frameCount) {
		start();
	}

	if(!isActive())
		return;
	
	// Increment the frame counter
	_frameNumber++;
	
	if (shouldStartNextPoint()) {
		pointStart();
	} else {
		if (isFinished()) {
			testingEnded();
		}
	}
}


// Start testing procedure. Reset frame counter and save target point locations
void TestWindow::start() {
	_frameNumber = 0;
	_window.show();
	
	Application::status = Application::STATUS_TESTING;
}

// Displays the next target on screen
void TestWindow::pointStart() {
	// Get the current target
	Point target = getActivePoint();
	
	// Calculate bounds for copying target image to screen image
	cv::Rect targetBounds = cv::Rect(0, 0, _targetImage.size().width, _targetImage.size().height);
	cv::Rect screenBounds = cv::Rect(target.x - _targetImage.size().width/2, target.y - _targetImage.size().height/2, _targetImage.size().width, _targetImage.size().height);
	
	// Check bounds
	if (screenBounds.x < 0) {
		screenBounds.width += screenBounds.x;		// Remove the amount from the width
		targetBounds.x -= screenBounds.x;
		targetBounds.width += screenBounds.x;
		screenBounds.x = 0;
	}

	if (screenBounds.y < 0) {
		screenBounds.height += screenBounds.y;		// Remove the amount from the height
		targetBounds.y -= screenBounds.y;
		targetBounds.height += screenBounds.y;
		screenBounds.y = 0;
	}

	if (screenBounds.width + screenBounds.x > _screenImage.size().width) {
		screenBounds.width = _screenImage.size().width - screenBounds.x;
	}

	if (screenBounds.height + screenBounds.y > _screenImage.size().height) {
		screenBounds.height = _screenImage.size().height - screenBounds.y;
	}
	
	// Fill the screen image with solid color and copy the target image
	_screenImage.setTo(cv::Scalar(153, 75, 75));
	_targetImage(targetBounds).copyTo(_screenImage(screenBounds));
	_window.showImage(_screenImage);
}

// Aborts calibration
void TestWindow::abortTesting() {
	_window.hide();
	_frameNumber = -1;
}

void TestWindow::testingEnded() {
	_window.hide();
	_frameNumber = -1;
	Application::status = Application::STATUS_CALIBRATED;
}

// Checks if calibrator is active
bool TestWindow::isActive() {
	return _frameNumber >= 0;
}

// Returns current point's coordinates
Point TestWindow::getActivePoint() {
	if(isActive())
		return _points[getPointNumber()];
	else
		return Point(-1, -1);
}

// Returns the index of current point
int TestWindow::getPointNumber() {
	if(isActive())
		return (_frameNumber-1) / Application::testDwelltimeParameter;
	else
		return -1;
}

// Returns the frame number for the current point
int TestWindow::getPointFrameNo() {
	return (_frameNumber-1) % Application::testDwelltimeParameter;
}

// 
bool TestWindow::isLastFrame() {
	return getPointFrameNo() == (Application::testDwelltimeParameter-1);
}

// Checks if current point is the last
bool TestWindow::isLastPoint() {
	return getPointNumber() == (_points.size() - 1);
}

// Checks if calibration is finished
bool TestWindow::isFinished() {
	return getPointNumber() >= _points.size();
}

// Decides whether calibrator should switch to next point (display it on screen)
bool TestWindow::shouldStartNextPoint() {
	return (getPointFrameNo() == 0) && !isFinished();
}

void TestWindow::draw() {
	if(isActive()) {
		Point activePoint = getActivePoint();
		
		cv::circle(Application::Components::videoInput->debugFrame, 
				Utils::mapFromSecondMonitorToDebugFrameCoordinates(cv::Point(activePoint.x, activePoint.y)), 
				8, cv::Scalar(0, 0, 255), -1, 8, 0);
	}
}
