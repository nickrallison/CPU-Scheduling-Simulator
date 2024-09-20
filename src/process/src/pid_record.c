#include "pid_record.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ##### PID RECORD #####
// typedef struct pid_record_t {
//     uint32_t pid;
//     uint32_t arrival_time;
//     uint32_t time_until_first_response;
//     uint32_t actual_cpu_burst;
//     // ######
//     uint32_t start_time;
//     uint32_t running_cpu_burst;
//     uint32_t running_time_until_first_response;
//     uint32_t first_response_time;
//     uint32_t added_to_queue;
//     uint32_t* exp_time_remaining_chart;
//
//     // ###################
//     uint32_t completion_time;
//
//
// } pid_record_t;
pid_record_t pid_record_new(uint32_t pid, uint32_t arrival_time,
                            uint32_t time_until_first_response,
                            uint32_t actual_cpu_burst) {
    uint32_t start_time = 0;
    uint8_t has_started = 0;
    uint32_t running_cpu_burst = actual_cpu_burst;
    uint32_t running_time_until_first_response = time_until_first_response;
    uint32_t first_response_time = 0;
    uint32_t added_to_queue = 0;
    uint32_t exp_time_remaining = 10;
    uint32_t completion_time = 0;
    pid_record_t pid_record = {
        pid, arrival_time, time_until_first_response, actual_cpu_burst,
        start_time, has_started, running_cpu_burst, running_time_until_first_response, first_response_time,
        added_to_queue, NULL, completion_time};
    return pid_record;
}

int pid_record_print(pid_record_t *self) {
    printf("PID: %hd, Arrival: %hd, Time Until First Response: %hd, Actual CPU "
           "Burst: %hd\n",
           self->pid, self->arrival_time, self->time_until_first_response,
           self->actual_cpu_burst);
    return 0;
}

int pid_record_compare_arrival_time(const void *a, const void *b) {
    pid_record_t *pid_record_a = (pid_record_t *) a;
    pid_record_t *pid_record_b = (pid_record_t *) b;
    return pid_record_a->arrival_time - pid_record_b->arrival_time;
}

// ##### PID RECORDS CONTAINER #####
pid_records_t pid_records_new() {
    uint32_t size = 0;
    uint32_t capacity = 32;
    pid_record_t *records_array = malloc(capacity * sizeof(pid_record_t));
    pid_records_t pid_records = {size, capacity, records_array};
    return pid_records;
}

int pid_records_append(pid_records_t *self, const pid_record_t pid_record) {
    if (self->size == self->capacity - 1) {
        // malloc an array with double the space
        self->capacity *= 2;
        pid_record_t *new_buffer = malloc(self->capacity * sizeof(pid_record_t));

        // copy the elements
        memcpy(new_buffer, self->pid_records, self->size * sizeof(pid_record_t));

        // free the old array
        free(self->pid_records);

        // copy the new buffer to self->pid_records
        self->pid_records = new_buffer;
    }

    self->pid_records[self->size] = pid_record;
    self->size++;
    return 0;
}

int pid_records_print(const pid_records_t *self) {
    for (int i = 0; i < self->size; i++) {
        printf("PID: %d, Arrival: %d, Time Until First Response: %d, Actual CPU "
               "Burst: %d\n",
               self->pid_records[i].pid, self->pid_records[i].arrival_time,
               self->pid_records[i].time_until_first_response,
               self->pid_records[i].actual_cpu_burst);
    }
    return 0;
}

pid_records_t create_pid_records() {
    uint32_t pid;
    uint32_t arrival_time;
    uint32_t time_until_first_response;
    uint32_t actual_cpu_burst;

    pid_records_t pid_records = pid_records_new();

    // get rid of the first line
    char *first_line_char = malloc(sizeof(char));
    *first_line_char = '\0';
    while (*first_line_char != '\n') {
        *first_line_char = getchar();
    }

    // Reads stdin line by line and creates a pid_record_t for each line
    while (scanf("%d,%d,%d,%d\n", &pid, &arrival_time,
                 &time_until_first_response, &actual_cpu_burst) != EOF) {
        pid_record_t pid_record = pid_record_new(
            pid, arrival_time, time_until_first_response, actual_cpu_burst);
        pid_records_append(&pid_records, pid_record);
    }
    return pid_records;
}

int pid_records_sort_by(pid_records_t *self,
                        int (*compare)(const void *, const void *)) {
    qsort(self->pid_records, self->size, sizeof(pid_record_t), compare);
    return 0;
}

// ##### PID COMPLETION RECORD #####
// pid_record_t
// pid_completion_record_new(uint32_t pid, uint32_t arrival_time,
//                           uint32_t time_until_first_response,
//                           uint32_t actual_cpu_burst, uint32_t completion_time) {
//     pid_record_t pid_completion_record = {
//         pid, arrival_time, time_until_first_response, actual_cpu_burst,
//         completion_time
//     };
//     return pid_completion_record;
// }
//
// int pid_completion_record_print(pid_completion_record_t *self) {
//     printf("PID: %hd, Arrival: %hd, Time Until First Response: %hd, Actual CPU "
//            "Burst: %hd\n",
//            self->pid, self->arrival_time, self->time_until_first_response,
//            self->actual_cpu_burst);
//     return 0;
// }
//
// // ##### PID COMPLETION RECORDS CONTAINER #####
// pid_completion_records_t pid_completion_records_new() {
//     uint32_t size = 0;
//     uint32_t capacity = 32;
//     pid_completion_record_t *records_array = (pid_completion_record_t *) malloc(
//         capacity * sizeof(pid_completion_record_t));
//     pid_completion_records_t pid_completion_records = {
//         size, capacity,
//         records_array
//     };
//     return pid_completion_records;
// }
//
// int pid_completion_records_append(
//     pid_completion_records_t *self,
//     const pid_completion_record_t pid_completion_record) {
//     if (self->size == self->capacity - 1) {
//         // malloc an array with double the space
//         self->capacity *= 2;
//         pid_completion_record_t *new_buffer = (pid_completion_record_t *) malloc(
//             self->capacity * sizeof(pid_completion_record_t));
//
//         // copy the elements
//         memcpy(new_buffer, self->completion_records,
//                self->size * sizeof(pid_completion_record_t));
//
//         // free the old array
//         free(self->completion_records);
//
//         // copy the new buffer to self->completion_records
//         self->completion_records = new_buffer;
//     }
//
//     self->completion_records[self->size] = pid_completion_record;
//     self->size++;
//     return 0;
// }
//

uint32_t min_uint32_t(uint32_t a, uint32_t b) {
    return a < b ? a : b;
}
uint32_t max_uint32_t(uint32_t a, uint32_t b) {
    return a > b ? a : b;
}

int pid_completion_records_print(pid_records_t *self) {
    // allocate array of  51 pids, times 8 u32s
    uint32_t* results = malloc(50 * 7 * sizeof(uint32_t));

    // Here we see how to calculate these metrics for a whole process
    //
    // Arrive: Earliest arrival of all requests
    // Burst: Sum of all individual bursts
    // Start: Earliest start(might not be the same request as earliest arrive)
    // Finish: The latest finish of all requests
    // Wait: Sum of all individual wait times
    // Turnaround: Process finish minus process arrive
    // Response: Earliest response of any request, minus process arrival


    for (int pid = 0; pid < 50; pid++) {
        results[pid * 7] = pid + 1;
        results[pid * 7 + 1] = UINT32_MAX;
        results[pid * 7 + 2] = 0;
        results[pid * 7 + 3] = UINT32_MAX;
        results[pid * 7 + 4] = 0;
        results[pid * 7 + 5] = 0;
        results[pid * 7 + 6] = UINT32_MAX;
    }

    for (int i = 0; i < self->size; i++) {
        uint32_t pid = self->pid_records[i].pid - 1;
        uint32_t arrival = self->pid_records[i].arrival_time;
        uint32_t burst = self->pid_records[i].actual_cpu_burst;
        uint32_t start = self->pid_records[i].start_time;
        uint32_t finish = self->pid_records[i].completion_time;
        uint32_t wait = start - arrival;
        uint32_t response_time = self->pid_records[i].first_response_time - arrival;

        results[pid * 7 + 1] = min_uint32_t(results[pid * 7 + 1], arrival);
        results[pid * 7 + 2] += burst;
        results[pid * 7 + 3] = min_uint32_t(results[pid * 7 + 3], start);
        results[pid * 7 + 4] = max_uint32_t(results[pid * 7 + 4], finish);
        results[pid * 7 + 5] += wait;
        results[pid * 7 + 6] = min_uint32_t(results[pid * 7 + 6], response_time);
    }

    printf("+----+---------+--------+--------+--------+--------+-------------+---------------+\n");
    printf("| ID | Arrival | Burst  | Start  | Finish | Wait   | Turnaround  | Response Time |\n");
    printf("+----+---------+--------+--------+--------+--------+-------------+---------------+\n");
    for (int i = 0; i < 50; i++) {
        uint32_t pid = results[i * 7];
        uint32_t arrival = results[i * 7 + 1];
        uint32_t burst = results[i * 7 + 2];
        uint32_t start = results[i * 7 + 3];
        uint32_t finish = results[i * 7 + 4];
        uint32_t wait = results[i * 7 + 5];
        uint32_t turnaround = finish - arrival;
        uint32_t response_time = results[i * 7 + 6] - arrival;


        printf("| %2d | %7d | %6d | %6d | %6d | %6d | %11d | %13d |\n", pid, arrival, burst, start, finish, wait, turnaround, response_time);
    }

    uint32_t total_wait = 0;
    uint32_t total_turnaround = 0;
    uint32_t total_response = 0;

    for (int i = 0; i < 50; i++) {
        total_wait += results[i * 7 + 5];
        total_turnaround += results[i * 7 + 4] - results[i * 7 + 1];
        total_response += results[i * 7 + 6];
    }

    double average_wait = (double) total_wait / 50;
    double average_turnaround = (double) total_turnaround / 50;
    double average_response = (double) total_response / 50;

    printf("+----+---------+--------+--------+--------+--------+-------------+---------------+\n");
    printf("Average waiting time: %.2f ms\n", average_wait);
    printf("Average turnaround time: %.2f ms\n", average_turnaround);
    printf("Average response time: %.2f ms\n", average_response);
    return 0;
}
float average_waiting_time(pid_records_t *self) {
    float total_waiting_time = 0;
    for (int i = 0; i < self->size; i++) {
        total_waiting_time += self->pid_records[i].completion_time - self->pid_records[i].arrival_time;
    }
    return total_waiting_time / self->size;
}
float average_turnaround_time(pid_records_t *self) {
    float total_turnaround_time = 0;
    for (int i = 0; i < self->size; i++) {
        total_turnaround_time += self->pid_records[i].completion_time - self->pid_records[i].arrival_time;
    }
    return total_turnaround_time / self->size;
}
float average_response_time(pid_records_t *self) {
    float total_response_time = 0;
    for (int i = 0; i < self->size; i++) {
        total_response_time += self->pid_records[i].first_response_time - self->pid_records[i].arrival_time;
    }
    return total_response_time / self->size;
}
//
// int pid_completion_records_sort_by(pid_completion_records_t *self,
//                                    int (*compare)(const void *, const void *)) {
//     qsort(self->completion_records, self->size, sizeof(pid_completion_record_t),
//           compare);
//     return 0;
// }
