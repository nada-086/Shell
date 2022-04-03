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
    ![Run](/Screenshots/run.png)
## Features
### The program supports the following commands
  - Commands with no arguments such as: exit, ls, rm...etc
    ![General Commands](/Screenshots/commands.png)
  - Commands with single or multiple arguments such as ls -l, ls -a -h -l,...etc
    ![Single and Multiple Commands](/Screenshots/arguments.png)
  - Running applications either in the foreground or the background.
  - Built-in Commands which are cd and echo.
  - Some special uses for the cd command
    - cd 
    - cd ~
    - cd ..
    - cd absolute_path
    - cd relative_path
      ![cd Command](/Screenshots/cd.png)
   - Expression Evaluation using the export command
      ![Expression Evaluation](/Screenshots/expression-evaluation.png)
      ![Expression Evaluation 2](/Screenshots/expression-evaluation2.png)
  - when a child process terminates its parent's id is added to a log file.
    ![Log file](/Screenshots/log.png)