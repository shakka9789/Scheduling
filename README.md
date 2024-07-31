# Scheduling Algorithm in C++

## Overview

This C++ program implements a scheduling algorithm to manage jobs with their respective execution times on multiple processors. The program reads job and graph data from input files, processes the scheduling using the provided algorithm, and writes detailed results to output files.

## Classes and Methods

### `node` Class

The `node` class represents a job with its ID, execution time, and a link to the next node in the list.

**Attributes:**

- `int jobID`: The ID of the job.
- `int jobTime`: The execution time required for the job.
- `node* next`: Pointer to the next job in the list.

**Constructor:**

- `node()`: Default constructor initializing `jobID` and `jobTime` to 0 and `next` to `NULL`.
- `node(int jobID, int jobTime, node* next)`: Initializes a node with a given `jobID`, `jobTime`, and `next` pointer.

**Methods:**

- `void printnode(node *node, ofstream &outFile)`: Prints the job details to the specified file.

### `schedule` Class

The `schedule` class manages the job scheduling across multiple processors.

**Attributes:**

- `int numNodes`: Number of nodes (jobs) in the graph.
- `int numProcs`: Number of processors available for scheduling.
- `int procUsed`: Number of processors used.
- `int currentTime`: Current time in the scheduling process.
- `int totalJobTimes`: Total time required for all jobs.
- `int *jobTimeAry`: Array holding the execution time for each job.
- `int *jobStatus`: Array indicating the status of each job (1 for pending, 2 for in-progress, 0 for completed).
- `int **Matrix`: Adjacency matrix representing job dependencies.
- `int **Table`: Scheduling table showing which jobs are assigned to which processors at each time unit.
- `node *OPEN`: Head of the linked list containing jobs that are available for scheduling.

**Constructor:**

- `schedule(int numNodes, int numProcs)`: Initializes the scheduling system with the given number of nodes and processors, setting up arrays and matrices.

**Methods:**

- `void loadTable()`: Allocates memory and initializes the scheduling table.
- `void loadMatrix(ifstream &inFile)`: Loads job dependencies from the input file into the adjacency matrix.
- `int loadJobTimeAry(ifstream &inFile)`: Loads job IDs and their execution times from the input file and calculates the total job times.
- `void setJobStatus(int i)`: Sets the status of a job to 'in-progress'.
- `void printMatrix(ofstream &outFile)`: Prints the adjacency matrix to the output file.
- `int findOrphan()`: Finds and returns the ID of a job with no dependencies (orphan job).
- `void OpenInsert(node *newNode)`: Inserts a job into the `OPEN` list in sorted order based on the number of dependencies.
- `void printOPEN(ofstream &outFile)`: Prints the `OPEN` list to the output file.
- `int getNextProc(int currentTime)`: Finds the next available processor at the current time.
- `void fillOPEN(ofstream &outFile)`: Fills the `OPEN` list with orphan jobs.
- `void putJobOnTable(int availProc, int currentTime, int jobId, int jobTime)`: Schedules a job on a processor for a specific time period.
- `void fillTable()`: Assigns jobs from the `OPEN` list to available processors.
- `void printTable(ofstream &outFile, int currentTime)`: Prints the scheduling table to the output file.
- `bool checkCycle()`: Checks for cycles in the job dependency graph.
- `bool isGraphEmpty()`: Checks if there are no more jobs left to schedule.
- `void deleteJob(int jobId)`: Removes a completed job from the dependency matrix.
- `void deleteDoneJobs(ofstream &outFile)`: Deletes completed jobs from the scheduling table and updates the matrix.

### `main` Function

The `main` function orchestrates the scheduling process.

**Steps:**

1. **File Operations:**
   - Opens input and output files.
   - Reads the number of nodes and processors.

2. **Initialization:**
   - Initializes the `schedule` class with the number of nodes and processors.
   - Loads job times and dependency matrix from the input files.
   - Prints the initial state of the matrix and scheduling table.

3. **Scheduling Process:**
   - Iteratively fills the `OPEN` list with orphan jobs, schedules jobs on available processors, and prints the updated scheduling table.
   - Checks for cycles in the job graph and terminates if a cycle is detected.
   - Updates and prints the scheduling table at each time unit.

4. **Completion:**
   - Closes all opened files.
