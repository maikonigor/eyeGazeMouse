#pragma once

#include <boost/scoped_ptr.hpp>
#include "FeatureDetector.h"

class BlinkDetector {
 public:
	typedef enum { OPEN, CLOSED, VOLUNTARY_BLINK, DOUBLE_BLINK } STATE;

	BlinkDetector();
	bool isBlinking;
	void update(const cv::Mat &image);
	void resetState();
	bool checkDoubleBlink();
	STATE getState();

	STATE state;

private:
	boost::scoped_ptr<cv::Mat> _averageEye;
	//LambdaAccumulator _accumulator;
	//LinearStateSystem _states;
	bool _initialized;
	int _frameCount;

	//static std::vector<StateNode> constructStates();
};
