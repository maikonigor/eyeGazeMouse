#pragma once
#include "utils.h"
#include "EyeExtractor.h"
#include "Component.h"

#define HORIZONTAL_BIN_SIZE 128
#define VERTICAL_BIN_SIZE 64

#define GRAY_LEVEL 127

#define VECTOR_SIZE 10

class HistogramFeatureExtractor: public Component {
	cv::Mat _matches[VECTOR_SIZE];
	cv::Mat _matchesSmoothed[VECTOR_SIZE];
	cv::Mat _ellipseMask;
	cv::Mat _irisTemplateDisk[VECTOR_SIZE];
	cv::Mat _irisTemplate;
	cv::Mat _gaussian2D[VECTOR_SIZE];

	EyeExtractor *_groundTruth;
    EyeExtractor *_eyeExtractor;

public:
	cv::Mat horizontalFeatures, verticalFeatures, horizontalFeaturesLeft, verticalFeaturesLeft;
	cv::Mat wholeSegmentation, wholeSegmentationLeft;
	cv::Mat eyeSegmentation, eyeSegmentationLeft;

	HistogramFeatureExtractor();
	~HistogramFeatureExtractor(void);
	void process();
	void draw();

	// Functions to calculate the segmentation and extract the features
	void extractFeatures();
	void calculateSegmentation(cv::Mat, cv::Mat);
	void calculateHistogram(cv::Mat, cv::Mat, cv::Mat);

	// Initialization functions
	void createTemplates(int, int);
	void createGaussians(int);
};
