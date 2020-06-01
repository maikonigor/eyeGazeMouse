#include "GoogleGlassWindow.h"
#include "Application.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

GoogleGlassWindow::GoogleGlassWindow():
	_window(1, false),
	_window2(2, false)
{
	std::cout << "GGW constructor" << std::endl;
	_window.setWindowTitle("Opengazer Images");

	_window2.setWindowTitle("Opengazer Text");

	//_interfaceStimuli.create(cv::Size(rect.get_width(), rect.get_height()), CV_8UC3);

	_interfaceStimuli.create(cv::Size(_window.size().width(),
								_window.size().height()),
								CV_8UC3);
	_interfaceStimuliWithOverlay.create(cv::Size(_window.size().width(),
								_window.size().height()),
								CV_8UC3);


	_pictureResized.create(cv::Size(_window.size().width()/picturesX,
								 	 _window.size().height()/picturesY),
								 	 CV_8UC3);


	_interfaceText.create(cv::Size(_window2.size().width(),
								_window2.size().height()),
								CV_8UC3);



	//_window.show();
	_window2.show();


	// Load images and prepare the interface image
	std::string imagesDir = "./glass";

	for (int i = 0; i < picturesX; i++) {

		for (int j = 0; j < picturesY; j++) {
			int imageIndex = i * picturesY + j + 1;

			// Load the stimuli image (photos of places) and store in the base image
			cv::Mat picture = cv::imread(imagesDir + "/" + boost::lexical_cast<std::string>(imageIndex) + ".jpg", CV_LOAD_IMAGE_COLOR);
			cv::resize(picture, _pictureResized, _pictureResized.size());
			cv::Rect roiRect = cv::Rect(_pictureResized.size().width * i,
										_pictureResized.size().height * j,
										_pictureResized.size().width,
										_pictureResized.size().height);

			_pictureResized.copyTo(_interfaceStimuli(roiRect));


			// Load the corresponding text image
			cv::Mat text = cv::imread(imagesDir + "/" + boost::lexical_cast<std::string>(imageIndex) + "_text.jpg", CV_LOAD_IMAGE_COLOR);

			cv::resize(text, text, _interfaceText.size());

			_textImages.push_back(text);
		}
	}

}

GoogleGlassWindow::~GoogleGlassWindow() {}

// Main processing function
void GoogleGlassWindow::process() {

	if(!isActive())
		return;

	if(Application::status == Application::STATUS_CALIBRATED) {
		_window.show();
	}
	else {
		_window.hide();
		return;
	}

	// Get the current estimation and calculate the image index it corresponds to
	int estimation_x = Application::Data::gazePoints[0].x;
	int estimation_y = Application::Data::gazePoints[0].y;

	//std::cout << "Estimations: " << estimation_x << ", " << estimation_y << std::endl;

	int index_x = estimation_x / (_interfaceStimuli.size().width / picturesX);
	int index_y = estimation_y / (_interfaceStimuli.size().height / picturesY);
	int gazedImageIndex = (index_x * picturesX) + index_y;

	// Show the related text information on the second screen
	if (gazedImageIndex < (picturesX * picturesY)) {
		cv::resize(_textImages[gazedImageIndex], _interfaceText, _interfaceText.size());
	}
	else {
		_interfaceText.setTo(cv::Scalar(255,255,255));
	}

	// Draw rectangle around the image subject is looking at
	_interfaceStimuli.copyTo(_interfaceStimuliWithOverlay);
	cv::Rect roiRect = cv::Rect(index_x * _interfaceStimuli.size().width/picturesX, index_y * _interfaceStimuli.size().height/picturesY, _interfaceStimuli.size().width/picturesX, _interfaceStimuli.size().height/picturesY);
	cv::rectangle(_interfaceStimuliWithOverlay, roiRect, cv::Scalar(255,0,0), 10, 8);


	_window.showImage(_interfaceStimuliWithOverlay);
	_window2.showImage(_interfaceText);
}


// Start testing procedure. Reset frame counter and save target point locations
void GoogleGlassWindow::start() {
	_window.show();

}

// Aborts calibration
void GoogleGlassWindow::abortGoogleGlassWindow() {
	_window.hide();
}

// Checks if calibrator is active
bool GoogleGlassWindow::isActive() {
	return true;
	//return _frameNumber >= 0;
}

void GoogleGlassWindow::draw() {
	if(isActive()) {



	}
}
