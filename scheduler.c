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

void fcfs(Process *p_list, SchedulerStats *stats)
{

    Process *rp = NULL; // Running process
    Process *p = NULL;  // Process
    printf("**** \n\n\n\nFirst Come First Serve\n");
    while (stats->TOTAL_FINISHED_PROCESSES < stats->TOTAL_CREATED_PROCESSES)
    {

        for (uint32_t i = 0; i < stats->TOTAL_CREATED_PROCESSES; i++)
        {
            uint32_t index = i;
            if (rp != NULL)
            {
                index = (rp - p_list) + i;
            }

            if (index >= stats->TOTAL_CREATED_PROCESSES)
            {
                index -= stats->TOTAL_CREATED_PROCESSES; 
            }

            p = &p_list[index];

            
            if (p->status == TERMINATED)
            {
                continue;
            }

            
            if (p->status == BLOCKED)
            {
                p->currentIOBlockedTime++;
                p->IOBurst--;

                // if the process is done being blocked
                if (p->IOBurst <= 0)
                {
                    p->status = READY;
                }

                stats->TOTAL_NUMBER_OF_CYCLES_SPENT_BLOCKED++;
            }
            else if (p->status == NOT_STARTED)
            {
                p->status = READY;
                stats->TOTAL_STARTED_PROCESSES++;
                p->isFirstTimeRunning = 1;
            }
            if (p->status == READY)
            {
                // Ready -> Running
                if (rp == NULL)
                {
                    setBursts(p);
                    p->status = RUNNING;
                    rp = p;
                }
                else
                {
                    p->currentWaitingTime++;
                }
            }
            // Running
            else if (p->status == RUNNING)
            {
                p->currentCPUTimeRun++;
                p->CPUBurst--;
                stats->TOTAL_STARTED_PROCESSES += p->isFirstTimeRunning;
                p->isFirstTimeRunning = 0;

                if (p->currentCPUTimeRun >= p->C)
                {
                    p->status = TERMINATED;
                    p->finishingTime = stats->CURRENT_CYCLE;
                    rp = NULL;

                    stats->TOTAL_FINISHED_PROCESSES++;
                }

                // Running -> Block
                else if (p->CPUBurst <= 0)
                {
                    rp = NULL;
                    p->status = BLOCKED;
                }
            }
        }

        stats->CURRENT_CYCLE++;
    }
    printStart(p_list, *stats);
    printFinal(p_list, *stats);
    printProcessSpecifics(p_list, *stats);
    printSummaryData(p_list, *stats);
}