# CPUSCHED
# CPU Scheduling
# Description: This project simulates a CPU scheduler. The entire project contains a process generator and a CPU scheduler. The process generator takes three parameters: the max CPU burst time, the minimum CPU burst time, and the interarrival time. It generates 10000 processes, each process has its own ID, arrival time, CPU burst time, and priority, and then they're stored into the text file "processesInput.txt". The scheduler retrieves the data from processesInput.txt and simulates a real process arrival. It has four algorithms, FIFO, SJF with preemption, RR(with specified time quantum), and Priority with preemption. Users can select which algorithms they want to print the statistics for the run, and store the data to a output file, CPUSCHED-output.txt.
# How to run this code:
# For Windows, Way 1: In Windows CMD, change to the directory of the file using "cd", without quotes. Then input "main.exe", without quotes.
# For example:
# cd C:\Users\nameofUser\Desktop\PROJECT\CPUSCHED
# main.exe  
# Way 2: Double click the main.exe. (If Way 1 and 2 do not work properly, please use Way 3.)
# Way 3: Just simply use CodeBlock to import the project, click build and run the program.
# NOTE: 1.This program is only designed for this project.
#       2.The ID and the arrival time of the process of the input file must be in ascedning order.
#       3.The difference between process ID and process Index is 1.
#       4.Max, minimum CPU burst time, and the interarrival time must be greater than 0.
#       5.Highly recommend CPU burst time and interarrival time are small numbers. A good example of this will be 5, 1, 3.
# Background: In order to test each algorithm functions correctly, I had chose few homework questions to run the test on. The sample input section shows the process, the sample output section shows the theoretical solution, and the program solution section shows the actual output of this program.
# Sample input:
# Index Arrival time  CPU Burst Length   Priority
#   1          3                20       6
#   2          5                 8       3
#   3          12               10       2
#   4          18               12       4
#   5          20                4       1
#   6          25                6       5
# Sample output:
# Process Av. Wait time     Av. Turnaround time
# FCFS    20.8              30.8
# SJF(non-preemptive) omit, this project uses SJF(with preemption)
# Preemptive priority 13.5  23.5
# RR(10)  21.5              31.5
# Program output:
# FIFO
# Statistics
#
# Number of processes: 6
# Total elapsed time(for the scheduler): 63
# Throughput: 0.1
# CPU utilization: 95.2381%
# Average waiting time: 20.8333
# Average turnaround time: 30.8333
# Average response time: 20.8333
#
# ---------------------------------------
# SJF with preemption
# Statistics
#
# Number of processes: 6
# Total elapsed time(for the scheduler): 63
# Throughput: 0.1
# CPU utilization: 95.2381%
# Average waiting time: 10.1667
# Average turnaround time: 20.1667
# Average response time: 3.5
#
# ---------------------------------------
# Round Robin with Time Quantum: 10
# Statistics
#
# Number of processes: 6
# Total elapsed time(for the scheduler): 63
# Throughput: 0.1
# CPU utilization: 95.2381%
# Average waiting time: 21.5
# Average turnaround time: 31.5
# Average response time: 16.8333
#
# ---------------------------------------
# Priority with Preemption
# Statistics
#
# Number of processes: 6
# Total elapsed time(for the scheduler): 63
# Throughput: 0.1
# CPU utilization: 95.2381%
# Average waiting time: 13.5
# Average turnaround time: 23.5
# Average response time: 3.83333
#
# ---------------------------------------
#
#
# In conclusion, the CPU scheduler output results matches the theoretical results, which indicates the algorithms are correct.
