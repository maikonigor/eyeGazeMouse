#pragma once

#include "Point.h"
#include "Component.h"
#include "PointTracker.h"

#include <opencv2/objdetect/objdetect.hpp>

class AnchorPointSelector: public Component {
	cv::CascadeClassifier faceCascade;
	cv::CascadeClassifier eyeCascade;
	cv::CascadeClassifier noseCascade;
	cv::CascadeClassifier mouthCascade;
    PointTracker* _pointTracker;

	void choosePoints();
	void loadCascades();
	bool detectLargestObject(cv::CascadeClassifier cascade, cv::Mat image, cv::Rect &largestObject, double scaleFactor = 1.1, int minNeighbors = 3, int flags = 0, cv::Size minSize = cv::Size());
	bool detectNose(cv::Mat image, double resolution, cv::Rect noseRect, cv::Point2f points[]);
	bool detectMouth(cv::Mat image, double resolution, cv::Rect mouthRect, cv::Point2f points[]);
	bool detectEyeCorners(cv::Mat image, double resolution, cv::Point2f points[]);
	void detectEyebrowCorners(cv::Mat image, double resolution, cv::Rect eyebrowRect, cv::Point2f points[]);
	std::vector<cv::Point2f>* detectCornersInGrayscale(cv::Mat eyeRegionImageGray, int cornerCount);
	void checkRectSize(const cv::Mat &image, cv::Rect *rect);

public:
    AnchorPointSelector();
    ~AnchorPointSelector(void);
	void process();
	void draw();
};
