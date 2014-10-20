#include "../include/manage.h"

char *get_process_cmd(pid_t pid)
{
    char name[64];
    sprintf(name, "/proc/%d/cmdline", pid);
    int fd = open(name, O_RDONLY);
    char *ret = malloc(128 * sizeof (char));
    int nbbytes = read(fd, ret, 128);
    int i;
    for (i = 0; i < nbbytes; i++)
        ret[i] = ret[i] == '\0' ? ' ' : ret[i];
    ret[i] = '\0';
    close(fd);
    return ret;
}

static int is_a_number(char *s)
{
    for (size_t i = 0; i < strlen(s); i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return 0;
    }
    return 1;
}

static int is_a_dir(char *file)
{
    char *path = malloc((7 + strlen(file)) * sizeof (char));
    char *base = "/proc/";
    size_t i = 1;
    for (i = 0; i < strlen(base); i++)
        path[i] = base[i];
    path[i] = '\0';
    path = strcat(path, file);
    struct stat st;
    stat(path, &st);
    free(path);
    if (S_ISDIR(st.st_mode))
        return 1;
    return 0;
}

static int is_a_deamon(char *dir)
{
    char name[64];
    sprintf(name, "/proc/%s/stat", dir);
    int fd = open(name, O_RDONLY);
    int spaces = 0;
    char c;
    while (spaces < 3 && read(fd, &c, 1))
        if (c == ' ')
            spaces++;
    char ppid[6];
    int i = 0;
    while(read(fd, &c, 1) && c != ' ')
        ppid[i++] = c;
    ppid[i] = '\0';
    close(fd);
    if (atoi(ppid) == 1)
        return 1;
    return 0;
}

void get_deamons()
{
    for (int i = 0; i < DEAMONS_SIZE; i++)
        deamons[i] = 0;
    DIR *rep = opendir("/proc");
    struct dirent *file;
    int i = 0;
    while ((file = readdir(rep)))
    {
        if (is_a_number(file->d_name) && is_a_dir(file->d_name))
            if (is_a_deamon(file->d_name))
                deamons[i++] = atoi(file->d_name);
    }
    closedir(rep);
}

void display_deamons()
{
    printf("idx:\tpid:\tcmdline:\n");
    printf("----\t----\t--------\n\n");
    get_deamons();
    for (int i = 0; deamons[i] != 0; i++)
    {
        char *cmdline = get_process_cmd(deamons[i]);
        printf("[%d]\t%d\t%s\n", i, deamons[i], cmdline);
        free(cmdline);
    }
}
