#include "../include/pid_file.h"

static char *itoa(int n)
{
    char *txt = malloc (16 * sizeof (char));
    sprintf(txt, "%d", n);
    return txt;
}

static s_pidlist *add_pid(s_pid *pid, s_pidlist *list)
{
    s_pidlist *node = malloc(sizeof (s_pidlist));
    node->pid = pid;
    if (!list)
        node->next = NULL;
    else
        node->next = list;
    return node;
}

s_pidlist *parse_file()
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    FILE *f = fopen(path, "r");
    char buff[128];
    s_pidlist *list = NULL;
    while(fgets(buff, 100, f))
    {
        s_pid *pid = malloc(sizeof (s_pid));
        char *idx = malloc(128 * sizeof (char));
        int i = 0;
        while (buff[i] != '\t')
        {
            idx[i] = buff[i];
            i++;
        }
        idx[i] = '\0';
        pid->idx = atoi(idx);
        int j = 0;
        while (buff[i] != '\t')
        {
            idx[j] = buff[i];
            i++;
            j++;
        }
        idx[j] = '\0';
        pid->process = idx;
        char tpid[16];
        j = 0;
        while (buff[i] && buff[i] != '\n')
        {
            tpid[j] = buff[i];
            i++;
            j++;
        }
        tpid[j] = '\0';
        pid->pid = atoi(tpid);
        list = add_pid(pid, list);
    }
    fclose(f);
    return list;
}

pid_t get_pid(int process)
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    char *str = malloc(4096 * sizeof (char));
    read(fd, str, 4096);
    int i = 0;
    while (str[i])
    {
        char *line = malloc (128 * sizeof (char));
        int j = 0;
        while (str[i] != '\t')
        {
            line[j] = str[i];
            i++;
            j++;
        }
        line[j] = '\0';
        if (atoi(line) == process)
        {
            i++;
            while (str[i] != '\t')
                i++;
            i++;
            j = 0;
            while (str[i] != '\n')
            {
                line[j] = str[i];
                i++;
                j++;
            }
            line[j] = '\0';
            return atoi(line);
        }
        while (str[i] != '\n')
            i++;
        i++;
        free(line);
    }
    return -1;
}

void set_pid(char *process, pid_t pid)
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    int fd = open(path, O_RDWR | O_CREAT, 0644);
    char *str = malloc(4096 * sizeof (char));
    read(fd, str, 4096);
    int i = 0;
    for (int j = 0; str[j]; j++)
        if (str[j] == '\n')
            i++;
    write(fd, itoa(i), strlen(itoa(i)));
    write(fd, "\t", 1);
    write(fd, process, strlen(process));
    write(fd, "\t", 1);
    write(fd, itoa(pid), strlen(itoa(pid)));
    write(fd, "\n", 1);
    close(fd);
}

void clean()
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    FILE *f = fopen(path, "w");
    fclose(f);
    exit(0);
}

int main()
{
    parse_file();
}
