# CPU-scheduler
# Process Scheduling Algorithms and GUI Implementation

## Table of Contents
1. [Introduction](#introduction)
2. [Objective](#objective)
3. [Process Scheduling Algorithms](#process-scheduling-algorithms)
   - [First-Come, First-Served (FCFS)](#first-come-first-served-fcfs)
   - [Shortest Job First (SJF)](#shortest-job-first-sjf)
   - [Priority Scheduling](#priority-scheduling)
   - [Round Robin (RR)](#round-robin-rr)
4. [Implementation Details](#implementation-details)
   - [C++ Program for Process Scheduling](#c-program-for-process-scheduling)
   - [Python GUI using Tkinter](#python-gui-using-tkinter)


## Introduction
This project aims to implement and compare different process scheduling algorithms, including First-Come, First-Served (FCFS), Shortest Job First (SJF), Priority Scheduling, and Round Robin (RR). Additionally, a basic GUI is developed using Python's Tkinter to visualize the results.

## Objective
The objective of this project is to understand and demonstrate the working of different process scheduling algorithms, calculate their average waiting time and turnaround time, and identify the best algorithm based on performance.

## Process Scheduling Algorithms

### First-Come, First-Served (FCFS)
In FCFS, the process that arrives first gets executed first. It's simple but can lead to the convoy effect.

### Shortest Job First (SJF)
SJF selects the process with the shortest burst time next. This can lead to the lowest average waiting time but can cause starvation for longer processes.

### Priority Scheduling
In Priority Scheduling, processes are executed based on priority. Higher priority processes are executed before lower priority ones. This can also cause starvation for lower priority processes.

### Round Robin (RR)
RR assigns a fixed time quantum and cycles through processes. It ensures fair CPU time distribution but can lead to higher average waiting time.

### C++ Program for Process Scheduling
The main C++ program includes the implementation of all four scheduling algorithms, calculates their average waiting time and turnaround time, and determines the best algorithm based on performance.

## Python GUI using Tkinter
To make the results more user-friendly, a GUI is implemented using Python's Tkinter library. The GUI allows users to input process details and view the results of different scheduling algorithms graphically.

### Key Features:

- Input fields for process details (ID, arrival time, burst time, priority).
- Buttons to run each scheduling algorithm.
- Display of Gantt charts for each algorithm.
- Display of average waiting and turnaround times.
- Display of the best algorithm based on performance metrics.

## Conclusion
This project demonstrates the implementation and comparison of various process scheduling algorithms using both C++ and a Python GUI. The results show that different algorithms have their strengths and weaknesses, and the best algorithm can vary based on the specific requirements and characteristics of the processes.

## Future Work
### Future enhancements could include:

Implementation of additional scheduling algorithms such as Multilevel Queue and Multilevel Feedback Queue.
More detailed GUI with real-time process visualization.
Handling of edge cases and more comprehensive input validation.
Performance optimization for large sets of processes.
