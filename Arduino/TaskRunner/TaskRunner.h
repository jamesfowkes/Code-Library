#ifndef _TASK_RUNNER_H_
#define _TASK_RUNNER_H_

typedef bool (*TASK_RUNNER_FN)(int, int);

class TaskRunner
{
public:
	TaskRunner(TASK_RUNNER_FN fn);
	void run(int times=1, unsigned long interval=0);
	void tick(unsigned long millisec=0);

private:
	void call_once();
	void stop_and_reset();

	void run_with_interval(unsigned long millisec);
	void run_without_interval();

	TASK_RUNNER_FN m_fn;
	int m_call_counter;
	int m_remaining_calls;
	unsigned long m_interval;
	unsigned long m_last_time;
};
#endif