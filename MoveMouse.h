#include "Component.h"
#include "Application.h"
#include "EyeExtractor.h"
#include "BlinkDetector.h"

#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>

#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>


class MoveMouse: public Component{

#define MAX_PAST_POINT 5

public:
    MoveMouse();
    ~MoveMouse();
  // Main processing function

  void process();
  void draw();
  void mouseClick(int button);
  void moveCursor(int x, int y);
  void addPastPoint(int x, int y);

  int xPoint;
  int yPoint;

private:
  BlinkDetector _blinkDetector;

  Display *_display;
  Window _root;
  EyeExtractor* _eyeExtractor;
  bool _isBrowserOpen;
  std::vector<Point> pastGaze;


};
