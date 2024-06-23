# Non-Preemptive Scheduler Project

This project implements a non-preemptive scheduler in C.

## Project Structure

- `build/`: Compiled objects and binaries.
- `datastruct/`: Data structure implementations.
  - `dll.c`, `dll.h`: Doubly linked list implementation.
  - `sthread.c`, `sthread.h`: Thread implementation.
- `include/`: Header files.
  - `main.h`: Main header file.
  - `scheduler.h`: Scheduler header file.
- `scheduler/`: Scheduler implementation.
  - `scheduler.c`, `scheduler.h`: Scheduler source and header files.
- `tests/`: Test files for the components.
  - `test_dll.c`: Tests for the doubly linked list.
  - `test_sthread.c`: Tests for the process list.
  - `test_scheduler.c`: Tests for the scheduler.
- `compile.sh`: Script to compile the project.
- `main.c`: Entry point of the application.
- `readme.md`: Project documentation.

## Build Instructions

To build the project, run the following command:
```sh
./compile.sh