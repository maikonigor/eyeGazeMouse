#include "Calibrator.h"
#include "Application.h"

Calibrator::Calibrator(const std::vector<Point> &points) :
    _window(1, false)	// Create this window in the second screen
{
	_points = points;
    
    // Clear gaze tracker components' calibration info
    for (int i=0; i<Application::config.gaze_components.size(); i++){
        GazeTrackerComponent* comp = (GazeTrackerComponent*) Application::getComponent(Application::config.gaze_components[i]);
        comp->clear();
    }
	
	_frameNumber = -1;
	needRecalibration = false;
	
	_screenImage.create(cv::Size(_window.size().width(), _window.size().height()), CV_8UC3);
	_targetImage = cv::imread("./target.png", CV_LOAD_IMAGE_COLOR);
	_window.setWindowTitle("Opengazer Calibrator");
}

Calibrator::~Calibrator() {}

// Main processing function
void Calibrator::process() {
	if(Application::Signals::initiateCalibrationFrameNo == Application::Components::videoInput->frameCount) {
		start();
	}

	if(!isActive())
		return;
	
	needRecalibration = false;
	
	// Increment the frame counter
	_frameNumber++;
	
//	if(getPointFrameNo() == 0 )
//		std::cout << "\n******** " << getPointFrameNo() <<  std::endl;

//	std::cout << "CALIBRATOR:: frameNumber " << getPointFrameNo() <<  std::endl;

	if (shouldStartNextPoint()) {
		pointStart();
//		std::cout << "CALIBRATOR:: novo target " << std::endl;
	} else {
		if (isLastFrame()) {
			pointEnd();
		}
		
		if (isFinished()) {
			calibrationEnded();
		}
	}
}


// Start calibration procedure. Reset frame counter and save target point
// locations
void Calibrator::start() {
	_frameNumber = 0;
	_window.show();
	Application::Data::calibrationTargets.clear();
	Application::status = Application::STATUS_CALIBRATING;
}

// Displays the next target on screen
void Calibrator::pointStart() {

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

void Calibrator::pointEnd() {
//	std::cout << "CALIBRATOR:: pointEnd: needRecalibration " << std::endl;
	Application::Data::calibrationTargets.push_back(getActivePoint());
	needRecalibration = true;
}

// Aborts calibration
void Calibrator::abortCalibration() {
	_window.hide();
	_frameNumber = -1;
}

void Calibrator::calibrationEnded() {
	_window.hide();
	_frameNumber = -1;
	Application::status = Application::STATUS_CALIBRATED;
	Application::isTrackerCalibrated = true;
}

// Checks if calibrator is active
bool Calibrator::isActive() {
	return _frameNumber >= 0;
}

// Returns current point's coordinates
Point Calibrator::getActivePoint() {
	if(isActive())
		return _points[getPointNumber()];
	else
		return Point(-1, -1);
}

// Returns the index of current point
int Calibrator::getPointNumber() {
	if(isActive())
		return (_frameNumber-1) / Application::dwelltimeParameter;
	else
		return -1;
}

// Returns the frame number for the current point
int Calibrator::getPointFrameNo() {
	return (_frameNumber-1) % Application::dwelltimeParameter;
}

// 
bool Calibrator::isLastFrame() {
	return getPointFrameNo() == (Application::dwelltimeParameter-1);
}

// Checks if current point is the last
bool Calibrator::isLastPoint() {
	return getPointNumber() == (_points.size() - 1);
}

// Checks if calibration is finished
bool Calibrator::isFinished() {
	return getPointNumber() >= _points.size();
}

// Decides whether calibrator should switch to next point (display it on screen)
bool Calibrator::shouldStartNextPoint() {
	return (getPointFrameNo() == 0) && !isFinished();
}

void Calibrator::draw() {
	if(isActive()) {
		Point activePoint = getActivePoint();
		
		cv::circle(Application::Components::videoInput->debugFrame,
				Utils::mapFromSecondMonitorToDebugFrameCoordinates(cv::Point(activePoint.x, activePoint.y)),
				8, cv::Scalar(0, 0, 255), -1, 8, 0);
	}
}
