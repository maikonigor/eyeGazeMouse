#include <opencv2/highgui/highgui.hpp>

#include "EyeExtractorSegmentationGroundTruth.h"
#include "Application.h"
#include "utils.h"

const int EyeExtractorSegmentationGroundTruth::eyeDX = 32;
const int EyeExtractorSegmentationGroundTruth::eyeDY = 16;
const cv::Size EyeExtractorSegmentationGroundTruth::eyeSize = cv::Size(eyeDX * 2, eyeDY * 2);

EyeExtractorSegmentationGroundTruth::EyeExtractorSegmentationGroundTruth():
	eyeGrey(new cv::Mat(eyeSize, CV_8UC1)),
	eyeFloat(new cv::Mat(eyeSize, CV_32FC1)),
	
 	// ONUR DUPLICATED CODE FOR LEFT EYE
	eyeGreyLeft(new cv::Mat(eyeSize, CV_8UC1)),
	eyeFloatLeft(new cv::Mat(eyeSize, CV_32FC1)),
	
	eyeImage(new cv::Mat(eyeSize, CV_8UC3)),
	eyeImageLeft(new cv::Mat(eyeSize, CV_8UC3)),

	_isBlinking(false)
{
}

EyeExtractorSegmentationGroundTruth::~EyeExtractorSegmentationGroundTruth() {}

void EyeExtractorSegmentationGroundTruth::process() {
	if (Application::Data::isTrackingSuccessful) {
		// Extract eye images using point tracker results

		char file_detect_eye[50];

		sprintf(file_detect_eye, "../video1/PHOTOSHOP_frameVideo/img_%d.png", Application::Components::videoInput->frameCount-1);

	    cv::Mat image = cv::imread(file_detect_eye);

		extractEye(image);
		extractEyeLeft(image);

		// Blink detection
		//_blinkDetector.update(eyeFloat);
		//_blinkDetectorLeft.update(eyeFloatLeft);

		//if (_blinkDetector.getState() >= 2 && _blinkDetectorLeft.getState() >= 2) {
		//	_isBlinking = true;
		//} else {
		//	_isBlinking = false;
		//}
/*
		// If calibration is active, collect eye image samples, calculate averages and train the system
		if(Application::Components::calibrator->isActive()) {
			if (Application::Components::calibrator->shouldStartNextPoint()) {
				// Switch to next calibration point
				//pointStart();
			} else if (hasValidSample()) {
				// Add the valid training samples
				//averageEye->addSample(eyeFloat.get());
				//averageEyeLeft->addSample(eyeFloatLeft.get());

				// TODO MOVE ADD SAMPLES TO NN CODE
				//Application::Components::gazeTracker->addSampleToNN(Application::Components::calibrator->getActivePoint(), eyeFloat.get(), eyeGrey.get());
				//Application::Components::gazeTracker->addSampleToNNLeft(Application::Components::calibrator->getActivePoint(), eyeFloatLeft.get(), eyeGreyLeft.get());

				//if (Application::Components::calibrator->getPointFrameNo() == Application::dwelltimeParameter - 1) {
				//	pointEnd();
				
					// TODO : NOT NECESSARY FOR THIS COMPONENT
					//if (Application::Components::calibrator->isLastPoint())
					//	calibrationEnded();
				}
			}
		}
	*/
	}
}

bool EyeExtractorSegmentationGroundTruth::isBlinking() {
	return _isBlinking;
}

void EyeExtractorSegmentationGroundTruth::extractEye(cv::Mat originalImage) {
	//cv::imwrite("aaa_segmentation_frame.png", originalImage);

	double x0 = Application::Data::anchorPoints[PointTracker::eyePoint1].x;
	double y0 = Application::Data::anchorPoints[PointTracker::eyePoint1].y;
	double x1 = Application::Data::anchorPoints[PointTracker::eyePoint2].x;
	double y1 = Application::Data::anchorPoints[PointTracker::eyePoint2].y;

	//std::cout << "Points: " << x0 << ", " << y0 << " and " << x1 << ", " << y1 << std::endl;
	// Move the tracked points a little towards center (using weighted sum)
	// so that the extracted image contains more the important area (iris & sclera)
	double alpha = 0.95;

	double temp_x0 = alpha*x0 + (1-alpha)*x1;
	double temp_y0 = alpha*y0 + (1-alpha)*y1;

	x1 = (1-alpha)*x0 + alpha*x1;
	y1 = (1-alpha)*y0 + alpha*y1;

	x0 = temp_x0;
	y0 = temp_y0;

	
	double dx = abs(x1 - x0);	// Horizontal distance between eye corners in image
	double dy = 0.17 * dx;		// %20 percent of horizontal distance (used as height of extracted eyes)

	// Calculate the roll angle of the head (using eye corner positions)
	double rollAngle = atan((y1-y0)/(x1 - x0));

	cv::Point2f originalImagePoints[3];
	cv::Point2f extractedImagePoints[3];

	// Top left pixel
	originalImagePoints[0].x = x0 + sin(rollAngle)*dy/2;
	originalImagePoints[0].y = y0 - cos(rollAngle)*dy/2;

	extractedImagePoints[0].x = 0;
	extractedImagePoints[0].y = 0;

	// Bottom left pixel
	originalImagePoints[1].x = x0 - sin(rollAngle)*dy/2;
	originalImagePoints[1].y = y0 + cos(rollAngle)*dy/2;

	extractedImagePoints[1].x = 0;
	extractedImagePoints[1].y = eyeSize.height-1;
	
	// Top right pixel
	originalImagePoints[2].x = originalImagePoints[0].x + cos(rollAngle)*dy*2;
	originalImagePoints[2].y = originalImagePoints[0].y + sin(rollAngle)*dy*2;

	extractedImagePoints[2].x = eyeSize.width-1;
	extractedImagePoints[2].y = 0;

	// Calculate the transformation matrix between two sets of points, and use it to extract eye image
	cv::Mat transform = cv::getAffineTransform(originalImagePoints, extractedImagePoints);
	warpAffine(originalImage, *eyeImage.get(), transform, eyeSize);
	//cv::cvtColor(*eyeImage.get(), *eyeGrey.get(), CV_RGB2GRAY);
	
	// Apply blurring and normalization
	//Utils::normalizeGrayScaleImage(eyeGrey.get(), 127, 50);	// TODO ONUR UNCOMMENT
	//eyeGrey->convertTo(*eyeFloat, CV_32FC1);
	//cv::GaussianBlur(*eyeFloat, *eyeFloat, cv::Size(3,3), 0);
	//cv::equalizeHist(*eyeGrey, *eyeGrey);

	//cv::circle(originalImage, cv::Point(x0, y0), 3, CV_RGB(0,255,0), -1, 8, 0);
	//cv::circle(originalImage, cv::Point(x1, y1), 3, CV_RGB(0,255,0), -1, 8, 0);
	//cv::imwrite("aaa_segmentation_frame_pts.png", originalImage);
}

void EyeExtractorSegmentationGroundTruth::extractEyeLeft(cv::Mat originalImage) {
	double x0 = Application::Data::anchorPoints[PointTracker::eyePoint2].x;
	double y0 = Application::Data::anchorPoints[PointTracker::eyePoint2].y;
	double x1 = Application::Data::anchorPoints[PointTracker::eyePoint1].x;
	double y1 = Application::Data::anchorPoints[PointTracker::eyePoint1].y;

	// Move the tracked points a little towards center (using weighted sum)
	// so that the extracted image contains more the important area (iris & sclera)
	double alpha = 0.95;

	double temp_x0 = alpha*x0 + (1-alpha)*x1;
	double temp_y0 = alpha*y0 + (1-alpha)*y1;

	x1 = (1-alpha)*x0 + alpha*x1;
	y1 = (1-alpha)*y0 + alpha*y1;

	x0 = temp_x0;
	y0 = temp_y0;


	double dx = abs(x1 - x0);	// Horizontal distance between eye corners in image
	double dy = 0.17 * dx;		// %20 percent of horizontal distance (used as height of extracted eyes)

	// Calculate the roll angle of the head (using eye corner positions)
	double rollAngle = atan((y1-y0)/(x1 - x0));

	cv::Point2f originalImagePoints[3];
	cv::Point2f extractedImagePoints[3];

	// Top right pixel
	originalImagePoints[0].x = x0 + sin(rollAngle)*dy/2;
	originalImagePoints[0].y = y0 - cos(rollAngle)*dy/2;

	extractedImagePoints[0].x = eyeSize.width-1;
	extractedImagePoints[0].y = 0;

	// Bottom right pixel
	originalImagePoints[1].x = x0 - sin(rollAngle)*dy/2;
	originalImagePoints[1].y = y0 + cos(rollAngle)*dy/2;

	extractedImagePoints[1].x = eyeSize.width-1;
	extractedImagePoints[1].y = eyeSize.height-1;
	
	// Top left pixel
	originalImagePoints[2].x = originalImagePoints[0].x - cos(rollAngle)*dy*2;
	originalImagePoints[2].y = originalImagePoints[0].y - sin(rollAngle)*dy*2;

	extractedImagePoints[2].x = 0;
	extractedImagePoints[2].y = 0;

	// Calculate the transformation matrix between two sets of points, and use it to extract eye image
	cv::Mat transform = cv::getAffineTransform(originalImagePoints, extractedImagePoints);
	warpAffine(originalImage, *eyeImageLeft.get(), transform, eyeSize);
	//cv::cvtColor(*eyeImageLeft.get(), *eyeGreyLeft.get(), CV_RGB2GRAY);
	
	// Apply blurring and normalization
	//Utils::normalizeGrayScaleImage(eyeGreyLeft.get(), 127, 50);	// TODO ONUR UNCOMMENT
	//eyeGreyLeft->convertTo(*eyeFloatLeft, CV_32FC1);
	//cv::GaussianBlur(*eyeFloatLeft, *eyeFloatLeft, cv::Size(3,3), 0);
	//cv::equalizeHist(*eyeGreyLeft, *eyeGreyLeft);
}

void EyeExtractorSegmentationGroundTruth::draw() {
	if (!Application::Data::isTrackingSuccessful)
		return;
	
	cv::Mat image = Application::Components::videoInput->debugFrame;
	int eyeDX = eyeSize.width;
	int eyeDY = eyeSize.height;

	int baseX = 0;
	int baseY = 0;
	int stepX = 0;
	int stepY = 2*eyeDY;

	eyeImage->copyTo(image(cv::Rect(baseX + stepX * 1, baseY + stepY * 1, eyeDX, eyeDY)));
	eyeImageLeft->copyTo(image(cv::Rect(baseX + 100, baseY + stepY * 1, eyeDX, eyeDY)));
}

// Prepares the eye extractor for calibration
void EyeExtractorSegmentationGroundTruth::start() {
	pointStart();
}

// When a new calibration point is starting, prepare the average eye accumulators
void EyeExtractorSegmentationGroundTruth::pointStart() {
	averageEye.reset(new FeatureDetector(EyeExtractorSegmentationGroundTruth::eyeSize));
	averageEyeLeft.reset(new FeatureDetector(EyeExtractorSegmentationGroundTruth::eyeSize));
}

void EyeExtractorSegmentationGroundTruth::pointEnd() {}
void EyeExtractorSegmentationGroundTruth::abortCalibration() {}
void EyeExtractorSegmentationGroundTruth::calibrationEnded() {}

bool EyeExtractorSegmentationGroundTruth::hasValidSample() {
	return Application::Components::calibrator->getPointFrameNo() >= 11 && !isBlinking();
}