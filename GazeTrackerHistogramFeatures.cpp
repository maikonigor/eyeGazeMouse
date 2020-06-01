#include <fstream>
#include <boost/lexical_cast.hpp>

#include "GazeTrackerHistogramFeatures.h"
#include "Point.h"
#include "mir.h"
#include "Application.h"

GazeTrackerHistogramFeatures::GazeTrackerHistogramFeatures()
{
	// Initialize calibration sample accumulation variables
	_currentTargetSamples.create(cv::Size(MAX_SAMPLES_PER_TARGET, FEATURE_DIM), CV_32FC1);
	_currentTargetSamplesLeft.create(cv::Size(MAX_SAMPLES_PER_TARGET, FEATURE_DIM), CV_32FC1);

	clearTargetSamples();

	// And the variables to hold the current sample
	_currentSample.create(cv::Size(1, FEATURE_DIM), CV_32FC1);
	_currentSampleLeft.create(cv::Size(1, FEATURE_DIM), CV_32FC1);

	gazePoint.x = 0;
	gazePoint.y = 0;
	
	_histFeatureExtractor = NULL;
	_eyeExtractor = NULL;
}

void GazeTrackerHistogramFeatures::process() {
    if(_histFeatureExtractor == NULL) {
        _histFeatureExtractor = (HistogramFeatureExtractor*) Application::getComponent("HistogramFeatureExtractor");
    }
    if(_eyeExtractor == NULL) {
        _eyeExtractor = (EyeExtractor*) Application::getComponent("EyeExtractor");
    }
    
	if (!Application::Data::isTrackingSuccessful) {
		return;
	}
    
	// If recalibration is necessary (there is a new target), recalibrate the Gaussian Processes
	if(Application::Components::calibrator->needRecalibration) {
		addExemplar();
	}

	// Combine the current horizontal and vertical features to come up with the final features
	cv::Mat horizontalTransposed = _histFeatureExtractor->horizontalFeatures.t();
	horizontalTransposed.copyTo(_currentSample(cv::Rect(0, 0, 1, HORIZONTAL_BIN_SIZE)));

	_histFeatureExtractor->verticalFeatures.copyTo(_currentSample(cv::Rect(0, HORIZONTAL_BIN_SIZE, 1, VERTICAL_BIN_SIZE)));

	horizontalTransposed = _histFeatureExtractor->horizontalFeaturesLeft.t();
	horizontalTransposed.copyTo(_currentSampleLeft(cv::Rect(0, 0, 1, HORIZONTAL_BIN_SIZE)));

	_histFeatureExtractor->verticalFeaturesLeft.copyTo(_currentSampleLeft(cv::Rect(0, HORIZONTAL_BIN_SIZE, 1, VERTICAL_BIN_SIZE)));

	// If active and there is a usable frame
	if(Application::Components::calibrator->isActive()
		&& Application::Components::calibrator->getPointFrameNo() >= 11
		&& !_eyeExtractor->isBlinking()) {
			// Copy the current histogram feature sample to the corresponding row in the accumulation matrices (currentTargetSamples)
			_currentSample.copyTo(_currentTargetSamples(cv::Rect(_currentTargetSampleCount, 0, 1, FEATURE_DIM)));
			_currentSampleLeft.copyTo(_currentTargetSamplesLeft(cv::Rect(_currentTargetSampleCount, 0, 1, FEATURE_DIM)));

			// Increment the sample counter
			_currentTargetSampleCount++;
	}

	// Update the left and right estimations
	updateEstimations();
}

void GazeTrackerHistogramFeatures::draw() {
	if (!Application::Data::isTrackingSuccessful)
		return;

	cv::Mat image = Application::Components::videoInput->debugFrame;

	// If not blinking, draw the estimations to debug window
	if (isActive() && !_eyeExtractor->isBlinking()) {
		cv::Point estimation(gazePoint.x, gazePoint.y);

		cv::circle(image,
			Utils::mapFromSecondMonitorToDebugFrameCoordinates(estimation),
			8, cv::Scalar(255, 0, 0), -1, 8, 0);
	}
}

// Decides if the tracker is active
bool GazeTrackerHistogramFeatures::isActive() {
	return _histX.get() && _histY.get();
}

void GazeTrackerHistogramFeatures::clear() {
    // Clear previous calibration information
    _exemplars.clear();
    _exemplarsLeft.clear();
    
    _histX.reset(NULL);
    _histY.reset(NULL);
    _histXLeft.reset(NULL);
    _histYLeft.reset(NULL);
}

// Adds the exemplar for the current calibration target into the exemplars vectors
void GazeTrackerHistogramFeatures::addExemplar() {
	// Create the new sample that is added (create in heap)
	cv::Mat *exemplar 		= new cv::Mat(cv::Size(1, FEATURE_DIM), CV_32FC1);
	cv::Mat *exemplarLeft 	= new cv::Mat(cv::Size(1, FEATURE_DIM), CV_32FC1);

	// Calculate the average of collected samples for this target and store in exemplar
	cv::reduce(_currentTargetSamples(cv::Rect(0, 0, _currentTargetSampleCount, FEATURE_DIM)), 		*exemplar, 		1, CV_REDUCE_AVG, CV_32FC1);
	cv::reduce(_currentTargetSamplesLeft(cv::Rect(0, 0, _currentTargetSampleCount, FEATURE_DIM)), 	*exemplarLeft,	1, CV_REDUCE_AVG, CV_32FC1);

	//cv::imwrite("aaa_histFeatures_" + boost::lexical_cast<std::string>(Application::Components::calibrator->getPointNumber()) + "_exemplar.png", *exemplar);

	// Add the exemplar to the targets vector
	_exemplars.push_back(*exemplar);
	_exemplarsLeft.push_back(*exemplarLeft);

	// Clear the used samples
	clearTargetSamples();

	trainGaussianProcesses();
}

// Uses the current sample to calculate the gaze estimation
void GazeTrackerHistogramFeatures::updateEstimations() {
	if (isActive()) {
		// Update estimations
		gazePoint.x = (_histX->getmean(_currentSample) + _histXLeft->getmean(_currentSampleLeft)) / 2;
		gazePoint.y = (_histY->getmean(_currentSample) + _histYLeft->getmean(_currentSampleLeft)) / 2;

        // Make sure estimation stays in the screen area
		Utils::boundToScreenArea(gazePoint);
	}
}

void GazeTrackerHistogramFeatures::trainGaussianProcesses() {
	std::vector<double> xLabels;
	std::vector<double> yLabels;

	// Prepare separate vector of targets for X and Y directions
	for (int i = 0; i < Application::Data::calibrationTargets.size(); i++) {
		xLabels.push_back(Application::Data::calibrationTargets[i].x);
		yLabels.push_back(Application::Data::calibrationTargets[i].y);
	}

	// Re-train the GP estimators with the exemplars for the calibration targets
	// and the corresponding X and Y labels
	_histX.reset(new HistProcess(_exemplars, xLabels, covarianceFunctionSE, 0.01));
	_histY.reset(new HistProcess(_exemplars, yLabels, covarianceFunctionSE, 0.01));

	_histXLeft.reset(new HistProcess(_exemplarsLeft, xLabels, covarianceFunctionSE, 0.01));
	_histYLeft.reset(new HistProcess(_exemplarsLeft, yLabels, covarianceFunctionSE, 0.01));
}

// Clears the matrix buffer where samples are added for each calibratin target
// Prepares the tracker for the next calibration target samples
void GazeTrackerHistogramFeatures::clearTargetSamples() {
	_currentTargetSamples.setTo(cv::Scalar(0,0,0));
	_currentTargetSamplesLeft.setTo(cv::Scalar(0,0,0));

	_currentTargetSampleCount = 0;
}

double GazeTrackerHistogramFeatures::covarianceFunctionSE(const cv::Mat &histogram1, const cv::Mat &histogram2) {
	//const double sigma = 125.0; // 1.0; // 50.0
	//const double lscale = 250.0; // 500.00; // 100.0
	static double sigma = Utils::getParameterAsDouble("sigma", 5.0);
	static double lscale = Utils::getParameterAsDouble("lscale", 250.0);

	// Calculate the squared L2 norm (sum of squared diff)
	double norm = cv::norm(histogram1, histogram2, cv::NORM_L2); // NORM_L1
	norm = norm*norm; // Commented out maybe

	// Return the squared exponential kernel output
    return sigma*sigma*exp(-norm / (2*lscale*lscale) );
}

/*
// Covariance function for the GP gaze estimator for X axis
double GazeTrackerHistogramFeatures::covarianceFunctionOrderedX(cv::Mat histogram1, cv::Mat histogram2) {
	return covarianceFunctionBaseOrdered(histogram1, histogram2, 125.0, 250.0, 1, 1);
}

// Covariance function for the GP gaze estimator for Y axis
double GazeTrackerHistogramFeatures::covarianceFunctionOrderedY(cv::Mat histogram1, cv::Mat histogram2) {
	return covarianceFunctionBaseOrdered(histogram1, histogram2, 175.0, 1000.0, 1, 25);
}

// Base covariance function shared between X and Y axes
double GazeTrackerHistogramFeatures::covarianceFunctionBaseOrdered(cv::Mat histogram1, cv::Mat histogram2, double sigma, double lscale, int horizontalIncrement, int verticalIncrement) {


	// TODO ONUR Sort histograms according to the following, wherever necessary
	// SortHistogram(horizontalFeatures, verticalFeatures, histPositionSegmentedPixels);

	// Order the histograms and calculate the covariance between the ordered elements
	double norm = 0.0;

	int numberToLookFor;

	bool numberFound = false;

	int aux_hist1 = 0;
	int aux_hist2 = 0;

	double horizontal_hist1 = 0, vertical_hist1 = 0, horizontal_hist2 = 0, vertical_hist2 = 0;

	for (int i = 0; i < histogram1.size(); i++) {
		for (int j = 0; j < histogram1[i].size(); j++) {
			// Get the next number from the first histogram
			numberToLookFor = histogram1[i][j];

			// Search the number in the second histogram
			for (int e = 0; e < histogram2.size(); e++) {
				for (int r = 0; r < histogram2[e].size(); r++) {
					if (numberToLookFor == histogram2[e][r]) {
						// If number is found, add the
						if (histogram1[i][j] < 128) {
							norm += abs(horizontal_hist1 - horizontal_hist2);
						} else {
							norm += abs(vertical_hist1 - vertical_hist2);
						}

						numberFound = true;
						break;
					}
				} // End search in the second dimension

				// If the number is not found in this subvector (second dimension of histogram2)
				if (numberFound == false) {
					for (int r = 0; r < histogram2[e].size(); r++) {
						if (histogram2[e][r] < 128) {
							horizontal_hist2 += horizontalIncrement;
						} else {
							vertical_hist2 += verticalIncrement;
						}
					}
				} else {
					horizontal_hist2 = 0;
					vertical_hist2 = 0;
					aux_hist2 = 0;
					numberFound = false;
					break;
				}
			} // End search for the number
		} // End second dimension of histogram1

		for (int j = 0; j < histogram1[i].size(); j++) {
			if (histogram1[i][j] < 128) {
				horizontal_hist1 += horizontalIncrement;
			} else {
				vertical_hist1 += verticalIncrement;
			}
		}
	}

	norm = sigma*sigma*exp(-norm / (2*lscale*lscale) );

	return norm;
}

void ExtractEyeFeaturesSegmentation::SortHistogram(	std::vector<int>* histHorizontalPixelsSegmented,
													std::vector<int>* histVerticalPixelsSegmented,
													std::vector<std::vector<int> >* histPositionSegmentedPixels) {

	int left_pos = 0, right_pos = 0, pos_mean = 0, acum = 0;
	bool bool_left_pos = false, bool_right_pos = false;

	std::cout << "RIGHT ACUM SORT" << std::endl;

	for (int j = 0; j < histHorizontalPixelsSegmented->size(); j++) {

	    if ((left_pos != histHorizontalPixelsSegmented->operator[](j)) && (bool_left_pos == false)) {
	        left_pos = j;
	        bool_left_pos = true;
	    }

	    if ((right_pos != histHorizontalPixelsSegmented->operator[](histHorizontalPixelsSegmented->size()-j)) && (bool_right_pos == false)) {
	        right_pos = histHorizontalPixelsSegmented->size()-j;
	        bool_right_pos = true;
	    }

	    acum += histHorizontalPixelsSegmented->operator[](j);
	}

	std::cout << "RIGHT MEAN SORT" << std::endl;

	acum = acum / 2;

	for (int j = 0; j < histHorizontalPixelsSegmented->size(); j++) {

	    if (acum > 0) {
	    	acum -= histHorizontalPixelsSegmented->operator[](j);
	    	if (acum <= 0) {
	    		pos_mean = j;
	    	}
	    }
	}

	std::cout << "RIGHT GAUSSIAN SORT" << std::endl;

	double sigma = 20;
	double mean = pos_mean;
	double diff = 0.0;

	for (int j = 0; j < histHorizontalPixelsSegmented->size(); j++) {
		diff = mean-j;

		histHorizontalPixelsSegmented->operator[](j) = floor(histHorizontalPixelsSegmented->operator[](j) * exp( - diff*diff/ (2*sigma*sigma) ) );

	}




	bool_left_pos = false, left_pos = 0, bool_right_pos = false, right_pos = 0, pos_mean = 0, acum = 0;

	std::cout << "LEFT ACUM SORT" << std::endl;

	for (int j = 0; j < histVerticalPixelsSegmented->size(); j++) {

	    if ((left_pos != histVerticalPixelsSegmented->operator[](j)) && (bool_left_pos == false)) {
	        left_pos = j;
	        bool_left_pos = true;
	    }

	    if ((right_pos != histVerticalPixelsSegmented->operator[](histVerticalPixelsSegmented->size()-j)) && (bool_right_pos == false)) {
	        right_pos = histVerticalPixelsSegmented->size()-j;
	        bool_right_pos = true;
	    }

	    acum += histVerticalPixelsSegmented->operator[](j);
	}

	std::cout << "LEFT MEAN SORT" << std::endl;

	acum = acum / 2;

	for (int j = 0; j < histVerticalPixelsSegmented->size(); j++) {

	    if (acum > 0) {
	    	acum -= histVerticalPixelsSegmented->operator[](j);
	    	if (acum <= 0) {
	    		pos_mean = j;
	    	}
	    }
	}

	std::cout << "LEFT GAUSSIAN SORT" << std::endl;


	sigma = 20;
	mean = pos_mean;

	for (int j = 0; j < histVerticalPixelsSegmented->size(); j++) {
		diff = mean-j;

		histVerticalPixelsSegmented->operator[](j) = floor(histVerticalPixelsSegmented->operator[](j) * exp( - diff*diff/ (2*sigma*sigma) ) );

	}



	histPositionSegmentedPixels->clear();

	std::cout << "HIST" << std::endl;

	int max = -1;

	// Obtencion del maximo

	for (int j = 0; j < histHorizontalPixelsSegmented->size(); j++) {

	    if (max < histHorizontalPixelsSegmented->operator[](j)) {
	        max = histHorizontalPixelsSegmented->operator[](j);
	    }
	}

	for (int j = 0; j < histVerticalPixelsSegmented->size(); j++) {

	    if (max < histVerticalPixelsSegmented->operator[](j)) {
	        max = histVerticalPixelsSegmented->operator[](j);
	    }
	}

	int aux_max = -1;

	// Colocacion de los elementos que contengan ese maximo en un vector

	std::vector<int> aux_histPosition;

	while (max > -1) {

	    for (int j = 0; j < histHorizontalPixelsSegmented->size(); j++) {

	        if (histHorizontalPixelsSegmented->operator[](j) == max) {

	            aux_histPosition.push_back(j);

	        } else if ((histHorizontalPixelsSegmented->operator[](j) < max) &&
	            (aux_max < histHorizontalPixelsSegmented->operator[](j))) {

	            aux_max = histHorizontalPixelsSegmented->operator[](j);
	        }
	    }

	    for (int j = 0; j < histVerticalPixelsSegmented->size(); j++) {

	        if (histVerticalPixelsSegmented->operator[](j) == max) {

	            aux_histPosition.push_back(j + histHorizontalPixelsSegmented->size());

	        } else if ((histVerticalPixelsSegmented->operator[](j) < max) &&
	            (aux_max < histVerticalPixelsSegmented->operator[](j))) {

	            aux_max = histVerticalPixelsSegmented->operator[](j);
	        }
	    }

	    histPositionSegmentedPixels->push_back(aux_histPosition);

		aux_histPosition.clear();

	    max = aux_max;
	    aux_max = -1;

	}

	std::cout << "histPositionSegmentedPixels: " << std::endl;

	for (int i = 0; i < histPositionSegmentedPixels->size(); i++) {
		for (int j = 0; j < histPositionSegmentedPixels->operator[](i).size(); j++) {
			std::cout << histPositionSegmentedPixels->operator[](i).operator[](j) << " ";
		}
		std::cout << std::endl;
	}


	std::cout << "Size of vector[1]: " << histPositionSegmentedPixels->size() << std::endl;

}

*/
