Developing for CVC Eye Tracker
===========================
With the latest structural changes, the eye tracker has become completely modular. Hopefully, developing it will be easier for all of us. This document will be a short guide on how to create a new component from scratch and how to incorporate it in the application.


Code structure
--------------
The code is built up of essential modules (Calibrator, DebugWindow, utils, etc.) and additional modules (AnchorPointSelector, EyeExtractor, GazeTracker, etc.). Some of the most important files are the following:

**MainGazeTracker.cpp:** Contains the main application loop. In its constructor all the modules are created, and its process() function handles one iteration of the main loop. It reads the module configuration (by default config/standard.xml) and runs the configured modules. Here, the modules are divided into three:

1. Pre modules: Modules that have to run before the gaze trackings modules (point selection, point tracking, etc.).
2. Gaze modules: Modules that calculate the gaze (there may be several).
3. Post modules: Modules that run after the gaze estimation is ready (games, etc.)

These modules inherit either the Component class, or the GazeTrackerComponent class (defined in Component.h header), according to their type. These two classes provide standard interfaces to run the components. Some important examples are:

1. process(): Executes the main task of the component. It is called in each iteration of the main application loop, in the same component order as in the configuration file.
2. draw(): Draws debug information on the debug frame, which is then shown in the debug window (the window where camera image is visible) at the end of the main loop.
3. gazepoint: The member variable where the gaze estimation is stored for the gaze modules.

Back to the MainGazeTracker, we see that in the main loop (MainGazeTracker.process()) the system executes the process() method for the active components, then executes the draw() method for in the same order.

**Application.cpp:** Contains the configuration, the settings, component data as well as (almost) global pointers to some components (Calibrator, DebugWindow, etc.). Also has a function to get a pointer to any active component by name (getComponent()). If you need to access other components' data and results, you will have to use things from this namespace.

**utils.cpp:** Some utility functions used throughout the code. 

**FrogGame.cpp:** An example application to run when the system is calibrated. Illustrates the use of gaze information.


Implementing a new module
--------------
If you want to build your own module and place it in the application pipeline, here are the steps to follow:

1. Create your class by inheriting from Component or GazeTrackerComponent, depending on whether it will estimate the gaze or not.
2. Implement the virtual methods (at least process()). Here you can use information from Application namespace to decide when to do your things (e.g. Application::Data::isTrackingSuccessful to see if there are active anchor points and they are tracked successfully).
3. Add your files to the opengazer.pro file.
4. Initialize and add your component in MainGazeTracker constructor. It is where several consecutive calls are made to addComponent() function.
5. Modify the configuration file or create a new one, and add your component in the best ordering possible. For example, if it is going to do point tracking, it should come before eye extraction.
5. Configure (./configure), compile (make) and run (./opengazer --config=CONFIGFILENAME.xml) the application. After this point, if you want to remove the component, just remove it from the configuration file or go back to using the standard.xml configuration file (by not passing any --config parameters).