#pragma once

#include "BlinkDetector.h"
#include "FeatureDetector.h"
#include "Component.h"

class EyeExtractorSegmentationGroundTruth: public Component {
public:
	static const int eyeDX;
	static const int eyeDY;
	static const cv::Size eyeSize;
	
	boost::scoped_ptr<FeatureDetector> averageEye;
	boost::scoped_ptr<FeatureDetector> averageEyeLeft;

	boost::scoped_ptr<cv::Mat> eyeGrey, eyeFloat, eyeImage;
	boost::scoped_ptr<cv::Mat> eyeGreyLeft, eyeFloatLeft, eyeImageLeft;
	
	EyeExtractorSegmentationGroundTruth();
	~EyeExtractorSegmentationGroundTruth();
	void process();
	bool isBlinking();
	bool hasValidSample();
	void draw();

	void start();
	void pointStart();
	void pointEnd();
	void abortCalibration();
	void calibrationEnded();
	
private:
	BlinkDetector _blinkDetector;
	BlinkDetector _blinkDetectorLeft;
	bool _isBlinking;

	void extractEye(cv::Mat originalImage);
	void extractEyeLeft(cv::Mat originalImage);
};

