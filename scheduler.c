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

static void setBursts(Process *p)
{
    FILE *random_num_file = fopen("random-numbers", "r");
    if (random_num_file == NULL)
    {
        printf("Error: Cannot open file random-numbers\n");
        exit(1);
    }
    // Set the CPU burst time
    p->CPUBurst = randomOS(p->B, p->processID, random_num_file);
    // Set the IO burst time
    p->IOBurst = p->M * p->CPUBurst;

    fclose(random_num_file);
}
