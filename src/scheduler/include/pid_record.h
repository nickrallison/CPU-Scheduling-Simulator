// pid_record.h
#ifndef PID_RECORD_H
#define PID_RECORD_H

#include <stddef.h>
#include <stdint.h>


// process#, arrival time, time until first response, actual CPU burst>
typedef struct pid_record_t {
    uint16_t pid;
    uint16_t arrival_time;
    uint16_t time_until_first_response;
    uint16_t actual_cpu_burst;
} pid_record_t;

pid_record_t pid_record_new(uint16_t pid, uint16_t arrival_time, uint16_t time_until_first_response, uint16_t actual_cpu_burst);
int pid_record_print(pid_record_t* self);

// num records
typedef struct pid_records_t {
    uint32_t size;
    uint32_t capacity;
    pid_record_t* pid_records;
} pid_records_t;

pid_records_t pid_records_new();
int pid_records_append(pid_records_t* self, pid_record_t pid_record);
int pid_records_print(const pid_records_t* self);
int pid_records_sort_by(pid_records_t* self, int (*get_value)(pid_record_t* pid_records));

// creates from stdin file
pid_records_t create_pid_records();

#endif // PID_RECORD_H