#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <stdlib.h>

#include "utils.h"
#include "Application.h"

namespace Utils {
	boost::shared_ptr<cv::Mat> createImage(const CvSize &size, int type) {
		return boost::shared_ptr<cv::Mat>(new cv::Mat(size, type), releaseImage);
	}

	void releaseImage(cv::Mat *image) {
		//cout << "deleting shared image" << endl;
		image->release();
	}

	cv::Rect* getMonitorGeometryByIndex(int screenIndex) {
		QDesktopWidget* desktop = QApplication::desktop();

		return new cv::Rect(desktop->availableGeometry(screenIndex).x(),
						desktop->availableGeometry(screenIndex).y(),
						desktop->availableGeometry(screenIndex).width(),
						desktop->availableGeometry(screenIndex).height());
	}

	cv::Rect* getSecondMonitorGeometry() {
		QDesktopWidget* desktop = QApplication::desktop();

		// Return last monitor geometry
		return getMonitorGeometryByIndex(desktop->screenCount()-1);

		// For replaying experiment videos, return false geometry for 1920x1080 monitor
		//return new cv::Rect(0, 0, 1280, 777);
	}

	cv::Rect* getFirstMonitorGeometry() {
		// Return default monitor geometry
		return getMonitorGeometryByIndex(-1);

		// For using a smaller debug screen, override the debug monitor resolution
		//return new cv::Rect(0, 0, 1280, 720);
	}

	void mapToFirstMonitorCoordinates(Point monitor2Point, Point &monitor1Point) {
		cv::Rect *monitor1Geometry = Utils::getFirstMonitorGeometry();
		cv::Rect *monitor2Geometry = Utils::getSecondMonitorGeometry();

		monitor1Point.x = (monitor2Point.x / monitor2Geometry->width) * (monitor1Geometry->width - 40) + monitor1Geometry->x;
		monitor1Point.y = (monitor2Point.y / monitor2Geometry->height) * monitor1Geometry->height + monitor1Geometry->y;
	}

	cv::Point mapFromCameraToDebugFrameCoordinates(cv::Point point) {
		double factor  = Application::Components::videoInput->debugFrame.size().width / (double) Application::Components::videoInput->frame.size().width;

		return cv::Point(factor*point.x, factor*point.y);
	}

	cv::Point mapFromSecondMonitorToDebugFrameCoordinates(cv::Point point) {
		cv::Rect *geometry = Utils::getSecondMonitorGeometry();
		double xFactor  = Application::Components::videoInput->debugFrame.size().width / (double) geometry->width;
		double yFactor  = Application::Components::videoInput->debugFrame.size().height / (double) geometry->height;

		return cv::Point(xFactor*point.x, yFactor*point.y);
	}

	void boundToScreenArea(Point &estimate) {
		cv::Rect *rect = Utils::getSecondMonitorGeometry();

		// If x or y coordinates are outside screen boundaries, correct them
        if (estimate.x < 0) {
			estimate.x = 0;
		}

		if (estimate.y < 0) {
			estimate.y = 0;
		}

		if (estimate.x >= rect->width) {
			estimate.x = rect->width;
		}

		if (estimate.y >= rect->height) {
			estimate.y = rect->height;
		}
	}


	void mapToVideoCoordinates(Point monitor2Point, double resolution, Point &videoPoint, bool reverseX) {
		cv::Rect *monitor1Geometry = new cv::Rect(0, 0, 1280, 720);
		cv::Rect *monitor2Geometry = Utils::getSecondMonitorGeometry();

		if (resolution == 480) {
			monitor1Geometry->width = 640;
			monitor1Geometry->height = 480;
		} else if (resolution == 1080) {
			monitor1Geometry->width = 1920;
			monitor1Geometry->height = 1080;
		}

		if (reverseX) {
			videoPoint.x = monitor1Geometry->width - (monitor2Point.x / monitor2Geometry->width) * monitor1Geometry->width;
		} else {
			videoPoint.x = (monitor2Point.x / monitor2Geometry->width) * monitor1Geometry->width;
		}

		videoPoint.y = (monitor2Point.y / monitor2Geometry->height) * monitor1Geometry->height;
	}

	// Neural network
	void mapToNeuralNetworkCoordinates(Point point, Point &nnPoint) {
		cv::Rect *monitor1Geometry = new cv::Rect(0, 0, 1, 1);
		cv::Rect *monitor2Geometry = Utils::getSecondMonitorGeometry();

		nnPoint.x = ((point.x - monitor2Geometry->x) / monitor2Geometry->width) * monitor1Geometry->width + monitor1Geometry->x;
		nnPoint.y = ((point.y - monitor2Geometry->y) / monitor2Geometry->height) * monitor1Geometry->height + monitor1Geometry->y;

		//cout << "ORIG: " << point.x << ", " << point.y << " MAP: " << nnPoint.x << ", " << nnPoint.y << endl;
	}


	void mapFromNeuralNetworkToScreenCoordinates(Point nnPoint, Point &point) {
		cv::Rect *monitor1Geometry = Utils::getSecondMonitorGeometry();
		cv::Rect *monitor2Geometry = new cv::Rect(0, 0, 1, 1);

		point.x = ((nnPoint.x - monitor2Geometry->x) / monitor2Geometry->width) * monitor1Geometry->width + monitor1Geometry->x;
		point.y = ((nnPoint.y - monitor2Geometry->y) / monitor2Geometry->height) * monitor1Geometry->height + monitor1Geometry->y;

		//cout << "ORIG: " << point.x << ", " << point.y << " MAP: " << nnPoint.x << ", " << nnPoint.y << endl;
	}

	std::string getUniqueFileName(std::string directory, std::string baseFileName) {
		std::string fileAbsName;
		boost::filesystem::path currentDir(directory);
		int maximumExistingNumber = 0;
		//boost::regex pattern(base_file_name + ".*"); // list all files having this base file name

		// Check all the files matching this base file name and find the maximum serial number until now
		for (boost::filesystem::directory_iterator iter(currentDir),end; iter != end; ++iter) {
			std::string name = iter->path().filename().string();
			if (strncmp(baseFileName.c_str(), name.c_str(), baseFileName.length()) == 0) { // regex_match(name, pattern)) {
				//cout << "MATCH: base=" << baseFileName << ", file=" << name << endl;
				int currentNumber = 0;
				name = name.substr(baseFileName.length() + 1);

				//cout << "After substr=" << name << endl;
				sscanf(name.c_str(), "%d", &currentNumber);
				//cout << "NO= " << currentNumber << endl;

				if (currentNumber > maximumExistingNumber) {
					maximumExistingNumber = currentNumber;
				}
			}
		}

		//cout << "Max. existing no = " << maximum_existing_no << endl;

		// Return the next serial number
		return directory + "/" + baseFileName +  "_" + boost::lexical_cast<std::string>(maximumExistingNumber + 1) + ".txt";
	}

	std::string getParameter(std::string name) {
		return Application::Components::mainTracker->args.getOptionValue(name);
	}

	double getParameterAsDouble(std::string name, double defaultValue) {
		double returnValue = defaultValue;

		try {
			returnValue = boost::lexical_cast<double>(getParameter(name));
		} catch(boost::bad_lexical_cast &) {
		}

		return  returnValue;
	}


	std::vector<Point> readAndScalePoints(std::ifstream &in) {
		cv::Rect *rect = getSecondMonitorGeometry();

		return scaled(loadPoints(in), rect->width, rect->height);
	}

	std::vector<Point> loadPoints(std::ifstream &in) {
		std::vector<Point> result;

		for(;;) {
			double x, y;
			in >> x >> y;
			if (in.rdstate()) {
				// break if any error
				break;
			}
			result.push_back(Point(x, y));
		}

		return result;
	}

	std::vector<Point> scaled(const std::vector<Point> &points, double x, double y) {
		std::vector<Point> result;

		xForEach(iter, points) {
			result.push_back(Point(iter->x * x, iter->y * y));
		}

		return result;
	}

	// Normalize by making mean and standard deviation equal in all images
	void normalizeGrayScaleImage(cv::Mat *image, double standardMean, double standardStd) {
		cv::Mat scalarMean;
		cv::Mat scalarStd;
		double mean;
		double std;

		cv::meanStdDev(*image, scalarMean, scalarStd);

		mean = scalarMean.data[0];
		std = scalarStd.data[0];

		//cout << "Image mean and std is " << mean << ", " << std << endl;

		double ratio = standardStd / std;
		double shift = standardMean - mean * ratio;

		cv::convertScaleAbs(*image, *image, ratio, shift);   // Move the mean from 0 to 127
	}

/*
	// Normalize to 50-200 interval
	void normalizeGrayScaleImage2(cv::Mat *image, double standardMean, double standardStd) {
		double minVal;
		double maxVal;
		double scale = 1;
		double intervalStart = 25;
		double intervalEnd = 230;

		cv::minMaxLoc(*image, &minVal, &maxVal);
		cv::convertScale(*image, *image, 1, -1 * minVal);   // Subtract the minimum value
		image->convertTo(*image, )

		// If pixel intensities are between 0 and 1
		if (maxVal < 2) {
			intervalStart = intervalStart / 255.0;
			intervalEnd = intervalEnd / 255.0;
		}

		scale = (intervalEnd - intervalStart) / (maxVal - minVal);

		// Scale the image to the selected interval
		cvConvertScale(*image, *image, scale, 0);
		cvConvertScale(*image, *image, 1, intervalStart);
	}
*/
	void convertAndResize(const cv::Mat &src, cv::Mat& gray, cv::Mat& resized, double scale)
	{
		if (src.channels() == 3) {
			cv::cvtColor(src, gray, CV_BGR2GRAY);
		}
		else {
			gray = src;
		}

		cv::Size sz(cvRound(gray.cols * scale), cvRound(gray.rows * scale));

		if (scale != 1) {
			cv::resize(gray, resized, sz);
		}
		else {
			resized = gray;
		}
	}

	void printMat(CvMat *mat) {
		printf("(%dx%d)\n", mat->cols, mat->rows);
		for (int i = 0; i < mat->rows; i++) {
			if (i == 0) {
				for (int j = 0; j < mat->cols; j++) {
					printf("%10d", j + 1);
				}
			}

			printf("\n%4d: ", i + 1);
			for (int j = 0; j < mat->cols; j++) {
				printf("%10.2f", cvGet2D(mat, i, j).val[0]);
			}
		}

		printf("\n");
	}

	void printMat(cv::Mat mat) {
		printf("(%dx%d)\n", mat.cols, mat.rows);
		for (int i = 0; i < mat.rows; i++) {
			if (i == 0) {
				for (int j = 0; j < mat.cols; j++) {
					printf("%10d", j + 1);
				}
			}

			printf("\n%4d: ", i + 1);
			for (int j = 0; j < mat.cols; j++) {
				printf("%10.2f", mat.at<float>(i, j));
			}
		}

		printf("\n");
	}
}

namespace boost {
	template <> void checked_delete(cv::Mat *image) {
		//cout << "deleting scoped image" << endl;
		if (image) {
			image->release();
		}
	}
}
