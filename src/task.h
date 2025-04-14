#ifndef TASK_H
#define TASK_H

// Task structure definition
typedef struct {
    int pid;
    char name[256];
    char state;
    int priority;
    unsigned long mem;
} Task;

// Function declarations
//int is_digit(char chr);

int get_process_list(Task **task_list);
void display_task(const Task *task);
void display_tasks(const Task *task_list, int task_count);
void display_tasks_limit(const Task *task_list, int task_count, int limit);
int kill_process(int pid);
int renice_process(int pid, int new_nice);
void free_task_list(Task *task_list);


#endif // TASK_H

