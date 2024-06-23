# Scheduler Project with sthread Library

!>Code is only for demo version: Can't be compiled/execute without priavte files<!

This project implements a simple scheduler using the custom created `sthread` library. 

## Folder Structure

- `datastruct/dll.c`, `datastruct/dll.h`: Implementation and header files for doubly linked lists (DLL) as a data structure.
- `scheduler/sthread.c`, `scheduler/sthread.h`: Implementation and header files for the scheduler using the `sthread` library.
- `tests/test_dll.c`, `tests/test_scheduler.c`, `tests/test_sthread.c`: Test programs for doubly linked lists, scheduler, and `sthread` functionalities respectively.

## Requirements

To run the project, you'll need:
- A POSIX-compliant operating system (e.g., Linux)
- GCC compiler or a compatible C compiler

## Compilation and Execution

1. Open a terminal and navigate to the directory of this project.  
2. Compile the program with the command: make  
This command will create the executable file `main`.  

3. Run the program with: ./main
The program will start and execute the implemented functions.  

## Project Structure

The `sthread` library includes functions for thread creation (`sthr_spawn`), synchronization (`sthr_waitpid`), and termination (`sthr_exit`).  
These were used to implement the various threads in this project.

