#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

// Constants
#define SCHEDULER_MAX_TASKS 10
#define TASK_ID_NONE 0xFFFFFFFF

// Task structure definition
typedef struct {
    void (*callback)(void);  // Function pointer to the task
    uint32_t delay;          // Initial delay before execution
    uint32_t interval;       // Periodic interval (0 for one-time tasks)
    uint8_t executeCount;    // Number of times the task is ready to run
    uint32_t taskID;         // Unique task ID
} Task;

// Function prototypes
void Scheduler_Init(void);
uint32_t Scheduler_Add_Task(void (*callback)(void), uint32_t delay, uint32_t interval);
uint8_t Scheduler_Remove_Task(uint32_t taskID);
void Scheduler_Update(void);
void Scheduler_Dispatch(void);
void Scheduler_Clear_Task(Task* task);


#endif /* SCHEDULER_H_ */
