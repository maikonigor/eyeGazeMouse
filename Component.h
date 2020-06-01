#pragma once

#include "Point.h"

class Component {
public:
    Component() {};
    ~Component() {};
    
    // Returns whether component is active
    bool isActive() {
        return true;
    }
    
    // Does whatever the component is supposed to do in every iteration
	virtual void process() = 0;
    
    // Draws debug information onto the camera frame
	virtual void draw() = 0;
};

class GazeTrackerComponent: public Component {
public:
    GazeTrackerComponent() {};
    ~GazeTrackerComponent() {};
    
    Point gazePoint;
    
    void clear() {
    }
};
