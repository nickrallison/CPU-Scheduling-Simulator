# CPU Scheduling Simulator

## Overview
This project is a CPU scheduling simulator designed to simulate various scheduling algorithms, including First-Come-First-Served (FCFS), Shortest Job First (SJF), Round Robin (RR), Priority Scheduling, and Shortest Remaining Time (SRT). The simulator is implemented in C and provides a framework for testing and analyzing the performance of different scheduling algorithms.

## Features
- **Multiple Scheduling Algorithms**: Supports FCFS, SJF, RR, Priority, and SRT scheduling algorithms.
- **Input File Support**: Processes input files containing process details such as PID, arrival time, burst length, and time until the first response.
- **Simulation Framework**: Includes a simulator module that runs the scheduling algorithms and tracks process completion times, wait times, turnaround times, and response times.
- **Testing Framework**: Provides test cases to validate the correctness of the scheduling algorithms.

## Project Structure
The project is organized into several directories and files:

- **src/scheduler**: Contains the implementation of the scheduling algorithms.
- **src/simulator**: Implements the simulation framework that runs the scheduling algorithms.
- **src/process**: Manages process records and their attributes.
- **src/test**: Includes test cases to validate the scheduling algorithms.
- **src/executable**: Contains the main executable code for running the simulator.
- **src/graphing**: (Optional) May include code for visualizing simulation results.

## Key Files
- **main.c**: The entry point for the simulator, responsible for reading input files and running the scheduling algorithms.
- **scheduler.c**: Implements the core scheduling algorithms (FCFS, SJF, RR, Priority, SRT).
- **simulator.c**: Manages the simulation process, including process queues and time steps.
- **pid_record.c**: Defines the data structures and functions for managing process records.
- **test.c**: Contains test cases to validate the scheduling algorithms.

## Usage
To build and run the project, follow these steps:

1. **Build the Project**:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

2. **Run the Simulator**:
   ```bash
   ./scheduler <algorithm> [time_quantum] < input_file
   ```
   Replace `<algorithm>` with one of the supported scheduling algorithms (FCFS, SJF, RR, Priority, SRT). For RR, specify the time quantum.

3. **Run Tests**:
   ```bash
   ./test
   ```

## Input File Format
The input file should be in CSV format with the following columns:
- **Pid**: Process ID.
- **Arrival Time**: Time at which the process arrives in the system.
- **Time until first Response**: Time until the process receives its first response.
- **Burst Length**: CPU burst length of the process.

Example:
```
Pid,Arrival Time,Time until first Response,Burst Length
42,0,15,42
24,10,100,186
42,15,7,36
14,16,152,158
```

## Output
The simulator outputs the following metrics for each process:
- **ID**: Process ID.
- **Arrival**: Arrival time.
- **Burst**: CPU burst length.
- **Start**: Time when the process starts execution.
- **Finish**: Time when the process completes.
- **Wait**: Total wait time.
- **Turnaround**: Total turnaround time.
- **Response Time**: Time until the first response.

## Testing
The project includes a testing framework to validate the correctness of the scheduling algorithms. Test cases are defined in `test.c` and can be run using the `./test` executable.

## Acknowledgments
- This project was developed as part of a course assignment for CPSC 457.
- Special thanks to the teaching staff for their guidance and support.
