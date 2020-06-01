################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AnchorPointSelector.cpp \
../Application.cpp \
../BlinkDetector.cpp \
../Calibrator.cpp \
../Command.cpp \
../Configuration.cpp \
../DebugWindow.cpp \
../EyeCenterDetector.cpp \
../EyeExtractor.cpp \
../EyeExtractorSegmentationGroundTruth.cpp \
../FMatrixAffineCompute.cpp \
../FaceDetector.cpp \
../FeatureDetector.cpp \
../FrogGame.cpp \
../GaussianProcess.cpp \
../GazeTracker.cpp \
../GazeTrackerHistogramFeatures.cpp \
../GoogleGlassWindow.cpp \
../HeadTracker.cpp \
../HiResTimer.cpp \
../HistogramFeatureExtractor.cpp \
../ImageWidget.cpp \
../ImageWindow.cpp \
../LeastSquares.cpp \
../MainGazeTracker.cpp \
../MoveMouse.cpp \
../OutputMethods.cpp \
../Point.cpp \
../PointTracker.cpp \
../PointTrackerWithTemplate.cpp \
../TestWindow.cpp \
../Video.cpp \
../mir.cpp \
../moc_ImageWidget.cpp \
../moc_MainGazeTracker.cpp \
../opengazer.cpp \
../rbf_interp.cpp \
../utils.cpp 

O_SRCS += \
../AnchorPointSelector.o \
../Application.o \
../BlinkDetector.o \
../Calibrator.o \
../Command.o \
../Configuration.o \
../DebugWindow.o \
../EyeCenterDetector.o \
../EyeExtractor.o \
../EyeExtractorSegmentationGroundTruth.o \
../FaceDetector.o \
../FeatureDetector.o \
../FrogGame.o \
../GazeTracker.o \
../GazeTrackerHistogramFeatures.o \
../GoogleGlassWindow.o \
../HeadTracker.o \
../HiResTimer.o \
../HistogramFeatureExtractor.o \
../ImageWidget.o \
../ImageWindow.o \
../LeastSquares.o \
../MainGazeTracker.o \
../MoveMouse.o \
../OutputMethods.o \
../Point.o \
../PointTracker.o \
../PointTrackerWithTemplate.o \
../TestWindow.o \
../Video.o \
../mir.o \
../moc_ImageWidget.o \
../moc_MainGazeTracker.o \
../opengazer.o \
../utils.o 

OBJS += \
./AnchorPointSelector.o \
./Application.o \
./BlinkDetector.o \
./Calibrator.o \
./Command.o \
./Configuration.o \
./DebugWindow.o \
./EyeCenterDetector.o \
./EyeExtractor.o \
./EyeExtractorSegmentationGroundTruth.o \
./FMatrixAffineCompute.o \
./FaceDetector.o \
./FeatureDetector.o \
./FrogGame.o \
./GaussianProcess.o \
./GazeTracker.o \
./GazeTrackerHistogramFeatures.o \
./GoogleGlassWindow.o \
./HeadTracker.o \
./HiResTimer.o \
./HistogramFeatureExtractor.o \
./ImageWidget.o \
./ImageWindow.o \
./LeastSquares.o \
./MainGazeTracker.o \
./MoveMouse.o \
./OutputMethods.o \
./Point.o \
./PointTracker.o \
./PointTrackerWithTemplate.o \
./TestWindow.o \
./Video.o \
./mir.o \
./moc_ImageWidget.o \
./moc_MainGazeTracker.o \
./opengazer.o \
./rbf_interp.o \
./utils.o 

CPP_DEPS += \
./AnchorPointSelector.d \
./Application.d \
./BlinkDetector.d \
./Calibrator.d \
./Command.d \
./Configuration.d \
./DebugWindow.d \
./EyeCenterDetector.d \
./EyeExtractor.d \
./EyeExtractorSegmentationGroundTruth.d \
./FMatrixAffineCompute.d \
./FaceDetector.d \
./FeatureDetector.d \
./FrogGame.d \
./GaussianProcess.d \
./GazeTracker.d \
./GazeTrackerHistogramFeatures.d \
./GoogleGlassWindow.d \
./HeadTracker.d \
./HiResTimer.d \
./HistogramFeatureExtractor.d \
./ImageWidget.d \
./ImageWindow.d \
./LeastSquares.d \
./MainGazeTracker.d \
./MoveMouse.d \
./OutputMethods.d \
./Point.d \
./PointTracker.d \
./PointTrackerWithTemplate.d \
./TestWindow.d \
./Video.d \
./mir.d \
./moc_ImageWidget.d \
./moc_MainGazeTracker.d \
./opengazer.d \
./rbf_interp.d \
./utils.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


