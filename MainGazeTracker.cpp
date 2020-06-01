#include <fstream>
#include <map>
#include <boost/filesystem.hpp>

#include "MainGazeTracker.h"


#include "PointTracker.h"
#include "GazeTracker.h"
#include "HeadTracker.h"
#include "EyeCenterDetector.h"
#include "HistogramFeatureExtractor.h"
#include "PointTrackerWithTemplate.h"
#include "GazeTrackerHistogramFeatures.h"
#include "FrogGame.h"
//#include "MoveMouse.h"
#include "Scroll.h"

#include "Application.h"
#include "utils.h"
#include "FaceDetector.h"
#include "AnchorPointSelector.h"
#include "DebugWindow.h"
#include "TestWindow.h"

#include "HiResTimer.h"

MainGazeTracker::MainGazeTracker(int argc, char **argv):
	_stores(Application::getStores()),
	_commandIndex(-1),
	args(argc, argv)
{
	Application::Components::mainTracker = this;

	if (args.getOptionValue("help").length()) {
		std::cout << std::endl;
		std::cout << "CVC Eye Tracker" << std::endl;
		std::cout << std::endl << std::endl;
		std::cout << "Usage:    eyetracker --subject=SUBJECT_NAME [--resolution=[480|720]] [--setup=SETUP_FOLDER_NAME] [--headdistance=DISTANCE] [--record=[0|1]] [--overlay=[0|1]] [--input=INPUT_FILE_PATH] [--nowindows=[0|1]]" << std::endl;
		std::cout << std::endl << std::endl;

		std::cout << "OPTIONS:" << std::endl;
        std::cout << "  config:      Path to the configuration file (for system components)." << std::endl;
		std::cout << std::endl << std::endl;


        if (args.getOptionValue("advanced").length()) {
            std::cout << "  subject:      Subject\'s name to be used in the output file name." << std::endl;
            std::cout << "  resolution:   Resolution for the camera. 480 for 640x480 resolution, 720 for 1280x720." << std::endl;
            std::cout << "  setup:        Experiment setup name and also the folder to read the test and calibration point locations." << std::endl;
            std::cout << "  headdistance: Subject\'s head distance in cm for automatic angular error calculation purposes." << std::endl;
            std::cout << "  record:       Whether a video of the experiment should be recorded for offline processing purposes." << std::endl;
            std::cout << "  overlay:      Whether target point and estimation pointers are written as an overlay to the recorded video. Should not be used when offline processing is desired on the output video." << std::endl;
            std::cout << "  input:        Input video path in case of offline processing." << std::endl;
            std::cout << "  nowindows:    Do not show any windows, used for re-running experiments." << std::endl;
            std::cout << "  autocalibrate: Read the saved points and image of last calibration process, and start the auto calibration routine." << std::endl;
            std::cout << std::endl << std::endl;

            std::cout << "SAMPLE USAGES:" << std::endl;
            std::cout << "  Basic usage without video recording:" << std::endl;
            std::cout << "    ./eyetracker --subject=david --resolution=720 --setup=std --headdistance=80 --record=0" << std::endl;
            std::cout << "  Basic usage with auto calibration:" << std::endl;
			std::cout << "    ./eyetracker --subject=david --resolution=720 --setup=std --headdistance=80 --record=0 --autocalibrate=1" << std::endl;
            std::cout << std::endl;
            std::cout << "  Usage during experiments to enable video recording for offline processing:" << std::endl;
            std::cout << "    ./eyetracker --subject=david --resolution=720 --setup=std --headdistance=80 --record=1" << std::endl;
            std::cout << std::endl;
            std::cout << "  Usage during offline processing:" << std::endl;
            std::cout << "    ./eyetracker --subject=david --resolution=720 --setup=std --headdistance=80 --record=1 --overlay=1 --input=../outputs/david_std_720_1.avi --nowindows=1" << std::endl;
            std::cout << std::endl;
            std::cout << "  Usage during offline processing with lower resolution:" << std::endl;
            std::cout << "    ./eyetracker --subject=david --resolution=480 --setup=std --headdistance=80 --record=1 --overlay=1 --input=../outputs/david_std_720_1.avi --nowindows=1" << std::endl;
        }
        else {
            std::cout << "  Run  'eyetracker --help --advanced' for advanced options." << std::endl;
        }
		std::cout << std::endl << std::endl;
		exit(0);
	}

	if (args.getOptionValue("input").length()) {
		Application::Components::videoInput.reset(new VideoInput(args.getOptionValue("resolution"), args.getOptionValue("input"), true));

		// Read the commands (SELECT, CLEAR, CALIBRATE, TEST)
		std::string inputCommandFilename = args.getOptionValue("input");
		inputCommandFilename = inputCommandFilename.substr(0, inputCommandFilename.length()-4) + "_commands.txt";

		_commandInputFile.open(inputCommandFilename.c_str());

		for(;;) {
			long number;
			std::string name;
			_commandInputFile >> number >> name;

			if (_commandInputFile.rdstate()) {
				break; // break if any error
			}

			_commands.push_back(Command(number, name));
		}

		_commandIndex = 0;

		std::cout << _commands.size() << " commands read." << std::endl;
	} else {
		// --resolution parameter
		if (args.getOptionValue("resolution").length()) {
			Application::Components::videoInput.reset(new VideoInput(args.getOptionValue("resolution")));
		} else {
			Application::Components::videoInput.reset(new VideoInput("480"));
		}
	}

	std::string subject = args.getOptionValue("subject");
	std::string setup = args.getOptionValue("setup");

	if (subject.length() == 0) {
		subject = "default";
	}

	if (setup.length() == 0) {
		setup = "std";
	}

	if (args.getOptionValue("overlay") == "1") {
		Application::Settings::videoOverlays = true;
	}

    std::string configFile = args.getOptionValue("config");

    if (configFile.length() == 0) {
        configFile = "standard.xml";
    }

    // Load component configuration file
    Application::config.load("./config/" + configFile);

	// --headdistance parameter
	if (args.getOptionValue("headdistance").length()) {
		_headDistance = atoi(args.getOptionValue("headdistance").c_str());
	} else {
		_headDistance = 70;
	}

	// --dwelltime parameter
	if (args.getOptionValue("dwelltime").length()) {
		Application::dwelltimeParameter = atoi(args.getOptionValue("dwelltime").c_str());
	} else {
		Application::dwelltimeParameter = 30;
	}

	// --testdwelltime parameter
	if (args.getOptionValue("testdwelltime").length()) {
		Application::testDwelltimeParameter = atoi(args.getOptionValue("testdwelltime").c_str());
	} else {
		Application::testDwelltimeParameter = 20;
	}

	// --sleep parameter
	if (args.getOptionValue("sleep").length()) {
		Application::sleepParameter = atoi(args.getOptionValue("sleep").c_str());
	 } else {
		Application::sleepParameter = 0;
	 }

	if (args.getOptionValue("autocalibrate") == "1") {
			Application::Settings::autocalibrate = true;
	}

	// --folder parameter
	std::string folderParameter = "./outputs";

	if (args.getOptionValue("outputfolder").length()) {
		folderParameter = args.getOptionValue("outputfolder");
	}

	// Make sure the output folders exist before continuing
	boost::filesystem::path folder(folderParameter);
	boost::filesystem::create_directories(folder);

	// --subject parameter
	_basePath = Utils::getUniqueFileName(folderParameter, subject + "_" + setup + "_" + args.getOptionValue("resolution"));

	// --record parameter
	if (args.getOptionValue("record") == "1") {
		if(Application::Settings::videoOverlays) {
			Application::Components::video.reset(new VideoWriter(Application::Components::videoInput->debugFrame.size(), _basePath.substr(0, _basePath.length() - 4) + ".avi"));
		}
		else {
			Application::Components::video.reset(new VideoWriter(Application::Components::videoInput->size, _basePath.substr(0, _basePath.length() - 4) + ".avi"));
		}

		Application::Settings::recording = true;
	}

	// --nowindows parameter
	if (args.getOptionValue("nowindows") == "1") {
		Application::Settings::noWindows = true;
	}

	// --usegt parameter
	if (args.getOptionValue("usegt") == "1") {
		Application::Settings::useGroundTruth = true;
	}

	// --notracking parameter
	if (args.getOptionValue("notracking") == "1") {
		Application::Settings::noTracking = true;
	}

    // Create output file only if the "subject" parameter is given
    if(args.getOptionValue("subject").length() > 0) {
        Application::resultsOutputFile.open((_basePath + "_").c_str());

        // First write the system time
        time_t currentTime = time(NULL);
        Application::resultsOutputFile << ctime(&currentTime) << std::endl;

        // Then write the setup parameters
        Application::resultsOutputFile << "--input=" << args.getOptionValue("input") << std::endl;
        Application::resultsOutputFile << "--record=" << args.getOptionValue("record") << std::endl;
        Application::resultsOutputFile << "--overlay=" << (Application::Settings::videoOverlays ? "true" : "false") << std::endl;
        Application::resultsOutputFile << "--headdistance=" << _headDistance << std::endl;
        Application::resultsOutputFile << "--resolution=" << args.getOptionValue("resolution") << std::endl;
        Application::resultsOutputFile << "--setup=" << setup << std::endl;
        Application::resultsOutputFile << "--subject=" << subject << std::endl << std::endl;
        Application::resultsOutputFile << "--sigma=" << args.getOptionValue("sigma") << std::endl;
        Application::resultsOutputFile << "--lscale=" << args.getOptionValue("lscale") << std::endl;
    }

	// Finally the screen resolution
	cv::Rect *rect = Utils::getSecondMonitorGeometry();

	Application::resultsOutputFile << "Screen resolution: " << rect->width << " x " << rect->height << " (Position: "<< rect->x << ", "<< rect->y << ")" << std::endl << std::endl;
	Application::resultsOutputFile.flush();

	// If recording, create the file to write the commands for button events
	if (Application::Settings::recording) {
		std::string commandFileName = _basePath.substr(0, _basePath.length() - 4) + "_commands.txt";
		_commandOutputFile.open(commandFileName.c_str());
	}

	_directory = setup;

	// Create system components
    // Pre-gaze components
    addComponent("AnchorPointSelector", new AnchorPointSelector());
    addComponent("EyeCenterDetector", new EyeCenterDetector());
    addComponent("EyeExtractor", new EyeExtractor());
    addComponent("HeadTracker", new HeadTracker());
    addComponent("HistogramFeatureExtractor", new HistogramFeatureExtractor());
    addComponent("PointTracker", new PointTracker());
    addComponent("PointTrackerWithTemplate", new PointTrackerWithTemplate());

    _eyeExtractor = (EyeExtractor*) getComponent("EyeExtractor");

    // Gaze estimation components
    addComponent("GazeTracker", new GazeTracker());
    addComponent("GazeTrackerHistogramFeatures", new GazeTrackerHistogramFeatures());

    // Post-gaze components
//    addComponent("FrogGame", new FrogGame());
	addComponent("MoveMouse", new MoveMouse());
	addComponent("Scroll", new Scroll());
    //addComponent("GoogleGlassWindow", new GoogleGlassWindow());

	// Read calibration target list and create the calibrator object
	std::ifstream calibrationTargetFile((_directory + "/calpoints.txt").c_str());
	std::vector<Point> calibrationTargets = Utils::readAndScalePoints(calibrationTargetFile);
	Application::Components::calibrator = new Calibrator(calibrationTargets);

	// Read test target list and create the test window object
	std::ifstream testTargetFile((_directory + "/testpoints.txt").c_str());
	std::vector<Point> testTargets = Utils::readAndScalePoints(testTargetFile);
	Application::Components::testWindow = new TestWindow(testTargets);

    // Create the debug window
	Application::Components::debugWindow = new DebugWindow();
	Application::Components::debugWindow->raise();


    // Add gaze point output variables for each gaze component
    for (int i=0; i<Application::config.gaze_components.size(); i++){
        Application::Data::gazePoints.push_back(Point(0, 0));
    }

	// Create timer for initiating main loop
	connect(&_timer, SIGNAL (timeout()), this, SLOT (process()));
	_timer.start(1);
}

MainGazeTracker::~MainGazeTracker() {
	cleanUp();
}

void MainGazeTracker::process() {
    double errorHorizontal, errorVertical, errorCombined;

	// Read the next camera/video frame
	Application::Components::videoInput->updateFrame();
	usleep(Application::sleepParameter);
	// Wait a little so that the marker stays on the screen for a longer time
//	if ((Application::status == Application::STATUS_CALIBRATING || Application::status == Application::STATUS_TESTING) && !Application::Components::videoInput->captureFromVideo) {
//		usleep(Application::sleepParameter);
//	}

	// Execute any pending signals (start calibration, etc.)
	simulateClicks();
	processActionSignals();

	// Process obligatory components
	Application::Components::calibrator->process();
	Application::Components::testWindow->process();

    // Process rest of the components (read from config file)
    // 1) Components to run before gaze estimation
    for (int i=0; i<Application::config.pre_components.size(); i++){
        Component* comp = getComponent(Application::config.pre_components[i]);
        comp->process();
    }

    // 2) Gaze estimation components
    for (int i=0; i<Application::config.gaze_components.size(); i++){
        GazeTrackerComponent* comp = (GazeTrackerComponent *) getComponent(Application::config.gaze_components[i]);
        comp->process();

        // Copy the gaze outputs to the shared vector
        Application::Data::gazePoints[i].x = comp->gazePoint.x;
        Application::Data::gazePoints[i].y = comp->gazePoint.y;
    }

    // 3) Components to run after gaze estimation (games, etc.)
    for (int i=0; i<Application::config.post_components.size(); i++){
        Component* comp = getComponent(Application::config.post_components[i]);
        comp->process();
    }

	// Draw components' debug information on debug image
	Application::Components::calibrator->draw();
//	Application::Components::testWindow->draw();

    // Draw rest of the components (read from config file)
    for (int i=0; i<Application::config.all_components.size(); i++){
        Component* comp = getComponent(Application::config.all_components[i]);
        comp->draw();
    }


    for (int i=0; i<Application::config.gaze_components.size(); i++){
        GazeTrackerComponent* comp = (GazeTrackerComponent*) getComponent(Application::config.gaze_components[i]);

        if (comp->isActive()) {
            // Only for the first gaze tracker component,
            // communicate the output through all channels
            if(i == 0) {
                xForEach(iter, _stores) {
                    (*iter)->store();
                }
            }

            // If there is no result output file,
            // or if we are not in the test phase,
            // or there is a blink,
            // or we are in the first half of dwell time,
            // (We assume that the subject is still looking at the previous target
            // during these frames and discard them.)
            // skip writing output
            if (!Application::resultsOutputFile.is_open() ||
                Application::status != Application::STATUS_TESTING ||
                _eyeExtractor->isBlinking() ||
                Application::Components::testWindow->getPointFrameNo() < Application::testDwelltimeParameter/2) {
                continue;
            }

            // Write the fixed part of the output file rows
            if(i == 0) {
                Application::resultsOutputFile << Application::Components::testWindow->getPointFrameNo() + 1 << "\t"
                << Application::Components::testWindow->getActivePoint().x << "\t" << Application::Components::testWindow->getActivePoint().y;
            }

            // Write the error information to the output text file
            calculateError(comp->gazePoint, Application::Components::testWindow->getActivePoint(), errorHorizontal, errorVertical, errorCombined);

            Application::resultsOutputFile << "\t" << comp->gazePoint.x << "\t" << comp->gazePoint.y;
            Application::resultsOutputFile << "\t" << errorHorizontal << "\t" << errorVertical << "\t" << errorCombined;

            // Write end of line
            if(i == Application::config.gaze_components.size()-1) {
                Application::resultsOutputFile << std::endl;
            }
        }
    }

	// If video output is requested
	if (Application::Settings::recording) {
        // Write frame with or without the overlays (debug information)

		if (Application::Settings::videoOverlays) {
			Application::Components::video->write(Application::Components::videoInput->debugFrame);
		}
		else {
			Application::Components::video->write(Application::Components::videoInput->frame);
		}
	}

    // Draw the onscreen instructions
    // X -> CHOOSE POINTS
	// Z -> CLEAR POINTS
	// C -> CALIBRATE
	// T -> TEST
	cv::Mat &debugFrame = Application::Components::videoInput->debugFrame;
	int fontFace = cv::FONT_HERSHEY_PLAIN;
	double fontScale = 2;
	int thickness = 2;

    int baseline=0;
    cv::Size textSize = cv::getTextSize("X : CHOOSE POINTS", fontFace, fontScale, thickness, &baseline);
    cv::Point textPosition(debugFrame.cols - textSize.width - 30, 50);

	// Write the text
	cv::putText(debugFrame, "X : CHOOSE POINTS", textPosition, fontFace, fontScale, cv::Scalar::all(255), thickness, 8);
	textPosition.y = textPosition.y + textSize.height + 10;

	cv::putText(debugFrame, "Z : CLEAR POINTS", textPosition, fontFace, fontScale, cv::Scalar::all(255), thickness, 8);
	textPosition.y = textPosition.y + textSize.height + 10;

	cv::putText(debugFrame, "C : CALIBRATE", textPosition, fontFace, fontScale, cv::Scalar::all(255), thickness, 8);
	textPosition.y = textPosition.y + textSize.height + 10;

	cv::putText(debugFrame, "T : TEST", textPosition, fontFace, fontScale, cv::Scalar::all(255), thickness, 8);

	// Display debug image in the window
	Application::Components::debugWindow->display();
}

void MainGazeTracker::simulateClicks() {
	if (_commands.size() > 0) {
		while(_commandIndex >= 0 && _commandIndex <= (_commands.size() - 1) && _commands[_commandIndex].frameNumber == Application::Components::videoInput->frameCount) {
			std::cout << "Command: " << _commands[_commandIndex].commandName << std::endl;
			if(strcmp(_commands[_commandIndex].commandName.c_str(), "SELECT") == 0) {
				//std::cout << "Choosing points automatically" << std::endl;
				Application::Signals::initiatePointSelectionFrameNo = Application::Components::videoInput->frameCount;
			}
			else if(strcmp(_commands[_commandIndex].commandName.c_str(), "CLEAR") == 0) {
				//std::cout << "Clearing points automatically" << std::endl;
				Application::Signals::initiatePointClearingFrameNo = Application::Components::videoInput->frameCount;
			}
			else if(strcmp(_commands[_commandIndex].commandName.c_str(), "CALIBRATE") == 0) {
				//std::cout << "Calibrating automatically" << std::endl;
				Application::Signals::initiateCalibrationFrameNo = Application::Components::videoInput->frameCount;
			}
			else if(strcmp(_commands[_commandIndex].commandName.c_str(), "TEST") == 0) {
				//std::cout << "Testing automatically" << std::endl;
				Application::Signals::initiateTestingFrameNo = Application::Components::videoInput->frameCount;
			}

			_commandIndex++;
		}
	}
}

// Process the signals for initiating certain tracker processes (calibration, testing, etc.)
void MainGazeTracker::processActionSignals() {
	// If should start calibration
	if(Application::Signals::initiateCalibrationFrameNo == Application::Components::videoInput->frameCount) {
		std::cout << "Choosing calibration" << std::endl;

		// Write action to commands file
		if (Application::Settings::recording) {
			_commandOutputFile << Application::Components::videoInput->frameCount << " CALIBRATE" << std::endl;
		}
	}

	// If shoud start testing
	if(Application::Signals::initiateTestingFrameNo == Application::Components::videoInput->frameCount) {
		std::cout << "Starting testing" << std::endl;

		// Write action to commands file
		if (Application::Settings::recording) {
			_commandOutputFile << Application::Components::videoInput->frameCount << " TEST" << std::endl;
		}

		Application::resultsOutputFile << "TESTING" << std::endl << std::endl;
	}

	// If should choose points in next frame
	if(Application::Signals::initiatePointSelectionFrameNo == Application::Components::videoInput->frameCount) {
		std::cout << "Choosing points" << std::endl;

		// Write action to commands file
		if (Application::Settings::recording) {
			_commandOutputFile << Application::Components::videoInput->frameCount << " SELECT" << std::endl;
		}
	}

	// If should clear points in next frame
	if(Application::Signals::initiatePointClearingFrameNo == Application::Components::videoInput->frameCount) {
		std::cout << "Clearing points" << std::endl;

		// Write action to commands file
		if (Application::Settings::recording) {
			_commandOutputFile << Application::Components::videoInput->frameCount << " CLEAR" << std::endl;
		}
	}
}

void MainGazeTracker::cleanUp() {
    if (Application::resultsOutputFile.is_open())
	   Application::resultsOutputFile.close();

	rename((_basePath + "_").c_str(), _basePath.c_str());
}

// These are the events triggered by the UI (Using keyboard keys: X -> Choose points, C -> Start calibration, Z -> Clear points)
// As they have effect on the next frame (because of parallel execution), the next frame no is saved
void MainGazeTracker::startCalibration() {
	Application::Signals::initiateCalibrationFrameNo = Application::Components::videoInput->frameCount + 1;
}

void MainGazeTracker::startTesting() {
	Application::Signals::initiateTestingFrameNo = Application::Components::videoInput->frameCount + 1;
}

void MainGazeTracker::choosePoints() {
	Application::Signals::initiatePointSelectionFrameNo = Application::Components::videoInput->frameCount + 1;
}

void MainGazeTracker::clearPoints() {
	Application::Signals::initiatePointClearingFrameNo = Application::Components::videoInput->frameCount + 1;
}

void MainGazeTracker::addComponent(std::string name, Component *component) {
    Application::components.insert(std::make_pair(name, component));
}

Component* MainGazeTracker::getComponent(std::string name) {
    return Application::components[name];
}

void MainGazeTracker::calculateError(Point estimation, Point target, double &errorHorizontal, double &errorVertical, double &errorCombined) {
    // Hardcoded width and height for the monitor for which to calculate the angular errors
    double monitorWidthInCm = 53.15;
    double monitorHeightInCm = 29.89;

    cv::Rect* geometry = Utils::getSecondMonitorGeometry();

    // Conversion factors for converting from pixels to cm
    double horizontalConversionFactor = monitorWidthInCm / geometry->width;
    double verticalConversionFactor  = monitorHeightInCm / geometry->height;

    // Center point for the main (second) monitor
    Point centerPoint(geometry->width/2, geometry->height/2);

    // Calculate errors in both directions separately
    // Angular error is: absolute difference between estimation angle and target angle
    // with respect to the center of the monitor (assuming subject is facing the center of monitor)
    errorHorizontal = (180/M_PI) * fabs(
                atan(horizontalConversionFactor * (estimation.x - centerPoint.x)/ _headDistance) -
                atan(horizontalConversionFactor * (target.x - centerPoint.x)    / _headDistance));

    errorVertical   = (180/M_PI) * fabs(
                atan(verticalConversionFactor * (estimation.y - centerPoint.y)/ _headDistance) -
                atan(verticalConversionFactor * (target.y - centerPoint.y)    / _headDistance));

    // Calculate (approximate) error in both directions combined
    errorCombined = sqrt(pow(errorHorizontal, 2) + pow(errorVertical, 2));

    /*
    std::cout << "Conversion factors: " << horizontalConversionFactor << " and " << verticalConversionFactor << std::endl;
    std::cout << "Target    : " << target.x << ", " << target.y << std::endl;
    std::cout << "Estimation: " << estimation.x << ", " << estimation.y << std::endl;
    std::cout << "Center    : " << centerPoint.x << ", " << centerPoint.y << std::endl;

    std::cout << "Errors    : " << errorHorizontal << ", " << errorVertical << ", " << errorCombined << std::endl;

    std::cout << "Estimation hor angle : " << atan(horizontalConversionFactor * (estimation.x - centerPoint.x)/ _headDistance) << std::endl;
    std::cout << "Target     hor angle : " << atan(horizontalConversionFactor * (target.x - centerPoint.x)    / _headDistance) << std::endl;
    std::cout << "Abs difference       : " << fabs(
                atan(horizontalConversionFactor * (estimation.x - centerPoint.x)/ _headDistance) -
                atan(horizontalConversionFactor * (target.x - centerPoint.x)    / _headDistance)) << std::endl;
    std::cout << "In degrees           : " << (180/M_PI) * fabs(
                atan(horizontalConversionFactor * (estimation.x - centerPoint.x)/ _headDistance) -
                atan(horizontalConversionFactor * (target.x - centerPoint.x)    / _headDistance)) << std::endl;
                */

}
