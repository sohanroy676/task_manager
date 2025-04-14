#include "task.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <sys/resource.h>

// ANSI color macros
#define COLOR_RESET "\033[0m"
#define COLOR_RED   "\033[1;31m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_GREEN "\033[1;32m"

// int is_digit(char chr) {
//	return chr >= '0' && chr <= '9';
// }


int get_process_info(int pid, Task *task) {
	char path[256];
	snprintf(path, sizeof(path), "/proc/%d/status", pid);
	FILE *file = fopen(path, "r");
	if (!file) {
		return 0;
	}
	task->pid = pid;
	char line[256];
	while (fgets(line, sizeof(line), file)) {
		if (strncmp(line, "Name:", 5) == 0)
		    sscanf(line, "Name:\t%255s", task->name);
		else if (strncmp(line, "State:", 6) == 0)
		    sscanf(line, "State:\t%c", &task->state);
		else if (strncmp(line, "VmRSS:", 6) == 0)
		    sscanf(line, "VmRSS:\t%lu", &task->mem);
		else if (strncmp(line, "Uid:", 4) == 0);
	}
	fclose(file);
	snprintf(path, sizeof(path), "/proc/%d/stat", pid);
	file = fopen(path, "r");
	if (file) {
		int temp;
		char comm[256], state;
		long priority;
		fscanf(file, "%d %s %c", &temp, comm, &state);
		for (int i = 0; i < 15; i++) fscanf(file, "%*s");
		fscanf(file, "%ld", &priority);
		task->priority = priority;
		fclose(file);
	}
	return 1;
}

int get_process_list(Task **task_list) {
	DIR *dir = opendir("/proc");
	if (!dir) {
		return -1;
	}
	struct dirent *entry;
	int count = 0;
	*task_list = (Task *)malloc(sizeof(Task) * 1024);
	
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
			int pid = atoi(entry->d_name);
			if (get_process_info(pid, &(*task_list)[count])) {
				count++;
			}
		}
	}
	closedir(dir);
	return count;
}

void display_task(const Task *task) {
    const char *color;
    if (task->priority < 0)
        color = COLOR_RED;
    else if (task->priority <= 10)
        color = COLOR_YELLOW;
    else
        color = COLOR_GREEN;

    printf("%s[%-5d] %-20s | %-5c | %-8d | %lu KB%s\n",
           color, task->pid, task->name, task->state, task->priority, task->mem, COLOR_RESET);
}

// Display all tasks
void display_tasks(const Task *task_list, int task_count) {
	printf("[%-5s] %-20s | %s | %s | %s\n", "PID", "Name", "State", "Priority", "Mem");
	for (int i = 0; i < task_count; i++) {
		display_task(&task_list[i]);
	}
}

// Display limit-no. of tasks
void display_tasks_limit(const Task *task_list, int task_count, int limit) {
	int l = limit < task_count ? limit : task_count;
	printf("[%-5s] %-20s | %s | %s | %s\n", "PID", "Name", "State", "Priority", "Mem");
	for (int i = 0; i < l; i++) {
		display_task(&task_list[i]);
	}
}

int kill_process(int pid) {
	return kill(pid, SIGTERM); // Send TERM signal
}

int renice_process(int pid, int new_nice) {
	return setpriority(PRIO_PROCESS, pid, new_nice);
}

void free_task_list(Task *task_list) {
	free(task_list);
}
