#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>
#include <iostream>
#include <iomanip>

#define START 	static auto t_start = std::chrono::high_resolution_clock::now();

#define STOP	static auto t_end = std::chrono::high_resolution_clock::now(); \
                std::cout << std::fixed << std::setprecision(6) << "CPU time: " \
                << std::chrono::duration<double, std::milli>(t_end - t_start).count() \
                <<" ms\n";
#endif