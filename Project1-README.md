# Project1-README

## Overview

This README provides an overview of the implementation of two custom system calls, `sys_waitx` and `sys_ps`, in the xv6 operating system. The system environment used for development and testing, along with instructions for compiling and running the code, is provided below.

## System Environment

- Development Environment: Windows Subsystem for Linux (WSL) on Ubuntu
- Operating System: Ubuntu 20.04.3 LTS (WSL)
- Compiler: GCC version 9.3.0
- xv6 Source: [xv6-public](https://github.com/mit-pdos/xv6-public)

## Implementation Details

### Custom System Calls

1. `sys_waitx`: This system call extends the standard `wait` system call by adding the ability to retrieve timing information for a child process. The timing information includes process creation time, end time, and total execution time.

2. `sys_ps`: The `sys_ps` system call is designed to provide information about the processes currently running in the system. It displays the PID (Process ID), process name, process state, creation time, end time, and total time for each process in the process table.

### Files Modified

The following files were modified to implement the custom system calls:

- `sysproc.c`: Added syscall implementations for `sys_waitx` and `sys_ps`.
- `proc.c` and `proc.h`: Extended the `struct proc` definition to include fields for process timing information.
- `syscall.c` and `syscall.h`: Defined the syscall numbers for `sys_waitx` and `sys_ps`.
- `usys.S`: Added user-level syscall stubs for the new system calls.
- `defs.h`: Added function prototypes for the new system calls.
- `user.h`: In the `user.h` header file, we added function prototypes for the `sys_ps` and `sys_waitx` system calls. 
- `Makefile`: Modified the Makefile to compile the new system calls and user-level programs.


### User-Level Programs

Two user-level programs were created to test the custom system calls:

- `test.c`: Tests the `sys_waitx` system call by forking child processes `uniq` and `head` and and measuring their execution time.
- `ps.c`: Implements the `ps` command to test the `sys_ps` system call. The `ps` command displays process information, including timing data, for all running processes and also for selected processes providing their pid.

## Compilation and Execution

To compile the code and test the custom system calls, follow these steps:

1. Open a terminal in the xv6 source directory.

2. Clean the existing build:

```shell
make clean
```

3. Build xv6 with the custom system calls:

```shell
make qemu-nox
```

4. Create a text file named `file.txt` and enter content (e.g., one two three ....(line by line)):

```shell
cat > file.txt
```

5. Compile and run the `test` program to test `sys_waitx`:

```shell
test
```

6. To run the `ps` command and test `sys_ps`, simply execute:

```shell
ps
```
or

```shell
ps <any_process_pid>
```


## Conclusion

This README provides an overview of the implementation of custom system calls in xv6, including details about the system environment, modifications made to the code, and instructions for compilation and testing. The custom system calls `sys_waitx` and `sys_ps` enhance the functionality of xv6 by allowing users to gather timing information about processes.





