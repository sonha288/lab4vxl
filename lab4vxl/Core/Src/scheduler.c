#include "scheduler.h"

static SchedulerTask SCH_tasks[SCH_MAX_TASKS];  // Danh sách các tác vụ
static uint32_t currentTaskId = 1;              // ID bắt đầu từ 1

/**
 * @brief Khởi tạo scheduler, đặt tất cả các task về trạng thái không hợp lệ.
 */
void SCH_Init(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks[i].taskFunction = (void *)0;  // Đặt giá trị mặc định
        SCH_tasks[i].delay = 0;
        SCH_tasks[i].period = 0;
        SCH_tasks[i].isReady = 0;
        SCH_tasks[i].taskId = NO_TASK_ID;      // Đánh dấu ô trống
    }
}

/**
 * @brief Thêm một tác vụ mới vào danh sách scheduler.
 * @param taskFunction Con trỏ hàm callback của tác vụ.
 * @param delay Thời gian trễ (ms) trước khi thực thi lần đầu.
 * @param period Chu kỳ lặp lại (ms), 0 nếu chỉ chạy 1 lần.
 * @return ID của tác vụ hoặc NO_TASK_ID nếu thất bại.
 */
uint32_t SCH_AddTask(void (*taskFunction)(void), uint32_t delay, uint32_t period) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks[i].taskId == NO_TASK_ID) {  // Tìm ô trống bằng cách kiểm tra taskId
            SCH_tasks[i].taskFunction = taskFunction;
            SCH_tasks[i].delay = delay;
            SCH_tasks[i].period = period;
            SCH_tasks[i].isReady = 0;
            SCH_tasks[i].taskId = currentTaskId++;
            return SCH_tasks[i].taskId;  // Trả về ID của tác vụ
        }
    }
    return NO_TASK_ID;  // Không còn chỗ trống
}

/**
 * @brief Xóa một tác vụ khỏi danh sách scheduler.
 * @param taskId ID của tác vụ cần xóa.
 * @return 1 nếu xóa thành công, 0 nếu thất bại.
 */
uint8_t SCH_DeleteTask(uint32_t taskId) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks[i].taskId == taskId) {  // Tìm tác vụ theo ID
            SCH_tasks[i].taskFunction = (void *)0;  // Đặt lại giá trị mặc định
            SCH_tasks[i].delay = 0;
            SCH_tasks[i].period = 0;
            SCH_tasks[i].isReady = 0;
            SCH_tasks[i].taskId = NO_TASK_ID;  // Đánh dấu ô trống
            return 1;  // Xóa thành công
        }
    }
    return 0;  // Không tìm thấy tác vụ
}

/**
 * @brief Hàm cập nhật trạng thái của tất cả tác vụ.
 * Gọi hàm này trong ngắt timer định kỳ 1ms.
 */
void SCH_Update(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks[i].taskId != NO_TASK_ID) {  // Chỉ xử lý các task hợp lệ
            if (SCH_tasks[i].delay == 0) {
                SCH_tasks[i].isReady = 1;  // Task sẵn sàng thực thi
                if (SCH_tasks[i].period > 0) {  // Lặp lại
                    SCH_tasks[i].delay = SCH_tasks[i].period;
                }
            } else {
                SCH_tasks[i].delay--;  // Giảm độ trễ
            }
        }
    }
}

/**
 * @brief Thực thi tất cả các tác vụ đã sẵn sàng.
 */
void SCH_DispatchTasks(void) {
    for (int i = 0; i < SCH_MAX_TASKS; i++) {
        if (SCH_tasks[i].isReady) {
            SCH_tasks[i].isReady = 0;       // Reset trạng thái
            (*SCH_tasks[i].taskFunction)(); // Thực thi callback
            if (SCH_tasks[i].period == 0) { // Nếu không lặp lại, xóa task
                SCH_DeleteTask(SCH_tasks[i].taskId);
            }
        }
    }
}
