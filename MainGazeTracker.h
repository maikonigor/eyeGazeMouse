#pragma once

#include <fstream>


#include "EyeExtractor.h"

#include "Calibrator.h"
#include "OutputMethods.h"
#include "Video.h"
#include "Command.h"

#include <QObject>
#include <QTimer>

class MainGazeTracker : public QObject {
	Q_OBJECT
	
public:
	MainGazeTracker(int argc, char **argv);
	~MainGazeTracker();
	void simulateClicks();
	void processActionSignals();
	void cleanUp();
    Component* getComponent(std::string name);
    CommandLineArguments args;
	
public slots:
	void process();
	void startCalibration();
	void startTesting();
	void choosePoints();
	void clearPoints();
	
private:
	std::vector<boost::shared_ptr<AbstractStore> > _stores;
	
	std::string _directory;
	std::string _basePath;
	
	// Member variables to save/load the user actions (start calibration, testing, choose points, etc.)
	// for record/playback mechanism
	std::ofstream _commandOutputFile;
	std::ifstream _commandInputFile;
	std::vector<Command> _commands;
	int _commandIndex;
    
    EyeExtractor* _eyeExtractor;
	
	int _headDistance;
	QTimer _timer;
    
    void addComponent(std::string name, Component *component);
    void calculateError(Point estimation, Point target, double &errorHorizontal, double &errorVertical, double &errorCombined);
};

