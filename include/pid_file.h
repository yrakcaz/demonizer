#ifndef PID_FILE_H
# define PID_FILE_H

# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# include <pwd.h>

typedef struct pid
{
    int idx;
    char *process;
    pid_t pid;
} s_pid;

typedef struct pidlist
{
    s_pid *pid;
    struct pidlist *next;
} s_pidlist;

s_pidlist *parse_file();
pid_t get_pid(int process);
s_pid *get_s_pid(int process);
void set_pid(char *process, pid_t pid);
void display_pidlist();
void delete_pid(int process);
void clean();

#endif /* !PID_FILE_H */
