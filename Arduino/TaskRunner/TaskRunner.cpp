/*
 * Arduino Libraries
 */

#include <Arduino.h>
#include "TaskRunner.h"

TaskRunner::TaskRunner(TASK_RUNNER_FN fn)
{
	m_fn = fn;
	m_remaining_calls = 0;
	m_last_time = 0;
	m_interval = 0;
}

void TaskRunner::run(int times, unsigned long interval)
{
	if (!m_fn) { return; }
	if (times == 0) { return; }

	m_call_counter = 0;
	m_remaining_calls = times;
	m_interval = interval;
}

void TaskRunner::tick(unsigned long millisec)
{
	if (m_remaining_calls == 0) { return; }

	if (m_interval ==  0)
	{
		run_without_interval();
	}
	else
	{
		run_with_interval(millisec);
	}
}

void TaskRunner::run_with_interval(unsigned long millisec = 0)
{
	#ifdef ARDUINO
	millisec = millisec ? millisec : millis();
	#endif

	if(millisec - m_last_time >= m_interval)
    {
        m_last_time = millisec;
        call_once();
    }
}

void TaskRunner::run_without_interval()
{
	while(m_remaining_calls)
	{
		call_once();		
	}
}

void TaskRunner::stop_and_reset()
{
	m_remaining_calls = 0;
	m_call_counter = 0;
}

void TaskRunner::call_once()
{
	m_remaining_calls--;
	bool halt = !m_fn(m_call_counter, m_remaining_calls);
	m_call_counter++;

	if (halt) { stop_and_reset(); }
}