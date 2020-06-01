#pragma once

#include "BlinkDetector.h"
#include "FeatureDetector.h"
#include "Component.h"
#include "PointTracker.h"

class EyeExtractor: public Component {
public:
	static const int eyeDX;
	static const int eyeDY;
	static const cv::Size eyeSize;

	boost::scoped_ptr<FeatureDetector> averageEye;
	boost::scoped_ptr<FeatureDetector> averageEyeLeft;

	cv::Mat eyeGrey, eyeFloat, eyeImage;
	cv::Mat eyeGreyLeft, eyeFloatLeft, eyeImageLeft;

	EyeExtractor(bool fromGroundTruth=false);
	~EyeExtractor();
	void process();
	bool isBlinking();
	bool hasValidSample();
	void draw();

	void start();
	void pointStart();
	void pointEnd();
	void abortCalibration();
	void calibrationEnded();
	bool isVolBlinking();
	void resetEyeState();
	bool isDoubleBlink();
	BlinkDetector::STATE getBlinkState();


private:
	BlinkDetector _blinkDetector;
	BlinkDetector _blinkDetectorLeft;
	bool _isBlinking;
	bool _fromGroundTruth;
    PointTracker *_pointTracker;

	void extractEye(const cv::Mat originalImage);
	void extractEyeLeft(const cv::Mat originalImage);
};
