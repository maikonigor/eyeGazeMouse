#include "HiResTimer.h"
#include <stdio.h>

HiResTimer::HiResTimer()
{
    gettimeofday(&t1, NULL);
}

void HiResTimer::start()
{
    gettimeofday(&t1, NULL);
}

void HiResTimer::stop()
{
    gettimeofday(&t2, NULL);
}

double HiResTimer::getElapsedTime()
{
    double elapsedTime = ((double)t2.tv_sec - (double)t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += ((double)t2.tv_usec - (double)t1.tv_usec) / 1000.0;   // us to ms

    return elapsedTime;
}
