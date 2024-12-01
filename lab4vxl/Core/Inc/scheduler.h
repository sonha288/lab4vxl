#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#define SCH_MAX_TASKS 10
#define NO_TASK_ID 0xFFFFFFFF

typedef struct {
    void (*taskFunction)(void);  // Con trỏ hàm tác vụ
    uint32_t delay;              // Độ trễ ban đầu (ms)
    uint32_t period;             // Chu kỳ lặp (ms, 0 nếu không lặp)
    uint8_t isReady;             // Cờ đánh dấu tác vụ đã sẵn sàng thực thi
    uint32_t taskId;             // ID của tác vụ
} SchedulerTask;

void SCH_Init(void);
uint32_t SCH_AddTask(void (*taskFunction)(void), uint32_t delay, uint32_t period);
uint8_t SCH_DeleteTask(uint32_t taskId);
void SCH_Update(void);
void SCH_DispatchTasks(void);

#endif // SCHEDULER_H
