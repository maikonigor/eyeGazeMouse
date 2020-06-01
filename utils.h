#pragma once

#include <boost/shared_ptr.hpp>
#include <iostream>

#include "Point.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>

namespace Utils {
	typedef boost::shared_ptr<const cv::Mat> SharedImage;

	#define xForEach(iter, container) \
		for (typeof(container.begin()) iter = container.begin(); iter != container.end(); iter++)

	#define xForEachBack(iter, container) \
		for (typeof(container.rbegin()) iter = container.rbegin(); iter != container.rend(); iter++)

	struct QuitNow: public std::exception {
		using std::exception::what;

		QuitNow() { }
		virtual ~QuitNow() throw() { }
		virtual const char* what() throw() {
			return "QuitNow: request normal termination of program.\n(You should not see this message. Please report it if you do.)";
		}
	};

	template <class T> inline T square(T a) {
		return a * a;
	}

	template <class T> std::ostream &operator<< (std::ostream &out, std::vector<T> const &vec) {
		out << vec.size() << std::endl;
		xForEach(iter, vec) {
			out << *iter << std::endl;
		}
		return out;
	}

	template <class T> std::istream &operator>> (std::istream &in, std::vector<T> &vec) {
		int size;
		T element;

		vec.clear();
		in >> size;
		for (int i = 0; i < size; i++) {
			in >> element;
			vec.push_back(element);
		}

		return in;
	}

	template <class T> T teeFunction(T source, char *prefix, char *postfix="\n") {
		std::cout << prefix << source << postfix;
		return source;
	}

	#define debugTee(x) teeFunction(x, #x ": ")

	class ConstancyDetector {
	public:
		ConstancyDetector(int maxCounter):
			_value(-1),
			_counter(0),
			_maxCounter(maxCounter)
		{
		}

		bool isStable() {
			return _counter >= _maxCounter;
		}

		bool isStableExactly() {
			return _counter == _maxCounter;
		}

		bool observe(int newValue) {
			if (newValue != _value || newValue < 0) {
				_counter = 0;
			} else {
				_counter++;
			}

			_value = newValue;
			return isStable();
		}

		void reset() {
			_counter = 0;
			_value = -1;
		}

	private:
		int _value;
		int _counter;
		int _maxCounter;
	};

	// #define output(X) { std::cout << #X " = " << X << std::endl; }

	template <class T> int maxAbsIndex(T const &vec, int size) {
		int maxIndex = 0;

		for (int i = 0; i < size; i++) {
			if (fabs(vec[i]) > fabs(vec[maxIndex])) {
				maxIndex = i;
			}
		}

		return maxIndex;
	}


	boost::shared_ptr<cv::Mat> createImage(const CvSize &size, int type);
	void releaseImage(cv::Mat *image);

	cv::Rect* getMonitorGeometryByIndex(int screenIndex);
	cv::Rect* getSecondMonitorGeometry();
	cv::Rect* getFirstMonitorGeometry();
	
	void mapToFirstMonitorCoordinates(Point monitor2Point, Point &monitor1Point);
	cv::Point mapFromCameraToDebugFrameCoordinates(cv::Point point);
	cv::Point mapFromSecondMonitorToDebugFrameCoordinates(cv::Point point);
	
	void mapToVideoCoordinates(Point monitor2Point, double resolution, Point &videoPoint, bool reverseX=true);
	void mapToNeuralNetworkCoordinates(Point point, Point &nnPoint);
	void mapFromNeuralNetworkToScreenCoordinates(Point nnPoint, Point &point);

	void boundToScreenArea(Point &estimate);
	
	std::string getUniqueFileName(std::string directory, std::string baseFileName);
	std::string getParameter(std::string name);
	double getParameterAsDouble(std::string name, double defaultValue=0.0);
	
	// Functions for reading calibration and testing targets
	std::vector<Point> readAndScalePoints(std::ifstream &in);
	std::vector<Point> loadPoints(std::ifstream &in);
	std::vector<Point> scaled(const std::vector<Point> &points, double x, double y);
	
	void normalizeGrayScaleImage(cv::Mat *image, double standardMean=127, double standardStd=50);
	//void normalizeGrayScaleImage2(cv::Mat *image, double standardMean=127, double standardStd=50);

	void convertAndResize(const cv::Mat &src, cv::Mat& gray, cv::Mat& resized, double scale);
	
	void printMat(CvMat *mat);
	void printMat(cv::Mat mat);
}

namespace boost {
	template <> void checked_delete(cv::Mat *image);
}

