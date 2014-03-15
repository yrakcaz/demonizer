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

static void write_pidlist(s_pidlist *list)
{
    struct passwd *pw = getpwuid(getuid());
    char *homedir = pw->pw_dir;
    char *path = malloc (128 * sizeof (char));
    sprintf(path, "%s/.dem.pid", homedir);
    FILE *f = fopen(path, "w");
    s_pidlist *tmp = list;
    while (tmp)
    {
        s_pid *pid = tmp->pid;
        fprintf(f, "%d\t%s\t%d\n", pid->idx, pid->process, pid->pid);
        tmp = tmp->next;
    }
}

static void delete_pid(int process)
{
    s_pidlist *list = parse_file();
    s_pidlist *tmp = list;
    s_pidlist *prec = NULL;
    while (tmp)
    {
        if (tmp->pid->idx == process)
        {
            if (!prec)
                list = tmp->next;
            else
                prec->next = tmp->next;
            break;
        }
        prec = tmp;
        tmp = tmp->next;
    }
    write_pidlist(list);
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
        i++;
        while (buff[i] != '\t')
        {
            idx[j] = buff[i];
            i++;
            j++;
        }
        i++;
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
    s_pidlist *list = parse_file();
    s_pidlist *tmp = list;
    while (tmp)
    {
        if (tmp->pid->idx == process)
        {
            delete_pid(process);
            //free(list);
            return tmp->pid->pid;
        }
        tmp = tmp->next;
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
