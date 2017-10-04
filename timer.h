#ifndef _TIMER_H
#define _TIMER_H

#include <windows.h>
#include <cstdio>

static LARGE_INTEGER nFreq;
static LARGE_INTEGER nBeginTime;
static LARGE_INTEGER nEndTime;

#define START 	QueryPerformanceFrequency(&nFreq);\
				QueryPerformanceCounter(&nBeginTime);

#define STOP	QueryPerformanceCounter(&nEndTime);\
				printf("%f milisecond\n", double(nEndTime.QuadPart-nBeginTime.QuadPart)*1000.0/nFreq.QuadPart);\
				fflush(stdout);

#endif
				//cout<<(nEndTime.QuadPart-nBeginTime.QuadPart)*1000.0/nFreq.QuadPart<<' '<<"milisecond"<<endl;