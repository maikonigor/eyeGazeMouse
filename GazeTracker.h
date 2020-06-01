#pragma once

#include <boost/scoped_ptr.hpp>

#include "utils.h"
#include "GaussianProcess.cpp"
#include "Component.h"
#include "EyeExtractor.h"
#include "PointTracker.h"

class GazeTracker: public GazeTrackerComponent {
	typedef MeanAdjustedGaussianProcess<Utils::SharedImage> ImProcess;

public:
	GazeTracker();
	bool isActive();
	void addExemplar();

	void draw();
	void process();
	void clear();
	void updateEstimations();
	void autoCalibrate();
	void readPointCalibrationFiles();

private:
	std::vector<Utils::SharedImage> _calibrationTargetImages;
	std::vector<Utils::SharedImage> _calibrationTargetImagesLeft;
	std::vector<Point> _calibrationTargetPoints;

	std::vector<Utils::SharedImage> _calibrationTargetImagesAllFrames;
	std::vector<Utils::SharedImage> _calibrationTargetImagesLeftAllFrames;
	std::vector<Point> _calibrationTargetPointsAllFramesFile;
	std::vector<Point> _calibrationTargetPointsAllFrames;

	boost::scoped_ptr<ImProcess> _gaussianProcessX, _gaussianProcessY;
	boost::scoped_ptr<ImProcess> _gaussianProcessXLeft, _gaussianProcessYLeft;
    
	PointTracker* _pointTracker;
    EyeExtractor* _eyeExtractor;


	static double imageDistance(const cv::Mat *image1, const cv::Mat *image2);
	static double covarianceFunction(const Utils::SharedImage &image1, const Utils::SharedImage &image2);

	void updateGaussianProcesses();
};
