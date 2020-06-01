#include "Application.h"

namespace Application {
	trackerStatus status = STATUS_IDLE;
	bool isTrackerCalibrated = false;
	int dwelltimeParameter = 20;
	int testDwelltimeParameter = 20;
	int sleepParameter = 0;
	std::ofstream resultsOutputFile;
    
    Configuration config;

	namespace Settings {
		bool videoOverlays = false;
		bool recording = false;
		bool noWindows = false;
		bool noTracking = false;
		bool useGroundTruth = false;
		bool autocalibrate = false;
	}
    
    std::map<std::string, Component*> components;

	namespace Components {
		// Video input and output
		boost::scoped_ptr<VideoInput> videoInput;
		boost::scoped_ptr<VideoWriter> video;

		// The main class containing the processing loop
		MainGazeTracker *mainTracker;

		// Other components mainly taking care of calibration/test flow and display
		Calibrator *calibrator;
		DebugWindow *debugWindow;
		TestWindow *testWindow;
	}
    
    Component* getComponent(std::string name) {
        return Components::mainTracker->getComponent(name);
    }

	namespace Signals {
		int initiateCalibrationFrameNo = -1;
		int initiateTestingFrameNo = -1;
		int initiatePointSelectionFrameNo = -1;
		int initiatePointClearingFrameNo = -1;
	}

	namespace Data {
		std::vector<Point> calibrationTargets;

		// Outputs for all gaze estimators
        std::vector<Point> gazePoints;
        std::vector<Point> blinkGazePoints;
        
        // Positions of facial anchor points
        std::vector<cv::Point2f> anchorPoints;
        bool isTrackingSuccessful;
	}

	std::vector<boost::shared_ptr<AbstractStore> > getStores() {
		static std::vector<boost::shared_ptr<AbstractStore> > stores;

		if (stores.size() < 1) {
			stores.push_back(boost::shared_ptr<AbstractStore>(new SocketStore()));
			//stores.push_back(boost::shared_ptr<AbstractStore>(new StreamStore(std::cout)));
		}

		return stores;
	}
}
