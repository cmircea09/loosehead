#include "timer.hpp"

Timer::Timer()
{
	_seconds_per_count = 0.0;
	_delta_time = -1.0;
	_base_time = 0;
	_paused_time = 0;
	_prev_time = 0;
	_curr_time = 0;
	_stopped = false;

	int64_t counts_per_sec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_sec);
	_seconds_per_count = 1.0 / (double)counts_per_sec;
}

float Timer::total_time()
{
	if(_stopped)
		return (float)(((_stop_time - _paused_time) - _base_time) * _seconds_per_count);
	else
		return (float)(((_curr_time - _paused_time) - _base_time) * _seconds_per_count);
}

float Timer::delta_time()
{
	return (float)(_delta_time);
}

void Timer::start()
{
	int64_t start_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&start_time);

	if(_stopped) {
		_paused_time += (start_time - _stop_time);

		_prev_time = start_time;
		_stop_time = 0;
		_stopped = 0;
	}
}

void Timer::stop()
{
	if(_stopped) {
		int64_t curr_time;
		QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);

		_stop_time = curr_time;
		_stopped = true;
	}
}

void Timer::reset()
{
	int64_t curr_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);

	_base_time = curr_time;
	_prev_time = curr_time;
	_stop_time = 0;
	_stopped = false;
}

void Timer::tick()
{
	if(_stopped) {
		_delta_time = 0.0;
		return;
	}

	int64_t curr_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&curr_time);

	_curr_time = curr_time;
	_delta_time = (_curr_time - _prev_time) * _seconds_per_count;
	_prev_time = _curr_time;

	if(_delta_time < 0.0)
		_delta_time = 0.0;
}