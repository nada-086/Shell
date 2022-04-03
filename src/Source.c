#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <libgen.h>
#include<sys/wait.h>
#include <sys/resource.h>
#include<signal.h>
#define BUFFER_SIZE 100


// Global Variables
int background = 0;
char* path;

char* read_input();
char** command(char* line);
void execute(char** cmd);
void shell();
int cd(char** command);
void echo(char** str);
int built_in(char** cmd);
void parent_main();
char* parent_path(char* path);
void on_child_exit();
char* remove_quotes(char* str);
char* import_env(char* var);
void export_env(char** command);
char** repair_command(char** command);
void check_background(char** command);


int main() {
    parent_main();
    return 0;
}

char** break_var(char** cmd, int index) {
    char** new_command = malloc(sizeof(cmd) + sizeof(char*) * 10);
    int i = 0;
    int k = 0;
    while (cmd[i]) {
        if (i == index) {
            char** array = malloc(sizeof(cmd[i]) + sizeof(char*) * 10);
            array = command(cmd[i]);
            int j = 0;
            while (array[j]) {
                new_command[k++] = array[j++];
            }
        }
        else {
            new_command[k++] = cmd[i];
        }
        i++;
    }
    return new_command;
}

char* import_env(char* var) {
    int i = 1;
    var = remove_quotes(var);
    char* v = malloc(sizeof(var));
    while (var[i]) {
        v[i - 1] = var[i];
        i++;
    }
    v = getenv(v);
    return v;
}

void export_env(char** command) {
    int i = 0;
    command[1] = remove_quotes(command[1]);
    char* name = malloc(sizeof(command[1]));
    char* value = malloc(sizeof(command[1]));
    while (command[1][i] && command[1][i] != '=') {
        name[i] = command[1][i];
        i++;
    }
    i++;
    int index = 0;
    int j = 1;
    while (command[j]) {
        command[j] = remove_quotes(command[j]);
        while (command[j][i]) {
            value[index++] = command[j][i++];
        }
        if (command[j + 1]) {
            value[index++] = ' ';
        }
        j++;
        i = 0;
    }
    setenv(name, value, 1);
}

char* read_input() {
    char* line;
    size_t length = 0;
    printf(">>");
    while (getline(&line, &length, stdin) == -1) {
        if (feof(stdin)) {
            exit(0);
        }
        else {
            exit(1);
        }
    }
    return line;
}

void check_background(char** command) {
    int i = 0;
    while (command[i]) {
        if (strcmp(command[i], "&") == 0) {
            background = 1;
        }
        i++;
    }
}

char** repair_command(char** command) {
    int i = 1;
    int flag = 0;
    char* var;
    while (command[i]) {
        var = remove_quotes(command[i]);
        if (var[0] == '$') {
            var = import_env(var);
            flag = 1;
        }
        command[i] = var;
        if (flag == 1) {
            command = break_var(command, i);
            flag = 0;
        }
        i++;
    }
    return command;
}

char** command(char* line) {
    int size = BUFFER_SIZE;
    int index = 0;
    char** array = malloc(size * sizeof(char*));
    char* c = strtok(line, " \t\n");
    while (c != NULL) {
        array[index++] = c;
        if (index >= size) {
            size += BUFFER_SIZE;
            array = realloc(array, sizeof(char*) * size);
        }
        c = strtok(NULL, " \t\n");
    }
    check_background(array);
    array = repair_command(array);
    return array;
}

void on_child_exit(int sig) {
    int id = getpid();
    FILE* file = fopen("log.txt", "a+");
    fprintf(file, "Child process with id %d is terminated.\n", id);
    fflush(file);
}

void execute(char** command) {
    pid_t pid = fork();
    int status = 0;
    if (pid == 0) {
        if (execvp(command[0], command) == -1) {
            perror("Error: ");
        }
    }
    else if (pid > 0) {
        if (background == 0) {
            do {
                pid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        else {
            return;
        }
    }
    else {
        background = 0;
        printf("Error: ");
    }
}

char* parent_path(char* path) {
    int i = strlen(path) - 1;
    while (i >= 0 && path[i] != '/') {
        path[i--] = ' ';
    }
    path[i] = ' ';
    char* real = malloc(BUFFER_SIZE);
    i = 0;
    while(path[i] != ' '){
       real[i] = path[i];
       i++;
    }
    return real;
}

int cd(char** command) {
    char* buffer = NULL; size_t size = 0;
    path = getcwd(buffer, size);
    if (command[1] == NULL || strcmp(command[1], "~") == 0) {
        path = getenv("HOME");
    }
    else if (strcmp(command[1], "..") == 0) {
        path = parent_path(path);
    }
    else {
        path = realpath(command[1], path);
        if (chdir(path) != 0) {
            return 0;
        }
    }
    chdir(path);
    printf("%s\n", path);
    return 1;
}

void setup_environment() {
    char* buffer = NULL;
    size_t size = 0;
    char* path = getcwd(buffer, size);
    chdir(path);
}

char* remove_quotes(char* str) {
    int i = 0, j = 0;
    char* temp = malloc(sizeof(str) * sizeof(char));
    while (str[i++]) {
        if (str[i - 1] != '"')
            temp[j++] = str[i - 1];
    }
    temp[j] = '\0';
    return temp;
}

void echo(char** str) {
    str = repair_command(str);
    int i = 1;
    while (str[i]) {
        printf("%s ", str[i]);
        i++;
    }
}

int built_in(char** command) {
    if (strcmp(command[0], "cd") == 0) {
        return cd(command);
    }
    else if (strcmp(command[0], "echo") == 0) {
        echo(command);
        printf("\n");
        return 1;
    }
    else if (strcmp(command[0], "export") == 0) {
        export_env(command);
        return 1;
    }
    return 0;
}

void shell() {
    while (1) {
        char* line = read_input();
        char** cmd = command(line);
        if (strcmp(cmd[0], "exit") == 0) {
            printf("\n>>>>>>>>>>>>>> Shell is closed\n");
            exit(0);
        }
        else if (built_in(cmd) == 0) {
            execute(cmd);
        }
    }
}

void parent_main() {
    printf(">>>>>>>>>>>>>> Shell is opened\n\n");
    setup_environment();
    signal(SIGCHLD, on_child_exit);
    shell();
}
