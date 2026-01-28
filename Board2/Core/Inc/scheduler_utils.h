#ifndef SCHEDULER_UTILS_H
#define SCHEDULER_UTILS_H

#include "cmsis_os.h"

/**
 * @brief Sets the priority of the current task to osPriorityHigh5 to prevent preemption by normal tasks.
 *
 * This allows creating a critical section where the task cannot be preempted by other tasks,
 * but interrupts remain enabled (unlike osKernelLock which might affect ISR latency depending on implementation).
 *
 * @return osPriority_t The previous priority of the task.
 */
osPriority_t scheduler_set_high_priority(void);

/**
 * @brief Restores the priority of the current task to a previously saved value.
 *
 * @param old_prio The priority to restore.
 */
void scheduler_restore_priority(osPriority_t old_prio);

#endif // SCHEDULER_UTILS_H
