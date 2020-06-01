#pragma once

#include "Point.h"
#include "Component.h"

#include <boost/scoped_ptr.hpp>

class PointTracker: public Component {
public:
	static const int eyePoint1 = 0;
	static const int eyePoint2 = 1;
	std::vector<uchar> status;
	std::vector<cv::Point2f> origPoints, currentPoints, lastPoints;
	cv::Mat grey;

	PointTracker();
	
	void clearTrackers();
	void addTracker(const cv::Point2f &point);
	
	void process();
	void track();
	void retrack();
	int countActivePoints();
	bool areAllPointsActive();
	int pointCount();
	std::vector<Point> getPoints(const std::vector<cv::Point2f> PointTracker::*points, bool allPoints=true);
	void draw();
	void normalizeOriginalGrey();

private:
	static const int _winSize = 11;
	int _flags;
	cv::Mat _origGrey, _lastGrey;
	static const int _pyramidDepth = 2;
	
	void synchronizePoints();
};

