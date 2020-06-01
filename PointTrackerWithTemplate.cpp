#include "PointTrackerWithTemplate.h"
#include "Application.h"

#define TEMPLATE_SIZE 30
#define SEARCH_AREA_SIZE 60
#define	THRESHOLD_LIKELIHOOD 0.8

PointTrackerWithTemplate::PointTrackerWithTemplate() :
	_templatesInitialized(false),
	_temporarySearchArea(cv::Size(SEARCH_AREA_SIZE, SEARCH_AREA_SIZE), CV_8UC1),
	_temporarySearchResults(cv::Size(SEARCH_AREA_SIZE - TEMPLATE_SIZE + 1, SEARCH_AREA_SIZE - TEMPLATE_SIZE + 1), CV_32FC1)
{

}

PointTrackerWithTemplate::~PointTrackerWithTemplate() {
}

void PointTrackerWithTemplate::process() {
	// If new set of points are selected at this frame, extract the new templates around eye corners
	if(Application::Signals::initiatePointSelectionFrameNo == Application::Components::videoInput->frameCount) {
		extractTemplates();
	}

	// If the templates are already extracted and the points are being tracked,
	// try to improve the OF tracking with template matching
	if (Application::Data::isTrackingSuccessful && _templatesInitialized) {
		updatePointPositions();
    }
}

void PointTrackerWithTemplate::extractTemplates() {
	cv::Point2f point1 = Application::Data::anchorPoints[PointTracker::eyePoint1];
	cv::Point2f point2 = Application::Data::anchorPoints[PointTracker::eyePoint2];

	cv::Mat frame = Application::Components::videoInput->frameGrey;

	std::cout << cv::Rect(point1.x - TEMPLATE_SIZE/2,
					point1.y - TEMPLATE_SIZE/2,
					TEMPLATE_SIZE,
					TEMPLATE_SIZE) << std::endl;

	frame(cv::Rect(point1.x - TEMPLATE_SIZE/2,
					point1.y - TEMPLATE_SIZE/2,
					TEMPLATE_SIZE,
					TEMPLATE_SIZE)
		 ).copyTo(_templateRight);

	frame(cv::Rect(point2.x - TEMPLATE_SIZE/2,
					point2.y - TEMPLATE_SIZE/2,
					TEMPLATE_SIZE,
					TEMPLATE_SIZE)
		 ).copyTo(_templateLeft);

	_templatesInitialized = true;
}

void PointTrackerWithTemplate::updatePointPositions() {
	double minValue, maxValue;
	cv::Point minLocation, maxLocation;

	// Repeat for right and left sides
	for(int side=0; side<2; side++) {
		cv::Point2f *point = &(Application::Data::anchorPoints[side]);
		cv::Mat grey = Application::Components::videoInput->frameGrey;
		cv::Mat cornerTemplate = _templateRight;

		if(side == 0) {
			cornerTemplate = _templateLeft;
		}

		// Extract the search area, and look for the previously extracted anchor point templates
//		std::cout << cv::Rect(point->x - SEARCH_AREA_SIZE/2, point->y - SEARCH_AREA_SIZE/2, SEARCH_AREA_SIZE, SEARCH_AREA_SIZE) << std::endl;
//		std::cout << grey.size().width << "x" << grey.size().height << std::endl;
		grey(cv::Rect(point->x - SEARCH_AREA_SIZE/2, point->y - SEARCH_AREA_SIZE/2, SEARCH_AREA_SIZE, SEARCH_AREA_SIZE)).copyTo(_temporarySearchArea);
		matchTemplate(_temporarySearchArea, cornerTemplate, _temporarySearchResults, CV_TM_CCOEFF_NORMED);
		minMaxLoc(_temporarySearchResults, &minValue, &maxValue, &minLocation, &maxLocation);

		// If the found position is very likely, update anchor point positions
		if (maxValue > THRESHOLD_LIKELIHOOD) {
			point->x = maxLocation.x + point->x - SEARCH_AREA_SIZE/2 + TEMPLATE_SIZE/2;
			point->y = maxLocation.y + point->y - SEARCH_AREA_SIZE/2 + TEMPLATE_SIZE/2;
		}
	}
}

void PointTrackerWithTemplate::draw() {
}
