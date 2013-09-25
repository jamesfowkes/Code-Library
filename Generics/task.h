#ifndef _TASK_H_
#define _TASK_H_

typedef void (*TASK_FN)(void);

struct task
{
	uint8_t msTick;
	uint32_t msCount;
	uint32_t msPerBeat;
	TASK_FN fn;
};
typedef struct task TASK;

void Task_Init(TASK * task);
bool Task_Tick(TASK * task);

#endif
