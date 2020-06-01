#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <opencv2/opencv.hpp>

#include "GazeTracker.h"
#include "ImageWindow.h"
#include "Component.h"

class FrogGame: public Component {
public:
	FrogGame();
	~FrogGame();
	void process();
	void draw();
	void calculateNewFrogPosition();

private:
	ImageWindow _window;
	
	// Images
	cv::Mat _screenImage;
	cv::Mat _clutter;
	cv::Mat _backgroundWithFrog;
	cv::Mat _frog;
	cv::Mat _frogMask;
	cv::Mat _gaussianMask;
	cv::Mat _gaussianMaskNegative;
	cv::Mat _solidBackground;
	
	// Limits for the game area
	int _gameAreaX;
	int _gameAreaY;
	int _gameAreaWidth;
	int _gameAreaHeight;

	// Frog information
	int _frogX;
	int _frogY;
	int _frogCounter;	
	
	// Timing variables for frog disappearance
	long _startTime;
	long _futureTime;
	long _tempTime;
};

