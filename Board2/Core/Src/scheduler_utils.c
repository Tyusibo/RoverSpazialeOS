#include "scheduler_utils.h"

osPriority_t scheduler_set_high_priority(void)
{
    osThreadId_t thread_id = osThreadGetId();
    osPriority_t old_prio = osThreadGetPriority(thread_id);
    
    // Set priority to High5 (highest non-realtime priority in typical cmsis v2 configs)
    // This effectively prevents preemption by other normal tasks.
    osThreadSetPriority(thread_id, osPriorityHigh5);
    
    return old_prio;
}

void scheduler_restore_priority(osPriority_t old_prio)
{
    osThreadId_t thread_id = osThreadGetId();
    osThreadSetPriority(thread_id, old_prio);
}
