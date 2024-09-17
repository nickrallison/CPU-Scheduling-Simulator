#include "simulator.h"
#include "pid_record.h"

#include <stdint.h>

// #### PROCESS QUEUE ####
int process_queue_new(pid_records_t pid_records,
                      int (*compare)(const void *, const void *)) {
    process_queue_t process_queue = {compare, pid_records};
    return 0;
}

int process_queue_add(process_queue_t *process_queue, pid_record_t process) {
    pid_records_append(&process_queue->processes_queue, process);
    pid_records_sort_by(&process_queue->processes_queue, process_queue->compare);
    return 0;
}

pid_record_t process_queue_pop(process_queue_t *process_queue) {
    pid_record_t process = process_queue->processes_queue.pid_records[0];
    for (int i = 0; i < process_queue->processes_queue.size - 1; i++) {
        process_queue->processes_queue.pid_records[i] =
                process_queue->processes_queue.pid_records[i + 1];
    }
    process_queue->processes_queue.size--;
    return process;
}

// #### SIMULATOR ####
simulator_t simulator_new(pid_records_t pid_records,
                          int (*compare)(const void *, const void *), uint8_t preemptive) {
    uint32_t current_time = 0;
    int current_index = 0;
    uint8_t has_current_process = 0;
    pid_record_t current_process_option = {0, 0, 0, 0, 0};

    pid_records_t pid_records_in_order = pid_records;
    pid_records_sort_by(&pid_records_in_order, &pid_record_compare_arrival_time);

    pid_records_t empty_pid_records = pid_records_new();
    process_queue_t process_queue = {compare, empty_pid_records};

    pid_completion_records_t pid_completion_records =
            pid_completion_records_new();

    simulator_t simulator = {
        preemptive,
        current_time, current_index,
        has_current_process, current_process_option,
        pid_records_in_order, process_queue,
        pid_completion_records
    };
    return simulator;
}

// 1. if another process is ready to run, add it to the process queue
// 2. if using preemptive scheduling, check if the current process should be
// added back to the process queue
// 3. if no process is running, get the next process from the process queue
// 4. run the process for one time step
// 5. if the process is done, add it to the completion records
// 6. increment the current time

int simulator_time_step(simulator_t *simulator) {
    // 1. if another process is ready to run, add it to the process queue
    // using the current index to keep track of the next process to add
    while (simulator->current_index < simulator->pid_records_in_order.size &&
           simulator->pid_records_in_order.pid_records[simulator->current_index]
           .arrival_time == simulator->current_time) {
        process_queue_add(&simulator->process_queue,
                          simulator->pid_records_in_order
                          .pid_records[simulator->current_index]);
        simulator->current_index++;
    }

    // 2. if using preemptive scheduling, check if the current process should be
    // added back to the process queue
    if (simulator->preemptive && simulator->has_current_process &&
        simulator->current_process_option.running_cpu_burst > 0) {
        process_queue_add(&simulator->process_queue,
                          simulator->current_process_option);
        simulator->has_current_process = 0;

    }

    // 3. if no process is running, get the next process from the process queue
    if (!simulator->has_current_process) {
        if (simulator->process_queue.processes_queue.size > 0) {
            simulator->current_process_option =
                    process_queue_pop(&simulator->process_queue);
            simulator->has_current_process = 1;
        }
    }

    // 4. run the process for one time step
    simulator->current_process_option.running_cpu_burst--;
    simulator->current_process_option.running_time_until_first_response--;

    // 5. if the process is done, add it to the completion records
    if (simulator->has_current_process &&
        simulator->current_process_option.running_cpu_burst == 0) {
        pid_completion_record_t pid_completion_record = pid_completion_record_new(
            simulator->current_process_option.pid,
            simulator->current_process_option.arrival_time,
            simulator->current_process_option.time_until_first_response,
            simulator->current_process_option.actual_cpu_burst,
            simulator->current_time + 1);
        pid_completion_records_append(&simulator->pid_completion_records,
                                      pid_completion_record);
        simulator->has_current_process = 0;
    }

    // 6. increment the current time
    simulator->current_time++;
    return 0;
}

pid_completion_records_t *simulator_run(simulator_t *simulator) {
    while (simulator->pid_completion_records.size <
           simulator->pid_records_in_order.size) {
        simulator_time_step(simulator);
    }
    return &simulator->pid_completion_records;
}
