#pragma once

#include "Component.h"
#include "EyeExtractor.h"

/*
 * Eye center detection algorithm by Timm, F. and Barth, E.
 * Implementation adapted from https://github.com/trishume/eyeLike
 */

// Debugging
const bool kPlotVectorField = false;

// Size constants
const int kEyePercentTop = 25;
const int kEyePercentSide = 13;
const int kEyePercentHeight = 30;
const int kEyePercentWidth = 35;

// Preprocessing
const bool kSmoothFaceImage = false;
const float kSmoothFaceFactor = 0.005;

// Algorithm Parameters
const int kFastEyeWidth = 50;
const int kWeightBlurSize = 5;
const bool kEnableWeight = true;
const float kWeightDivisor = 1.0;
const double kGradientThreshold = 50.0;

// Postprocessing
const bool kEnablePostProcess = true;
const float kPostProcessThreshold = 0.97;

// Eye Corner
const bool kEnableEyeCorner = false;

class EyeCenterDetector: public Component {
public:
	EyeCenterDetector();
	cv::Point findEyeCenter(cv::Mat eyeImage);
	void process();
	void draw();
	
	cv::Point eyeCenter;
	cv::Point eyeCenterLeft;
	
private:
    EyeExtractor* _eyeExtractor;
    
	//cv::Point unscalePoint(cv::Point p, cv::Mat origSize);
	//void scaleToFastSize(const cv::Mat &src,cv::Mat &dst);
	cv::Mat computeMatXGradient(const cv::Mat &mat);
	void testPossibleCentersFormula(int x, int y, const cv::Mat &weight,double gx, double gy, cv::Mat &out);
	bool floodShouldPushPoint(const cv::Point &np, const cv::Mat &mat);
	cv::Mat floodKillEdges(cv::Mat &mat);
	
	bool inMat(cv::Point p,int rows,int cols);
	cv::Mat matrixMagnitude(const cv::Mat &matX, const cv::Mat &matY);
	double computeDynamicThreshold(const cv::Mat &mat, double stdDevFactor);
};