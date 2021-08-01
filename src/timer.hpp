#pragma once

#include "strict_win.hpp"
#include <cinttypes>

class Timer 
{
public:
	Timer();

	float total_time();
	float delta_time();

	void start();
	void stop();
	void reset();
	void tick();

private:
	double _seconds_per_count;
	double _delta_time;

	int64_t _base_time;
	int64_t _paused_time;
	int64_t _stop_time;
	int64_t _prev_time;
	int64_t _curr_time;

	bool _stopped;
};