#include "pid_record.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "string_t.h"


pid_record_t pid_record_new(uint16_t pid, uint16_t arrival_time, uint16_t time_until_first_response, uint16_t actual_cpu_burst) {

    pid_record_t pid_record = {
        pid,
        arrival_time,
        time_until_first_response,
        actual_cpu_burst
    };
    return pid_record;
}

int pid_record_print(pid_record_t* self) {
    printf("PID: %hd, Arrival: %hd, Time Until First Response: %hd, Actual CPU Burst: %hd\n", self->pid, self->arrival_time, self->time_until_first_response, self->actual_cpu_burst);
    return 0;
}

pid_records_t pid_records_new() {
    uint32_t size = 0;
    uint32_t capacity = 32;
    pid_record_t* records_array = (pid_record_t*) malloc(capacity * sizeof(pid_record_t));
    pid_records_t pid_records = { size, capacity, records_array };
    return pid_records;
}

int pid_records_append(pid_records_t* self, pid_record_t pid_record) {
    if (self->size == self->capacity - 1) {
        // malloc an array with double the space
        self->capacity *= 2;
        pid_record_t* new_buffer = (pid_record_t*) malloc(self->capacity * sizeof(pid_record_t));

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

int pid_records_print(pid_records_t* self) {
    for (int i = 0; i < self->size; i++) {
        printf("PID: %d, Arrival: %d, Time Until First Response: %d, Actual CPU Burst: %d\n", self->pid_records[i].pid, self->pid_records[i].arrival_time, self->pid_records[i].time_until_first_response, self->pid_records[i].actual_cpu_burst);
    }
    return 0;
}

// used CLRS partition function as a reference (page 184 on 4th ed)
int pid_records_partition(pid_records_t* self, int (*get_value)(pid_record_t* pid_record), uint32_t lower, uint32_t upper) {


    const int p = lower;
    const int r = upper;
    const int x = get_value(&self->pid_records[r]);
    int i = p - 1;

    for (int j = p; j < r - 1; j++) {
        if (get_value(&self->pid_records[j]) <= x) {
            i++;
            // swap A[i] with A[j]
            pid_record_t temp = self->pid_records[i];
            self->pid_records[i] = self->pid_records[j];
            self->pid_records[j] = temp;
        }
    }
    // swap A[i + 1] with A[r]
    pid_record_t temp = self->pid_records[i + 1];
    self->pid_records[i + 1] = self->pid_records[r];
    self->pid_records[r] = temp;
    return i + 1;
}

// uses quicksort as an inner wrapper to sort pid_records
// used CLRS as a reference again
int pid_records_sort_inner(pid_records_t* self, int (*get_value)(pid_record_t* pid_record), uint32_t lower, uint32_t upper) {

    if (lower < upper) {
        if (upper == 2) {
            printf("lower: %d, upper: %d\n", lower, upper);
        }
        uint32_t pivot = pid_records_partition(self, get_value, lower, upper);
        pid_records_sort_inner(self, get_value, lower, pivot);
        pid_records_sort_inner(self, get_value, pivot + 1, upper);

    }
    return 0;
}

// Sorts by some arbitrary value as found by the function pointer "get_value"
int pid_records_sort_by(pid_records_t* self, int (*get_value)(pid_record_t* pid_record)) {

    pid_records_sort_inner(self, get_value, 0, self->size - 1);
    return 0;
}

pid_records_t create_pid_records() {
    uint16_t pid;
    uint16_t arrival_time;
    uint16_t time_until_first_response;
    uint16_t actual_cpu_burst;

    pid_records_t pid_records = pid_records_new();

    // get rid of the first line
    char* line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);

    while (getline(&line, &len, stdin) != -1) {
        sscanf(line, "%hd,%hd,%hd,%hd\n", &pid, &arrival_time, &time_until_first_response, &actual_cpu_burst);
        pid_record_t pid_record = pid_record_new(pid, arrival_time, time_until_first_response, actual_cpu_burst);
        pid_records_append(&pid_records, pid_record);
    }
    return pid_records;
}
