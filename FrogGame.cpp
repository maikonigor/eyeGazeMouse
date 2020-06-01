#include <opencv2/highgui/highgui.hpp>
#include <sys/time.h>

#include "FrogGame.h"
#include "Application.h"
#include "utils.h"

FrogGame::FrogGame():
	_window(1, false)
{
	// Read the source images for background (clutter) and frog
	_clutter = cv::imread("./background_full.png", CV_LOAD_IMAGE_COLOR);
	cvtColor(_clutter, _clutter, CV_RGB2BGR);

	_frog = cv::imread("./frog.png", CV_LOAD_IMAGE_COLOR);
	cvtColor(_frog, _frog, CV_RGB2BGR);

	// Read the mask images and convert the gaussian mask to float type
	_frogMask = cv::imread("./frog-mask.png", CV_LOAD_IMAGE_COLOR);
	cv::Mat maskImage = cv::imread("./gaussian-mask.png", CV_LOAD_IMAGE_COLOR);
	maskImage.convertTo(_gaussianMask, CV_32FC3);
	_gaussianMask = _gaussianMask/255.0;
	
	// Create the negative of the gaussian mask and create a patch of solid background (to make operations faster)
	_solidBackground.create(cv::Size(_gaussianMask.size().width, _gaussianMask.size().height), CV_32FC3);
	_solidBackground.setTo(cv::Scalar(1, 1, 1));
	_gaussianMaskNegative = _solidBackground - _gaussianMask;
	
	_solidBackground.setTo(cv::Scalar(153, 75, 75));
	
	_backgroundWithFrog.create(cv::Size(_window.size().width(), _window.size().height()), CV_8UC3);
	_screenImage.create(cv::Size(_window.size().width(), _window.size().height()), CV_8UC3);
	
	//std::cout << "IMAGES CREATED WITH SIZE: " << _backgroundWithFrog.size().width << "x" << _backgroundWithFrog.size().height << std::endl;

	// Fill background with black
	_backgroundWithFrog.setTo(cv::Scalar(0,0,0));

	// Calculate the usable game area
	_gameAreaX = (_window.size().width() - _clutter.size().width) / 2;
	_gameAreaY = (_window.size().height() - _clutter.size().height) / 2;
	_gameAreaWidth = _clutter.size().width;
	_gameAreaHeight = _clutter.size().height;

	// Initialize timing variables
	_frogCounter = 0;
	calculateNewFrogPosition();

	timeval time;
	gettimeofday(&time, NULL);
	_futureTime = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	_futureTime += 1000 * 20000;

	_startTime = _futureTime;
}

FrogGame::~FrogGame() {}

void FrogGame::process() {
	static double estimationXRunningAverage = 0;
	static double estimationYRunningAverage = 0;
	
	if(Application::status == Application::STATUS_CALIBRATED) {
		_window.show();
	}
	else {
		_window.hide();
		return;
	}
	
    Point mappedEstimation;
    Utils::mapToFirstMonitorCoordinates(Application::Data::gazePoints[0], mappedEstimation);
    
	int lastEstimationX = mappedEstimation.x;
	int lastEstimationY = mappedEstimation.y;

	//const int width = _backgroundWithFrog.size().width;
	//const int height = _backgroundWithFrog.size().height;

	double alpha = 0.6;
	estimationXRunningAverage = (1 - alpha) * lastEstimationX + alpha * estimationXRunningAverage;
	estimationYRunningAverage = (1 - alpha) * lastEstimationY + alpha * estimationYRunningAverage;

	// Determine the bounds to copy the corresponding area (under the gazepoint) to the screen
	cv::Rect bounds = cv::Rect(estimationXRunningAverage - _gaussianMask.size().width/2, 
								estimationYRunningAverage - _gaussianMask.size().height/2, 
								_gaussianMask.size().width, 
								_gaussianMask.size().height);
	cv::Rect gaussianBounds = cv::Rect(0, 0, _gaussianMask.size().width, _gaussianMask.size().height);

	if (bounds.x < 0) {
		bounds.width += bounds.x;		// Remove the amount from the width
		gaussianBounds.x -= bounds.x;
		gaussianBounds.width += bounds.x;
		bounds.x = 0;
	}

	if (bounds.y < 0) {
		bounds.height += bounds.y;		// Remove the amount from the height
		gaussianBounds.y -= bounds.y;
		gaussianBounds.height += bounds.y;
		bounds.y = 0;
	}

	if (bounds.width + bounds.x > _backgroundWithFrog.size().width) {
		bounds.width = _backgroundWithFrog.size().width - bounds.x;
	}

	if (bounds.height + bounds.y > _backgroundWithFrog.size().height) {
		bounds.height = _backgroundWithFrog.size().height - bounds.y;
	}
	gaussianBounds.width = bounds.width;
	gaussianBounds.height = bounds.height;

	if (estimationXRunningAverage <= 0) {
		estimationXRunningAverage = 1;
	}

	if (estimationYRunningAverage <= 0) {
		estimationYRunningAverage = 1;
	}

	if (estimationXRunningAverage >= _backgroundWithFrog.size().width) {
		estimationXRunningAverage = _backgroundWithFrog.size().width -1;
	}
	if (estimationYRunningAverage >= _backgroundWithFrog.size().height) {
		estimationYRunningAverage = _backgroundWithFrog.size().height -1;
	}
	
	// Fill the screen with white
	_screenImage.setTo(cv::Scalar(153, 75, 75));

	//std::cout << "Bounds: " << bounds.x << ", " << bounds.y << "," << bounds.width << ", " << bounds.height << std::endl;
	//std::cout << "Gaussian Bounds: " << gaussianBounds.x << ", " << gaussianBounds.y << "," << gaussianBounds.width << ", " << gaussianBounds.height << std::endl;

	// Copy the corresponding area from the cluttered background w/ frog to the screen image
	if (bounds.width > 0 && bounds.height > 0) {
		if (estimationXRunningAverage != 0 || estimationYRunningAverage != 0) {
			// Take the piece of background that is visible, convert to float type
			cv::Mat visibleBackground(cv::Size(gaussianBounds.width, gaussianBounds.height), CV_32FC3);
			_backgroundWithFrog(bounds).convertTo(visibleBackground, CV_32FC3);
			
			cv::Mat copiedArea(cv::Size(gaussianBounds.width, gaussianBounds.height), CV_32FC3);
			cv::Mat copiedArea2(cv::Size(gaussianBounds.width, gaussianBounds.height), CV_8UC3);
			
			// Apply the gaussian mask so that the visible area has a nice gradient around it
			copiedArea = visibleBackground.mul(_gaussianMask(gaussianBounds)) + _solidBackground(gaussianBounds).mul(_gaussianMaskNegative(gaussianBounds));
			copiedArea.convertTo(copiedArea2, CV_8UC3);
			
			// Copy the shown area to the screen image
			copiedArea2.copyTo(_screenImage(bounds));
		}
	}

	// Draw on the window
	_window.showImage(_screenImage);
	
	// Timing stuff
	int diff = ((estimationXRunningAverage - _frogX) * (estimationXRunningAverage - _frogX)) + ((estimationYRunningAverage - _frogY) * (estimationYRunningAverage - _frogY));
	if (diff < 35000) {	// If less than 150 pix, count
		//std::cout << "Difference is fine!" << std::endl;
		timeval time;
		gettimeofday(&time, NULL);
		_tempTime = (time.tv_sec * 1000) + (time.tv_usec / 1000);

		if (_startTime == _futureTime) {
			_startTime = _tempTime;
		} else if (_tempTime - _startTime > 2500) {	// If fixes on the same point for 2.5 seconds
			_frogCounter++;
			calculateNewFrogPosition();
			_startTime = _futureTime;
		}
	} else {	// If cannot focus for a while, reset
		_startTime = _futureTime;
	}
}

void FrogGame::draw() {}

void FrogGame::calculateNewFrogPosition() {
	_frogX = rand() % (_gameAreaWidth - _frog.size().width/2) + _gameAreaX + _frog.size().width/2;
	_frogY = rand() % (_gameAreaHeight - _frog.size().height/2) + _gameAreaY + _frog.size().height/2;

	// Copy the initial background image to "background"
	_backgroundWithFrog.setTo(cv::Scalar(0,0,0));
	_clutter.copyTo(_backgroundWithFrog(cv::Rect(_gameAreaX, _gameAreaY, _gameAreaWidth, _gameAreaHeight)));
	
	// Copy the frog image to the background
	_frog.copyTo(_backgroundWithFrog(cv::Rect(_frogX - _frog.size().width/2, 
												_frogY - _frog.size().height/2, 
												_frog.size().width, 
												_frog.size().height)), 
									_frogMask);
}


