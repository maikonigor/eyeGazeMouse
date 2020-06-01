#pragma once

#include <fstream>
#include <map>

#include "Configuration.h"
#include "OutputMethods.h"

// Include files for system components
#include "Video.h"
#include "MainGazeTracker.h"
#include "Calibrator.h"
#include "DebugWindow.h"
#include "TestWindow.h"

namespace Application {

	// Tracker status
	enum trackerStatus {
		STATUS_IDLE			= 1,	// App is not doing anything
		STATUS_CALIBRATED	= 2,	// App is not doing anything, but is calibrated
		STATUS_CALIBRATING	= 11,	// App is calibrating
		STATUS_TESTING		= 12,	// App is testing
		STATUS_PAUSED		= 13	// App is paused
	};

	extern trackerStatus status;
	extern bool isTrackerCalibrated;
	extern int dwelltimeParameter;
	extern int testDwelltimeParameter;
	extern int sleepParameter;
	extern std::ofstream resultsOutputFile;
    
    extern Configuration config;


	namespace Settings {
		extern bool videoOverlays;
		extern bool recording;
		extern bool noWindows;
		extern bool noTracking;
		extern bool useGroundTruth;
		extern bool autocalibrate;
	}
    
    extern std::map<std::string, Component*> components;

	namespace Components {
		extern boost::scoped_ptr<VideoInput> videoInput;
		extern boost::scoped_ptr<VideoWriter> video;

		// The main class containing the processing loop
		extern MainGazeTracker *mainTracker;

		// Other components mainly taking care of calibration/test flow and display
		extern Calibrator *calibrator;
		extern DebugWindow *debugWindow;
		extern TestWindow *testWindow;
	}
    
    Component* getComponent(std::string name);

	namespace Signals {
		extern int initiateCalibrationFrameNo;
		extern int initiateTestingFrameNo;
		extern int initiatePointSelectionFrameNo;
		extern int initiatePointClearingFrameNo;
	}

	namespace Data {
		extern std::vector<Point> calibrationTargets;

		// Outputs for all gaze estimators
        extern std::vector<Point> gazePoints;
        extern std::vector<Point> blinkGazePoints;
        
        // Positions of facial anchor points
        extern std::vector<cv::Point2f> anchorPoints;
        extern bool isTrackingSuccessful;
	}

	std::vector<boost::shared_ptr<AbstractStore> > getStores();
}
