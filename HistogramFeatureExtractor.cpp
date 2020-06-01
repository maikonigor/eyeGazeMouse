#include "HistogramFeatureExtractor.h"
#include "Application.h"
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include <fstream>

HistogramFeatureExtractor::HistogramFeatureExtractor() {
	_ellipseMask = cv::imread("./elipse.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    
    int sizeImageDisk = 30;	// TODO ONUR Check disk size 5? 10? lastly 30?
	for (int j=0; j<VECTOR_SIZE; j++){
		createTemplates(j, sizeImageDisk+j*2);

		_gaussian2D[j].create(cv::Size(EyeExtractor::eyeSize.width - _irisTemplateDisk[j].size().width + 1, EyeExtractor::eyeSize.height - _irisTemplateDisk[j].size().height + 1), CV_32FC1);
		_matches[j].create(_gaussian2D[j].size(), CV_32FC1);

		createGaussians(j);
	}

	this->wholeSegmentation.create(cv::Size(EyeExtractor::eyeSize), CV_8UC1);
	this->wholeSegmentationLeft.create(cv::Size(EyeExtractor::eyeSize), CV_8UC1);
	this->eyeSegmentation.create(cv::Size(EyeExtractor::eyeSize), CV_8UC1);
	this->eyeSegmentationLeft.create(cv::Size(EyeExtractor::eyeSize), CV_8UC1);

	this->horizontalFeatures.create(cv::Size(HORIZONTAL_BIN_SIZE, 1), CV_32SC1);
	this->horizontalFeaturesLeft.create(cv::Size(HORIZONTAL_BIN_SIZE, 1), CV_32SC1);
	this->verticalFeatures.create(cv::Size(1, VERTICAL_BIN_SIZE), CV_32SC1);
	this->verticalFeaturesLeft.create(cv::Size(1, VERTICAL_BIN_SIZE), CV_32SC1);

	_groundTruth = new EyeExtractor(true);
	_eyeExtractor = NULL;
}

HistogramFeatureExtractor::~HistogramFeatureExtractor() {
}

void HistogramFeatureExtractor::process() {
    if(_eyeExtractor == NULL) {
        _eyeExtractor = (EyeExtractor*) Application::getComponent("EyeExtractor");
    }
    
	if (Application::Data::isTrackingSuccessful) {
		// If using ground truth, first call it's process() to prepare the segmentation GT
		if(Application::Settings::useGroundTruth) {
			_groundTruth->process();
		}
		// Extract the histogram features (iris detection + segmentation + projection)
    	extractFeatures();
    }
}

// Segmentation and feature extraction
void HistogramFeatureExtractor::extractFeatures(){
	static double minVal[VECTOR_SIZE], maxVal[VECTOR_SIZE];
	static cv::Point minLoc[VECTOR_SIZE], maxLoc[VECTOR_SIZE];

	cv::Mat maskedGreyImage(cv::Size(EyeExtractor::eyeSize), CV_8UC1);

	// Repeat for right and left eyes
	for(int side = 0; side < 2; side++) {
		cv::Mat *eyeGrey 		= (side == 0) ? &_eyeExtractor->eyeGrey 	: &_eyeExtractor->eyeGreyLeft;

		cv::Mat &horizontalFeatures 	= (side == 0) ? this->horizontalFeatures 	: this->horizontalFeaturesLeft;
		cv::Mat &verticalFeatures 		= (side == 0) ? this->verticalFeatures 		: this->verticalFeaturesLeft;

		cv::Mat *wholeSegmentation			= (side == 0) ? &this->wholeSegmentation	: &this->wholeSegmentationLeft;
		cv::Mat *eyeSegmentation			= (side == 0) ? &this->eyeSegmentation		: &this->eyeSegmentationLeft;

		cv::Mat *gtImage					= (side == 0) ? &_groundTruth->eyeImage: &_groundTruth->eyeImageLeft;

		// Copy the masked region to the temporary image and fill the rest with grey
        maskedGreyImage.setTo(cv::Scalar(100,100,100));
        eyeGrey->copyTo(maskedGreyImage, _ellipseMask);

		int j;
		int comparisonMethod = CV_TM_CCOEFF_NORMED;

		// Apply template matching with varying disk sizes to find the iris bounding box
		for (j=0; j<VECTOR_SIZE; j++){
			// Call template matching
			matchTemplate(maskedGreyImage, _irisTemplateDisk[j], _matches[j], comparisonMethod);

			// Multiply with Gaussian to prioritize the central regions
			_matches[j] = _matches[j].mul(_gaussian2D[j]);

			minMaxLoc(_matches[j], &minVal[j], &maxVal[j], &minLoc[j], &maxLoc[j]);
		}

		// Find the disk size with the best likelihood
		double maxProbability = (double) maxVal[0];
		int bestDiskSize = 0;

		for (j=1; j<VECTOR_SIZE; j++){
			// Calculate the likelihood for the best match for this disk size
			double tmp = (double) maxVal[j] * (1+j*j/300.0);	// Prioritize larger disk sizes

			if (tmp > maxProbability){
				maxProbability = tmp;
				bestDiskSize = j;
			}
		}

		// Apply segmentation on the whole eye image
		if(Application::Settings::useGroundTruth) {
			calculateSegmentation(*gtImage, *wholeSegmentation);
		}
		else {
			calculateSegmentation(maskedGreyImage, *wholeSegmentation);
		}

		// Get the bounding box for the best iris match, and copy the segmentation corresponding to that region into eyeSegmentation
		if(Application::Settings::useGroundTruth) {
			(*wholeSegmentation).copyTo(*eyeSegmentation);
		}
		else {
			cv::Rect roi = cv::Rect(maxLoc[bestDiskSize].x, maxLoc[bestDiskSize].y, _irisTemplateDisk[bestDiskSize].size().width, _irisTemplateDisk[bestDiskSize].size().height);
			eyeSegmentation->setTo(cv::Scalar(0));
            
            (*wholeSegmentation)(roi).copyTo((*eyeSegmentation)(roi));
		}

		// Extract histogram features from eye segmentation
		calculateHistogram(*eyeSegmentation, horizontalFeatures, verticalFeatures);
	}

}

void HistogramFeatureExtractor::calculateSegmentation(cv::Mat image, cv::Mat result) {
	if(Application::Settings::useGroundTruth) {
		// specify the range of colours that you want to include, you can play with the borders here
		cv::Scalar lowerb = cv::Scalar(0,150,0);
		cv::Scalar upperb = cv::Scalar(30,255,30);

		cv::inRange(image, lowerb, upperb, result); // if the frame has any orange pixel, this will be painted in the mask as white
	}
	else {
		threshold(image, result, 80, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);	// TODO ONUR Check: 0-255 limits
		bitwise_not(result, result);
	}
}

void HistogramFeatureExtractor::calculateHistogram(	cv::Mat bwIris,
														cv::Mat horizontalFeatures,
														cv::Mat verticalFeatures) {
	// Use matrix reduction to calculate sum of rows and columns for horizontal and vertical features
	cv::Mat iris = bwIris / 255;

	cv::reduce(iris, horizontalFeatures, 0, CV_REDUCE_SUM, CV_32SC1);	// iris(cv::Rect(0, 0, 128, 20)) takes some piec for calculations
	cv::reduce(iris, verticalFeatures, 1, CV_REDUCE_SUM, CV_32SC1);

	return;
}

// Initialization
void HistogramFeatureExtractor::createTemplates(int index, int sizeDisk) {
	_irisTemplateDisk[index].create(cv::Size(sizeDisk, sizeDisk), CV_8UC1);

	cv::Point center;
	int radius = sizeDisk/2;
	center.x = floor(radius);
	center.y = floor(radius);

	_irisTemplateDisk[index].setTo(cv::Scalar(255,255,255));

	cv::circle(_irisTemplateDisk[index], center, radius, cv::Scalar(0, 0, 0), -1);
}

void HistogramFeatureExtractor::createGaussians(int index) {
	cv::Point center;
	center.x = floor(_gaussian2D[index].size().width/2);
	center.y = floor(_gaussian2D[index].size().height/2);

	float tmp = 0;
	float sigma = 200;	// With this sigma, the furthest eye pixel (corners) have around 0.94 probability
	float max_prob = exp( - (0) / (2.0 * sigma * sigma)) / (2.0 * M_PI * sigma * sigma);


	for (int x = 0; x < _gaussian2D[index].size().width; x++) {
		for (int y = 0; y < _gaussian2D[index].size().height; y++) {
			float fx = abs(x - center.x);
			float fy = abs(y - center.y);

			tmp = exp( - (fx * fx + fy * fy) / (2.0 * sigma * sigma)) / (2.0 * M_PI * sigma * sigma);
			tmp = tmp / max_prob;	// Divide by max prob. so that values are in range [0, 1]

			_gaussian2D[index].at<float>(cv::Point(x,y)) = tmp;
		}
	}
}

// Drawing on debug window
void HistogramFeatureExtractor::draw() {
	if (!Application::Data::isTrackingSuccessful)
		return;

	cv::Mat image = Application::Components::videoInput->debugFrame;
	int eyeDX = EyeExtractor::eyeSize.width;
	int eyeDY = EyeExtractor::eyeSize.height;

	int baseX = 0;
	int baseY = 0;
	int stepX = 0;
	int stepY = eyeDY+10;

	static cv::Mat temp(cv::Size(EyeExtractor::eyeSize), CV_8UC3);
	temp.setTo(cv::Scalar(0,255,0));

	// Draw the histogram features on the debug window
	for(int side = 0; side < 2; side++) {
		cv::Mat segmentationImage	= image(cv::Rect(baseX + side*140, baseY + stepY * 1, eyeDX, eyeDY));
		cv::Mat horizontalImage 	= image(cv::Rect(baseX + side*140, baseY + stepY * 2, eyeDX, eyeDY));
		cv::Mat verticalImage 		= image(cv::Rect(baseX + side*140, baseY + stepY * 3, eyeDX, eyeDY));

		cv::Mat &eyeImage				= (side == 0) ? _eyeExtractor->eyeImage		: _eyeExtractor->eyeImageLeft;
		cv::Mat &eyeSegmentation		= (side == 0) ? this->eyeSegmentation		: this->eyeSegmentationLeft;
		cv::Mat &horizontalFeatures 	= (side == 0) ? this->horizontalFeatures 	: this->horizontalFeaturesLeft;
		cv::Mat &verticalFeatures 		= (side == 0) ? this->verticalFeatures 		: this->verticalFeaturesLeft;

		// Draw the segmentations
		eyeImage.copyTo(segmentationImage);
		temp.copyTo(segmentationImage, eyeSegmentation);

		// Draw horizontal features
		horizontalImage.setTo(cv::Scalar(0,0,0));

		//std::cout << "Horizontal features size: " << horizontalFeatures.size() << std::endl;

		for (int i = 0; i < horizontalFeatures.size().width; i++) {
		    line(horizontalImage,
		    	cv::Point(i, horizontalImage.size().height),
			    cv::Point(i, horizontalImage.size().height - horizontalFeatures.at<int>(i)),
			    cv::Scalar(255,255,255));
		}

		// Draw vertical features
		verticalImage.setTo(cv::Scalar(0,0,0));

		for (int i = 0; i < verticalFeatures.size().height; i++) {
		    line(verticalImage,
					cv::Point(verticalFeatures.at<int>(i), i),
					cv::Point(0, i),
			    cv::Scalar(255,255,255));
		}
	}

}
