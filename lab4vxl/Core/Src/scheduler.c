#include "scheduler.h"

// Array to store tasks
static Task schedulerTasks[SCHEDULER_MAX_TASKS];
static uint32_t nextTaskID = 1;  // Task ID counter

/**
 * @brief Initialize the scheduler.
 */
void Scheduler_Init(void) {
    for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
        Scheduler_Clear_Task(&schedulerTasks[i]);
    }
}

/**
 * @brief Clear a specific task.
 * @param task Pointer to the task to clear.
 */
void Scheduler_Clear_Task(Task* task) {
    task->callback = 0;
    task->delay = 0;
    task->interval = 0;
    task->executeCount = 0;
    task->taskID = TASK_ID_NONE;
}

/**
 * @brief Add a new task to the scheduler.
 * @param callback Function pointer to the task to be scheduled.
 * @param delay Initial delay before the task is first executed.
 * @param interval Time interval for periodic tasks (0 for one-time tasks).
 * @return The assigned Task ID, or TASK_ID_NONE if the task could not be added.
 */
uint32_t Scheduler_Add_Task(void (*callback)(void), uint32_t delay, uint32_t interval) {
    for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
        if (schedulerTasks[i].callback == 0) {  // Find an empty slot
            schedulerTasks[i].callback = callback;
            schedulerTasks[i].delay = delay;
            schedulerTasks[i].interval = interval;
            schedulerTasks[i].executeCount = 0;
            schedulerTasks[i].taskID = nextTaskID++;
            return schedulerTasks[i].taskID;
        }
    }
    return TASK_ID_NONE;  // No space available for a new task
}

/**
 * @brief Remove a task from the scheduler using its Task ID.
 * @param taskID The ID of the task to remove.
 * @return 1 if the task was successfully removed, 0 otherwise.
 */
uint8_t Scheduler_Remove_Task(uint32_t taskID) {
    for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
        if (schedulerTasks[i].taskID == taskID) {
            Scheduler_Clear_Task(&schedulerTasks[i]);
            return 1;  // Task successfully removed
        }
    }
    return 0;  // Task not found
}

/**
 * @brief Update the scheduler, decrementing delays and marking tasks ready to execute.
 */
void Scheduler_Update(void) {
    for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
        Task* task = &schedulerTasks[i];
        if (task->callback != 0) {
            if (task->delay == 0) {
                task->executeCount++;
                if (task->interval > 0) {
                    task->delay = task->interval;  // Reset delay for periodic tasks
                }
            } else {
                task->delay--;
            }
        }
    }
}

/**
 * @brief Execute tasks that are ready to run.
 */
void Scheduler_Dispatch(void) {
    for (int i = 0; i < SCHEDULER_MAX_TASKS; i++) {
        Task* task = &schedulerTasks[i];
        if (task->executeCount > 0) {
            task->callback();  // Execute the task
            task->executeCount--;
            if (task->interval == 0) {
                Scheduler_Clear_Task(task);  // Remove one-time tasks
            }
        }
    }
}
