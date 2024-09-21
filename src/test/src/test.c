
#include "pid_record.h"
#include "scheduler.h"

char* test_1_input = "Pid,Arrival Time,Time until first Response,Burst Length\n42,0,15,42\n24,10,100,186\n42,15,7,36\n14,16,152,158\n";

int fcfs_test_1(int argc, char *argv[], char* input) {
    pid_results_t pid_results = main_runner_no_stdin(argc, argv, input);

    return 0;
}

int main() {

    // fcfs test
    char* fcfs_argv[] = {"test", "FCFS", "0"};
    fcfs_test_1(2, fcfs_argv, test_1_input);

}