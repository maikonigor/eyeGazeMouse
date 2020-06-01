#define BOOST_FILESYSTEM_VERSION 3

//#define EXPERIMENT_MODE
//#define DEBUG

#include "MainGazeTracker.h"
#include <QApplication>

int main(int argc, char **argv) {
	try {

		QApplication app(argc, argv);
		MainGazeTracker tracker(argc, argv);

		return app.exec();
	}
	catch (Utils::QuitNow) {
		std::cout << "Caught it!\n";
	}

	return 0;
}
