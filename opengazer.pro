# To configure the project and create the Makefile, run the following on the terminal:
#			qmake -spec macx-g++ opengazer.pro

CONFIG	+=	qt
QT += gui widgets

HEADERS += 	Calibrator.h HeadTracker.h LeastSquares.h EyeExtractor.h GazeTracker.h MainGazeTracker.h OutputMethods.h PointTracker.h FaceDetector.h Point.h utils.h BlinkDetector.h FeatureDetector.h mir.h DebugWindow.h Application.h Video.h AnchorPointSelector.h Command.h ImageWindow.h ImageWidget.h TestWindow.h Prefix.hpp HiResTimer.h EyeCenterDetector.h GoogleGlassWindow.h EyeExtractorSegmentationGroundTruth.h FrogGame.h HistogramFeatureExtractor.h PointTrackerWithTemplate.h GazeTrackerHistogramFeatures.h Component.h Configuration.h MoveMouse.h Scroll.h

SOURCES += 	opengazer.cpp Calibrator.cpp HeadTracker.cpp LeastSquares.cpp EyeExtractor.cpp GazeTracker.cpp MainGazeTracker.cpp OutputMethods.cpp PointTracker.cpp FaceDetector.cpp Point.cpp utils.cpp BlinkDetector.cpp FeatureDetector.cpp mir.cpp DebugWindow.cpp Application.cpp Video.cpp AnchorPointSelector.cpp Command.cpp ImageWindow.cpp ImageWidget.cpp TestWindow.cpp HiResTimer.cpp EyeCenterDetector.cpp GoogleGlassWindow.cpp EyeExtractorSegmentationGroundTruth.cpp  FrogGame.cpp HistogramFeatureExtractor.cpp PointTrackerWithTemplate.cpp GazeTrackerHistogramFeatures.cpp Configuration.cpp MoveMouse.cpp Scroll.cpp

TARGET  = 	opengazer

QMAKE_CFLAGS 	+= `pkg-config opencv --cflags` -include Prefix.hpp
QMAKE_CXXFLAGS 	+= `pkg-config opencv --cflags` -include Prefix.hpp
QMAKE_LIBS 	+= `pkg-config opencv --libs`

macx {
	QMAKE_MAC_SDK=macosx10.11

	# Mac OS X linker parameters and include directories
	QMAKE_LIBS += -L/opt/local/lib -L/usr/local/lib -lm -ldl -lfann -lboost_filesystem-mt -lboost_system-mt -lgsl -lgslcblas

	# NOTE: Depending on your Mac OS X / XCode version you might need to use the
	# configuration that is commented out
	QMAKE_CFLAGS 	+= -stdlib=libc++
	QMAKE_CXXFLAGS 	+= -stdlib=libc++
    QMAKE_LFLAGS    += -stdlib=libc++

    QMAKE_CXXFLAGS += -I`brew --prefix boost`/include
    QMAKE_CXXFLAGS += -I`brew --prefix gsl`/include

	#QMAKE_CFLAGS 	+= -stdlib=libstdc++
	#QMAKE_CXXFLAGS 	+= -stdlib=libstdc++
}

unix:!macx {
	# Linux linker parameters and include directories
	QMAKE_LIBS += -L/usr/local/lib -L/opt/local/lib -lm -ldl -lgthread-2.0 -lfann  -lX11 -lboost_filesystem -lboost_system -lgsl -lgslcblas
	#QMAKE_LIBS += -L/usr/local/cuda-6.5/lib64/ -lGLEW

	INCLUDEPATH += /usr/local/include
}
