#include <opencv2/highgui/highgui.hpp>
#include <math.h>

#include "AnchorPointSelector.h"
#include "Application.h"
#include "utils.h"

AnchorPointSelector::AnchorPointSelector() {
	_pointTracker = NULL;
	loadCascades();
}

AnchorPointSelector::~AnchorPointSelector() {
}

void AnchorPointSelector::process() {
    if(_pointTracker == NULL) {
        _pointTracker = (PointTracker*) Application::getComponent("PointTracker");
    }
    
	if(Application::Signals::initiatePointSelectionFrameNo == Application::Components::videoInput->frameCount) {
		choosePoints();
	}
}

void AnchorPointSelector::draw() {
}

void AnchorPointSelector::choosePoints() {
	try {
		cv::Point2f eyes[2];
		cv::Point2f nose[2];
		cv::Point2f mouth[2];
		cv::Point2f eyebrows[2];

		if(!detectEyeCorners(Application::Components::videoInput->frame, Application::Components::videoInput->getResolution(), eyes)) {
			std::cout << "EYE CORNERS NOT DETECTED" << std::endl;
			return;
		}

		cv::Rect noseRect = cv::Rect(eyes[0].x, eyes[0].y, fabs(eyes[0].x - eyes[1].x), fabs(eyes[0].x - eyes[1].x));
		checkRectSize(Application::Components::videoInput->frame, &noseRect);

		if (!detectNose(Application::Components::videoInput->frame, Application::Components::videoInput->getResolution(), noseRect, nose)) {
			std::cout << "NO NOSE" << std::endl;
			return;
		}

		cv::Rect mouthRect = cv::Rect(eyes[0].x, nose[0].y, fabs(eyes[0].x - eyes[1].x), 0.8 * fabs(eyes[0].x - eyes[1].x));
		checkRectSize(Application::Components::videoInput->frame, &mouthRect);

		if (!detectMouth(Application::Components::videoInput->frame, Application::Components::videoInput->getResolution(), mouthRect, mouth)) {
			std::cout << "NO MOUTH" << std::endl;
			return;
		}

		cv::Rect eyebrowRect = cv::Rect(eyes[0].x + fabs(eyes[0].x - eyes[1].x) * 0.25, eyes[0].y - fabs(eyes[0].x - eyes[1].x) * 0.40, fabs(eyes[0].x - eyes[1].x) * 0.5, fabs(eyes[0].x - eyes[1].x) * 0.25);
		checkRectSize(Application::Components::videoInput->frame, &eyebrowRect);
		detectEyebrowCorners(Application::Components::videoInput->frame, Application::Components::videoInput->getResolution(), eyebrowRect, eyebrows);

		//cvSaveImage("cframe.jpg", Application::Components::videoInput->frame);

		_pointTracker->clearTrackers();

		_pointTracker->addTracker(eyes[0]);
		_pointTracker->addTracker(eyes[1]);
		_pointTracker->addTracker(nose[0]);
		_pointTracker->addTracker(nose[1]);
		_pointTracker->addTracker(mouth[0]);
		_pointTracker->addTracker(mouth[1]);
		_pointTracker->addTracker(eyebrows[0]);
		_pointTracker->addTracker(eyebrows[1]);
	}
	catch (std::ios_base::failure &e) {
		std::cout << e.what() << std::endl;
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

void AnchorPointSelector::loadCascades() {
	if(/*!faceCascade.load("haarcascade_frontalface_alt.xml") || */
		!eyeCascade.load("DetectorEyes.xml") ||
		!noseCascade.load("DetectorNose2.xml") ||
		!mouthCascade.load("DetectorMouth.xml")) {
        std::cout << "ERROR: Could not load cascade classifiers!" << std::endl;
		exit(1);
	}
}

bool AnchorPointSelector::detectLargestObject(cv::CascadeClassifier cascade, cv::Mat image, cv::Rect &largestObject, double scaleFactor, int minNeighbors, int flags, cv::Size minSize) {
	std::vector<cv::Rect> results;

	largestObject.x = 0;
	largestObject.y = 0;
	largestObject.width = 0;
	largestObject.height = 0;

	cascade.detectMultiScale(image, results, scaleFactor, minNeighbors, flags, minSize);

	// Save the largest object
	if (results.size() > 0) {
    	for(int i=0; i<results.size(); i++) {
    		cv::Rect temp = results[0];

    		if((temp.width * temp.height) > (largestObject.width*largestObject.height)) {
    			largestObject = temp;
			}
    	}

		return true;
	}

	return false;
}

bool AnchorPointSelector::detectNose(cv::Mat image, double resolution, cv::Rect noseRect, cv::Point2f points[]) {
	cv::Rect largestObject(0, 0, 0, 0);
	double scaleFactor = 1.1;
	int minNeighbors = 3;
	int flags = CV_HAAR_DO_CANNY_PRUNING;
	cv::Size minSize(24, 20);

	if (resolution != 480) {
		double factor = resolution/480;
		minSize.width = round(factor*minSize.width);
		minSize.height = round(factor*minSize.height);
	}

	// Detect objects
	if(!detectLargestObject(noseCascade, image(noseRect), largestObject, scaleFactor, minNeighbors, CV_HAAR_DO_CANNY_PRUNING, minSize)) {
		return false;
	}

	points[0] = cv::Point2f(noseRect.x + largestObject.x + largestObject.width * 0.33,
						noseRect.y + largestObject.y + largestObject.height * 0.6);
	points[1] = cv::Point2f(noseRect.x + largestObject.x + largestObject.width * 0.67,
						noseRect.y + largestObject.y + largestObject.height * 0.6);

	//cv::Rectangle(image, cv::Point2f(noseRect.x + nose->x, noseRect.y + nose->y), cv::Point2f(noseRect.x + nose->x + nose->width, noseRect.y + nose->y + nose->height), CV_RGB(0, 255, 0), 2, 8, 0);
	//cv::circle(image, cv::Point2f(points[0].x, points[0].y), 3, CV_RGB(0,255,0), -1, 8, 0);
	//cv::circle(image, cv::Point2f(points[1].x, points[1].y), 3, CV_RGB(0,255,0), -1, 8, 0);

	return true;
}

bool AnchorPointSelector::detectMouth(cv::Mat image, double resolution, cv::Rect mouthRect, cv::Point2f points[]) {
	cv::Rect largestObject(0, 0, 0, 0);
	double scaleFactor = 1.1;
	int minNeighbors = 3;
	int flags = 0;
	cv::Size minSize(25, 15);

	if (resolution != 480) {
		double factor = resolution/480;
		minSize.width = round(factor*minSize.width);
		minSize.height = round(factor*minSize.height);
	}

	// Detect objects
	if(!detectLargestObject(mouthCascade, image(mouthRect), largestObject, scaleFactor, minNeighbors, CV_HAAR_DO_CANNY_PRUNING, minSize)) {
		return false;
	}

	points[0] = cv::Point2f(mouthRect.x + largestObject.x + largestObject.width * 0.1,
						mouthRect.y + largestObject.y + largestObject.height * 0.4);
	points[1] = cv::Point2f(mouthRect.x + largestObject.x + largestObject.width * 0.9,
						mouthRect.y + largestObject.y + largestObject.height * 0.4);

	//cv::Rectangle(image, cv::Point2f(mouthRect.x + largestObject.x, mouthRect.y + largestObject.y), cv::Point2f(mouthRect.x + largestObject.x + largestObject.width, mouthRect.y + largestObject.y + largestObject.height), CV_RGB(0, 255, 0), 2, 8, 0 );
	//cv::circle(image, cv::Point2f(points[0].x, points[0].y), 3, CV_RGB(0,255,0), -1, 8, 0);
	//cv::circle(image, cv::Point2f(points[1].x, points[1].y), 3, CV_RGB(0,255,0), -1, 8, 0);

	return true;
}

bool AnchorPointSelector::detectEyeCorners(cv::Mat image, double resolution, cv::Point2f points[]) {
	cv::Rect largestObject(0, 0, 0, 0);
	double scaleFactor = 1.1;
	int minNeighbors = 10;
	int flags = CV_HAAR_DO_CANNY_PRUNING;
	cv::Size minSize(64, 16);

	if (resolution != 480) {
		double factor = resolution/480;
		minSize.width = round(factor*minSize.width);
		minSize.height = round(factor*minSize.height);
	}

	// Detect objects
	if(!detectLargestObject(eyeCascade, image, largestObject, scaleFactor, minNeighbors, CV_HAAR_DO_CANNY_PRUNING, minSize)) {
		std::cout << "Detect largest object (for eyes) failed!!" << std::endl;
		return false;
	}

	//std::cout << "Resolution: " << resolution << ", both eye reg.:" << largestObject.width << ", " << largestObject.height << std::endl;

	//cv::Rectangle(image, cv::Point2f(largestObject.x, largestObject.y), cv::Point2f(largestObject.x + largestObject.width, largestObject.y + largestObject.height), CV_RGB(0, 255, 0), 2, 8, 0);

	int cornerCount = 100;
	cv::Mat eyeRegionImage(cv::Size(largestObject.width, largestObject.height), CV_8UC3);
	cv::Mat eyeRegionImageGray(cv::Size(largestObject.width, largestObject.height), CV_8UC1);

	image(largestObject).copyTo(eyeRegionImage);
	cv::cvtColor(eyeRegionImage, eyeRegionImageGray, CV_RGB2GRAY);

	Utils::normalizeGrayScaleImage(&eyeRegionImageGray, 127, 80);

	std::vector<cv::Point2f> *corners = detectCornersInGrayscale(eyeRegionImageGray, cornerCount);

	int leftEyeCornersXSum = 0;
	int leftEyeCornersYSum = 0;
	int leftEyeCornersCount = 0;

	int rightEyeCornersXSum = 0;
	int rightEyeCornersYSum = 0;
	int rightEyeCornersCount = 0;

	/// Drawing a circle around corners
	for (int j = 0; j < corners->size(); j++ ) {
		if ((*corners)[j].x < largestObject.width * 0.4) {
			leftEyeCornersXSum += (*corners)[j].x;
			leftEyeCornersYSum += (*corners)[j].y;
			leftEyeCornersCount++;
			//cv::circle(eyeRegionImage, cv::Point2f(corners[j].x, corners[j].y), 3, CV_RGB(255,0,0), -1, 8,0);
		} else if ((*corners)[j].x > largestObject.width * 0.6) {
			rightEyeCornersXSum += (*corners)[j].x;
			rightEyeCornersYSum += (*corners)[j].y;
			rightEyeCornersCount++;
			//cv::circle(eyeRegionImage, cv::Point2f(corners[j].x, corners[j].y), 3, CV_RGB(255,0,0), -1, 8,0);
		}
	}

	double leftEyeCenterX = largestObject.x + (leftEyeCornersXSum / (double)leftEyeCornersCount);
	double leftEyeCenterY = largestObject.y + (leftEyeCornersYSum / (double)leftEyeCornersCount);

	double rightEyeCenterX = largestObject.x + (rightEyeCornersXSum / (double)rightEyeCornersCount);
	double rightEyeCenterY = largestObject.y + (rightEyeCornersYSum / (double)rightEyeCornersCount);

	double xDiff = rightEyeCenterX - leftEyeCenterX;
	double yDiff = rightEyeCenterY - leftEyeCenterY;

	double eyeSeparation = 0.29;
	points[0] = cv::Point2f(leftEyeCenterX - eyeSeparation * xDiff, leftEyeCenterY - eyeSeparation * yDiff);
	points[1] = cv::Point2f(rightEyeCenterX + eyeSeparation * xDiff, rightEyeCenterY + eyeSeparation * yDiff);

	/// Drawing a circle around corners
	//for (int i = 0; i < cornerCount; i++) {
	//	cv::circle(eyeRegionImage, cv::Point2f(corners[i].x, corners[i].y), 3, CV_RGB(255,0,0), -1, 8, 0);
	//}

	//cv::circle(image, cv::Point2f(points[0].x, points[0].y), 3, CV_RGB(0,255,0), -1, 8, 0);
	//cv::circle(image, cv::Point2f(points[1].x, points[1].y), 3, CV_RGB(0,255,0), -1, 8, 0);
	return true;
}

void AnchorPointSelector::detectEyebrowCorners(cv::Mat image, double resolution, cv::Rect eyebrowRect, cv::Point2f points[]) {
	eyebrowRect.width = eyebrowRect.width / 2;

	cv::Mat eyebrowRegionImage(cv::Size(eyebrowRect.width, eyebrowRect.height), CV_8UC3);
	cv::Mat eyebrowRegionImageGray(cv::Size(eyebrowRect.width, eyebrowRect.height), CV_8UC1);
	cv::Mat eyebrowRegionImage2(cv::Size(eyebrowRect.width, eyebrowRect.height), CV_8UC3);
	cv::Mat eyebrowRegionImageGray2(cv::Size(eyebrowRect.width, eyebrowRect.height), CV_8UC1);

	//std::cout << "EYEBROW x, y = " << eyebrowRect.x << " - " << eyebrowRect.y << " width, height =" << eyebrowRect.width << " - " << eyebrowRect.height << std::endl;

	image(eyebrowRect).copyTo(eyebrowRegionImage);

	cv::Rect eyebrowRect2 = cv::Rect(eyebrowRect.x + eyebrowRect.width, eyebrowRect.y, eyebrowRect.width, eyebrowRect.height);
	image(eyebrowRect2).copyTo(eyebrowRegionImage2);

	//cvSaveImage("eyebrows.png", eyebrowRegionImage);
	cv::cvtColor(eyebrowRegionImage, eyebrowRegionImageGray, CV_RGB2GRAY);
	cv::cvtColor(eyebrowRegionImage2, eyebrowRegionImageGray2, CV_RGB2GRAY);

	std::vector<cv::Point2f> *corners = detectCornersInGrayscale(eyebrowRegionImageGray, 1);
	std::vector<cv::Point2f> *corners2 = detectCornersInGrayscale(eyebrowRegionImageGray2, 1);

	points[0] = cv::Point2f(eyebrowRect.x + (*corners)[0].x, eyebrowRect.y + (*corners)[0].y);
	points[1] = cv::Point2f(eyebrowRect2.x + (*corners2)[0].x, eyebrowRect2.y + (*corners2)[0].y);
}


std::vector<cv::Point2f>* AnchorPointSelector::detectCornersInGrayscale(cv::Mat eyeRegionImageGray, int cornerCount) {
	std::vector<cv::Point2f> *corners = new std::vector<cv::Point2f>();

	double qualityLevel = 0.01;
	double minDistance = 2;
	int eigBlockSize = 3;
	int useHarris = false;

	cv::goodFeaturesToTrack(eyeRegionImageGray, *corners, cornerCount, qualityLevel, minDistance, cv::noArray(), eigBlockSize, useHarris);

	return corners;
}

void AnchorPointSelector::checkRectSize(const cv::Mat &image, cv::Rect *rect) {
	if (rect->x < 0) {
		rect->x = 0;
	}

	if (rect->y < 0) {
		rect->y = 0;
	}

	if (rect->x + rect->width >= image.size().width) {
		rect->width = image.size().width - rect->x - 1;
	}

	if (rect->y + rect->height >= image.size().height) {
		rect->height = image.size().height - rect->y - 1;
	}
}
