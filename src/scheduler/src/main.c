#include "pid_record.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int fcfs_value(pid_record_t* pid_record) {
    return pid_record->arrival_time;
}

int main(int argc, char* argv[]) {
    // Checking if the number of arguments is correct
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [FCFS,SJF,Priority,RR] < [inputfile]\n", argv[0]);
        return 1;
    }

    // Finding which scheduling algorithm was chosen
    char* scheduling_algorithm = argv[1];
    char* scheduling_algorithms[] = {"FCFS", "SJF", "Priority", "RR"};
    int num_scheduling_algorithms = sizeof(scheduling_algorithms)/sizeof(char*);
    int algorithm_chosen = 0;

    while (algorithm_chosen < num_scheduling_algorithms) {
        if (!strcmp(scheduling_algorithm, scheduling_algorithms[algorithm_chosen])) {
            break;
        }
        algorithm_chosen++;
    }
    // erroring if none of the valid choices are used
    if (algorithm_chosen == num_scheduling_algorithms) {
        fprintf(stderr, "Invalid scheduling algorithm: %s, please choose one of the following: ", scheduling_algorithm);
        int i = 0;

        while (i < num_scheduling_algorithms - 1) {
            fprintf(stderr, "%s, ", scheduling_algorithms[i]);
            i++;
        }
        fprintf(stderr, "%s\n", scheduling_algorithms[i]);
        return 1;
    }

    pid_records_t pid_records = create_pid_records();
    pid_records_print(&pid_records);
    // sort by arrival time
    pid_records_sort_by(&pid_records, &fcfs_value);


    pid_records_print(&pid_records);
    return 0;
}
