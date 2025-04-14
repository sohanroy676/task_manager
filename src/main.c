#include <stdio.h>
#include <stdlib.h>
#include "task.h"

#define MAX_TASKS 1024

int cmp(const void *A, const void *B) {
	return ((Task*)B)->priority - ((Task*)A)->priority;
}

void sort_tasks_by_priority(Task **task_list, int task_count) {
	qsort(*task_list, task_count, sizeof(Task), cmp);
}

int main() {
	Task *task_list;
	// Fetching the processes
	int task_count = get_process_list(&task_list);
	if (task_count < 0) {
		printf("Failed to fetch processes\n");
		return 1;
	}

	sort_tasks_by_priority(&task_list, task_count);

	int cmd, limit, pid, new_priority;
	
	do {
		printf("[0] Quit [1] List-All [2] List-Limit [3] Kill [4] Change-Priority: ");
		scanf("%d", &cmd);

		switch (cmd) {
			case 0:
				printf("Quitting Task Manager\n");
				break;
			case 1:
				printf("Task List sorted by priority:\n");
				display_tasks(task_list, task_count);
				break;
			case 2:
				printf("Limit: ");
				scanf("%d", &limit);
				printf("Task List sorted by priority:\n");
				display_tasks_limit(task_list, task_count, limit);
				break;
			case 3:
				printf("PID to kill: ");
				scanf("%d", &pid);
				if (kill_process(pid) == 0) {
					printf("Terminated successfully.\n");
					for (int i = 0; i < task_count; i++) {
						if (task_list[i].pid == pid) {
							task_list[i].priority = -100;
							break;
						}
					}
					sort_tasks_by_priority(&task_list, task_count);
					task_count--;
				} else {
					printf("Termination failed.\n");
				}
				break;
			case 4:
				printf("PID: ");
				scanf("%d", &pid);
				printf("Enter new priority (-20 to 19): ");
				scanf("%d", &new_priority);
				if (renice_process(pid, new_priority) == 0) {
					printf("Priority updated.\n");
					for (int i = 0; i < task_count; i++) {
						if (task_list[i].pid == pid) {
							task_list[i].priority = new_priority;
							break;
						}
					}
					sort_tasks_by_priority(&task_list, task_count);

				} else {
					printf("Update Failed.\n");
				}
				break;
			default:
				printf("Invalid Command\n");

		}
	} while (cmd != 0);

	free_task_list(task_list);

	return 0;
}

