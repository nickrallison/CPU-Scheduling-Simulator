#include "simulator.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pid_record.h"

int fcfs_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->arrival_time - second_pid_record->arrival_time;
}

int sjn_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->actual_cpu_burst- second_pid_record->actual_cpu_burst;
}

int rr_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->added_to_queue - second_pid_record->added_to_queue;
}

int priority_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return second_pid_record->pid - first_pid_record->pid;
}

int srt_comp(const void *first, const void *second) {
  pid_record_t *first_pid_record = (pid_record_t *)first;
  pid_record_t *second_pid_record = (pid_record_t *)second;
  return first_pid_record->exp_time_remaining_chart[first_pid_record->pid] - second_pid_record->exp_time_remaining_chart[second_pid_record->pid];
}

pid_results_t main_runner(int argc, char *argv[]) {
  // Checking if the number of arguments is correct
  if (argc != 2 && argc != 3) {
    fprintf(stderr, "Usage: %s (FCFS,SJF,RR,Priority,SRT) [exp_weight | rr time quantum] < (inputfile)\n",
            argv[0]);
    exit(1);
  }

  // Finding which scheduling algorithm was chosen
  char *scheduling_algorithm = argv[1];
  char *scheduling_algorithms[] = {"FCFS", "SJF","RR", "Priority", "SRT"};
  int num_scheduling_algorithms =
      sizeof(scheduling_algorithms) / sizeof(char *);
  int algorithm_chosen = 0;

  while (algorithm_chosen < num_scheduling_algorithms) {
    if (!strcmp(scheduling_algorithm,
                scheduling_algorithms[algorithm_chosen])) {
      break;
    }
    algorithm_chosen++;
  }
  // erroring if none of the valid choices are used
  if (algorithm_chosen == num_scheduling_algorithms) {
    fprintf(stderr,
            "Invalid scheduling algorithm: %s, please choose one of the "
            "following: ",
            scheduling_algorithm);
    int i = 0;

    while (i < num_scheduling_algorithms - 1) {
      fprintf(stderr, "%s, ", scheduling_algorithms[i]);
      i++;
    }
    fprintf(stderr, "%s\n", scheduling_algorithms[i]);
    exit(1);
  }

  // if the algorithm chosen is RR, check if the time quantum is provided
  int time_quantum = -1;
  float exp_weight = -1;
  if (algorithm_chosen == 2) {
    if (argc != 3) {
      fprintf(stderr, "RR requires a time quantum argument\n");
      exit(1);
    }
    time_quantum = atoi(argv[2]);
    if (time_quantum <= 0) {
      fprintf(stderr, "Time quantum must be a positive integer\n");
      exit(1);
    }
  }

  // if the algorithm chosen is SRT, check if the exponential weight is provided

  else if (algorithm_chosen == 4) {
    if (argc != 3) {
      fprintf(stderr, "SRT requires an exponential weight argument\n");
      exit(1);
    }
    exp_weight = atof(argv[2]);
    if (exp_weight <= 0 || exp_weight >= 1) {
      fprintf(stderr, "Exponential weight must be a float between 0 and 1\n");
      exit(1);
    }
  }
  else if (argc == 3) {
    fprintf(stderr, "The algorithm chosen does not require an argument\n");
    exit(1);
  }


  pid_records_t pid_records = create_pid_records();

  simulator_t simulator;
  if (algorithm_chosen == 0) {
    simulator = simulator_new(&pid_records, &fcfs_comp, 0, 0);
  } else if (algorithm_chosen == 1) {
    simulator = simulator_new(&pid_records, &sjn_comp, 0, 0);
  } else if (algorithm_chosen == 2) {
    simulator = simulator_new(&pid_records, &rr_comp, time_quantum, 0);
  } else if (algorithm_chosen == 3) {
    simulator = simulator_new(&pid_records, &priority_comp, 0, 0);
  } else if (algorithm_chosen == 4) {
    simulator = simulator_new(&pid_records, &srt_comp, 1, exp_weight);
  } else {
    fprintf(stderr,"Simulator could not be created\n");
    exit(1);
  }

  pid_records_t *pid_completion_records = simulator_run(&simulator);
  pid_results_t pid_results = pid_results_from_pid_records(pid_completion_records);
  return pid_results;
}
