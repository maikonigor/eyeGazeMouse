#include <QApplication>
#include <QDesktopWidget>

#include "ImageWindow.h"
#include "Application.h"
#include "utils.h"

ImageWindow::ImageWindow(int screenIndex, bool debugSize) {
	cv::Rect* geometry = Utils::getSecondMonitorGeometry();

	if(screenIndex == 1) {
		geometry = Utils::getFirstMonitorGeometry();
	}
    
	cv::Size windowSize(geometry->width, geometry->height);

	// If it's a debug window, get the debug monitor geometry
	// and read the image resolution from video input
	if(debugSize) {
		windowSize = Application::Components::videoInput->debugFrame.size();
	}

	// Center window in the monitor it belongs to
	int x = geometry->x + (int) ((geometry->width - windowSize.width)/2);
	int y = geometry->y;

	this->setFixedSize(windowSize.width, windowSize.height);
	this->move(x, y);

	imageWidget = new ImageWidget(0, windowSize);
	this->setCentralWidget(imageWidget);
}

ImageWindow::~ImageWindow() {}

void ImageWindow::showImage(cv::Mat image) {
	imageWidget->showImage(image);
}

// Override the QMainWindow show function so that we can decide whether we want
// to hide all the windows (for experiment reruns) or not
void ImageWindow::show() {
	if(Application::Settings::noWindows)
		return;

	QMainWindow::show();
}
