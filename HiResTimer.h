#pragma once

#include <sys/time.h>

class HiResTimer
{
public:
	HiResTimer();
	
    void start();
    void stop();
    double getElapsedTime();
private:
    timeval t1, t2;
};

