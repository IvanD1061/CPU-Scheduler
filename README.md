# CPU Scheduler Simulator (FCFS · RR · SJF) — C

## Overview

This repository contains a CPU scheduling simulator written in C that models how processes move through typical OS states (READY, RUNNING, BLOCKED, TERMINATED). It reads a process-mix input file, simulates execution cycle-by-cycle, generates deterministic CPU/I/O bursts using a shared random number source, and prints both per-process results and overall system metrics.

A single run executes all three schedulers on the same input:
- FCFS (First Come First Serve)
- RR (Round Robin, quantum = 2)
- SJF (Shortest Job First, non-preemptive)

---

## Features

### Scheduling Strategies
- FCFS: runs ready processes in arrival order (FIFO-style)
- RR: time-sliced scheduling with a fixed quantum of 2 cycles
- SJF: selects the job with the smallest remaining CPU time (non-preemptive, but still performs I/O blocking)

### Burst Generation
CPU bursts are generated using `randomOS(U)` backed by the `random-numbers` file, which makes runs reproducible.

- CPU burst is generated in the range `1..B`
- I/O burst is computed as `M * CPU_burst`
- In this implementation, the random lookup is deterministic per process ID (so the same process receives consistent burst values across the simulation)

### Cycle Based State Simulation
Each cycle updates:
- RUNNING time (CPU execution)
- BLOCKED time (I/O)
- READY time (waiting)
- TERMINATION when total CPU requirement is reached

### Metrics Output
For each process, the simulator reports:
- Finishing time
- Turnaround time (finishing time − arrival time)
- I/O time (time spent BLOCKED)
- Waiting time (time spent READY)

Summary metrics include:
- Finishing time (overall completion cycle)
- CPU utilization
- I/O utilization
- Throughput (processes per 100 cycles)
- Average turnaround time
- Average waiting time

--- 
## Build Instructions

From the project root:

```bash
gcc -g -Wall -Werror -std=c99 -o scheduler scheduler.c schedulerio.c random.c
```

---

## Running the Simulator

The program expects one argument: the input file containing the process mix.

```bash
./scheduler <input-file>
```

Examples using the included inputs:

```bash
./scheduler input-1
./scheduler input-2
./scheduler input-3
```

The simulator prints results for:

1. First Come First Serve
2. Round Robin
3. Shortest Job First

(one after another, in a single execution)

---

## Input Format

The input begins with the number of processes `N`, followed by `N` process definitions:

```text
N (A B C M) (A B C M) ...
```

Where:

* `A` = arrival time
* `B` = upper bound for CPU burst generation
* `C` = total CPU time required
* `M` = multiplier used to compute I/O burst length (`I/O = M * CPU_burst`)

Example (from `input-1`):

```text
1 (0 1 5 1)
```

## How the Simulator Works

### Process State Model

Each process tracks:

* input parameters: A, B, C, M
* status: NOT_STARTED / READY / RUNNING / BLOCKED / TERMINATED
* counters: CPU time, I/O time, waiting time
* burst values: current CPU burst and I/O burst
* RR quantum value (2)

### Burst Rules

When a process is scheduled to RUN:

1. A CPU burst is generated using `randomOS(B)`
2. An I/O burst is computed as `M * CPU_burst`
3. The process runs until one of the following occurs:

   * total CPU time `C` is completed → TERMINATED
   * CPU burst ends → BLOCKED
   * (RR only) quantum boundary reached → preempted back to READY

---

## Source Code Guide

### `scheduler.c`

* Main simulation driver
* Implements FCFS, RR, and SJF logic
* Runs all three schedulers sequentially and prints per-scheduler results

### `scheduler.h`

* Function declarations for the schedulers

### `schedulerio.c`

* Input parsing (reads N and N process tuples)
* Output formatting:

  * original/sorted input display
  * per-process statistics
  * final summary metrics

### `schedulerio.h`

* Process struct and status enum
* Scheduler statistics struct
* Output and parsing function declarations

### `random.c`

* Implements `randomOS(U)` using the `random-numbers` file
* Provides deterministic burst generation

### `random.h`

* Header for `randomOS`

---

