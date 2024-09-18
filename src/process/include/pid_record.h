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
    // ######
    uint16_t running_cpu_burst;
    uint16_t running_time_until_first_response;
    uint16_t first_response_time;
    uint32_t added_to_queue;
    uint32_t* exp_time_remaining_chart;

    // ###################
    uint32_t completion_time;


} pid_record_t;

pid_record_t pid_record_new(uint16_t pid, uint16_t arrival_time, uint16_t time_until_first_response, uint16_t actual_cpu_burst);
int pid_record_print(pid_record_t* self);
int pid_record_compare_arrival_time(const void * a, const void * b);

typedef struct pid_records_t {
    uint32_t size;
    uint32_t capacity;
    pid_record_t* pid_records;
} pid_records_t;

pid_records_t pid_records_new();
int pid_records_append(pid_records_t* self, pid_record_t pid_record);
int pid_records_print(const pid_records_t* self);
int pid_records_sort_by(pid_records_t* self, int (*compare)(const void *, const void *));

// creates from stdin file
pid_records_t create_pid_records();

/// @brief A struct to store the completion records
// typedef struct pid_completion_record_t {
//     uint16_t pid;
//     uint16_t arrival_time;
//     uint16_t time_until_first_response;
//     uint16_t actual_cpu_burst;
//     // ###################
//     uint32_t completion_time;
//
// } pid_completion_record_t;

// pid_completion_record_t pid_completion_record_new(uint16_t pid, uint16_t arrival_time, uint16_t time_until_first_response, uint16_t actual_cpu_burst, uint32_t completion_time);
// int pid_completion_record_print(pid_completion_record_t* self);

// typedef struct pid_completion_records_t {
//     uint32_t size;
//     uint32_t capacity;
//     pid_completion_record_t* completion_records;
// } pid_completion_records_t;
// pid_completion_records_t pid_completion_records_new();
// int pid_completion_records_append(pid_completion_records_t* self, pid_completion_record_t pid_completion_record);
int pid_completion_records_print(pid_records_t* self);
// int pid_completion_records_sort_by(pid_completion_records_t* self, int (*compare)(const void *, const void *));


#endif // PID_RECORD_H