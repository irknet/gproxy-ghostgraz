#include "SleepThread.h"

void SleepThread::sleep (unsigned long seconds)
{
    QThread::sleep(seconds);
}

void SleepThread::msleep (unsigned long milliseconds)
{
    QThread::msleep(milliseconds);
}

void SleepThread::usleep (unsigned long microseconds)
{
    QThread::usleep(microseconds);
}