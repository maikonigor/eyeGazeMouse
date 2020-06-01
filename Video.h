#pragma once

#include <opencv2/highgui/highgui.hpp>

class VideoInput {
private:
	cv::VideoCapture _capture;
	long _lastFrameTime;
	
	void prepareDebugFrame();
	void copyToDebugFrame();
	
public:
	int frameCount;
	double frameRate;
	cv::Mat frame;
	cv::Mat frameGrey;
	cv::Mat debugFrame;
	cv::Size size;
	bool captureFromVideo;
	std::string resolutionParameter;
    double videoResolution;

	VideoInput();
	VideoInput(std::string resolution);
	VideoInput(std::string resolution, std::string filename, bool dummy);
	~VideoInput();
	void updateFrame();
	double getResolution();
};

class VideoWriter {
public:
	VideoWriter(cv::Size, std::string filename);
	~VideoWriter();
	void write(const cv::Mat &image);

private:
	cv::VideoWriter _video;
};

