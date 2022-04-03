# Simple Linux Shell
## Description
This program is implemented as an application on some operating systems concepts such as:
  - System calls in Unix environment
  - Processes and Multi-Processing
  - Signal Handling in Unix environment
## How to run
  - The program needs to be run in Linux environment either on a virtual machine or as the main operating system.
  - Download the source code and run the following command in the terminal
     ```
     make
    ./shell
    ```
## Features
### The program supports the following commands
  - Commands with no arguments such as: exit, ls, rm...etc
  - Commands with single or multiple arguments such as ls -l, ls -a -h -l,...etc
  - Running applications either in the foreground or the background.
  - Built-in Commands which are cd and echo.
  - Some special uses for the cd command
    - cd 
    - cd ~
    - cd ..
    - cd absolute_path
    - cd relative_path
   - Expression Evaluation using the export command
  - when a child process terminates its parent's id is added to a log file.
## Short Demo
https://youtu.be/A69IDv0hd-o
