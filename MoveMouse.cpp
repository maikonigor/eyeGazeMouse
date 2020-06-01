#include "MoveMouse.h"


MoveMouse::MoveMouse():
	_isBrowserOpen(false)
{
  _display = XOpenDisplay(0);
  _root = DefaultRootWindow(_display);
  _eyeExtractor = NULL;


}

void MoveMouse::mouseClick(int button){
    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
        fprintf(stderr, "Cannot initialize the display\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    XFlush(display);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    XFlush(display);

    XCloseDisplay(display);
}

/**
  Move the mouse cursor to positions (x,y)
*/
void MoveMouse::moveCursor(int x, int y){
  Display *display = XOpenDisplay(NULL);
  Window root = DefaultRootWindow(display);
  XWarpPointer(display, None, root, 0, 0, 0, 0, x, y);
  XFlush(display);
  XCloseDisplay(display);
  addPastPoint(0,0);

}

void MoveMouse::process(){


  //Verify if the system is calibrated
	if(Application::status != Application::STATUS_CALIBRATED) {
	 return;
	}

	if(! _isBrowserOpen){
		//system("firefox https://teste.projetolead.com.br/ead2pcd/app/home?execution=e1s1");
		_isBrowserOpen = true;
	}

	 if(_eyeExtractor == NULL) {
		_eyeExtractor = (EyeExtractor*) Application::getComponent("EyeExtractor");
	}

	Point mappedEstimation;
	Utils::mapToFirstMonitorCoordinates(Application::Data::gazePoints[0], mappedEstimation);

	static double estimationXRunningAverage = 0;
	static double estimationYRunningAverage = 0;
	int lastEstimationX = mappedEstimation.x;
	int lastEstimationY = mappedEstimation.y;
	float alpha = 0.6;

	estimationXRunningAverage = (1 - alpha) * lastEstimationX + alpha * estimationXRunningAverage;
	estimationYRunningAverage = (1 - alpha) * lastEstimationY + alpha * estimationYRunningAverage;

	xPoint = estimationXRunningAverage;
	yPoint = estimationYRunningAverage;


	if(_eyeExtractor->isVolBlinking() && Application::Data::isTrackingSuccessful){

		moveCursor(pastGaze.at(0).x, pastGaze.at(0).y);
		mouseClick(Button1);
		_eyeExtractor->resetEyeState();
	}else if(!_eyeExtractor->isBlinking() && !_eyeExtractor->isVolBlinking() ){
		moveCursor(xPoint,yPoint);
		addPastPoint(xPoint,yPoint);
	}

}

void MoveMouse::addPastPoint(int x, int y){
	if(pastGaze.size() < MAX_PAST_POINT){
		pastGaze.push_back( Point(x,y) );
		return;
	}

	for(int i=1; i< MAX_PAST_POINT; i++){
		pastGaze.at(i-1) = pastGaze.at(i);
	}
	//Remove the last element
	pastGaze.pop_back();

	//insert new gaze point at last position
	pastGaze.push_back(Point(x,y));
}

void MoveMouse::draw(){}

MoveMouse::~MoveMouse(){}
