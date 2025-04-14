# Task Manager

This is a simple task manager written in C for OS Project. This project uses Linux system calls and checks /proc directory to fetch the process.

## Features
1. List all process
2. Displays the top N processes sorted by priority (nice value)
3. Kill a process
4. Change the priority (nice value) of a process


## Requirements
1. Any Linux distro or WSL for Windows
2. GCC
3. CMAKE and MAKE

## How to Build

1. Clone the repository
2. Inside the project directory, create a build folder:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
