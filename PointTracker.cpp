#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <math.h>

#include "PointTracker.h"
#include "Application.h"
#include "FaceDetector.h"
#include "utils.h"

using Utils::operator<<;
using Utils::operator>>;

static Point pointBetweenRects(const Point &point, cv::Rect source, cv::Rect dest) {
	return Point((point.x - source.x) * (double(dest.width) / source.width) + dest.x,
		(point.y - source.y) * (double(dest.height) / source.height) + dest.y);
}

static std::vector<Point> pointBetweenRects(const std::vector<Point> &points, cv::Rect source, cv::Rect dest) {
	std::vector<Point> result;
	result.reserve(points.size());

	xForEach(iter, points) {
		result.push_back(pointBetweenRects(*iter, source, dest));
	}

	return result;
}

PointTracker::PointTracker():
	_flags(CV_LKFLOW_INITIAL_GUESSES),
	grey(Application::Components::videoInput->size, 1),
	_origGrey(Application::Components::videoInput->size, 1),
	_lastGrey(Application::Components::videoInput->size, 1)
{
}

void PointTracker::clearTrackers() {
	currentPoints.clear();
	synchronizePoints();
}

void PointTracker::addTracker(const cv::Point2f &point) {
	currentPoints.push_back(point);
	synchronizePoints();
}

void PointTracker::process() {
	if(Application::Signals::initiatePointClearingFrameNo == Application::Components::videoInput->frameCount) {
		clearTrackers();
	}
	track();
}

void PointTracker::track() {
	try {
		assert(lastPoints.size() == currentPoints.size());
		assert(origPoints.size() == currentPoints.size());
		status.resize(currentPoints.size());
        
		cvtColor(Application::Components::videoInput->frame, grey, CV_BGR2GRAY);

/*
		if (Application::faceRectangle != NULL) {
			Utils::normalizeGrayScaleImage_NEW(grey(*Application::faceRectangle), 90, 160);
		}
*/
		// Apply median filter of 5x5
		medianBlur(grey, grey, 5);

		if (!currentPoints.empty()) {
			if(Application::Settings::noTracking) {
				for (int i = 0; i < currentPoints.size(); i++) {
					currentPoints[i].x = origPoints[i].x;
					currentPoints[i].y = origPoints[i].y;

					status[i] = 1;
				}
			}
			else {
				// then calculate the position based on the original
				// template without any pyramids
				std::vector<float> err;

				calcOpticalFlowPyrLK(_origGrey, grey,
					origPoints, currentPoints,
					status, err,
					cv::Size(_winSize,_winSize),
					_pyramidDepth * 3,
					cv::TermCriteria(CV_TERMCRIT_EPS, 20, 0.03),
					_flags);

				_flags |= CV_LKFLOW_PYR_A_READY;
			}
		}

		grey.copyTo(_lastGrey);
		lastPoints = currentPoints;
        Application::Data::isTrackingSuccessful = (countActivePoints() >= 4);
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
		clearTrackers();
	}
}

void PointTracker::retrack() {
	try {
		currentPoints = origPoints;

		std::cout << "RETRACKING" << std::endl;
		for (int i = 0; i < (int)currentPoints.size(); i++) {
			std::cout << "CP["<< i <<"]" << currentPoints[i].x << ", " << currentPoints[i].y << std::endl;
		}

		_flags = 0;
		cvtColor(Application::Components::videoInput->frame, grey, CV_BGR2GRAY);

		// Apply median filter of 5x5
		medianBlur(grey, grey, 5);

		// then calculate the position based on the original
		// template without any pyramids
		std::vector<float> err;

		calcOpticalFlowPyrLK(_origGrey, grey,
			origPoints, currentPoints,
			status, err,
			cv::Size(_winSize,_winSize),
			_pyramidDepth * 3,
			cv::TermCriteria(CV_TERMCRIT_EPS, 200, 0.0001),
			_flags);
		//}

		_flags = CV_LKFLOW_INITIAL_GUESSES;
		_flags |= CV_LKFLOW_PYR_A_READY;

		grey.copyTo(_lastGrey);

		lastPoints = currentPoints;

        // Copy the required data to Application::Data variables
        Application::Data::isTrackingSuccessful = (countActivePoints() >= 4);
		for (int i = 0; i < (int)currentPoints.size(); i++) {
			Application::Data::anchorPoints[i].x = currentPoints[i].x;
			Application::Data::anchorPoints[i].y = currentPoints[i].y;
		}
        
		/*
        std::cout << std::endl << "AFTER RETRACKING" << std::endl;
		for (int i = 0; i < (int)currentPoints.size(); i++) {
			std::cout << "CP["<< i <<"]" << currentPoints[i].x << ", " << currentPoints[i].y << std::endl;
		}
        */
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
		clearTrackers();
	}
}

int PointTracker::countActivePoints() {
	return count_if(status.begin(), status.end(), bind1st(std::not_equal_to<char>(), 0));
}

bool PointTracker::areAllPointsActive() {
	return count(status.begin(), status.end(), 0) == 0;
}

int PointTracker::pointCount() {
	return currentPoints.size();
}

std::vector<Point> PointTracker::getPoints(const std::vector<cv::Point2f> PointTracker::*points, bool allPoints) {
	std::vector<Point> vec;
	for (int i = 0; i < pointCount(); i++) {
		if (allPoints || status[i]) {
			vec.push_back(Point((this->*points)[i].x, (this->*points)[i].y));
		}
	}
	return vec;
}

void PointTracker::draw() {
	cv::Mat image = Application::Components::videoInput->debugFrame;

	try {
		for (int i = 0; i < (int)currentPoints.size(); i++) {
			//std::cout << "Drawing point " << i+1 << "(" << currentPoints[i].x << ", " << currentPoints[i].y << ")" << std::endl;
			cv::circle(image, Utils::mapFromCameraToDebugFrameCoordinates(currentPoints[i]), 3, cv::Scalar(255,0,0), -1, 8, 0);
			//status[i] ? (i == eyePoint1 || i == eyePoint2 ? cv::Scalar(255,0,0) : cv::Scalar(0,255,0)) : cv::Scalar(0,0,255)
		}
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
		clearTrackers();
	}
}

void PointTracker::normalizeOriginalGrey() {
	/* TODO ONUR COMMENTED HERE
	cvSetImageROI(_origGrey.get(), *Application::faceRectangle);
	Utils::normalizeGrayScaleImage2(_origGrey.get(), 90, 160);
	cvResetImageROI(_origGrey.get());
	*/
}

void PointTracker::synchronizePoints() {
	swap(_origGrey, grey);
	origPoints = lastPoints = currentPoints;
}
