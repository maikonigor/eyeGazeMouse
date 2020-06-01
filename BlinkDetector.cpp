#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

#include "BlinkDetector.h"
#include "EyeExtractor.h"
#include <sys/time.h>
#include "Application.h"

BlinkDetector::BlinkDetector():
	_averageEye(new cv::Mat(EyeExtractor::eyeSize, CV_32FC1)),
	_frameCount(0),
	//_accumulator(0.1, 1000.0),
	//_states(constructStates()),
	_initialized(false),
    isBlinking(false),
	state(OPEN)

{
}

void BlinkDetector::resetState(){
	state = OPEN;
	_frameCount = 0;
}

/*
 * Versão que conta tempo
 *
void BlinkDetector::update(const cv::Mat &eyeFloat) {
	if (!_initialized) {
		eyeFloat.copyTo(*_averageEye.get());
		_initialized = true;
	}

	static long lastTimeCount = 0;
	long currentTimeCount;

	double distance = cv::norm(eyeFloat, *_averageEye, CV_L2);
    double norm = cv::norm(*_averageEye, CV_L2);
    
    // Normalize distance by the norm of the previous eye image
    distance = distance / norm;
    
    timeval tv;
	gettimeofday(&tv, 0);

	currentTimeCount = (long) tv.tv_usec;
    isBlinking = (distance > 0.18);

    if(isBlinking){
//    	std::cout << "Closed Distance is " << distance << std::endl;
    	if((currentTimeCount -  lastTimeCount > 200000) && state != VOLUNTARY_BLINK){
			std::cout<< "*****************" << std::endl;
			std::cout<< "last " << lastTimeCount << std::endl;
			std::cout<< "Current " << currentTimeCount << std::endl;
			std::cout<< "Count " << currentTimeCount -  lastTimeCount << std::endl;
			std::cout<< "PISCOU VOLUNTARIO ################" << std::endl;
    		state = VOLUNTARY_BLINK;
    		lastTimeCount = currentTimeCount;
    	}else if(state != VOLUNTARY_BLINK){
    		state = CLOSED;
    	}
    }else{
    	state = OPEN;
    	eyeFloat.copyTo(*_averageEye.get());
    	lastTimeCount = currentTimeCount;

    }

//    Application::dwelltimeParameter

//    std::cout << "Distance is " << distance << std::endl;
    
//    eyeFloat.copyTo(*_averageEye.get());
//	cv::accumulateWeighted(*eyeFloat, *_averageEye, 0.05); //TODO Maybe use running average of eye image
}
//*/


/*
 * Versão que conta frames
 */
void BlinkDetector::update(const cv::Mat &eyeFloat) {
	if (!_initialized) {
		eyeFloat.copyTo(*_averageEye.get());
		_initialized = true;
	}

	cv::Mat eye = cv::imread("calibrationData/eye0.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//	eye.convertTo(eye, CV_32FC1);
//	eye.copyTo(*_averageEye.get());

	double distance = cv::norm(eyeFloat, *_averageEye, CV_L2);
    double norm = cv::norm(*_averageEye, CV_L2);

    // Normalize distance by the norm of the previous eye image
    distance = distance / norm;

    timeval tv;
	gettimeofday(&tv, 0);
    isBlinking = (distance > 0.18);

    if(_frameCount == 0)
    	Application::Data::blinkGazePoints = Application::Data::gazePoints;

    if(isBlinking){
//    	std::cout << "Closed Distance is " << distance << std::endl;

    	if( (_frameCount == 5) && (state != VOLUNTARY_BLINK)){
			std::cout<< "Voluntary Blink" << std::endl;
    		state = VOLUNTARY_BLINK;
    		_frameCount = 0;
    	}else if(state != VOLUNTARY_BLINK){
    		if(checkDoubleBlink()){
    			state = DOUBLE_BLINK;
    		}else{
				state = CLOSED;
				_frameCount++;

    		}
    	}
    }else{
    	state = OPEN;
    	//set to current direction
    	eyeFloat.copyTo(*_averageEye.get());
    	_frameCount = 0;

    }

//    Application::dwelltimeParameter

//    std::cout << "Distance is " << distance << std::endl;

//    eyeFloat.copyTo(*_averageEye.get());
//	cv::accumulateWeighted(*eyeFloat, *_averageEye, 0.05); //TODO Maybe use running average of eye image
}
//*/


bool BlinkDetector::checkDoubleBlink(){

	static int countBlink = 0;

	if(state == OPEN)
		countBlink++;
	else
		countBlink = 0;

	if(countBlink >= 2){

		std::cout << "Double blink " << std::endl;
		countBlink = 0;
		return true;
	}

	return false;

}

BlinkDetector::STATE BlinkDetector::getState(){
	return state;
}
