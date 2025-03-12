#include "scheduler.h"

SchedulerStats initStats()
{
    SchedulerStats stats;
    stats.CURRENT_CYCLE = 0;
    stats.TOTAL_CREATED_PROCESSES = 0;
    stats.TOTAL_STARTED_PROCESSES = 0;
    stats.TOTAL_FINISHED_PROCESSES = 0;
    stats.TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED = 0;
    return stats;
}