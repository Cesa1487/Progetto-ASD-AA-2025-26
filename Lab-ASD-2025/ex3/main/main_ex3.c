#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/priority_queue.h"

/**
 * @file main_ex3.c
 * @brief Simulation of a non-preemptive priority scheduling system.
 *
 * This program reads tasks from a CSV file, simulates their execution using
 * a priority queue based on a ternary heap, and writes the execution log
 * to an output CSV file.
 */

/**
 * @brief Represents a task in the scheduling system.
 */
typedef struct {
    int id;
    int arrival;
    int duration;
    int priority;
} Task;

/**
 * @brief Hash function for Task pointers.
 *
 * Uses the memory address of the task pointer to generate a hash.
 * This is sufficient since we track specific task instances.
 */
unsigned long task_ptr_hash(const void* k) {

    unsigned long addr = (unsigned long)k;
    return (addr >> 3) * 2654435761;
}

/**
 * @brief Comparator function for Tasks.
 *
 * Orders tasks based on priority (Descending order for Max-Heap).
 */
int task_cmp(const void* a, const void* b) {
    const Task* t1 = (const Task*)a;
    const Task* t2 = (const Task*)b;
    return t1->priority - t2->priority;
}

/**
 * @brief Main entry point for the simulation.
 *
 * Usage: ./main_ex3 <tasks.csv> <output.csv>
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <tasks.csv> <output.csv>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in) { perror("Error opening input"); return 1; }

    int capacity = 100;
    int count = 0;
    Task **all_tasks = malloc(capacity * sizeof(Task*));

    char line[1024];
    while (fgets(line, sizeof(line), in)) {
        if (count >= capacity) {
            capacity *= 2;
            all_tasks = realloc(all_tasks, capacity * sizeof(Task*));
        }
        Task *t = malloc(sizeof(Task));
        if (sscanf(line, "%d,%d,%d,%d", &t->id, &t->arrival, &t->duration, &t->priority) == 4) {
            all_tasks[count++] = t;
        } else {
            free(t);
        }
    }
    fclose(in);

    FILE *out = fopen(argv[2], "w");
    if (!out) { perror("Error opening output"); return 1; }

    PriorityQueue *pq = priority_queue_create(task_cmp, task_ptr_hash);

    long long current_time = 0;
    int task_idx = 0;
    int tasks_done = 0;

    while (tasks_done < count) {
        while (task_idx < count && all_tasks[task_idx]->arrival <= current_time) {
            priority_queue_push(pq, all_tasks[task_idx]);
            task_idx++;
        }

        if (priority_queue_size(pq) == 0 && task_idx < count) {
            current_time = all_tasks[task_idx]->arrival;
            continue;
        }

        Task *t = (Task*)priority_queue_top(pq);
        if (t) {
            priority_queue_pop(pq);

            long long start_exec = current_time;
            long long end_exec = start_exec + t->duration;

            fprintf(out, "%d,%lld,%lld\n", t->id, start_exec, end_exec);

            current_time = end_exec;
            tasks_done++;

            free(t);
        }
    }

    priority_queue_free(pq);
    fclose(out);
    free(all_tasks);

    return 0;
}